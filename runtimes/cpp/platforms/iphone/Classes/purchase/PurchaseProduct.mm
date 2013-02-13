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
#import "PurchaseManager.h"
#import "NSDataExpanded.h"
#import "Platform.h"
#include "Syscall.h"

// Request timeout interval, in seconds.
NSTimeInterval const kTimeoutInterval = 6.0;

// POST method request.
NSString* const kRequestMethodTypePost = @"POST";

// JSON template used for verifying a receipt.
NSString* const kReceiptTemplateJSON = @"{ \"receipt-data\" : \"%@\" }";

// Tags for the confirmation receipt received from Apple App Store.
NSString* const kReceiptResponseStatusKey = @"status";
NSString* const kReceiptResponseReceiptKey = @"receipt";

// Get the date of the purchase in milliseconds.
NSString* const kReceiptDateMsKey = @"original_purchase_date_ms";

// Status code for a valid receipt.
#define RECEIPT_STATUS_CODE_OK [NSNumber numberWithInt:0]

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
 * Create JSON parser components.
 */
-(void) createParserComponents;

/**
 * The JSON receipt has been received and parsed.
 * Notify the user about it.
 * @param storeReceipt Response from the Apple App Store.
 */
-(void) handleReceiptResponse:(NSDictionary*) storeResponse;

/**
 * Get a field value from the product.
 * @param fieldName One of the following values:
 * - MA_PURCHASE_RECEIPT_PRICE
 * - MA_PURCHASE_RECEIPT_TITLE
 * - MA_PURCHASE_RECEIPT_DESCRIPTION
 * @return The field value or nil in case of any error.
 */
-(NSString*) getProductField:(NSString*) fieldName;

@end

@implementation PurchaseProduct

@synthesize handle = _handle;
@synthesize isPurchased = _isPurchased;

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
        _payment = nil;
        _restoredPayment = nil;
        _handle = productHandle;
        _productID = [productID retain];
        _isPurchased = NO;
        NSSet* productSet = [NSSet setWithObject:_productID];
        _productRequest = [[SKProductsRequest alloc] initWithProductIdentifiers:productSet];
        _productRequest.delegate = self;
        [_productRequest start];

        [self createParserComponents];
    }

    return self;
}

/**
 * Create a product product using a payment transaction.
 * Used for restored products.
 * @param transaction The transaction.
 * @param productHandle Handle for the new product.
 * @return A product object.
 */
-(id) initWithTransaction:(SKPaymentTransaction*) transaction
            productHandle:(MAHandle) productHandle
{
    self = [super init];
    if (self)
    {
        _handle = productHandle;
        _restoredPayment = [transaction.payment retain];
        _productID = [_restoredPayment.productIdentifier retain];
        _transaction = [transaction retain];
        _product = nil;
        _isPurchased = YES;
        [self createParserComponents];
    }

    return self;
}

/**
 * Get a payment object for the product.
 * @return A payment that can be send to the Apple App Store.
 */
-(SKMutablePayment*) payment
{
    if (!_payment && _product)
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
    int validProductsCount = [response.products count];
    int invalidProductsCount = [response.invalidProductIdentifiers count];
    BOOL isValid = (validProductsCount > invalidProductsCount) ? YES : NO;

    MAEvent event;
	event.type = EVENT_TYPE_PURCHASE;

    MAPurchaseEventData purchaseData;
    purchaseData.type = MA_PURCHASE_EVENT_PRODUCT_CREATE;
    purchaseData.state = isValid? MA_PURCHASE_STATE_PRODUCT_VALID : MA_PURCHASE_STATE_PRODUCT_INVALID;
    purchaseData.productHandle = _handle;

    event.purchaseData = purchaseData;
    Base::gEventQueue.put(event);

    // Store the product(if valid)
    if (isValid)
    {
        _product = [[response.products objectAtIndex:0] retain];
        _payment = [[SKMutablePayment paymentWithProduct:_product] retain];
        _payment.quantity = 1;
    }
}

/**
 * Called when the transaction that contains this product has been updated.
 * @param trasaction Tracsaction that has been updated.
 */
-(void) updatedTransaction:(SKPaymentTransaction*) transaction
{
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

    [_productID getCString:buffer maxLength:bufferSize encoding:NSUTF8StringEncoding];
    return [_productID length];
}

/**
 * Verify if the receipt came from App Store.
 * @param storeURL App Store url where to send the receipt for verification.
 */
-(void) verifyReceipt:(NSString*) storeURL
{
    if (!_transaction)
    {
        [self sendPurchaseEvent:MA_PURCHASE_EVENT_VERIFY_RECEIPT
                          state:MA_PURCHASE_STATE_RECEIPT_ERROR
                      errorCode:MA_PURCHASE_ERROR_NO_RECEIPT];
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
}

/**
 * Get a receipt field value.
 * @param fieldName The given field.
 * @param buffer Will contain the field value.
 * @param bufferSize Maximum size of the buffer.
 * @return The number of written bytes in case of success, or
 * one of the next result codes:
 * - MA_PURCHASE_RES_BUFFER_TOO_SMALL if the buffer is too small.
 * - MA_PURCHASE_RES_RECEIPT_NOT_AVAILABLE if the receipt has not been received or if
 * transaction is invalid.
 */
-(int) getReceiptField:(const char*) fieldName
                buffer:(char*) buffer
            bufferSize:(const int) bufferSize
{
    if (!_validationResponse || !_product)
    {
        return MA_PURCHASE_RES_RECEIPT_NOT_AVAILABLE;
    }
    NSString* key = [NSString stringWithUTF8String:fieldName];
    NSString* fieldValue = [self getProductField:key];
    if (!fieldValue)
    {
        fieldValue = [_validationResponse objectForKey:key];
        // If the required field is the date will use the milliseconds field value
        // and transform it to seconds.
        if ([key isEqualToString:@MA_PURCHASE_RECEIPT_PURCHASE_DATE])
        {
            fieldValue = [_validationResponse objectForKey:kReceiptDateMsKey];
            // Convert date from milliseconds to seconds.
            int substringLength = fieldValue.length - 3;
            fieldValue = [fieldValue substringToIndex:substringLength];
        }
    }

    if (!fieldValue)
    {
        return MA_PURCHASE_RES_INVALID_FIELD_NAME;
    }

    int fieldValueLength = [fieldValue length];
    if (fieldValueLength > bufferSize)
    {
        return MA_PURCHASE_RES_BUFFER_TOO_SMALL;
    }

    [fieldValue getCString:buffer maxLength:bufferSize encoding:NSUTF8StringEncoding];
    return fieldValueLength;
}

#pragma mark NSURLConnectionDelegate methods

/**
 * Sent as a connection loads data incrementally.
 * @param connection The connection sending the message.
 * @param data The newly available data.
 */
- (void)connection:(NSURLConnection *)connection didReceiveData:(NSData *)data
{
    // Parse the JSON.
	[_streamParser parse:data];
}

/**
 * Sent when a connection fails to load its request successfully.
 * @param connection The connection sending the message.
 * @param error An error object containing details of why the connection failed to load the request successfully.
 */
- (void)connection:(NSURLConnection *)connection didFailWithError:(NSError *)error
{
    NSLog(@"IN %s Connection failed! Error - %@ %@",
          __FUNCTION__,
         [error localizedDescription],
         [[error userInfo] objectForKey:NSURLErrorFailingURLStringErrorKey]);
    [self sendPurchaseEvent:MA_PURCHASE_EVENT_VERIFY_RECEIPT
                      state:MA_PURCHASE_STATE_RECEIPT_ERROR
                  errorCode:MA_PURCHASE_ERROR_CONNECTION_FAILED];
}

#pragma mark SBJsonStreamParserAdapterDelegate methods

/**
 * Called when a JSON object is found.
 * This method is called if a JSON object is found.
 */
- (void)parser:(SBJsonStreamParser*)parser foundObject:(NSDictionary*)dict
{
    [self handleReceiptResponse:dict];
}

/**
 * Called if a JSON array is found
 * This method is called if a JSON array is found.
 */
- (void)parser:(SBJsonStreamParser*)parser foundArray:(NSArray*)array
{
    // The respone should contain an object and not an array.
    [self sendPurchaseEvent:MA_PURCHASE_EVENT_VERIFY_RECEIPT
                      state:MA_PURCHASE_STATE_RECEIPT_ERROR
                  errorCode:MA_PURCHASE_ERROR_CANNOT_PARSE_RECEIPT];
}

/**
 * Release all contained objects.
 */
-(void) dealloc
{
    // If the product was restored destroy the placeholder.
    if (_restoredPayment)
    {
        maDestroyPlaceholder(_handle);
    }

    [_productID release];
    [_productRequest release];
    [_product release];
    [_payment release];
    [_transaction release];
    [_streamParser release];
    [_parserAdapter release];
    [_validationResponse release];
    [_restoredPayment release];

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
    [self sendPurchaseEvent:MA_PURCHASE_EVENT_REQUEST
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
    _isPurchased = YES;
    [self sendPurchaseEvent:MA_PURCHASE_EVENT_REQUEST
                      state:MA_PURCHASE_STATE_COMPLETED
                  errorCode:0];
}

/**
 * Handle a failed transaction.
 * @param transaction Payment transaction received from the Apple App Store.
 */
-(void) handleTransactionStateFailed:(SKPaymentTransaction*) transaction
{
    PurchaseManager* manager = [PurchaseManager getInstance];
    int errorCode = [manager purchaseErrorCode:transaction.error];
    [self sendPurchaseEvent:MA_PURCHASE_EVENT_REQUEST
                      state:MA_PURCHASE_STATE_FAILED
                  errorCode:errorCode];
}

/**
 Create JSON parser components
 */
-(void) createParserComponents
{
    _parserAdapter = [[SBJsonStreamParserAdapter alloc] init];

	// Set ourselves as the delegate, so we receive the messages
	// from the adapter.
	_parserAdapter.delegate = self;

	// Create a new stream parser..
	_streamParser = [[SBJsonStreamParser alloc] init];
    _streamParser.delegate = _parserAdapter;

    // Normally it's an error if JSON is followed by anything but
	// whitespace. Setting this means that the parser will be
	// expecting the stream to contain multiple whitespace-separated
	// JSON documents.
	_streamParser.supportMultipleDocuments = YES;
}

/**
 * The JSON receipt has been received and parsed.
 * Notify the user about it.
 * @param storeReceipt Response from the Apple App Store.
 */
-(void) handleReceiptResponse:(NSDictionary*) storeResponse
{
    NSNumber* statusCode = [storeResponse objectForKey:kReceiptResponseStatusKey];
    if (!statusCode)
    {
        [self sendPurchaseEvent:MA_PURCHASE_EVENT_VERIFY_RECEIPT
                          state:MA_PURCHASE_STATE_RECEIPT_ERROR
                      errorCode:MA_PURCHASE_ERROR_CANNOT_PARSE_RECEIPT];
        return;
    }
    if ([statusCode isEqualToNumber:RECEIPT_STATUS_CODE_OK])
    {
        [self sendPurchaseEvent:MA_PURCHASE_EVENT_VERIFY_RECEIPT
                          state:MA_PURCHASE_STATE_RECEIPT_VALID
                      errorCode:0];
        [_validationResponse release];
        NSDictionary* receiptDict = [storeResponse objectForKey:kReceiptResponseReceiptKey];
        if (receiptDict)
        {
            _validationResponse = [receiptDict retain];
        }
    }
    else
    {
        [self sendPurchaseEvent:MA_PURCHASE_EVENT_VERIFY_RECEIPT
                          state:MA_PURCHASE_STATE_RECEIPT_INVALID
                      errorCode:0];
    }
}

/**
 * Get a field value from the product.
 * @param fieldName One of the following values:
 * - MA_PURCHASE_RECEIPT_PRICE
 * - MA_PURCHASE_RECEIPT_TITLE
 * - MA_PURCHASE_RECEIPT_DESCRIPTION
 * @return The field value or nil in case of any error.
 */
-(NSString*) getProductField:(NSString*) fieldName
{
    if (!_product)
    {
        return nil;
    }
    if ([fieldName isEqualToString:@MA_PURCHASE_RECEIPT_PRICE])
    {
        return [NSString stringWithFormat:@"%@",_product.price];
    }
    else if ([fieldName isEqualToString:@MA_PURCHASE_RECEIPT_TITLE])
    {
        return _product.localizedTitle;
    }
    else if ([fieldName isEqualToString:@MA_PURCHASE_RECEIPT_DESCRIPTION])
    {
        return _product.localizedDescription;
    }
    return nil;
}

@end