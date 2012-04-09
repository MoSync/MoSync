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
 * @param productID String that identifies the object.
 * @return MA_PURCHASE_RES_INVALID if the productID string is empty or a handle
 * to the product object.
 */
-(MAHandle) createProduct:(const char*) productID
{
    NSString* productIDString = [NSString stringWithUTF8String:productID];
    if ([productIDString length] == 0)
    {
        return MA_PURCHASE_RES_INVALID;
    }

    NSNumber* key = [NSNumber numberWithInt:_countProducts];
    PurchaseProduct* product = [[PurchaseProduct alloc] initWithHandle:[key intValue]
                                                             productID:productIDString];
    [_productsDictionary setObject:product forKey:key];
    _countProducts++;

    return [key intValue];
}

/**
 * Destroy a product object.
 * @param productHandle Handle to the product to destroy.
 * If the given handle is invalid the method does nothing.
 */
-(void) destroyProduct:(MAHandle) productHandle
{
    NSNumber* key = [NSNumber numberWithInt:productHandle];
    PurchaseProduct* product = [_productsDictionary objectForKey:key];
    if (product)
    {
        [_productsDictionary removeObjectForKey:key];
        [product release];
    }
}

/**
 * Add a given product to the payment queue.
 * @param productHandle Handle to the product to be added.
 */
-(void) requestProduct:(MAHandle) productHandle
{
    NSNumber* key = [NSNumber numberWithInt:productHandle];
    PurchaseProduct* product = [_productsDictionary objectForKey:key];
    if (!product)
    {
        [self sendPurchaseEvent:MA_PURCHASE_EVENT_REQUEST_STATE_CHANGED
                          state:MA_PURCHASE_STATE_FAILED
                  productHandle:productHandle
                      errorCode:MA_PURCHASE_ERROR_INVALID_HANDLE];
        return;
    }

    SKPayment* payment = [product payment];
    [_paymentQueue addPayment:payment];
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
 * @return One of the next constants:
 * - MA_PURCHASE_RES_OK if the receipt was sent to the store for verifing.
 * - MA_PURCHASE_RES_INVALID_HANDLE if productHandle is invalid.
 * - MA_PURCHASE_RES_RECEIPT if the product has not been purchased.
 */
-(int) verifyReceipt:(MAHandle) productHandle
{
    NSNumber* key = [NSNumber numberWithInt:productHandle];
    PurchaseProduct* product = [_productsDictionary objectForKey:key];
    if (!product)
    {
        return MA_PURCHASE_RES_INVALID_HANDLE;
    }

    return [product verifyReceipt];
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

#pragma mark SKPaymentTransactionObserver methods

/**
 * Called when one or more transactions have been updated.
 * @param queue The payment queue that updated the transactions.
 * @param transactions An array of the transactions that were updated.
 */
- (void)paymentQueue:(SKPaymentQueue *)queue updatedTransactions:(NSArray *)transactions
{
    NSLog(@"updatedTransactions");
    for (SKPaymentTransaction *transaction in transactions)
    {
        PurchaseProduct* product = [self productUsingPayment:transaction.payment];
        if (product)
        {
            [product updatedTransaction:transaction];
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
    NSLog(@"restoreCompletedTransactionsFailedWithError");
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
    NSLog(@"productUsingPayment");
    for (PurchaseProduct* product in [_productsDictionary allValues])
    {
        SKPayment* productPayment = [product payment];
        if ([payment isEqual:productPayment])
        {
            return product;
        }
    }

    return nil;
}

@end