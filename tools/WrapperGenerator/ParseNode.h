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

#ifndef _PARSE_NODE_H_
#define _PARSE_NODE_H_

#include <string>
#include <map>
#include <vector>

class Base;

struct ParseNode {
	ParseNode();
	std::string getAttr(const std::string& name, bool force=true) const;

	int getIntAttr(const std::string& name, bool force=true) const;

	void getNodesFromIdList(const std::string& name, std::vector<const Base*>& output, bool force=true) const;

	std::string name;
	std::map<std::string, std::string> attributes;
	std::vector<const ParseNode*> children;

	ParseNode *parent;

	Base* base;

	int lineNumber;
};

#endif // _PARSE_NODE_H_