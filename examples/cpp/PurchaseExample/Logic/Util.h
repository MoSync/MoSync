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
 * @file Util.h
 * @author Emma Tresanszki.
 *
 * Utility functions for the application.
 */

#ifndef PURCHASE_UTIL_H_
#define PURCHASE_UTIL_H_

#include <mastring.h>		// C string functions
#include <conprint.h>
#include <mavsprintf.h>
#include <MAUtil/String.h>

	// Usual size for a buffer.
	const int BUF_SIZE = 256;

	#define BUF_MAX 256

	enum platform_code
	{
		ANDROID = 0,
		IOS = 1,
		WINDOWSPHONE7 = 2
	};

	/**
	 * Detects the current platform
	 * @return platform_code specific for Android, iPhone OS or WindowsPhone
	 */
	extern int getPlatform();

#endif /* PURCHASE_UTIL_H_ */
