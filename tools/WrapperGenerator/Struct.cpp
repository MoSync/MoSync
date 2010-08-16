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

#include "Struct.h"

using namespace std;

Struct::Struct() {
	mBaseType = EStruct;
}

void Struct::fromParseNode(const ParseNode& node) {
	mIncomplete = node.getIntAttr("incomplete", false)==1;
	Namespace::fromParseNode(node);
	node.getNodesFromIdList("bases", mBases, false);
	mLocation = new Location(node.getIntAttr("line"), (File*)getParseNodeFromId(node.getAttr("file"))->base);
	mSize = node.getIntAttr("size", false);
	mAlign = node.getIntAttr("align", false);
	mIsArtificial = node.getIntAttr("artificial", false)==1;
}

const Location* Struct::getLocation() const {
	return mLocation;
}
