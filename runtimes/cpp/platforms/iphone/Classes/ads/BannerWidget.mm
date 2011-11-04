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

// Default banner size for a ADBannerView type object.
#define BANNER_HEIGHT 50
#define BANNER_WIDTH 320

#import "BannerWidget.h"


@implementation BannerWidget


/**
 * Init function.
 * @param handle The handle that will be associated with the returned object.
 */
- (id)initWithHandle:(MAHandle) bannerHandle
{
    if (!view)
    {
        ADBannerView* bannerView = [[ADBannerView alloc] initWithFrame:CGRectZero];
        bannerView.requiredContentSizeIdentifiers = [NSSet setWithObject:ADBannerContentSizeIdentifierPortrait];
        bannerView.currentContentSizeIdentifier = ADBannerContentSizeIdentifierPortrait;
        bannerView.delegate = self;
        bannerView.hidden = NO;
        bannerView.userInteractionEnabled = YES;

        // The widget's width and height must be set to a fix size.
        [super setPropertyWithKey:@MAW_WIDGET_WIDTH toValue:[NSString stringWithFormat:@"%d", BANNER_WIDTH]];
        [super setPropertyWithKey:@MAW_WIDGET_HEIGHT toValue:[NSString stringWithFormat:@"%d", BANNER_HEIGHT]];
        view = bannerView;
    }

    mBannerHandle = bannerHandle;
    return [super init];
}

/**
 * Sets a banner property.
 * @param key A string representing which property to set.
 * @param value The value that will be assigned to the property.
 * @return One of the next result codes:
 * - MA_ADS_RES_OK if no error occurred.
 * - MA_ADS_RES_INVALID_PROPERTY_NAME if the property name is not valid.
 * - MA_ADS_RES_INVALID_PROPERTY_VALUE if the property value is not valid.
 */
- (int)setPropertyWithKey: (NSString*)key toValue: (NSString*)value
{
    NSLog(@"iOS setProperty");
    ADBannerView* bannerView  = (ADBannerView*) view;
    if ([key isEqualToString:@MA_ADS_ENABLE])
    {
        if ([value isEqualToString:@"true"])
        {
            bannerView.userInteractionEnabled = YES;
        }
        else if ([value isEqualToString:@"false"])
        {
            bannerView.userInteractionEnabled = NO;
        }
        else
        {
            return MA_ADS_RES_INVALID_PROPERTY_VALUE;
        }
    }
    else if ([key isEqualToString:@MA_ADS_VISIBLE])
    {
        if ([value isEqualToString:@"true"])
        {
            bannerView.hidden = NO;
        }
        else if ([value isEqualToString:@"false"])
        {
            bannerView.hidden = YES;
        }
        else
        {
            return MA_ADS_RES_INVALID_PROPERTY_VALUE;
        }
    }
    else
    {
        return MA_ADS_RES_INVALID_PROPERTY_NAME;
    }

	return MA_ADS_RES_OK;
}

/**
 * Retrieves a specified property value.
 * @param key A string representing for which property to get the value.
 * @return The value for the given property, or nil if the property is invalid.
 */
- (NSString*)getPropertyWithKey: (NSString*)key
{
    NSLog(@"iOS getProperty");
    ADBannerView* bannerView  = (ADBannerView*) view;
    if ([key isEqualToString:@MA_ADS_WIDTH])
    {
        return [[NSString alloc] initWithFormat:@"%d", BANNER_WIDTH];
    }
    else if ([key isEqualToString:@MA_ADS_HEIGHT])
    {
        return [[NSString alloc] initWithFormat:@"%d", BANNER_HEIGHT];
    }
    else if ([key isEqualToString:@MA_ADS_VISIBLE])
    {
        if (bannerView.hidden)
        {
            return [[NSString alloc] initWithString:@"false"];
        }
        return [[NSString alloc] initWithString:@"true"];
    }
    else if ([key isEqualToString:@MA_ADS_ENABLE])
    {
        if (bannerView.userInteractionEnabled)
        {
            return [[NSString alloc] initWithString:@"true"];
        }
        return [[NSString alloc] initWithString:@"false"];
    }

	return [[NSString alloc] init];
}

#pragma mark AdBannerViewDelagate

- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation
{
    // Return YES for supported orientations
    return (interfaceOrientation == UIInterfaceOrientationPortrait);
}

- (void)bannerView:(ADBannerView *)banner didFailToReceiveAdWithError:(NSError *)error
{
    NSLog(@"didFailToReceiveAdWithError:%d %@", error.code, [error description]);
    MAEvent event;
	event.type = EVENT_TYPE_ADS_BANNER;
	event.adsData.bannerHandle = mBannerHandle;
    event.adsData.bannerEventType = MA_ADS_EVENT_FAILED;

    // Get the error code.
    int errorCode;
    switch (error.code)
    {
        case ADErrorServerFailure:
        case ADErrorLoadingThrottled:
            errorCode = MA_ADS_ERROR_NETWORK;
            break;
        case ADErrorInventoryUnavailable:
            errorCode = MA_ADS_ERROR_NO_FILL;
            break;
        case ADErrorConfigurationError:
            errorCode = MA_ADS_ERROR_CONFIGURATION;
            break;
        case ADErrorBannerVisibleWithoutContent:
            errorCode = MA_ADS_ERROR_NO_CONTENT;
            break;
        case ADErrorApplicationInactive:
            errorCode = MA_ADS_ERROR_APPLICATION_INACTIVE;
            break;
        default:
            break;
    }
    event.adsData.bannerErrorCode = errorCode;
    Base::gEventQueue.put(event);
}

- (void)bannerViewActionDidFinish:(ADBannerView *)banner
{
    NSLog(@"bannerViewActionDidFinish");
    MAEvent event;
	event.type = EVENT_TYPE_ADS_BANNER;
	event.adsData.bannerHandle = mBannerHandle;
    event.adsData.bannerEventType = MA_ADS_EVENT_ON_DISMISS;
    Base::gEventQueue.put(event);
}

- (BOOL)bannerViewActionShouldBegin:(ADBannerView *)banner willLeaveApplication:(BOOL)willLeave
{
    NSLog(@"willLeaveApplication");
    MAEvent event;
	event.type = EVENT_TYPE_ADS_BANNER;
	event.adsData.bannerHandle = mBannerHandle;
    event.adsData.bannerEventType = MA_ADS_EVENT_ON_LEAVE_APPLICATION;
    Base::gEventQueue.put(event);
    return TRUE;
}

- (void)bannerViewDidLoadAd:(ADBannerView *)banner
{
    NSLog(@"bannerViewDidLoadAd");
    MAEvent event;
	event.type = EVENT_TYPE_ADS_BANNER;
	event.adsData.bannerHandle = mBannerHandle;
    event.adsData.bannerEventType = MA_ADS_EVENT_LOADED;
    Base::gEventQueue.put(event);
}

@end
