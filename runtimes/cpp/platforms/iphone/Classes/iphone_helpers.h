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

/*
 *  iphone_helpers.h
 *  MoSync
 *
 *  Created by Niklas Nummelin on 2/23/10.
 *
 */

#ifndef _IPHONEHELPERS_H_
#define _IPHONEHELPERS_H_

#include "ThreadPoolImpl.h"
#include <helpers/cpp_defs.h>

// do all the const char* become memory leaks or does the garbage collector take care of that?.
// must fix that.
void logWithNSLog(const char *str, int length);
const char *getReadablePath(const char* path);
const char *getWriteablePath(const char* path);
void sleepMillis(int ms);
const char *unicodeToAscii(const wchar_t* str);
int getFreeAmountOfMemory();
int getTotalAmountOfMemory();
bool platformRequest(const char *url);
size_t wcharLength(const wchar* str);

void getScreenResolution(int& w, int &h);

#endif