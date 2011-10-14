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

/**
 * @file WidgetUtil.h
 * @author Mattias Frånberg and Chris Hughes
 *
 * Implementation of basic utilities that get and set widget properties.
 */

// Include the standard libraries we need.
#include <mastdlib.h>
#include <mavsprintf.h>

// Include NativeUI.
#include <IX_WIDGET.h>

// Include the header file for this file.
#include "WidgetUtil.h"

/**
 * Set an integer property of a widget.
 *
 * @param handle Handle to the widget.
 * @param property String representing the property to be set.
 * @param value The value the property is to be set to.
 *
 * @return WIDGET_OK if the property was set, WIDGET_ERROR otherwise.
 */
int widgetSetPropertyInt(MAHandle handle, const char *property, int value)
{
	char buffer[256];
	sprintf(buffer, "%i", value);
	return maWidgetSetProperty( handle, property, buffer );
}

/**
 * Retrieve an integer property of a widget.
 *
 * @param handle Handle to the widget
 * @param property The property of the widget to get.
 *
 * @return The value of the property.
 */
int widgetGetPropertyInt(MAHandle handle, const char *property)
{
	char buffer[256];
	maWidgetGetProperty(handle, property, buffer, 256);
	return atoi(buffer);
}
