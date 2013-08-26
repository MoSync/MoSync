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

#ifndef _PARSER_H_
#define _PARSER_H_

#include "ParseNode.h"
#include <string>
#include <map>

const ParseNode* getParseNodeFromId(const std::string& name);
const Base* parseType(const ParseNode& node,  bool& isconst);

namespace Parser {
	void parse(const std::string& input, std::multimap<std::string, Base*>& output, const std::string& bindingName);
};

#endif // _PARSER_H_