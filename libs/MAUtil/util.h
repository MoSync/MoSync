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

/** \file util.h
 * \brief Utility functions
 */

#ifndef _SE_MSAB_MAUTIL_UTIL_H_
#define _SE_MSAB_MAUTIL_UTIL_H_

#include "String.h"

namespace MAUtil {
	/** Returns \a v clamped to the (a..b) range, inclusively.
	* If \a v is greater than \a b, \a b is returned. If \a v is less than \a a, \a a is returned.
	* Otherwise, v is returned. **/
	double clamp(double v, double a, double b);
	/** Returns \a v clamped to the (a..b) range, inclusively.
	* If \a v is greater than \a b, \a b is returned. If \a v is less than \a a, \a a is returned.
	* Otherwise, \a v is returned. **/
	int clamp(int v, int a, int b);
	/** Returns \a v clamped to the (a..b) range, inclusively.
	* If \a v is greater than \a b, \a b is returned. If \a v is less than \a a, \a a is returned.
	* Otherwise, \a v is returned. **/
	unsigned int clamp(unsigned int f, unsigned int a, unsigned int b);
	/** Returns \a v wrapped in the (a..b) range, inclusively.
	* If \a v is greater than \a b, it 'wraps' back from  a and conversely. **/
	double wrapInRange(double v, double a, double b);
	/** Returns f >= a && f <= b. **/
	bool inside(double a, double f, double b);
	/** Returns f >= a && f <= b. **/
	bool inside(int a, int b, int c);
	/** Returns f >= a && f <= b. **/
	bool inside(unsigned int a, unsigned int b, unsigned int c);	
	/** Converts \a f to its string representation, with \a decimals fractional digits. **/
	String doubleToString(double f, int decimals = 5);
	/** Converts \a f to its string representation in radix \a base. **/
	String integerToString(int i, int base = 10);
	/** Converts a MAUtil::String to lowercase. **/
	String lowerString(const String& s);
	/** Converts a MAUtil::String to uppercase. **/
	String upperString(const String& s);
	/** Splits \a str into multiple strings, one for each occurance of the entire \a delim substring.
	* \a output is cleared, then the resulting strings are added to it. **/
	void stringSplit(const String& str, const String& delim, MAUtil::Vector<String>& output);
	/** Converts a string to an integer, using the same semantics as strtoul(). **/
	int stringToInteger(const String &s, int base = 10);
	/** Converts a string to a double, using the same semantics as atof(). **/
	double stringToDouble(const String &s);

	/// @cond DUMMY
	int dummy(int);
	/// @endcond
}

#endif	//_SE_MSAB_MAUTIL_UTIL_H_
