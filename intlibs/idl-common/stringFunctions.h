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

#ifndef STRINGFUCNTIONS_H
#define STRINGFUCNTIONS_H

#include <string>
#include <sstream>

using namespace std;

//returns a string where the first character is toUpper'd
inline string firstUpper(string s) {
	s[0] = toupper(s[0]);
	return s;
}

//returns a string where the first character is toLower'd
inline string firstLower(string s) {
	s[0] = tolower(s[0]);
	return s;
}

//converts any type to a string
template<class T> string toString(const T& t) {
	stringstream ss;
	ss << t;
	return ss.str();
}

//returns a completely toUpper'd string
inline string toupper(string s) {
	for(size_t i=0; i<s.size(); i++) {
		s[i] = toupper(s[i]);
	}
	return s;
}

//returns a string suitable for use in preprocessor definitions
inline string toDef(string s) {
	for(size_t i=0; i<s.size(); i++) {
		s[i] = toupper(s[i]);
		if(s[i] == '.')
			s[i] = '_';
	}
	return s;
}

inline size_t getLastSlash(const string& path) {
	size_t anySlash = string::npos;
	for(size_t i=0; i<path.size(); i++) {
		if(path[i] == '/' || path[i] == '\\')
			anySlash = i;
	}
	return anySlash;
}

//returns the directory part of a path. if there is none, it returns "."
inline string getDirFromPath(const string& path) {
	size_t lastSlash = getLastSlash(path);
	if(lastSlash == string::npos)
		return ".";
	else
		return path.substr(0, lastSlash + 1);
}

inline string getFilenameFromPath(const string& path) {
	size_t lastSlash = getLastSlash(path);
	if(lastSlash == string::npos)
		return path;
	else
		return path.substr(lastSlash + 1);
}

inline ostream& operator<<(ostream& stream, const string& string) {
	return stream << string.c_str();
}
inline bool operator!=(const string& s1, const string& s2) {
	return s1.compare(s2) != 0;
}
inline bool operator==(const string& s1, const string& s2) {
	return s1.compare(s2) == 0;
}
inline string operator+(const string& s1, const string& s2) {
	string s = s1;
	s += s2;
	return s;
}

#endif	//STRINGFUCNTIONS_H
