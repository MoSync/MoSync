/* Copyright (C) 2011 MoSync AB

 This program is free software; you can redistribute it and/or modify it under
 the terms of the GNU General Public License, version 2, as published by
 the Free Software Foundation.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 for more details.

 You should have received a copy of the GNU General Public License
 along with this program; see the file COPYING.  If not, write to the Free
 Software Foundation, 59 Temple Place - Suite 330, Boston, MA
 02111-1307, USA.
 */

/**
 * @file Ads.mm
 * @author Bogdan Iusco
 * @date 1 Nov 2011
 *
 * @brief Design pattern: singleton.
 * Provide functions for creating and deleting banner widgets.
 * Store banner widgets.
 */

#import "Ads.h"

#import "IWidget.h"
#import "MoSyncUI.h"
#import "MoSyncUISyscalls.h"
#import "BannerWidget.h"
#import "NSObjectExpanded.h"

@implementation Ads

static Ads *sharedInstance = nil;

/**
 * Returns an instance to the shared singleton.
 * @return The shared object.
 */
+(Ads*) getInstance
{
    if (nil == sharedInstance)
    {
        sharedInstance = [[Ads alloc] init];
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

- (id)init
{
    mHandleCount = 0;
    mBannerDictionary = [[NSMutableDictionary alloc] init];

    return [super init];
}

/**
 * Creates a new banner.
 * @return
 *  - MA_ADS_RES_UNSUPPORTED if ads are not supported on current system.
 *  - MA_ADS_RES_ERROR if a error occurred while creating the banner widget.
 *  - a handle to a new banner widget(the handle value is >= 0).
 */
-(MAHandle) createBanner
{
    if (INT32_MAX == mHandleCount)
    {
        return MA_ADS_RES_ERROR;
    }
    mHandleCount++;

    // Check if ads are supported on current platform.
    Class cls = NSClassFromString(@"ADBannerView");
    if (cls)
    {
        [self performSelectorOnMainThread:@selector(createBannerMainThread:)
                               withObject:[NSNumber numberWithInt:mHandleCount]
                            waitUntilDone:YES];
    }
    else
    {
        return MA_ADS_RES_UNSUPPORTED;
    }

    return mHandleCount;
}

/**
 * Create a new BannerWidget type object.
 * Call this method only on main thread.
 */
-(void) createBannerMainThread:(NSNumber*) handle
{
    BannerWidget* widget = [[BannerWidget alloc] initWithHandle:[handle intValue]];
    [mBannerDictionary setObject:widget forKey:[NSNumber numberWithInt:mHandleCount]];
}

/**
 * Destroy a banner.
 * @param bannerHandle Handle to a banner.
 * @return One of the next constants:
 * - MA_ADS_RES_OK if no error occurred.
 * - MA_ADS_RES_INVALID_BANNER_HANDLE if the banner handle is invalid.
 */
-(int) bannerDestroy:(MAHandle) bannerHandle
{
    IWidget* widget = [mBannerDictionary objectForKey:[NSNumber numberWithInt:bannerHandle]];
    if (!widget)
    {
        return MA_ADS_RES_INVALID_BANNER_HANDLE;
    }
    [mBannerDictionary removeObjectForKey:[NSNumber numberWithInt:bannerHandle]];
    [widget release];

    return MA_ADS_RES_OK;
}

/**
 * Add a banner to a widget layout.
 * @param bannerHandle Handle to a banner.
 * @param layoutHandle Handle to a widget layout.
 * @return One of the next constants:
 * - MA_ADS_RES_OK if no error occurred.
 * - MA_ADS_RES_INVALID_BANNER_HANDLE if the banner handle is invalid.
 * - MA_ADS_RES_INVALID_LAYOUT_HANDLE if the layout handle is invalid.
 */
-(int) addBanner:(MAHandle) bannerHandle
        toLayout:(MAHandle) layoutHandle
{
    NSArray* args = [NSArray arrayWithObjects:[NSNumber numberWithInt:bannerHandle],
                     [NSNumber numberWithInt:layoutHandle],nil];
    int returnValue;
    [NSObject performSelectorOnMainThread:@selector(addBannerMainThread:toLayout:)
							   withTarget:self
							  withObjects:args
							waitUntilDone:YES
						   andReturnValue:&returnValue];

    return returnValue;
}

/**
 * Add a banner to a widget layout.
 * Call this method only on main thread.
 * @param bannerHandle Handle to a banner.
 * @param layoutHandle Handle to a widget layout.
 * @return One of the next constants:
 * - MA_ADS_RES_OK if no error occurred.
 * - MA_ADS_RES_INVALID_BANNER_HANDLE if the banner handle is invalid.
 * - MA_ADS_RES_INVALID_LAYOUT_HANDLE if the layout handle is invalid.
 */
-(int) addBannerMainThread:(NSNumber*) bannerHandle
                  toLayout:(NSNumber*) layoutHandle
{
    IWidget* widget = [mBannerDictionary objectForKey:bannerHandle];
    if (!widget)
    {
        return MA_ADS_RES_INVALID_BANNER_HANDLE;
    }

    MoSyncUI* moSyncUI = getMoSyncUI();
    IWidget* layoutWidget = [moSyncUI getWidget:[layoutHandle intValue]];
    if (!layoutWidget)
    {
        return MA_ADS_RES_INVALID_LAYOUT_HANDLE;
    }

    [layoutWidget addChild:widget];
    return MA_ADS_RES_OK;
}

/**
 * Remove a banner from a widget layout.
 * @param bannerHandle Handle to a banner.
 * @param layoutHandle Handle to a widget layout.
 * @return One of the next constants:
 * - MA_ADS_RES_OK if no error occurred.
 * - MA_ADS_RES_INVALID_BANNER_HANDLE if the banner handle is invalid.
 * - MA_ADS_RES_INVALID_LAYOUT_HANDLE if the layout handle is invalid.
 */
-(int) removeBanner:(MAHandle) bannerHandle
         fromLayout:(MAHandle) layoutHandle
{
    NSArray* args = [NSArray arrayWithObjects:[NSNumber numberWithInt:bannerHandle],
                     [NSNumber numberWithInt:layoutHandle],nil];
    int returnValue;
    [NSObject performSelectorOnMainThread:@selector(removeBannerMainThread:fromLayout:)
							   withTarget:self
							  withObjects:args
							waitUntilDone:YES
						   andReturnValue:&returnValue];

    return returnValue;
}

/**
 * Remove a banner from a widget layout.
 * Call this method only on main thread.
 * @param bannerHandle Handle to a banner.
 * @param layoutHandle Handle to a widget layout.
 * @return One of the next constants:
 * - MA_ADS_RES_OK if no error occurred.
 * - MA_ADS_RES_INVALID_BANNER_HANDLE if the banner handle is invalid.
 * - MA_ADS_RES_INVALID_LAYOUT_HANDLE if the layout handle is invalid.
 */
-(int) removeBannerMainThread:(NSNumber*) bannerHandle
                  fromLayout:(NSNumber*) layoutHandle
{
    IWidget* widget = [mBannerDictionary objectForKey:bannerHandle];
    if (!widget)
    {
        return MA_ADS_RES_INVALID_BANNER_HANDLE;
    }

    MoSyncUI* moSyncUI = getMoSyncUI();
    IWidget* layoutWidget = [moSyncUI getWidget:[layoutHandle intValue]];
    if (!layoutWidget)
    {
        return MA_ADS_RES_INVALID_LAYOUT_HANDLE;
    }

    [layoutWidget removeChild:widget];
    return MA_ADS_RES_OK;
}

/**
 * Set a banner property.
 * @param bannerHandle Handle to a banner.
 * @param property A string representing which property to set.
 * @param value The value that will be assigned to the property.
 * @return One of the next constants:
 * - MA_ADS_RES_OK if no error occurred.
 * - MA_ADS_RES_INVALID_BANNER_HANDLE if the banner handle is invalid.
 * - MA_ADS_RES_INVALID_PROPERTY_NAME if the property name is not valid.
 * - MA_ADS_RES_INVALID_PROPERTY_VALUE if the property value is not valid.
 */
-(int)bannerSetProperty:(MAHandle) bannerHandle
               property:(const char*) propertyName
                  value:(const char*) value
{
    IWidget* widget = [mBannerDictionary objectForKey:[NSNumber numberWithInt:bannerHandle]];
    if (!widget)
    {
        return MA_ADS_RES_INVALID_BANNER_HANDLE;
    }

    int returnValue;
    NSArray* args = [NSArray arrayWithObjects: [NSString stringWithUTF8String:propertyName],
                                               [NSString stringWithUTF8String:value], nil];
    [NSObject performSelectorOnMainThread:@selector(setPropertyWithKey:toValue:)
							   withTarget:widget
							  withObjects:args
							waitUntilDone:YES
						   andReturnValue:&returnValue];

    return returnValue;
}


/**
 * Retrieves a specified property from the given banner.
 * @param bannerHandle Handle to the banner.
 * @param property A string representing for which property to get the value.
 * @param value A buffer that will hold the value of the property, represented as a string.
 * @param bufSize Size of the buffer.
 * @return One of the next result codes:
 * - MA_ADS_RES_OK if no error occurred.
 * - MA_ADS_RES_INVALID_BANNER_HANDLE if the banner handle is invalid.
 * - MA_ADS_RES_INVALID_PROPERTY_NAME if the property name is not valid.
 * - MA_ADS_RES_INVALID_STRING_BUFFER_SIZE if the buffer size was to small.
 */
-(int) bannerGetProperty:(MAHandle) bannerHandle
                property:(const char*) property
                   value:(char*) value
                    size:(int) maxSize
{
    IWidget* widget = [mBannerDictionary objectForKey:[NSNumber numberWithInt:bannerHandle]];
    if (!widget)
    {
        return MA_ADS_RES_INVALID_BANNER_HANDLE;
    }

    if(!property)
    {
        return MA_ADS_RES_INVALID_PROPERTY_NAME;
    }

	NSString* propertyString = [NSString stringWithUTF8String:property];
	NSString* retval;
    NSArray* arguments = [NSArray arrayWithObjects:propertyString, nil];

	[NSObject performSelectorOnMainThread:@selector(getPropertyWithKey:)
							   withTarget:widget
							  withObjects:arguments
							waitUntilDone:YES
						   andReturnValue:&retval];

	if(!retval)
    {
        NSLog(@"Ads::bannerSetProperty invalid property name");
        return MA_ADS_RES_INVALID_PROPERTY_NAME;
    }

	int length = maxSize;
	int realLength = [retval length];
	if(realLength > length)
    {
        NSLog(@"Ads::bannerSetProperty buffer too small");
        [retval release];
		return MA_ADS_RES_INVALID_STRING_BUFFER_SIZE;
	}

	[retval getCString:value maxLength:length encoding:NSASCIIStringEncoding];
	[retval release];

	return realLength;
}

/**
 * Release all contained objects.
 */
-(void) dealloc
{
    [mBannerDictionary release];
    [super dealloc];
}

@end
