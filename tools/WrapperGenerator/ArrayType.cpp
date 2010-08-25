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

#include "ArrayType.h"
#include <stdio.h>

using namespace std;

ArrayType::ArrayType() : Base(EArrayType) {
}

void ArrayType::fromParseNode(const ParseNode& node) {
	bool isConst;
	mType = parseType(node, isConst);
	mMin = node.getIntAttr("min");
	mMaxString = node.getAttr("max");
	//mMax = atoi(mMaxString.c_str());
	int res = sscanf(mMaxString.c_str(), "%i", &mMax);
	SASSERT(res == 1);
	mAlign = node.getIntAttr("align");
}


const Base* ArrayType::getType() const {
	return mType;
}

int ArrayType::getMin() const {
	return mMin;
}

int ArrayType::getMax() const {
	return mMax;
}

int ArrayType::getAlign() const {
	return mAlign;
}

string ArrayType::toString() const {
	return System::genstr("%s[%s]", mType->toString().c_str(), mMaxString.c_str());
}
