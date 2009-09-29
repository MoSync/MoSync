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

#include "util.h"

#if !defined(_WIN32) && !defined(MAPIP)
#include <locale.h>
#endif

#include <mastdlib.h>
#include <mactype.h>
#include <maxtoa.h>
#include <mavsprintf.h>
#include <madmath.h>

namespace MAUtil {

#ifndef MAX
#	define MAX(a,b) (((a) > (b)) ? (a) : (b))
#endif

#ifndef MIN
#	define MIN(a,b) (((a) < (b)) ? (a) : (b))
#endif

#define CLAMPTO(f,a,b) (MIN( (MAX(f,a)), (b)))

	double clamp(double v, double a, double b) {
		return CLAMPTO(v,a,b);
	}

	int clamp(int v, int a, int b) {
		return CLAMPTO(v,a,b);
	}

	unsigned int clamp(unsigned int v, unsigned int a, unsigned int b) {
		return CLAMPTO(v,a,b);
	}
#ifdef IN
#	undef IN
#endif
#define IN(v,a,b) ((v) >= (a) && (v) <= (b))

	bool inside(double v,double a, double b) {
		return IN(v,a,b);
	}

	bool inside(int v, int a, int b) {
		return IN(v,a,b);
	}

	bool inside(unsigned int v, unsigned int a, unsigned int b) {
		return IN(v,a,b);
	}

	double wrapInRange(double v, double a, double b) {
		if(a>b) {
			double t = a; a = b; b = t;
		}
		if(v >= a && v <= b) return v;
		if(v > b) return a+fmod(v-b,b-a);
		if(v < a) return b-fmod(a-v,b-a);
		return a;
	}

	String integerToString(int i, int base) {
		char cBuf[64];
		return itoa(i, cBuf, base);
	}

	int stringToInteger(const String &s, int base) {
		return strtoul(s.c_str(), 0, base);
	}

	double stringToDouble(const String &s) {
#if !defined(_WIN32) && !defined(MAPIP)
		setlocale(LC_ALL, "C");
#endif
		return (double)atof(s.c_str());
	}

	String doubleToString(double f, int decimals) {
		char cBuf[64];
		sprintf(cBuf, "%.*f", decimals, f);
		return cBuf;
	}

	String lowerString(const String& s) {
		String ret = s;
		for(int i=0; i<ret.length(); i++) {
			ret[i] = tolower(ret[i]);
		}
		return ret;
	}

	String upperString(const String& s) {
		String ret = s;
		for(int i=0; i<ret.length(); i++) {
			ret[i] = toupper(ret[i]);
		}
		return ret;
	}

	void stringSplit(const String& str, const String& delim, MAUtil::Vector<String>& output)
	{
		int offset = 0;
		int delimIndex = 0;

		output.clear();
		if(str.length() == 0) return;

		delimIndex = str.find(delim, offset);

		while (delimIndex != String::npos)
		{
			MAUtil::String sub = str.substr(offset, delimIndex - offset);
			output.add(sub);
			offset += delimIndex - offset + delim.length();
			delimIndex = str.find(delim, offset);
		}

		output.add(str.substr(offset, str.length() - offset));
	}

	int dummy(int a) {
		return a;
	}
}
