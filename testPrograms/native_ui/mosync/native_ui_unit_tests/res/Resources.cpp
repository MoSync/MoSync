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
 * resources.cpp
 *
 *  Created on: Mar 15, 2011
 *      Author: fmattias
 */
#include <mavsprintf.h>

#include "Resources.h"

#include "MAHeaders.h"

#define BUFFER_SIZE 512

const char *
getIconHandleAsString()
{
	static const char *iconHandle = NULL;
	if( iconHandle != NULL )
	{
		return iconHandle;
	}

	static char valueAsString[ BUFFER_SIZE + 1 ];
	int charsWritten = sprintf( valueAsString, "%d", R_ICON );

	if( charsWritten > 0 )
	{
		return valueAsString;
	}
	else
	{
		return "";
	}
}
