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

/**
 * @file WidgetUtil.h
 * @author Mattias Frånberg and Chris Hughes
 *
 * Header file containing forward declarations for some basic
 * utilities that get and set widget properties.
 */

// Guard against multiple includes.
#ifndef WIDGETUTIL_H_
#define WIDGETUTIL_H_

// Include MoSync syscalls.
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
int widgetSetPropertyInt(MAHandle handle, const char *property, int value);

/**
 * Retrieve an integer property of a widget.
 *
 * @param handle Handle to the widget
 * @param property The property of the widget to get.
 *
 * @return The value of the property.
 */
int widgetGetPropertyInt(MAHandle handle, const char *property);

#endif // WIDGETUTIL_H_
