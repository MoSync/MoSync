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
 * @file PurchaseManager.h
 * @author Bogdan Iusco
 * @date 4 Apr 2012
 *
 * @brief Design pattern: singleton.
 * Provides a general mechanism for managing purchases.
 * It communicates with the Apple App Store through Store Kit.
 */

#import <Foundation/Foundation.h>
#import <StoreKit/StoreKit.h>

#import "helpers/cpp_defs.h"

/**
 * Manager for the purchase module.
 * Contains purchase related syscall implementation.
 */
@interface PurchaseManager : NSObject<SKPaymentTransactionObserver>
{
    /**
     * Used to send payment transactions to Apple App Store.
     */
    SKPaymentQueue* _paymentQueue;

    /**
     * Count the number of created products.
     */
    int _countProducts;

    /**
     * Store the local products.
     */
    NSMutableDictionary* _productsDictionary;

    /**
     * Apple App Store url.
     */
    NSString* _storeURL;
}

/**
 * Returns an instance to the shared singleton.
 * @return The shared object.
 */
+(PurchaseManager*) getInstance;

/**
 * Releases the class instance.
 */
+(void) deleteInstance;

/**
 * Check if purchase is availble on the device.
 * @return One of the next constants:
 * - MA_PURCHASE_RES_OK if purchase is available.
 * - MA_PURCHASE_RES_DISABLED otherwise.
 */
-(int) isPurchaseSupported;

/**
 * Create a product object.
 * @param productHandle A valid handle that will be used to indetify the new product.
 * @param productID String that identifies the object.
 */
-(void) createProduct:(MAHandle) productHandle
            productID:(const char*) productID;

/**
 * Destroy a product object.
 * @param productHandle Handle to the product to destroy.
 * @return One of the following values:
 * - MA_PURCHASE_RES_OK if product has been detroyed.
 * - MA_PURCHASE_RES_INVALID_HANDLE if the productHandle is invalid.
 */
-(int) destroyProduct:(MAHandle) productHandle;

/**
 * Add a given product to the payment queue.
 * @param productHandle Handle to the product to be added.
 * @param quantity How many products to be purchased. Must be a value greater than zero.
 */
-(void) requestProduct:(MAHandle) productHandle
              quantity:(const int) quantity;

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
        bufferSize:(const int) bufferSize;

/**
 * Verify if the receipt came from App Store.
 * @param productHandle Handle to the product you want to verify its receipt.
 */
-(void) verifyReceipt:(MAHandle) productHandle;

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
            bufferSize:(const int) bufferSize;

/**
 * Set the store url used for verifing the receipe.
 * @param url Store url.
 */
-(void) setStoreURL:(const char*) url;

/**
 * Restore transactions that were previously finished so that the user can process them again.
 */
-(void) restoreTransactions;

/**
 * Get the puchase error code from a NSError object.
 * @param error The native error object.
 * @return One of the following error codes:
 * - MA_PURCHASE_ERROR_UNKNOWN
 * - MA_PURCHASE_ERROR_NOT_ALLOWED
 * - MA_PURCHASE_ERROR_CANCELLED
 * - MA_PURCHASE_ERROR_INVALID_CLIENT
 */
-(int) purchaseErrorCode:(NSError*) error;

@end
