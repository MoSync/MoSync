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
