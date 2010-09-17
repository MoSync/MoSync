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

/**
 * This enum determines how variables are printed.
 * 
 * eNameOnly - print only the names of the variables;
 * eAllValues - print variable names and their values;
 * eSimpleValues print the name and value for simple data types and just the 
 *               name for arrays, structures and unions.
 */
enum PrintValueSimplicity {
	eNameOnly, eAllValues, eSimpleValues
};

/**
 * Splits the argument string on spaces and puts the resulting
 * strings in the output vector. Substrings inside '"' will not be split.
 * 
 * @param str Contains an argument string.
 * @param output The result is stored here.
 */
void splitArgs(const std::string& str, std::vector<std::string>& output);

/**
 * Tests if the specified string begins with a given string. 
 *
 * @param what The string which beginning is compared to 'with'.
 * @param with The string that beginning of 'what' is tested against.
 * @return True if the first letters of 'what' is equal to 'with', false
 *         otherwise.
 */
bool beginsWith(const std::string& what, const std::string& with);

/**
 * Converts the given string to an integer.
 *
 * @param src The string to be converted.
 * @param dst The result is stored here.
 * @return true if the given string could be converted into an integer,
 *         false otherwise.
 */
bool parseArgInt(const std::string& src, int* dst);

/**
 * Some GDB MI commands take a print-values parameter which indicates how
 * variables should be printed. This function converts the string form to 
 * an enum.
 *
 * @param src The parameter to convert.
 * @param dst The result will be stored here.
 * @return true if the given string was converted, false otherwise.
 */
bool parseArgPVS(const std::string& src, PrintValueSimplicity* dst);

/**
 * Converts a symbolic register name to its corresponding integer.
 *
 * @param src Symbolic register name e.g. sp.
 * @param dst The number of the register is stored here.
 * @return true if the register name could be converted to an integer, 
 *         false otherwise.
 */
bool parseArgRegName(const std::string& src, int* dst);

int eprintf(const char* fmt, ...) PRINTF_ATTRIB(1, 2);	//prints to stderr, logs
int oprintf(const char* fmt, ...) PRINTF_ATTRIB(1, 2);	//prints to stdout, logs
void oputc(int c);

/**
 * Prints the function name and filename of the function located at the given
 * program counter.
 *
 * @param pc The program counter.
 */
void oprintFrame(int pc);

typedef void (*ErrorCallback)();
void setErrorCallback(ErrorCallback ecb);
void error(const char* fmt, ...) PRINTF_ATTRIB(1, 2);

#define EFAIL(cond, msg) if(cond) { error(msg); return; }

#define CHECK_STABS if(!stabsIsLoaded()) { error("Stabs not loaded"); return; }

#define GDB_PROMPT "(gdb) \n"

//returns the first safe breakpoint address in the named function,
//or <0 on failure.
int mapFunctionBreakpoint(const char* name);

//returns false if an error has occured.
//otherwise, a valid address will be stored.
//location will be modified during parsing.
bool parseLocation(std::string& location, std::vector<int>& addresses);

#endif // _HELPERS_H_
