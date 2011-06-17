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

/** @file widgetutil.h
*
* Header file containing forward declarations for some basic
* utilities that get and set widget properties.
*
* @author Mattias Frånberg and Chris Hughes
*/


#ifndef WIDGETUTIL_H_
#define WIDGETUTIL_H_

//Include the MoSync syscall collection.
#include <maapi.h>

/**
 * Set an integer property of a widget.
 *
 * @param handle Handle to the widget.
 * @param property String representing the property to be set.
 * @param value The value the property is to be set to.
 *
 * @return WIDGET_OK if the property was set, WIDGET_ERROR otherwise.
 */
int maWidgetSetPropertyInt(MAHandle handle, const char *property, int value);

/**
 * Retrieve an integer property of a widget.
 *
 * @param handle Handle to the widget
 * @param property The property of the widget to get.
 *
 * @return The value of the property.
 */
int maWidgetGetPropertyInt(MAHandle handle, const char *property);

#endif /* WIDGETUTIL_H_ */
