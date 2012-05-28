/*
 Copyright (C) 2012 MoSync AB

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License,
 version 2, as published by the Free Software Foundation.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 MA 02110-1301, USA.
 */

/**
 * @file PurchaseManager.mm
 * @author Bogdan Iusco
 * @date 4 Apr 2012
 *
 * @brief Design pattern: singleton.
 * Provides a general mechanism for managing purchases.
 * It communicates with the Apple App Store through Store Kit.
 */

#import "PurchaseManager.h"
#import "PurchaseProduct.h"
#import "Platform.h"
#include "Syscall.h"

/**
 * Hidden methods for PurchaseManager.
 */
@interface PurchaseManager (hidden)

/**
 * Send a purchase event.
 * @param type Type of the purchase event.
 *             Must be one of the MA_PURCHASE_EVENT constants.
 * @param state State of the purchase.
 * @param productHandle Product handle that sends the event.
 * @param errorCode Reason why the purchase failed.
 *                  Valid if type is MA_PURCHASE_EVENT_REQUEST_STATE_CHANGED
 *                  and state is MA_PURCHASE_STATE_FAILED.
 */
-(void) sendPurchaseEvent:(const int) type
                    state:(const int) state
            productHandle:(MAHandle) productHandle
                errorCode:(const int) errorCode;

/**
 * Get the purchase product that contains a given payment.
 * @param payment The given payment send to Apple App Store.
 * @return The product containg the payment if found, otherwise nil.
 */
-(PurchaseProduct*) productUsingPayment:(SKPayment*) payment;

/**
 * Check if purchase is supported on the current device.
 * If it's not a purchase event will be sent.
 * @param purchaseEventType Purchase event type. Must be one of MA_PURCHASE_EVENT constants.
 * @param productHandle Handle to the product that will send the event.
 * @return YES if the puchase is supported(the event has not been send), otherwise NO
 * (the event has been send).
 */
-(BOOL) checkPurchaseSupported:(const int) purchaseEventType
                 productHandle:(MAHandle) productHandle;

/**
 * Restored a product that has been purchased by the user.
 * Send purchase related event for the restored product.
 * @param transaction The transaction for the purchased product.
 */
-(void) handleProductRestored:(SKPaymentTransaction*) transaction;

@end


@implementation PurchaseManager

static PurchaseManager *sharedInstance = nil;

/**
 * Returns an instance to the shared singleton.
 * @return The shared object.
 */
+(PurchaseManager*) getInstance
{
    if (nil == sharedInstance)
    {
        sharedInstance = [[PurchaseManager alloc] init];
    }

    return sharedInstance;
}

/**
 * Releases the class instance.
 */
+(void) deleteInstance
{
    [sharedInstance release];
}

/**
 * Init function.
 */
- (id)init
{
    self = [super init];
    if (self)
    {
        _paymentQueue = [SKPaymentQueue defaultQueue];
        [_paymentQueue addTransactionObserver:self];
        _countProducts = 0;
        _productsDictionary = [[NSMutableDictionary alloc] init];
    }

    return self;
}

/**
 * Check if purchase is availble on the device.
 * @return One of the next constants:
 * - MA_PURCHASE_RES_OK if purchase is available.
 * - MA_PURCHASE_RES_DISABLED otherwise.
 */
-(int) isPurchaseSupported
{
    return [SKPaymentQueue canMakePayments] ? MA_PURCHASE_RES_OK : MA_PURCHASE_RES_DISABLED;
}

/**
 * Create a product object.
 * @param productHandle A valid handle that will be used to indetify the new product.
 * @param productID String that identifies the object.
 */
-(void) createProduct:(MAHandle) productHandle
            productID:(const char*) productID
{
    // Check if purchase is supported.
    BOOL isPurchaseSupported = [self checkPurchaseSupported:MA_PURCHASE_EVENT_PRODUCT_CREATE
                                              productHandle:productHandle];
    if (!isPurchaseSupported)
    {
        return;
    }

    // Check if productHandle is unique.
    NSNumber* key = [NSNumber numberWithInt:productHandle];
    PurchaseProduct* product = [_productsDictionary objectForKey:key];
    if (product)
    {
        [self sendPurchaseEvent:MA_PURCHASE_EVENT_PRODUCT_CREATE
                          state:MA_PURCHASE_STATE_DUPLICATE_HANDLE
                  productHandle:productHandle
                      errorCode:0];
        return;
    }

    // Check if productID is not empty.
    NSString* productIDString = [NSString stringWithUTF8String:productID];
    if ([productIDString length] == 0)
    {
        [self sendPurchaseEvent:MA_PURCHASE_EVENT_PRODUCT_CREATE
                          state:MA_PURCHASE_STATE_PRODUCT_INVALID
                  productHandle:productHandle
                      errorCode:0];
        return;
    }

    // Create the product.
    product = [[PurchaseProduct alloc] initWithHandle:[key intValue]
                                            productID:productIDString];
    [_productsDictionary setObject:product forKey:key];
}

/**
 * Destroy a product object.
 * @param productHandle Handle to the product to destroy.
 * @return One of the following values:
 * - MA_PURCHASE_RES_OK if product has been detroyed.
 * - MA_PURCHASE_RES_INVALID_HANDLE if the productHandle is invalid.
 */
-(int) destroyProduct:(MAHandle) productHandle
{
    NSNumber* key = [NSNumber numberWithInt:productHandle];
    PurchaseProduct* product = [_productsDictionary objectForKey:key];
    if (!product)
    {
        return MA_PURCHASE_RES_INVALID_HANDLE;
    }

    [_productsDictionary removeObjectForKey:key];
    [product release];
    return MA_PURCHASE_RES_OK;
}

/**
 * Add a given product to the payment queue.
 * @param productHandle Handle to the product to be added.
 * @param quantity How many products to be purchased. Must be a value greater than zero.
 */
-(void) requestProduct:(MAHandle) productHandle
              quantity:(const int) quantity
{
    // Check if purchase is supported.
    BOOL isPurchaseSupported = [self checkPurchaseSupported:MA_PURCHASE_EVENT_REQUEST
                                              productHandle:productHandle];
    if (!isPurchaseSupported)
    {
        return;
    }

    NSNumber* key = [NSNumber numberWithInt:productHandle];
    PurchaseProduct* product = [_productsDictionary objectForKey:key];
    if (!product)
    {
        [self sendPurchaseEvent:MA_PURCHASE_EVENT_REQUEST
                          state:MA_PURCHASE_STATE_FAILED
                  productHandle:productHandle
                      errorCode:MA_PURCHASE_ERROR_INVALID_HANDLE];
        return;
    }

    // Check if quantity is valid.
    if (quantity <= 0)
    {
        [self sendPurchaseEvent:MA_PURCHASE_EVENT_REQUEST
                          state:MA_PURCHASE_STATE_FAILED
                  productHandle:productHandle
                      errorCode:MA_PURCHASE_ERROR_INVALID_QUANTITY];
        return;
    }

    SKMutablePayment* payment = [product payment];
    // If the product is valid it should contain a payment.
    if (!payment)
    {
        [self sendPurchaseEvent:MA_PURCHASE_EVENT_REQUEST
                          state:MA_PURCHASE_STATE_FAILED
                  productHandle:productHandle
                      errorCode:MA_PURCHASE_ERROR_INVALID_PRODUCT];
    }
    else
    {
        payment.quantity = quantity;
        [_paymentQueue addPayment:payment];
    }
}

/**
 * Get the product id for a given product.
 * @param productHandle Handle to a product.
 * @param buffer Will contain the product name.
 * @param bufferSize Maximum size of the buffer.
 * @return The number of written bytes in case of success, or
 * one of the next result codes:
 * - MA_PURCHASE_RES_INVALID_HANDLE if the productHandle is invalid.
 * - MA_PURCHASE_RES_BUFFER_TOO_SMALL if the buffer is too small.
 */
-(int) productName:(MAHandle) productHandle
            buffer:(char*) buffer
        bufferSize:(const int) bufferSize
{
    NSNumber* key = [NSNumber numberWithInt:productHandle];
    PurchaseProduct* product = [_productsDictionary objectForKey:key];
    if (!product)
    {
        return MA_PURCHASE_RES_INVALID_HANDLE;
    }

    return [product productName:buffer bufferSize:bufferSize];
}

/**
 * Verify if the receipt came from App Store.
 * @param productHandle Handle to the product you want to verify its receipt.
 */
-(void) verifyReceipt:(MAHandle) productHandle
{
    // Check if purchase is supported.
    BOOL isPurchaseSupported = [self checkPurchaseSupported:MA_PURCHASE_EVENT_REQUEST
                                              productHandle:productHandle];
    if (!isPurchaseSupported)
    {
        return;
    }

    NSNumber* key = [NSNumber numberWithInt:productHandle];
    PurchaseProduct* product = [_productsDictionary objectForKey:key];
    if (!product)
    {
        [self sendPurchaseEvent:MA_PURCHASE_EVENT_VERIFY_RECEIPT
                          state:MA_PURCHASE_STATE_RECEIPT_ERROR
                  productHandle:productHandle
                      errorCode:MA_PURCHASE_RES_INVALID_HANDLE];
    }

    [product verifyReceipt:_storeURL];
}

/**
 * Get a receipt field value.
 * @param productHandle Handle to the product containing the receipt.
 * @param fieldName The given field.
 * @param buffer Will contain the field value.
 * @param bufferSize Maximum size of the buffer.
 * @return The number of written bytes in case of success, or
 * one of the next result codes:
 * - MA_PURCHASE_RES_DISABLED if purchase is not allowed/enabled.
 * - MA_PURCHASE_RES_INVALID_HANDLE if the productHandle is invalid.
 * - MA_PURCHASE_RES_BUFFER_TOO_SMALL if the buffer is too small.
 * - MA_PURCHASE_RES_RECEIPT_NOT_AVAILABLE if the receipt has not been received or if
 * transaction is invalid.
 */
-(int) getReceiptField:(MAHandle) productHandle
             fieldName:(const char*) fieldName
                buffer:(char*) buffer
            bufferSize:(const int) bufferSize
{
    if (![SKPaymentQueue canMakePayments])
    {
        return MA_PURCHASE_RES_DISABLED;
    }

    NSNumber* key = [NSNumber numberWithInt:productHandle];
    PurchaseProduct* product = [_productsDictionary objectForKey:key];
    if (!product)
    {
        return MA_PURCHASE_RES_INVALID_HANDLE;
    }

    return [product getReceiptField:fieldName
                             buffer:buffer
                         bufferSize:bufferSize];
}

/**
 * Set the store url used for verifing the receipe.
 * @param url Store url.
 */
-(void) setStoreURL:(const char*) url
{
    [_storeURL release];
    _storeURL = [[NSString alloc] initWithUTF8String:url];
}

/**
 * Restore transactions that were previously finished so that the user can process them again.
 */
-(void) restoreTransactions
{
    [_paymentQueue restoreCompletedTransactions];
}

/**
 * Get the puchase error code from a NSError object.
 * @param error The native error object.
 * @return One of the following error codes:
 * - MA_PURCHASE_ERROR_UNKNOWN
 * - MA_PURCHASE_ERROR_NOT_ALLOWED
 * - MA_PURCHASE_ERROR_CANCELLED
 * - MA_PURCHASE_ERROR_INVALID_CLIENT
 */
-(int) purchaseErrorCode:(NSError*) error
{
    if (!error)
    {
        return MA_PURCHASE_ERROR_UNKNOWN;
    }

    int errorCode;
    switch (error.code)
    {
        case SKErrorClientInvalid:
            errorCode = MA_PURCHASE_ERROR_INVALID_CLIENT;
            break;
        case SKErrorPaymentCancelled:
            errorCode = MA_PURCHASE_ERROR_CANCELLED;
            break;
        case SKErrorPaymentNotAllowed:
            errorCode = MA_PURCHASE_ERROR_NOT_ALLOWED;
            break;
        case SKErrorUnknown:
        default:
            errorCode = MA_PURCHASE_ERROR_UNKNOWN;
            break;
    }

    return errorCode;
}

#pragma mark SKPaymentTransactionObserver methods

/**
 * Called when one or more transactions have been updated.
 * @param queue The payment queue that updated the transactions.
 * @param transactions An array of the transactions that were updated.
 */
- (void)paymentQueue:(SKPaymentQueue *)queue updatedTransactions:(NSArray *)transactions
{
    for (SKPaymentTransaction *transaction in transactions)
    {
        PurchaseProduct* product = [self productUsingPayment:transaction.payment];
        if (product)
        {
            [product updatedTransaction:transaction];
        }

        if (transaction.transactionState == SKPaymentTransactionStateRestored)
        {
            [self handleProductRestored:transaction];
        }

        // Remove the transaction from the payment queue.
        if (transaction.transactionState != SKPaymentTransactionStatePurchasing)
        {
            [_paymentQueue finishTransaction: transaction];
        }
    }
}

/**
 * Called when an error occurred while restoring transactions.
 * @param queue The payment queue that was restoring transactions.
 * @param error The error that occurred.
 */
- (void)paymentQueue:(SKPaymentQueue *)queue restoreCompletedTransactionsFailedWithError:(NSError *)error
{
    int errorCode = [self purchaseErrorCode:error];
    [self sendPurchaseEvent:MA_PURCHASE_EVENT_RESTORE
                      state:MA_PURCHASE_STATE_RESTORE_ERROR
              productHandle:MA_PURCHASE_ERROR_INVALID_HANDLE
                  errorCode:errorCode];
}

/**
 * Release all contained objects.
 */
-(void) dealloc
{
    [_productsDictionary release];
    [_storeURL release];

    [super dealloc];
}

@end

@implementation PurchaseManager (hidden)

/**
 * Send a purchase event.
 * @param type Type of the purchase event.
 *             Must be one of the MA_PURCHASE_EVENT constants.
 * @param state State of the purchase.
 * @param productHandle Product handle that sends the event.
 * @param errorCode Reason why the purchase failed.
 *                  Valid if type is MA_PURCHASE_EVENT_REQUEST_STATE_CHANGED
 *                  and state is MA_PURCHASE_STATE_FAILED.
 */
-(void) sendPurchaseEvent:(const int) type
                    state:(const int) state
            productHandle:(MAHandle) productHandle
                errorCode:(const int) errorCode
{
    MAEvent event;
	event.type = EVENT_TYPE_PURCHASE;

    MAPurchaseEventData purchaseData;
    purchaseData.type = type;
    purchaseData.state = state;
    purchaseData.productHandle = productHandle;
    purchaseData.errorCode = errorCode;

    event.purchaseData = purchaseData;
    Base::gEventQueue.put(event);
}

/**
 * Get the purchase product that contains a given payment.
 * @param payment The given payment send to Apple App Store.
 * @return The product containg the payment if found, otherwise nil.
 */
-(PurchaseProduct*) productUsingPayment:(SKPayment*) payment
{
    for (PurchaseProduct* product in [_productsDictionary allValues])
    {
        SKPayment* productPayment = [product payment];
        if (!productPayment)
        {
            continue;
        }
        if ([payment isEqual:productPayment])
        {
            return product;
        }
    }

    return nil;
}

/**
 * Check if purchase is supported on the current device.
 * If it's not a purchase event will be sent.
 * @param purchaseEventType Purchase event type. Must be one of MA_PURCHASE_EVENT constants.
 * @param productHandle Handle to the product that will send the event.
 * @return YES if the puchase is supported(the event has not been send), otherwise NO
 * (the event has been send).
 */
-(BOOL) checkPurchaseSupported:(const int) purchaseEventType
                 productHandle:(MAHandle) productHandle
{
    if (![SKPaymentQueue canMakePayments])
    {
        [self sendPurchaseEvent:purchaseEventType
                          state:MA_PURCHASE_STATE_DISABLED
                  productHandle:productHandle
                      errorCode:0];
        return NO;
    }
    return YES;
}

/**
 * Restored a product that has been purchased by the user.
 * Send purchase related event for the restored product.
 * @param transaction The transaction for the purchased product.
 */
-(void) handleProductRestored:(SKPaymentTransaction*) transaction
{
    MAHandle productHandle = maCreatePlaceholder();
    NSNumber* key = [NSNumber numberWithInt:productHandle];
    PurchaseProduct* product = [[PurchaseProduct alloc] initWithTransaction:transaction
                                                          productHandle:productHandle];
    [_productsDictionary setObject:product forKey:key];
    [self sendPurchaseEvent:MA_PURCHASE_EVENT_RESTORE
                      state:MA_PURCHASE_STATE_PRODUCT_RESTORED
              productHandle:productHandle
                  errorCode:0];
}

@end