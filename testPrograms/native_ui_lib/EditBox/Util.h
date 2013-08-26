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
 * Util.h
 *
 *  Created on: Sept 28, 2012
 *      Author: Spiridon Alexandru
 */

#include <mastring.h>
#include <MAUtil/String.h>
#include <MAUtil/Vector.h>
#include <NativeUI/ListViewItem.h>

#ifndef UTIL_H_
#define UTIL_H_

#define TITLE_FONT_SIZE 26.0

/**
 * Detects if the current platform is Android.
 * @return true if the platform is Android, false otherwise.
 */
bool isAndroid();

/**
 * Detects if the current platform is iOS.
 * @return true if the platform is iOS, false otherwise.
 */
bool isIOS();

/**
 * Detects if the current platform is Windows Phone.
 * @return true if the platform is Windows Phone, false otherwise.
 */
bool isWindowsPhone();

#endif /* UTIL_H_ */
