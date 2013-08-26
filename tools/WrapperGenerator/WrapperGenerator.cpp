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

#include <map>
#include <string>
#include <vector>
#include <stack>
#include <stdarg.h>
#include <stdio.h>
#include <fstream>
#include <stdlib.h>
#include <algorithm>

#include "Parser.h"
#include "IDLBackend.h"
#include "CPPBackend.h"

using namespace std;

namespace System  {
	std::string toUpperCase(const std::string& str) {
		std::string ret = str;
		std::transform(ret.begin(), ret.end(), ret.begin(), (int(*)(int))toupper);
		return ret;
	}

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

	if(argc <= 1) System::error("Usage: WrapperGenerator <name> input.h (|input2.h...) \n");

	vector<string> headers;
	std::string bindingName = argv[1];

	for(int i = 2; i < argc; i++) {
		headers.push_back(argv[i]);
	}

	for(size_t i = 0; i < headers.size(); i++) {
		string xmlName = System::genstr("%s.xml", headers[i].c_str());
		string cmd = System::genstr("gccxml %s -fxml=%s -Itests \"-include\" \"options/attribs.h\"", headers[i].c_str(), xmlName.c_str());
		printf("%s\n", cmd.c_str());
		if(system(cmd.c_str()) != 0)
			System::error("gccxml failed\n");
		Parser::parse(xmlName.c_str(), bases, bindingName);
	}

	// remove duplicates
	for( std::multimap< std::string, Base* >::iterator iter = bases.begin(); iter != bases.end(); iter++ ) {
		for( std::multimap< std::string, Base* >::iterator iter2 = bases.begin(); iter2 != iter; iter2++ ) {
			Base* a = iter->second;
			Base* b = iter2->second;

			if(a->toString() == b->toString()) {
				a->setGroup(bindingName);
				printf("collision: %s\n", a->toString().c_str());
				bases.erase(iter2);
				break;
			}
		}
	}
	// -----------------------


	string idlName = System::genstr("tests/%s.idl", bindingName.c_str());
	string cppName = System::genstr("tests/%s.cpp", bindingName.c_str());

	idlBackend->generate(bases, idlName.c_str());
	cppBackend->generate(bases, cppName.c_str());
}
