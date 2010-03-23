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

#include "Typedef.h"

using namespace std;

Typedef::Typedef(): Base(ETypedef) {
}

void Typedef::fromParseNode(const ParseNode& node) {
	mName = node.getAttr("name");
	mContext = getParseNodeFromId(node.getAttr("context"))->base;
	mLocation = new Location(node.getIntAttr("line"), (File*)getParseNodeFromId(node.getAttr("file"))->base);
	mType = getParseNodeFromId(node.getAttr("type"))->base;
}

const string& Typedef::getName() const {
	return mName;
}

const Base* Typedef::getType() const {
	return mType;
}

const Base* Typedef::getContext() const {
	return mContext;
}

const Location* Typedef::getLocation() const {
	return mLocation;
}

string Typedef::toString() const {
	return System::genstr("%s", mName.c_str());
}