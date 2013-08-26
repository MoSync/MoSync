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