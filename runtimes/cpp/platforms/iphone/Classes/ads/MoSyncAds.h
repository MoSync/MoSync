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

#ifndef MoSync_MoSyncAds_h
#define MoSync_MoSyncAds_h

#include <helpers/cpp_defs.h>
#include "Syscall.h"

SYSCALL(int, maAdsBannerCreate(int size, const char* publisherID));
SYSCALL(int, maAdsAddBannerToLayout(MAHandle bannerHandle, MAHandle layoutHandle));
SYSCALL(int, maAdsRemoveBannerFromLayout(MAHandle bannerHandle, MAHandle layoutHandle));
SYSCALL(int, maAdsBannerDestroy(MAHandle bannerHandle));
SYSCALL(int, maAdsBannerSetProperty(MAHandle bannerHandle, const char* property, const char* value));
SYSCALL(int, maAdsBannerGetProperty(MAHandle bannerHandle, const char* property, char* value, const int bufSize));

#endif
