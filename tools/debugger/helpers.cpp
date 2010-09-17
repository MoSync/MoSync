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

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "config.h"
#undef LOGGING_ENABLED
#include "helpers/log.h"
#include "stabs/stabs.h"

#include "helpers.h"
#include "commandInterface.h"
#include "sld.h"
#include "cmd_data.h"

using namespace std;

#if 0
void stringSplit(const string& str, const string& delim, vector<string>& output)
{
	int offset = 0;
	size_t delimIndex = 0;

	output.clear();
	if(str.length() == 0) return;

	delimIndex = str.find(delim, offset);

	while (delimIndex != string::npos)
	{
		string sub = str.substr(offset, delimIndex - offset);
		output.push_back(sub);
		offset += delimIndex - offset + delim.length();
		delimIndex = str.find(delim, offset);
	}

	output.push_back(str.substr(offset, str.length() - offset));
}
#endif

void splitArgs(const string& str, vector<string>& output)
{
	string temp;
	LOG("splitArgs(%s)\n", str.c_str());

	output.clear();

	for(size_t i=0; i<str.length(); i++) {
		char c = str[i];
		if(c == ' ') {
			if(temp.size() > 0) {
				output.push_back(temp);
				LOG("sa %s\n", temp.c_str());
				temp.clear();
			}
		} else if(c == '\"' && temp.size() == 0) {
			i++;
			size_t delimIndex = str.find('\"', i);
			if(delimIndex == string::npos)
				delimIndex = str.length();
			temp = str.substr(i, delimIndex - i);
			output.push_back(temp);
			LOG("saq %s\n", temp.c_str());
			temp.clear();
			i = delimIndex;
		} else {
			temp += c;
		}
	}
	if(temp.size() > 0) {
		output.push_back(temp);
		LOG("sae %s\n", temp.c_str());
	}
}

bool beginsWith(const std::string& what, const std::string& with) {
	return what.substr(0, with.length()) == with;
}

bool parseArgInt(const std::string& src, int* dst) {
	int len;
	int res = sscanf(src.c_str(), "%i%n", dst, &len);
	if(res != 1 || len != (int)src.size()) {
		error("Bad argument: '%s'", src.c_str());
		return false;
	}
	return true;
}

bool parseArgPVS(const std::string& src, PrintValueSimplicity* dst) {
	if(src == "--no-values") { *dst = eNameOnly; return true; }
	else if(src == "--all-values") { *dst = eAllValues; return true; }
	else if(src == "--simple-values") { *dst = eSimpleValues; return true; }

	switch(src[0]) {
	case '0':
	case '1':
	case '2':
		*dst = (PrintValueSimplicity)atoi(src.c_str());
		return true;
	default:
		error("Bad argument");
		return false;
	}
}

bool parseArgRegName(const std::string& src, int* dst) {
	for(int i=0; i<32; i++) {
		if(gRegsyms[i] == src) {
			*dst = i;
			return true;
		}
	}
	if(src[0] == 'k') {	//one of the 96 constant registers
		int i;
		int len;
		int res = sscanf(src.c_str() + 1, "%i%n", &i, &len);
		if(res == 1 && len+1 == (int)src.length()) {
			*dst = 32 + i;
			return true;
		}
	}
	error("Unrecognized register");
	return false;
}

void oprintFrame(int pc) {
	int line;
	string file;
	bool mapSuccess = mapIp(pc, line, file);
	const char* fname = mapFunction(pc);

	oprintf("addr=\"0x%X\"", pc);
	if(fname) {
		oprintf(",func=\"%s\",args=[]", fname);
	}
	if(mapSuccess) {
		const char* fullName = file.c_str();
		const char* shortName = strrchr(fullName, '/');
		if(shortName == NULL)
			shortName = fullName;
		else
			shortName++;
		oprintf(",file=\"%s\",fullname=\"%s\",line=\"%i\"",
			shortName, fullName, line);
	}
	oprintf("}");
}


//returns the first safe breakpoint address in the named function,
//or <0 on failure.
int mapFunctionBreakpoint(const char* name) {
	int address = mapFunction(name);
	if(address < 0)
		return address;
	return nextSldEntry(address);
}

bool parseLocation(string& location, vector<int>& addresses) {
	int ret = 0;

	addresses.clear();

	_ASSERT(location.size() != 0);
	if(location[0] == '*') {	//hex address
		bool okFormat = false;
		do {
			if(location.size() != 11)
				break;
			if(location.substr(1, 2) != "0x")
				break;
			okFormat = true;
			for(size_t i=3; i<location.size(); i++) {
				if(!isxdigit(location[i])) {
					okFormat = false;
					break;
				}
			}
		} while(0);
		if(!okFormat) {
			error("Invalid address format");
			return false;
		}
		int address;
		int res = sscanf(location.c_str() + 3, "%x", &address);
		_ASSERT(res == 1);
		addresses.push_back(address);
	}
	else	//file or function
	{
		size_t colonIndex = location.find_last_of(':');
		if(colonIndex != string::npos) {	//filename:<linenum | function>
			int fli = colonIndex + 1;
			if(isdigit(location[fli])) {	//linenum, since function names may not begin with a digit.
				for(size_t i=fli; i<location.size(); i++) {
					if(!isdigit(location[i])) {
						error("Invalid linenum format");
						return false;
					}
				}
				int linenum;
				int res = sscanf(location.c_str() + fli, "%i", &linenum);
				_ASSERT(res == 1);
				//Then what? Use SDL table to map to address.
				location[colonIndex] = 0;
				ret = mapFileLine(location.c_str(), linenum, addresses);
				if(ret<0)
					error("%s", getMapFileLineError(ret));
			}
			else if(iscsym(location[fli])) {	//function name (static)
				ret = mapFunctionBreakpoint(location.c_str() + fli);
				addresses.push_back(ret);
			} else {
				error("Invalid location format");
				return false;
			}
		}
		else	//function without file (global?)
		{
			ret = mapFunctionBreakpoint(location.c_str());
			addresses.push_back(ret);
		}
	}

	if(ret < 0) {
		return false;
	}
	return true;
}
