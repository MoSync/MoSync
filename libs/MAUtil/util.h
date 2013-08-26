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

/** \file util.h
 * \brief Utility functions (clamps, wraps, insides, string conversions and breaks)
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
	/** Converts \a l to its string representation in radix \a base. **/
	String longToString(long long l, int base);
	/** Converts a MAUtil::String to lowercase. **/
	String lowerString(const String& s);
	/** Converts a MAUtil::String to uppercase. **/
	String upperString(const String& s);
	/** Splits \a str into multiple strings, one for each occurance of the entire \a delim substring.
	* \a output is cleared, then the resulting strings are added to it. **/
	void stringSplit(const String& str, const String& delim, MAUtil::Vector<String>& output);
	void stringSplit(const WString& str, const WString& delim, MAUtil::Vector<WString>& output);
	/** Converts a string to an integer, using the same semantics as strtoul(). **/
	int stringToInteger(const String &s, int base = 10);
	/** Converts a string to a long long, using the same semantics as strtoull(). **/
        long long stringToLong(const String &s, int base = 10);
	/** Converts a string to a double, using the same semantics as atof(). **/
	double stringToDouble(const String &s);

	/** Splits \a str into multiple strings, breaking at the occurrence of any character in \a delim.
	* \a output is cleared, then the resulting strings are added to it. **/
	void stringBreak(const char* str, const char* delim, MAUtil::Vector<String>& output);
	void stringBreak(const wchar_t* str, const wchar_t* delim, MAUtil::Vector<WString>& output);

	/// @cond DUMMY
	int dummy(int);
	/// @endcond
}

#endif	//_SE_MSAB_MAUTIL_UTIL_H_
