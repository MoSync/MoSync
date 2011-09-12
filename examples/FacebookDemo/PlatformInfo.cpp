/*
 * PlatformInfo.cpp
 *
 *  Created on: Sep 9, 2011
 *      Author: gabi
 */

#include <maprofile.h>        // Profile database
#include <mastring.h>

#include "PlatformInfo.h"

namespace FacebookDemoApplication
{

bool isAndroid()
{
	if (NULL != strstr(MA_PROF_STRING_PLATFORM, "android"))
	{
		return true;
	}
	else
	{
		return false;
	}
}

}//namespace FacebookDemoApplication
