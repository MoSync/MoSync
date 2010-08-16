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

#include "Field.h"

using namespace std;

Field::Field() : Base(EField) {
}

void Field::fromParseNode(const ParseNode& node) {
	mType = NULL;

	bool isConst;
	mName = node.getAttr("name", false);
	mType = parseType(node, isConst);
	mLocation = new Location(node.getIntAttr("line"), (File*)getParseNodeFromId(node.getAttr("file"))->base);

	string context = node.getAttr("context", false);
	mContext = (context!="")?getParseNodeFromId(context)->base:NULL;

	mOffset = node.getIntAttr("offset", false);

	mAccess = node.getAttr("access", false);
}

const string& Field::getName() const {
	return mName;
}

const Base* Field::getType() const {
	return mType;
}

const Location* Field::getLocation() const {
	return mLocation;
}

string Field::toString() const {
	return System::genstr("%s %s", mType->toString().c_str(), mName.c_str());
}