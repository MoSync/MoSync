/*
 Copyright (C) 2013 MoSync AB

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

#import "MoSyncAds.h"
#import "Ads.h"

SYSCALL(int, maAdsBannerCreate(int size, const char* publisherID))
{
    return [[Ads getInstance] createBanner];
}

SYSCALL(int, maAdsAddBannerToLayout(MAHandle bannerHandle, MAHandle layoutHandle))
{
    return [[Ads getInstance] addBanner:bannerHandle toLayout:layoutHandle];
}

SYSCALL(int, maAdsRemoveBannerFromLayout(MAHandle bannerHandle, MAHandle layoutHandle))
{
    return [[Ads getInstance] removeBanner:bannerHandle fromLayout:layoutHandle];
}

SYSCALL(int, maAdsBannerDestroy(MAHandle bannerHandle))
{
    return [[Ads getInstance] bannerDestroy:bannerHandle];
}
SYSCALL(int, maAdsBannerSetProperty(MAHandle bannerHandle, const char* property, const char* value))
{
    return [[Ads getInstance] bannerSetProperty:bannerHandle property:property value:value];
}
SYSCALL(int, maAdsBannerGetProperty(MAHandle bannerHandle, const char* property, char* value, const int bufSize))
{
    return [[Ads getInstance] bannerGetProperty:bannerHandle property:property value:value size:bufSize];
}
