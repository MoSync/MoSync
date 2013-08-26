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
* \file MapConfig.h
* \brief Configuration header file for MAP library
* \author Lars Åke Vinberg
*/

#ifndef MAPCONFIG_H_
#define MAPCONFIG_H_

//
// Define as 1 to store downloaded tiles in cache in compressed state,
// i.e. the raw PNG or JPG binary data.
// Image resources will then be unpacked at time of rendering.
// This means rendering will be slower but cached tiles will use much
// less memory - possibly 75-90% less.
//
// #define StoreCompressedTilesInCache
//
// Maximum parallel downloaders
//
static const int			MapSourceDownloaders = 5;
//
// Default capacity in MapCache.
//
static const int MapCacheDefaultCapacity = 40;

#endif // MAPCONFIG_H

