/*
 *  iphone_helpers.h
 *  MoSync
 *
 *  Created by Niklas Nummelin on 2/23/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _IPHONEHELPERS_H_
#define _IPHONEHELPERS_H_

#include "ThreadPoolImpl.h"

// do all the const char* become memory leaks or does the garbage collector take care of that?.
const char *getReadablePath(const char* path);
const char *getWriteablePath(const char* path);
void sleepMillis(int ms);
const char *unicodeToAscii(const wchar_t* str);
int getFreeAmountOfMemory();
int getTotalAmountOfMemory();
bool platformRequest(const char *url);

#endif