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

#include "FunctionType.h"

using namespace std;

FunctionType::FunctionType() : Base(EFunctionType) {
}

void FunctionType::fromParseNode(const ParseNode& node) {
	mReturns = getParseNodeFromId(node.getAttr("returns"))->base;
}

const Base* FunctionType::getReturnType() const {
	return mReturns;
}

string FunctionType::toString() const {
	return System::genstr("%s", mReturns->toString().c_str());
}
