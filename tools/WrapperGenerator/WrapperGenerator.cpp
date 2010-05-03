/* Copyright (C) 2010 Mobile Sorcery AB

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

#include <map>
#include <string>
#include <vector>
#include <stack>
#include <stdarg.h>
#include <stdio.h>
#include <fstream>
#include <stdlib.h>

#include "Parser.h"
#include "IDLBackend.h"
#include "CPPBackend.h"

using namespace std;

namespace System  {
	void error(const char* fmt, ...) {
		va_list argptr;
		va_start(argptr, fmt);
		vprintf(fmt, argptr);
		va_end(argptr);

		exit(1);
	}

	string genstr(const char * fmt, ...) {
		va_list argptr;
		va_start(argptr, fmt);
		char str[1024];
		vsprintf(str, fmt, argptr);
		va_end(argptr);
		return str;
	}

	void split(const std::string& str, const std::string& delim, std::vector<std::string>& output)
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

	void parseAttributes(const std::string& str, map<string, string>& attr) {
		vector<string> output;
		System::split(str, " ", output);
		for(size_t i = 0; i < output.size(); i++) {
			string attribute = output[i];
			if(attribute.find("gccxml") != string::npos) {
				attribute = attribute.substr(7, attribute.length()-7-1);
				vector<string> values;
				System::split(attribute, ",", values);
				if(values.size() != 2) System::error("gccxml attribute invalid");
				attr[values[0]] = values[1];
			}
		}
	}
}

int main(int argc, char **argv) {
	Backend *idlBackend = new IDLBackend();
	Backend *cppBackend = new CPPBackend();

	Backend::BasesMap bases;

	if(argc != 2) System::error("Usage: WrapperGenerator input.h\n");
	const char *header = argv[1];

	string xmlName = System::genstr("%s.xml", header);

	if(system(System::genstr("gccxml %s -fxml=%s \"-include\" \"options/attribs.h\"", header, xmlName.c_str()).c_str()) != 0)
		System::error("gccxml failed\n");
	Parser::parse(xmlName.c_str(), bases);

	string idlName = System::genstr("%s.idl", header);
	string cppName = System::genstr("%s.cpp", header);

	idlBackend->generate(bases, idlName.c_str());
	cppBackend->generate(bases, cppName.c_str());
}
