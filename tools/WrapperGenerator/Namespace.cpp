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

#include "Namespace.h"

using namespace std;

Namespace::Namespace() : Base(ENamespace){
}

void Namespace::addMember(const Base* id) {
	mMembers.push_back(id);
}


void Namespace::fromParseNode(const ParseNode& node) {
	mName = node.getAttr("name");
	const string& members = node.getAttr("members");

	size_t startIndex = 0;
	size_t endIndex = 0;
	while((endIndex=members.find_first_of(" ", startIndex)) != string::npos) {
		string member = members.substr(startIndex, endIndex-startIndex);
		startIndex = endIndex + 1;
		mMembers.push_back(getParseNodeFromId(member)->base);
	}

	string context = node.getAttr("context", false);
	mContext = (context!="")?getParseNodeFromId(context)->base:NULL;
}

string Namespace::toString() const {
	return System::genstr("%s", mName.c_str());			
}

const string& Namespace::getName() const {
	return mName;
}

const Base* Namespace::getContext() const {
	return mContext;
}

const vector<const Base*>& Namespace::getMembers() const {
	return mMembers;
}
