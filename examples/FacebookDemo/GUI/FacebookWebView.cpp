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
 * FacebookWebView.cpp
 */

#include <maapi.h>
#include "FacebookWebView.h"

namespace FacebookDemoGUI
{

FacebookWebView::FacebookWebView():BUFFER_SIZE(16384)
{
}

void FacebookWebView::setStringPropertySize(int bufferSize)
{
	BUFFER_SIZE = bufferSize;
}

/**
	 * Widget override.
	 * Get a widget property as a string, setting also the result code.
	 */
MAUtil::String FacebookWebView::getPropertyString( const MAUtil::String& property ) const
{
	char *buffer = new char[BUFFER_SIZE];

	int result = maWidgetGetProperty(this->getWidgetHandle(), property.c_str(), buffer, BUFFER_SIZE);

	MAUtil::String temp;

	if( result == MAW_RES_INVALID_STRING_BUFFER_SIZE)
	{
		delete []buffer;
		int newBufferSize = 2*BUFFER_SIZE;
		buffer = new char[newBufferSize];
		result = maWidgetGetProperty(this->getWidgetHandle(), property.c_str(), buffer, newBufferSize);
	}

	if( result == RES_OK)
	{
		temp.append(buffer, BUFFER_SIZE);
	}

	delete []buffer;
	return temp;
}


}//namespace FacebookDemoGUI
