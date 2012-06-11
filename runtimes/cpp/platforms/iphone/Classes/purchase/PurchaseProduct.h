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
 * @file PurchaseProduct.h
 * @author Bogdan Iusco
 * @date 4 Apr 2012
 *
 * @brief MoSync purchase product class.
 */

#import <Foundation/Foundation.h>
#import <StoreKit/StoreKit.h>

#import "helpers/cpp_defs.h"
#import "SBJson.h"

/**
 * @brief MoSync product class.
 */
@interface PurchaseProduct: NSObject<SKProductsRequestDelegate,
                                     SBJsonStreamParserAdapterDelegate>
{
    /**
     * Local handle.
     */
    MAHandle _handle;

    /**
     * Product identifier.
     */
    NSString* _productID;

    /**
     * Used to verify if the product is available in the Apple App Store.
     */
    SKProductsRequest* _productRequest;

    /**
     * Product received from Apple App Store.
     * If nil than the product is invalid.
     */
    SKProduct* _product;

    /**
     * Payment that will be send to the Apple App Store.
     */
    SKMutablePayment* _payment;

    /**
     * Payment for a restored product.
     */
    SKPayment* _restoredPayment;

    /**
     * Valid only if the product has been purchased or restored.
     */
    SKPaymentTransaction* _transaction;

    /**
     * Used for parsing JSON documents.
     */
    SBJsonStreamParser* _streamParser;

    /**
     * Used for receiving messages through SBJsonStreamParserAdapterDelegate.
     */
    SBJsonStreamParserAdapter* _parserAdapter;

    /**
     * Validation response received from Apple App Store.
     */
    NSDictionary* _validationResponse;
}

/**
 * Create a product product using a handle and an id.
 * @param productHandle Handle for the new product.
 * @param productID Product identifier.
 * @return A product object.
 */
-(id) initWithHandle:(MAHandle) productHandle
           productID:(NSString*) productID;

/**
 * Create a product product using a payment transaction.
 * Used for restored products.
 * @param transaction The transaction.
 * @param productHandle Handle for the new product.
 * @return A product object.
 */
-(id) initWithTransaction:(SKPaymentTransaction*) transaction
        productHandle:(MAHandle) productHandle;

/**
 * Get a payment object for the product.
 * @return A payment that can be send to the Apple App Store.
 */
-(SKMutablePayment*) payment;

/**
 * Called when the transaction that contains this product has been updated.
 * @param trasaction Tracsaction that has been updated.
 */
-(void) updatedTransaction:(SKPaymentTransaction*) transaction;

/**
 * Get the product id.
 * @param buffer Will contain the product name.
 * @param bufferSize Maximum size of the buffer.
 * @return The number of written bytes in case of success, or
 * MA_PURCHASE_RES_BUFFER_TOO_SMALL if the buffer is too small.
 */
-(int) productName:(char*) buffer
        bufferSize:(const int) bufferSize;

/**
 * Verify if the receipt came from App Store.
 * @param storeURL App Store url where to send the receipt for verification.
 */
-(void) verifyReceipt:(NSString*) storeURL;

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
            bufferSize:(const int) bufferSize;

@property(nonatomic, readonly) MAHandle handle;

@end
