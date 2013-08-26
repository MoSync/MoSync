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
