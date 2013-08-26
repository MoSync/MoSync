/* Copyright 2013 David Axmark

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

	http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

/**
 * @file Ads.h
 * @author Bogdan Iusco
 * @date 1 Nov 2011
 *
 * @brief Design pattern: singleton.
 * Provide functions for creating and deleting banner widgets.
 * Store banner widgets.
 */

#import <Foundation/Foundation.h>
#import <iAd/iAd.h>

#import "helpers/cpp_defs.h"

/**
 * @brief Design pattern: singleton.
 * Provide functions for creating and deleting banner widgets.
 * Store banner widgets.
 */
@interface Ads : NSObject {
    /**
     * Stores pairs of:
     * - BannerWidget type objects(the object).
     * - NSNumber type objects(the key).
     */
    NSMutableDictionary* mBannerDictionary;

    /**
     * Counts the number of created banner widgets.
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
 * @return
 *  - MA_ADS_RES_UNSUPPORTED if ads are not supported on current system.
 *  - MA_ADS_RES_ERROR if a error occurred while creating the banner widget.
 *  - a handle to a new banner widget(the handle value is >= 0).
 */
-(MAHandle) createBanner;

/**
 * Destroy a banner.
 * @param bannerHandle Handle to a banner.
 * @return One of the next constants:
 * - MA_ADS_RES_OK if no error occurred.
 * - MA_ADS_RES_INVALID_BANNER_HANDLE if the banner handle is invalid.
 */
-(int) bannerDestroy:(MAHandle) bannerHandle;

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
