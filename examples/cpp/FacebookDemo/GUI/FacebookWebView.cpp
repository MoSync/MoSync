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
