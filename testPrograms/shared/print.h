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

#ifndef PRINT_H
#define PRINT_H

/** \file print.h
* These functions constitute an advanced text output console system.
*/

#include "maarg.h"

//#ifndef WIN32
int vsprintf(char *buf, const char *fmt, va_list args);
int sprintf(char *buf, const char *fmt, ...);
//#endif

void printlnf(const char* fmt, ...);

/**
* Clears the screen immediately, then again at the beginning of the next println*().
* Call this before entering graphical mode, to facilitate return to text mode.
* There is no need to call it at the start of the program.
*/
//void ClearScreen();

/**
* Prints one line of text to the screen.
*/
void println(const char* text);

/**
* Stores a string representation of \<a\> into \<dst\>.
* Returns strlen(dst).
*/
int i2a(int a, char* dst);
/**
* Stores a string representation of \<d\> into \<dst\>.
* Returns strlen(dst).
*/
int d2a(double d, char* dst);
/**
* This is the maximum number of chars that i2a can write to a buffer, including the terminating null.
*/
#define MAX_I2A_SIZE 12
/**
* This is the maximum number of chars that d2a can write to a buffer, including the terminating null.
*/
#define MAX_D2A_SIZE (MAX_I2A_SIZE + 1 + 5)

#define DUMPDOUBLE(a) printlnf("%s %f", #a, a)
#define DUMPINT(a) printlnf("%s %i", #a, a)

#endif	//PRINT_H
