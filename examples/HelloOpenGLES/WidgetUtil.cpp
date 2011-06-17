/* Copyright (C) 2011 MoSync AB

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License, version 2, as published by
the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, write to the Free
Software Foundation, 59 Temple Place - Suite 330, Boston, MA
02111-1307, USA.
*/

/** @file widgetutil.cpp
*
* Implementation of basic utilities that get and set widget properties.
*
* @author Mattias Frånberg and Chris Hughes
*/


#include "widgetutil.h"
#include <mastdlib.h>
#include <mavsprintf.h>

//Include NativeUI so that we can set widget properties.
#include <IX_WIDGET.h>

int maWidgetSetPropertyInt(MAHandle handle, const char *property, int value)
{
	char buffer[256];
	sprintf( buffer, "%d", value );

	maWidgetSetProperty( handle, property, buffer );
}

int maWidgetGetPropertyInt(MAHandle handle, const char *property)
{
	char buffer[256];
	maWidgetGetProperty( handle, property, buffer, 256);

	return atoi(buffer);
}
