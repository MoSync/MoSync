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
        _payment = [SKMutablePayment paymentWithProduct:_product];
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
        _product = [response.products objectAtIndex:0];
    }
}

/**
 * Release all contained objects.
 */
-(void) dealloc
{
    [_productID release];
    [_productRequest release];

    [super dealloc];
}

@end
