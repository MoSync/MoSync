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

/*
 * PlatformInfo.cpp
 *
 *  Created on: Sep 9, 2011
 *      Author: gabi
 */

#include <mastring.h>
#include <maapi.h>

#include "PlatformInfo.h"

#define BUF_MAX 256

namespace FacebookDemoApplication
{

bool isAndroid()
{
	char platform[BUF_MAX];
	maGetSystemProperty("mosync.device.OS", platform, BUF_MAX);
	if ( strcmp(platform,"Android") == 0 )
	{
		return true;
	}
	else
	{
		return false;
	}
}

}//namespace FacebookDemoApplication
