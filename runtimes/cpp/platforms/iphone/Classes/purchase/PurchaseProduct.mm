//
//  PurchaseProduct.m
//  MoSync
//
//  Created by Bogdan Iusco on 4/4/12.
//  Copyright (c) 2012 MoSync. All rights reserved.
//

#import "PurchaseProduct.h"

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
    }

    return self;
}

/**
 * Release all contained objects.
 */
-(void) dealloc
{
    [_productID release];

    [super dealloc];
}
@end
