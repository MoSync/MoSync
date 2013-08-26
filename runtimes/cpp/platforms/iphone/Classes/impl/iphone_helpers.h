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
size_t wchartLength(const wchar_t* str);

void getScreenResolution(int& w, int &h);
float getScreenScale();

#endif