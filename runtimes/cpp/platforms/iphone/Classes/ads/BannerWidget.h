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
 * @file BannerWidget.h
 * @author Bogdan Iusco
 * @date 1 Nov 2011
 *
 * @brief BannerWidget represents a MoSync widget used for displaying
 * advertisments. Advertisements are downloaded from iAd Network.
 * It uses the view from a ADBannerView object and adds it to
 * the super class(IWidget).
 */

#import <Foundation/Foundation.h>
#import <iAd/iAd.h>

#import "IWidget.h"
#import "helpers/cpp_defs.h"

/**
 * @brief BannerWidget represents a MoSync widget used for displaying
 * advertisments. Advertisements are downloaded from iAd Network.
 * It uses the view from a ADBannerView object and adds it to
 * the super class(IWidget).
 */
@interface BannerWidget : IWidget<ADBannerViewDelegate> {

    /**
     * Identifies the banner widget.
     */
    MAHandle mBannerHandle;
}

/**
 * Init function.
 * @param handle The handle that will be associated with the returned object.
 */
- (id)initWithHandle:(MAHandle) handle;

/**
 * Sets a banner property.
 * @param key A string representing which property to set.
 * @param value The value that will be assigned to the property.
 * @return One of the next result codes:
 * - MA_ADS_RES_OK if no error occurred.
 * - MA_ADS_RES_INVALID_PROPERTY_NAME if the property name is not valid.
 * - MA_ADS_RES_INVALID_PROPERTY_VALUE if the property value is not valid.
 */
- (int)setPropertyWithKey: (NSString*)key toValue: (NSString*)value;

/**
 * Retrieves a specified property value.
 * @param key A string representing for which property to get the value.
 * @return The value for the given property, or nil if the property is invalid.
 * The ownership of the returned object is passed to the caller.
 */
- (NSString*)getPropertyWithKey: (NSString*)key;

@end
