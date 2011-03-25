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
