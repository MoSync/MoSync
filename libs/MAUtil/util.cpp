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
#ifdef __WINDOWS_PHONE_8__
		return _itoa(i, cBuf, base);
#else
		return itoa(i, cBuf, base);
#endif
	}

	int stringToInteger(const String &s, int base) {
		return strtoul(s.c_str(), 0, base);
	}

	String longToString(long long l, int base) {
#ifndef lltoa
		// This is just plain stupid.
		return integerToString((int) l, base);
#else
		char cBuf[128];
        return lltoa(l, cBuf, base);
#endif
	}

	long long stringToLong(const String &s, int base) {
		return strtoull(s.c_str(), 0, base);
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

	template<class Tchar> void stringSplit(const BasicString<Tchar>& str,
		const BasicString<Tchar>& delim, MAUtil::Vector<BasicString<Tchar> >& output)
	{
		int offset = 0;
		int delimIndex = 0;

		output.clear();
		if(str.length() == 0) return;

		delimIndex = str.find(delim, offset);

		while (delimIndex != BasicString<Tchar>::npos)
		{
			MAUtil::BasicString<Tchar> sub = str.substr(offset, delimIndex - offset);
			output.add(sub);
			offset += delimIndex - offset + delim.length();
			delimIndex = str.find(delim, offset);
		}

		output.add(str.substr(offset, str.length() - offset));
	}

	void stringSplit(const String& str, const String& delim, MAUtil::Vector<String>& output) {
		stringSplit<char>(str, delim, output);
	}
	void stringSplit(const WString& str, const WString& delim, MAUtil::Vector<WString>& output) {
		stringSplit<wchar_t>(str, delim, output);
	}

	template<class Tchar> void stringBreak(const Tchar* str, const Tchar* delim,
		MAUtil::Vector<BasicString<Tchar> >& output)
	{
		const Tchar* s = str;
		const Tchar* start = str;
		output.clear();
		while(*s) {
			const Tchar* d = delim;
			bool b = false;
			while(*d) {
				if(*d == *s) {
					b = true;
					break;
				}
				d++;
			}
			if(b) {
				if(start != s) {
					output.add(BasicString<Tchar>(start, s - start));
				}
				s++;
				start = s;
			} else {
				s++;
			}
		}
		if(*start) {
			output.add(BasicString<Tchar>(start, s - start));
		}
	}

	void stringBreak(const char* str, const char* delim, MAUtil::Vector<String>& output) {
		stringBreak<char>(str, delim, output);
	}
	void stringBreak(const wchar_t* str, const wchar_t* delim, MAUtil::Vector<WString>& output) {
		stringBreak<wchar_t>(str, delim, output);
	}

	int dummy(int a) {
		return a;
	}
}
