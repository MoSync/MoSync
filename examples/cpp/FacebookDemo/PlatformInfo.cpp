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
