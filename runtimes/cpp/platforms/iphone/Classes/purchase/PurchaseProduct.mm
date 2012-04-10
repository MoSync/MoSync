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
 * @file PurchaseProduct.mm
 * @author Bogdan Iusco
 * @date 4 Apr 2012
 *
 * @brief MoSync purchase product class.
 */

#import "PurchaseProduct.h"
#import "Platform.h"
#import "NSDataExpanded.h"

// Request timeout interval, in seconds.
NSTimeInterval const kTimeoutInterval = 6.0;

// POST method request.
NSString* const kRequestMethodTypePost = @"POST";

// JSON template used for verifying a receipt.
NSString* const kReceiptTemplateJSON = @"{ \"receipt-data\" : \"%@\" }";

/**
 * Hidden methods for PurchaseProduct class.
 */
@interface PurchaseProduct (hidden)

/**
 * Send a purchase event.
 * @param type Type of the purchase event.
 *             Must be one of the MA_PURCHASE_EVENT constants.
 * @param state State of the purchase.
 * @param errorCode Reason why the purchase failed.
 *                  Valid if type is MA_PURCHASE_EVENT_REQUEST_STATE_CHANGED
 *                  and state is MA_PURCHASE_STATE_FAILED.
 */
-(void) sendPurchaseEvent:(const int) type
                    state:(const int) state
                errorCode:(const int) errorCode;

/**
 * Handle a purchasing state for this product.
 * @param transaction Payment transaction received from the Apple App Store.
 */
-(void) handleTransactionStatePurchasing:(SKPaymentTransaction*) transaction;

/**
 * Handle a purchased state for this product.
 * @param transaction Payment transaction received from the Apple App Store.
 */
-(void) handleTransactionStatePurchased:(SKPaymentTransaction*) transaction;

/**
 * Handle a failed transaction.
 * @param transaction Payment transaction received from the Apple App Store.
 */
-(void) handleTransactionStateFailed:(SKPaymentTransaction*) transaction;

/**
 * Handle a restored transaction.
 * @param transaction Payment transaction received from the Apple App Store.
 */
-(void) handleTransactionStateRestored:(SKPaymentTransaction*) transaction;

@end

@implementation PurchaseProduct

@synthesize handle = _handle;

/**
 * Create a product product using a handle and an id.
 * @param productHandle Handle for the new product.
 * @param productID Product identifier.
 * @return A product object.
 */
-(id) initWithHandle:(MAHandle) productHandle
           productID:(NSString*) productID
{
    self = [super init];
    if (self)
    {
        _handle = productHandle;
        _productID = [productID retain];
        NSSet* productSet = [NSSet setWithObject:_productID];
        _productRequest = [[SKProductsRequest alloc] initWithProductIdentifiers:productSet];
        _productRequest.delegate = self;
        [_productRequest start];
        _payment = nil;
    }

    return self;
}

/**
 * Get a payment object for the product.
 * @return A payment that can be send to the Apple App Store.
 */
-(SKPayment*) payment
{
    if (!_payment)
    {
        _payment = [[SKMutablePayment paymentWithProduct:_product] retain];
        _payment.quantity = 1;
    }

    return _payment;
}

#pragma mark SKProductsRequestDelegate method
/**
 * Called when the Apple App Store responds to the product request.
 * @param request The product request sent to the Apple App Store.
 * @param response Detailed information about the list of products.
 */
- (void)productsRequest:(SKProductsRequest *)request didReceiveResponse:(SKProductsResponse *)response
{
    NSLog(@"productsRequest-didReceiveResponse");
    int validProductsCount = [response.products count];
    int invalidProductsCount = [response.invalidProductIdentifiers count];
    BOOL isValid = (validProductsCount > invalidProductsCount) ? YES : NO;

    MAEvent event;
	event.type = EVENT_TYPE_PURCHASE;

    MAPurchaseEventData purchaseData;
    purchaseData.type = isValid? MA_PURCHASE_EVENT_PRODUCT_VALID : MA_PURCHASE_EVENT_PRODUCT_INVALID;
    purchaseData.productHandle = _handle;

    event.purchaseData = purchaseData;
    Base::gEventQueue.put(event);

    // Store the product(if valid)
    if (isValid)
    {
        _product = [[response.products objectAtIndex:0] retain];
    }
}

/**
 * Called when the transaction that contains this product has been updated.
 * @param trasaction Tracsaction that has been updated.
 */
-(void) updatedTransaction:(SKPaymentTransaction*) transaction
{
    NSLog(@"updatedTransaction for %@", _productID);
    switch (transaction.transactionState)
    {
        case SKPaymentTransactionStatePurchasing:
            [self handleTransactionStatePurchasing:transaction];
            break;
        case SKPaymentTransactionStatePurchased:
            [self handleTransactionStatePurchased:transaction];
            break;
        case SKPaymentTransactionStateFailed:
            [self handleTransactionStateFailed:transaction];
            break;
        case SKPaymentTransactionStateRestored:
            [self handleTransactionStateRestored:transaction];
            break;
        default:
            NSLog(@"unhandled transaction.");
            break;
    }
}

/**
 * Get the product id.
 * @param buffer Will contain the product name.
 * @param bufferSize Maximum size of the buffer.
 * @return The number of written bytes in case of success, or
 * MA_PURCHASE_RES_BUFFER_TOO_SMALL if the buffer is too small.
 */
-(int) productName:(char*) buffer
        bufferSize:(const int) bufferSize
{
    if ([_productID length] > bufferSize)
    {
        return MA_PURCHASE_RES_BUFFER_TOO_SMALL;
    }

    [_productID getCString:buffer maxLength:bufferSize encoding:NSASCIIStringEncoding];
    return [_productID length];
}

/**
 * Verify if the receipt came from App Store.
 * @param storeURL App Store url where to send the receipt for verification.
 * @return One of the next constants:
 * - MA_PURCHASE_RES_OK if the receipt was sent to the store for verifing.
 * - MA_PURCHASE_RES_RECEIPT if the product has not been purchased.
 */
-(int) verifyReceipt:(NSString*) storeURL
{
    NSLog(@"%s for %@", __FUNCTION__, _transaction.payment.productIdentifier);
    if (!_transaction)
    {
        return MA_PURCHASE_RES_RECEIPT;
    }

    // The connection needs to be done on the main thread.
    dispatch_queue_t mainQueue = dispatch_get_main_queue();
    dispatch_async(mainQueue,^{
        NSString* base64String = [_transaction.transactionReceipt base64Encode];
        NSString* json = [NSString stringWithFormat:kReceiptTemplateJSON, base64String];

        NSMutableURLRequest *request =
        [NSMutableURLRequest requestWithURL:[NSURL URLWithString:storeURL]
                                cachePolicy:NSURLRequestReloadIgnoringLocalCacheData
                            timeoutInterval:kTimeoutInterval];

        [request setHTTPMethod:kRequestMethodTypePost];
        [request setHTTPBody: [json dataUsingEncoding:NSUTF8StringEncoding]];

        NSURLConnection* urlConnection = [[NSURLConnection alloc] initWithRequest:request delegate:self];
        [urlConnection release];
    });

    return MA_PURCHASE_RES_OK;
}

#pragma mark NSURLConnectionDelegate methods

/*!
 Sent as a connection loads data incrementally.
 @param connection The connection sending the message.
 @param data The newly available data.
 */
- (void)connection:(NSURLConnection *)connection didReceiveData:(NSData *)data
{
    NSLog(@"IN %s", __FUNCTION__);
    NSString* text = [[NSString alloc] initWithData:data encoding:NSUTF8StringEncoding];
    NSLog(@"text received: %@", text);
    [text release];
}

/*!
 Sent when a connection fails to load its request successfully.
 @param connection The connection sending the message.
 @param error An error object containing details of why the connection failed to load the request successfully.
 */
- (void)connection:(NSURLConnection *)connection didFailWithError:(NSError *)error
{
    NSLog(@"Connection failed! Error - %@ %@",
         [error localizedDescription],
         [[error userInfo] objectForKey:NSURLErrorFailingURLStringErrorKey]);
}

/**
 * Release all contained objects.
 */
-(void) dealloc
{
    [_productID release];
    [_productRequest release];
    [_product release];
    [_payment release];
    [_transaction release];

    [super dealloc];
}

@end

@implementation PurchaseProduct (hidden)

/**
 * Send a purchase event.
 * @param type Type of the purchase event.
 *             Must be one of the MA_PURCHASE_EVENT constants.
 * @param state State of the purchase.
 * @param errorCode Reason why the purchase failed.
 *                  Valid if type is MA_PURCHASE_EVENT_REQUEST_STATE_CHANGED
 *                  and state is MA_PURCHASE_STATE_FAILED.
 */
-(void) sendPurchaseEvent:(const int) type
                    state:(const int) state
                errorCode:(const int) errorCode
{
    MAEvent event;
	event.type = EVENT_TYPE_PURCHASE;

    MAPurchaseEventData purchaseData;
    purchaseData.type = type;
    purchaseData.state = state;
    purchaseData.productHandle = _handle;
    purchaseData.errorCode = errorCode;

    event.purchaseData = purchaseData;
    Base::gEventQueue.put(event);
}

/**
 * Handle a purchasing state for this product.
 * @param transaction Payment transaction received from the Apple App Store.
 */
-(void) handleTransactionStatePurchasing:(SKPaymentTransaction*) transaction
{
    [self sendPurchaseEvent:MA_PURCHASE_EVENT_REQUEST_STATE_CHANGED
                      state:MA_PURCHASE_STATE_IN_PROGRESS
                  errorCode:0];
}

/**
 * Handle a purchased state for this product.
 * @param transaction Payment transaction received from the Apple App Store.
 */
-(void) handleTransactionStatePurchased:(SKPaymentTransaction*) transaction
{
    _transaction = [transaction retain];
    [self sendPurchaseEvent:MA_PURCHASE_EVENT_REQUEST_STATE_CHANGED
                      state:MA_PURCHASE_STATE_COMPLETED
                  errorCode:0];
}

/**
 * Handle a failed transaction.
 * @param transaction Payment transaction received from the Apple App Store.
 */
-(void) handleTransactionStateFailed:(SKPaymentTransaction*) transaction
{
    [self sendPurchaseEvent:MA_PURCHASE_EVENT_REQUEST_STATE_CHANGED
                      state:MA_PURCHASE_STATE_FAILED
                  errorCode:0];
}

/**
 * Handle a restored transaction.
 * @param transaction Payment transaction received from the Apple App Store.
 */
-(void) handleTransactionStateRestored:(SKPaymentTransaction*) transaction
{
    [self sendPurchaseEvent:MA_PURCHASE_EVENT_RESTORED
                      state:MA_PURCHASE_STATE_COMPLETED
                  errorCode:0];
}

@end