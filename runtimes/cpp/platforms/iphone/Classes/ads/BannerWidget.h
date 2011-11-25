/*
 Copyright (C) 2011 MoSync AB

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
