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

#if defined(MOSYNC_NATIVE) && !defined(__WINDOWS_PHONE_8__)
#include <stdio.h>
#endif

#include "ma.h"
#include "maarg.h"
#include "mastring.h"
#include "mavsprintf.h"

#if defined(MOSYNC_NATIVE) && !defined(__WINDOWS_PHONE_8__)
#define CON(ret, fn) ret con_##fn
#undef printf
#define printf con_printf
#undef puts
#define puts con_puts
#undef wputs
#define wputs con_wputs
#undef vprintf
#define vprintf con_vprintf
#undef wprintf
#define wprintf con_wprintf
#undef wvprintf
#define wvprintf con_wvprintf
#undef putchar
#define putchar con_putchar
#else
#define CON(ret, fn) ret fn
#endif

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
* \param str A C string
*
* \see InitConsole printf
*/

void PrintConsole(const wchar * str);

CON(int, puts(const char* str));
CON(int, wputs(const wchar* str));

/** \brief Prints a formatted string to the console.
* \param fmt A C string that may contain formatting
* \return Returns length of the printed string
*
* \warning This function uses a fixed buffer size.
* Outputting too much data at once will cause a buffer overrun.
*
* \see InitConsole PrintConsole
*/

CON(int, printf(const char *fmt, ...));

CON(int, vprintf(const char *fmt, va_list args));

CON(int, wprintf(const wchar_t *fmt, ...));

CON(int, wvprintf(const wchar_t *fmt, va_list args));

CON(int, putchar(int character));

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

/** \brief Console draw background option
*
* If non-zero, console will fill the background, otherwise not.
* The default value is non-zero.
* \see PrintConsole printf
*/
extern int gConsoleDrawBackground;

/** \brief Console force display option
*
* If non-zero, console will be displayed each time something is sent to it, otherwise not (is then displayed using DisplayConsole()).
* The default value is non-zero.
* \see PrintConsole printf
*/
extern int gConsoleForceDisplay;


#ifdef __cplusplus
}	//extern "C"
#endif

#endif /* CONSOLE */
