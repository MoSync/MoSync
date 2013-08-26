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
#include "Base.h"
#include <stack>
#include <expat.h>
#include <stdio.h>
#include "Internal.h"
#include "Parser.h"

using namespace std;

#define DEFINE_FACTORY(typeName, typeStr) \
	if(name==typeStr) return new typeName();

static Base* createNode(const string& name) {
	WRAPPER_TYPES(DEFINE_FACTORY)
	else return NULL;
}

static map<string, const ParseNode*> sIdToNode;

const ParseNode* getParseNodeFromId(const string& name) {
	map<string, const ParseNode*>::iterator iter = sIdToNode.find(name);
	if(iter != sIdToNode.end()) return iter->second;
	else System::error("Node id missing");
	return NULL;
}

const Base* parseType(const ParseNode& node,  bool& isconst) {
	string type = node.getAttr("type");
	isconst = type[type.length()-1] == 'c';
	if(isconst)
		type.resize(type.length()-1);
	const Base* retType = getParseNodeFromId(type)->base;
	return retType;
}

namespace Parser {

	static char *readFileIntoMem(const char* filename, int *len) {
		FILE *file = fopen(filename, "rb");
		if(!file) return NULL;
		fseek(file, 0, SEEK_END);
		int length = ftell(file);
		fseek(file, 0, SEEK_SET);
		char *memory = new char[length];
		fread(memory, length, 1, file);
		fclose(file);
		*len = length;
		return memory;
	}

	static stack<ParseNode*> sParseStack;
	static XML_Parser sXmlParser;
	static std::string sBindingName;

	static void start(void *data, const char *el, const char **attr) {
			ParseNode* parseNode = new ParseNode();
			parseNode->name = el;
			sParseStack.push(parseNode);
			for (int i = 0; attr[i]; i += 2) {
				parseNode->attributes[attr[i]] = attr[i+1];
			}
			parseNode->base = createNode(parseNode->name);

			if(parseNode->base) {
				parseNode->base->setGroup(sBindingName);

				string attributes = parseNode->getAttr("attributes", false);
				if(attributes != "") {
					map<string, string> attrMap;
					System::parseAttributes(attributes, attrMap);
					string group = attrMap["group"];
					// the global set is called ""
					parseNode->base->setGroup(group);
				}
			}

			parseNode->lineNumber = XML_GetCurrentLineNumber(sXmlParser);

	}  /* End of start handler */

	static void end(void *data, const char *el) {
			ParseNode* child = sParseStack.top();
			if(sParseStack.size()!=1) {
				sParseStack.pop();
				ParseNode* parent = sParseStack.top();
				child->parent = parent;
				parent->children.push_back(child);
			}

	}  /* End of end handler */


	void parse(const string& input, std::multimap<std::string, Base*>& bases, const std::string& bindingName) {
		sBindingName = bindingName;

		while(sParseStack.size()>0)
			sParseStack.pop();

		sXmlParser = XML_ParserCreate("UTF-8");
		XML_SetElementHandler(sXmlParser, start, end);

		int fileLength;
		char *file = readFileIntoMem(input.c_str(), &fileLength);
		SASSERT(file);
		XML_Parse(sXmlParser, file, fileLength, 1);
		delete file;
		XML_ParserFree(sXmlParser);

		const vector<const ParseNode*>& parseNodes = sParseStack.top()->children;

		// setup id map
		for(size_t i = 0; i < parseNodes.size(); i++) {
			string id = parseNodes[i]->getAttr("id", false);
			if(id!="") {
				sIdToNode[id] = parseNodes[i];
			}
		}

		// bind pointers.
		for(size_t i = 0; i < parseNodes.size(); i++) {
			if(parseNodes[i]->base) {
				parseNodes[i]->base->fromParseNode(*(parseNodes[i]));
				//bases.insert(pair<string, const Base*>(parseNodes[i]->name, parseNodes[i]->base));
			}
		}

//#if 0
		// write info.
		for(size_t i = 0; i < parseNodes.size(); i++) {
			if(parseNodes[i]->base) {
				bases.insert(pair<string, Base*>(parseNodes[i]->name, parseNodes[i]->base));
				//printf("%s\n", parseNodes[i]->base->toString().c_str());
			}
		}
//#endif
	}

}
