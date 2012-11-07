/*
Copyright (C) 2012 MoSync AB

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
 * Util.h
 *
 *  Created on: Sept 28, 2012
 *      Author: Spiridon Alexandru
 */

#include <mastring.h>
#include <MAUtil/String.h>
#include <MAUtil/Vector.h>
#include <NativeUI/ListViewItem.h>

#ifndef UTIL_H_
#define UTIL_H_

#define TITLE_FONT_SIZE 26.0

/**
 * Detects if the current platform is Android.
 * @return true if the platform is Android, false otherwise.
 */
bool isAndroid();

/**
 * Detects if the current platform is iOS.
 * @return true if the platform is iOS, false otherwise.
 */
bool isIOS();

/**
 * Detects if the current platform is Windows Phone.
 * @return true if the platform is Windows Phone, false otherwise.
 */
bool isWindowsPhone();

#endif /* UTIL_H_ */
