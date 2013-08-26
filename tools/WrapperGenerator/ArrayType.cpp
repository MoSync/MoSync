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
