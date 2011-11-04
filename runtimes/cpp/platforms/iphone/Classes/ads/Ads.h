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

#import <Foundation/Foundation.h>
#import <iAd/iAd.h>

#import "helpers/cpp_defs.h"

@interface Ads :  NSObject<ADBannerViewDelegate> {
    /**
     *
     */
    NSMutableDictionary* mBannerDictionary;

    /**
     *
     */
    int mHandleCount;
}

/**
 * Returns an instance to the shared singleton.
 * @return The shared object.
 */
+(Ads*) getInstance;

/**
 * Releases the class instance.
 */
+(void) deleteInstance;

/**
 * Creates a new banner.
 * @return A handle to a banner object.
 */
-(MAHandle) createBanner;

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
        toLayout:(MAHandle) layoutHandle;

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
        fromLayout:(MAHandle) layoutHandle;

/**
 * Destroy a banner.
 * @param bannerHandle Handle to a banner.
 * @return One of the next constants:
 * - MA_ADS_RES_OK if no error occurred.
 * - MA_ADS_RES_INVALID_BANNER_HANDLE if the banner handle is invalid.
 */
-(int) bannerDestroy:(MAHandle) bannerHandle;

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
                  value:(const char*) value;

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
                    size:(int) maxSize;
@end
