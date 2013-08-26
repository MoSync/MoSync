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

#include "PointerType.h"

using namespace std;

PointerType::PointerType() : Base(EPointerType) {
}

void PointerType::fromParseNode(const ParseNode& node) {
	mType = parseType(node, mIsConst);
	SASSERT(mType);

	mSize = node.getIntAttr("size");
	mAlign = node.getIntAttr("align");
}

const Base* PointerType::getType() const {
	return mType;
}

bool PointerType::isConst() const {
	return mIsConst;
}

string PointerType::toString() const {
	return System::genstr("%s%s*", (mIsConst?"const ":""), mType->toString().c_str());
}