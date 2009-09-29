/* Copyright (C) 2009 Mobile Sorcery AB

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
