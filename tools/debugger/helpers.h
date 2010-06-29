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

#ifndef _HELPERS_H_
#define _HELPERS_H_

#include <string>
#include <vector>
#include <helpers/attribute.h>

#ifdef __GNUC__
#include <assert.h>
#define _ASSERT assert
#endif

enum PrintValueSimplicity {
	eNameOnly, eAllValues, eSimpleValues
};

//void stringSplit(const std::string& str, const std::string& delim,
//	std::vector<std::string>& output);
void splitArgs(const std::string& str, std::vector<std::string>& output);

//returns true if 'what' begins with 'with'.
bool beginsWith(const std::string& what, const std::string& with);

//parses the string as an int,
//returning false if there's anything in the string but the int.
//also calls error().
bool parseArgInt(const std::string& src, int* dst);

bool parseArgPVS(const std::string& src, PrintValueSimplicity* dst);

//puts the register index into *dst.
bool parseArgRegName(const std::string& src, int* dst);

int eprintf(const char* fmt, ...) PRINTF_ATTRIB(1, 2);	//prints to stderr, logs
int oprintf(const char* fmt, ...) PRINTF_ATTRIB(1, 2);	//prints to stdout, logs
void oputc(int c);

void oprintFrame(int pc);

typedef void (*ErrorCallback)();
void setErrorCallback(ErrorCallback ecb);
void error(const char* fmt, ...) PRINTF_ATTRIB(1, 2);

#define EFAIL(cond, msg) if(cond) { error(msg); return; }

#define CHECK_STABS if(!stabsIsLoaded()) { error("Stabs not loaded"); return; }

#define GDB_PROMPT "(gdb) \n"

#endif // _HELPERS_H_
