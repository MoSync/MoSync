/*
 * FacebookWebView.cpp
 *
 *  Created on: Sep 3, 2011
 *      Author: gabi
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
