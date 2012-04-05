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
 * @param productID String that identifies the object.
 * @return MA_PURCHASE_RES_INVALID if the productID string is empty or a handle
 * to the product object.
 */
-(MAHandle) createProduct:(const char*) productID;

/**
 * Destroy a product object.
 * @param productHandle Handle to the product to destroy.
 * If the given handle is invalid the method does nothing.
 */
-(void) destroyProduct:(MAHandle) productHandle;

/**
 * Add a given product to the payment queue.
 * @param productHandle Handle to the product to be added.
 */
-(void) requestProduct:(MAHandle) productHandle;

@end
