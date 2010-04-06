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

#include "CvQualifiedType.h"

using namespace std;

CvQualifiedType::CvQualifiedType() : Base(ECvQualifiedType) {
}

void CvQualifiedType::fromParseNode(const ParseNode& node) {
	mType = parseType(node, mIsConst);
	if(!mType) {
		int a = 2;
	}

	string c = node.getAttr("const", false);
	if(!mIsConst && c=="1")
		mIsConst = true;	
}

string CvQualifiedType::toString() const {
	return System::genstr("%s%s*", (mIsConst?"const ":""), mType->toString().c_str());
}

const Base* CvQualifiedType::getType() const {
	return mType;
}

bool CvQualifiedType::isConst() const {
	return mIsConst;
}