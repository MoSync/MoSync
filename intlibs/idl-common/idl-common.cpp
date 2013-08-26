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
#include <stdio.h>

#include "idl-common.h"

class idl_exception : public exception {
public:
	idl_exception(const string& str) throw() : mStr(str) { }

	virtual ~idl_exception() throw() {}
	virtual const char* what() const throw() { return mStr.c_str(); }
protected:
	string mStr;
};

void throwException(const string& msg) {
	throw idl_exception(msg.c_str());
}

void runCommand(const string& cmd) {
	printf("%s\n", cmd.c_str());
	int res = system(cmd.c_str());
	if(res != 0) {
		throwException("External error!");
	}
}

bool isAnonStructName(const string& name) {
	if(name.size() > 2) {
		if(name[0] == '_' && name[1] >= 'A' && name[1] <= 'Z') {
			return true;
		}
	}
	return false;
}
