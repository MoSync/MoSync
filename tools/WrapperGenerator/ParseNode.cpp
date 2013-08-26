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

#include "ParseNode.h"
#include "WrapperGenerator.h"
#include "Parser.h"
#include <stdio.h>

using namespace std;

ParseNode::ParseNode() : parent(NULL) {
}

string ParseNode::getAttr(const string& _name, bool force) const {
	map<string, string>::const_iterator iter = attributes.find(_name);
	if(iter != attributes.end()) return iter->second;
	else if(force) {
		System::error("%s", System::genstr("Attribute '%s' missing on line %d\n", _name.c_str(), lineNumber).c_str());
	}
	return "";
}

int ParseNode::getIntAttr(const string& _name, bool force) const {
	string attr = getAttr(_name, force);
	int i;
	int res = sscanf(attr.c_str(), "%i", &i);
	SASSERT(res == 1);
	return i;
}

void ParseNode::getNodesFromIdList(const std::string& _name, vector<const Base*>& output, bool force) const {
	
	const string& members = getAttr(_name, force);

	size_t startIndex = 0;
	size_t endIndex = 0;
	while((endIndex=members.find_first_of(" ", startIndex)) != string::npos) {
		string member = members.substr(startIndex, endIndex-startIndex);
		startIndex = endIndex + 1;
		output.push_back(getParseNodeFromId(member)->base);
	}
}
