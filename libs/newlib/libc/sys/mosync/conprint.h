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

/** \file conprint.h
* \brief A simple text output console.
*/

#ifndef _CONSOLE_H_
#define _CONSOLE_H_

#include "ma.h"
#include "stdio.h"
#include "stdarg.h"

#ifdef __cplusplus
extern "C" {
#endif

/** \brief Initialize the printf console.
*
* \see PrintConsole printf
*/

void InitConsole(void);

/** \brief Display the printf console.
*
* \see PrintConsole printf
*/
void DisplayConsole(void);

/** \brief Prints a C string to the console.
* \param str A Latin-1 string.
*
* \see InitConsole printf
*/

void PrintConsole(const char * str);

/** \brief Prints a string to the console.
* \param str A UTF-8 string.
* \param len The string's length.
*
* \see InitConsole printf
*/
void WriteConsole(const char* str, int len);


/** \brief The console text color.
*
* The default color is 0x00ff00 (green).
* \see PrintConsole printf
*/
extern int gConsoleTextColor;

/** \brief The console background color.
*
* The default color is 0x000000 (black).
* \see PrintConsole printf
*/
extern int gConsoleBackgroundColor;

/** \brief Console logging option.
*
* If non-zero, all text sent to PrintConsole will also be sent to maWriteLog().
* The default value is non-zero.
* \see PrintConsole printf
*/
extern int gConsoleLogging;

/** \brief Console file logging handle
*
* If greater than zero, all text sent to PrintConsole will also be sent to the file
* specified by this handle.
* The default value is zero. You'll have to open the handle yourself.
* \see maFileOpen()
*/
extern MAHandle gConsoleFile;

/** \brief Console display option
*
* If non-zero, console will be displayed, otherwise not.
* The default value is non-zero.
* \see PrintConsole printf
*/
extern int gConsoleDisplay;

#ifdef __cplusplus
}	//extern "C"
#endif

#endif /* CONSOLE */
