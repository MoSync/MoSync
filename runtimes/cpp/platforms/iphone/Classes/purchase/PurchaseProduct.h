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

/**
 * @brief MoSync product class.
 */
@interface PurchaseProduct: NSObject<SKProductsRequestDelegate>
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
}

/**
 * Create a product product using a handle and an id.
 * @param productHandle Handle for the new product.
 * @param productID Product identifier.
 * @return A product object.
 */
-(id) initWithHandle:(MAHandle) productHandle
           productID:(NSString*) productID;

@property(nonatomic, readonly) MAHandle handle;

@end
