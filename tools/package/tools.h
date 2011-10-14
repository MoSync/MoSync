/* Copyright (C) 2010 MoSync AB

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License, version 2, as published by
the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

You should have received a copy of the GNU General Public License
along with this program; see the file COPYING.  If not, write to the Free
Software Foundation, 59 Temple Place - Suite 330, Boston, MA
02111-1307, USA.
*/

#ifndef TOOLS_H
#define TOOLS_H

#include <string>

/**
 * Injects an icon in a platform specific manner, using the icon injector.
 * Exits on failure.
 */
void injectIcon(const char* platform, const char* size, const char* srcIcon, const char* dst, bool silent);

/**
 * Returns a MoSync binary, located in the MOSYNCDIR/bin directory.
 */
std::string getBinary(const char* binaryName);

#endif	//TOOLS_H
