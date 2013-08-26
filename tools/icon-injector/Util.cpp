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
