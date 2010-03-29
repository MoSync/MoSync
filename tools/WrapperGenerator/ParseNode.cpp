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
#include "WrapperGenerator.h"
#include "Parser.h"

using namespace std;

ParseNode::ParseNode() : parent(NULL) {
}

string ParseNode::getAttr(const string& name, bool force) const {
	map<string, string>::const_iterator iter = attributes.find(name);
	if(iter != attributes.end()) return iter->second;
	else if(force) {
		System::error(System::genstr("Attribute '%s' missing on line %d\n", name.c_str(), lineNumber).c_str());
	}
	return "";
}

int ParseNode::getIntAttr(const string& name, bool force) const {
	string attr = getAttr(name, force);
	return atoi(attr.c_str());
}

void ParseNode::getNodesFromIdList(const std::string& name, vector<const Base*>& output, bool force) const {
	
	const string& members = getAttr(name, force);

	size_t startIndex = 0;
	size_t endIndex = 0;
	while((endIndex=members.find_first_of(" ", startIndex)) != string::npos) {
		string member = members.substr(startIndex, endIndex-startIndex);
		startIndex = endIndex + 1;
		output.push_back(getParseNodeFromId(member)->base);
	}
}
