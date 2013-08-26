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

#include "Function.h"
#include "Typedef.h"
#include "PointerType.h"

using namespace std;

bool Argument::usesPointer() const {
	return mUsePointer;
}

bool Argument::usesHandle() const {
	return mUseHandle;
}

bool Argument::isPointer() const {
	const Base* type = mType;
	if(type->getBaseType() == Base::ETypedef) type = ((const Typedef*)type)->resolveFully();
	if(type->getBaseType() == Base::EPointerType) {
		return true;
	}
	return false;
}

void Argument::fromParseNode(const ParseNode& node) {
	mType = NULL;

	if(node.name == "Ellipsis") {
		mIsEllipsis = true;
		// doesn't really support ellipsis.
		return;
	}

	mIsEllipsis = false;

	bool isConst;
	mName = node.getAttr("name", false);
	mType = parseType(node, isConst);


	// wtf? arguments don't have locations.
	//mLocation = new Location(node.getIntAttr("line"),
		//(File*)getParseNodeFromId(node.getAttr("file"))->base);
	
	mUsePointer = false;
	mUseHandle = false;

	string attributes = node.getAttr("attributes", false);
	if(attributes != "") {
		map<string, string> attrMap;
		System::parseAttributes(attributes, attrMap);
		mUsePointer = attrMap["use_pointer"]=="true";
		mUseHandle = attrMap["use_handle"]=="true";
	}
}

const string& Argument::getName() const {
	return mName;
}

const Base* Argument::getType() const {
	return mType;
}

const Location* Argument::getLocation() const {
	return mLocation;
}

string Argument::toString() const {
	if(mIsEllipsis) return "...";
	else return System::genstr("%s %s", mType->toString().c_str(), mName.c_str());
}

bool Argument::isEllipsis() const {
	return mIsEllipsis;
}

Function::Function() : Base(EFunction) {
}

bool Function::hasPointerArguments() const {
	for(size_t i = 0; i < mArguments.size(); i++) {
		if(mArguments[i]->usesPointer()) return true;
	}
	return false;
}

bool Function::hasHandleArguments() const {
	for(size_t i = 0; i < mArguments.size(); i++) {
		if(mArguments[i]->usesHandle()) return true;
	}
	return false;
}


void Function::fromParseNode(const ParseNode& node) {
	mName = node.getAttr("name");
	mReturns = getParseNodeFromId(node.getAttr("returns"))->base;
	mContext = getParseNodeFromId(node.getAttr("context"))->base;
	mIsExtern = node.getAttr("extern", false) !="";

	for(size_t i = 0; i < node.children.size(); i++) {
		const ParseNode* pn = node.children[i];
		Argument* arg = new Argument();
		arg->fromParseNode(*pn);
		mArguments.push_back(arg);
	}

	mLocation = new Location(node.getIntAttr("line"), (File*)getParseNodeFromId(node.getAttr("file"))->base);

	
	string attributes = node.getAttr("attributes", false);
	if(attributes != "") {
		map<string, string> attrMap;
		System::parseAttributes(attributes, attrMap);
		mRangeExpression = attrMap["range"];
	}
}

const std::string& Function::getRangeExpression() const {
	return mRangeExpression;
}

const string& Function::getName() const {
	return mName;
}

const Base* Function::getContext() const {
	return mContext;
}

const Base* Function::getReturnType() const {
	return mReturns;
}

const Location* Function::getLocation() const {
	return mLocation;
}

bool Function::isExtern() const {
	return mIsExtern;
}

const vector<const Argument*>& Function::getArguments() const {
	return mArguments;
}

string Function::toString() const {
	string argumentList;
	for(size_t i = 0; i < mArguments.size(); i++) {
		argumentList += mArguments[i]->toString();
		if(i!=mArguments.size()-1) argumentList+=", ";
	}

	return System::genstr("%s %s(%s)", mReturns->toString().c_str(), mName.c_str(), argumentList.c_str());
}