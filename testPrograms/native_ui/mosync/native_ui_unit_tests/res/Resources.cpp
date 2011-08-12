/*
Copyright (C) 2011 MoSync AB

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License,
version 2, as published by the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
MA 02110-1301, USA.
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
