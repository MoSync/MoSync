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

#ifndef UTIL_H_
#define UTIL_H_

#include <maprofile.h>	// Profile database.
#include <mastring.h>		// C string functions

    /**
     * Detects if the current platform is Android.
     * @return true if the platform is Android, false otherwise.
     */
    static bool isAndroid()
    {
		if ( NULL != strstr(MA_PROF_STRING_PLATFORM, "android"))
		{
			return true;
		}
		else
		{
			return false;
		}
    }

#endif /* UTIL_H_ */
