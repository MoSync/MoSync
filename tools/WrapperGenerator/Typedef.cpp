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

const Base* Typedef::resolveFully() const {
	if(mType->getBaseType() == Base::ETypedef) {
		return ((const Typedef*)mType)->resolveFully();
	} else return mType;
}