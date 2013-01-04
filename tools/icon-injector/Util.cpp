/* Copyright (C) 2010 MoSync AB

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

#include <stdlib.h>
#include <string.h>
#include <fstream>
#include <sstream>
#include <ostream>
#include <cerrno>
#include <sys/stat.h>
#include "Util.h"

using namespace std;

string arg(const char* arg) {
	string result = string(arg);
	bool isQuoted = (result.size() > 2) &&
			(result.at(0) == '\"');
	if (!isQuoted && result.find(' ') != string::npos) {
		result = "\"" + result + "\"";
	}
	return result;
}

string arg(string argArg) {
	return arg(argArg.c_str());
}

string file(const char* argArg) {
	string argStr = string(argArg);
#ifdef WIN32
	toBackSlashes(argStr);
#else
	toSlashes(argStr);
#endif
	return arg(argStr.c_str());
}

string file(std::string arg) {
	return file(arg.c_str());
}

void toSlashes(string& str) {
	for(size_t i=0; i<str.size(); i++) {
		str[i] = (str[i] == '\\' ? '/' : str[i]);
	}
}
void toBackSlashes(std::string& str) {
	for(size_t i=0; i<str.size(); i++) {
		str[i] = (str[i] == '/' ? '\\' : str[i]);
	}
}
