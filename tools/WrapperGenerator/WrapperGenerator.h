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

#ifndef _WRAPPER_GENERATOR_H_
#define _WRAPPER_GENERATOR_H_

#include <stdarg.h>
#include <string>
#include <vector>
#include <map>
#include "helpers/attribute.h"

namespace System {
	std::string toUpperCase(const std::string& str);
	void error(const char* fmt, ...) PRINTF_ATTRIB(1, 2) GCCATTRIB(noreturn);
	std::string genstr(const char * fmt, ...) PRINTF_ATTRIB(1, 2);
	void split(const std::string& str, const std::string& delim, std::vector<std::string>& output);
	void parseAttributes(const std::string& str, std::map<std::string, std::string>& attr);
} // namespace System

#define SASSERT(truthiness) if(!(truthiness)) {\
	System::error("Assertion failure (" #truthiness ") on line %i in %s", __LINE__, __FILE__); }

#endif // _WRAPPER_GENERATOR_H_