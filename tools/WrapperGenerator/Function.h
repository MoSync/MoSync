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

#ifndef _FUNCTION_H_
#define _FUNCTION_H_

#include "Base.h"
#include "Location.h"

class Argument {
public:
	void fromParseNode(const ParseNode& node);
	const std::string& getName() const;
	const Base* getType() const;
	const Location* getLocation() const;
	std::string toString() const;
	bool isEllipsis() const;

	bool usesPointer() const;
	bool usesHandle() const;
	bool isPointer() const;

protected:
	bool mIsEllipsis;
	std::string mName;
	const Base* mType;
	Location *mLocation;

	bool mUsePointer;
	bool mUseHandle;
};

class Function : public Base {
public:
	Function();
	void fromParseNode(const ParseNode& node);
	const std::string& getName() const;
	const Base* getContext() const;
	const Base* getReturnType() const;
	const Location* getLocation() const;
	bool isExtern() const;
	const std::vector<const Argument*>& getArguments() const;
	std::string toString() const;

	const std::string& getRangeExpression() const;

	bool hasPointerArguments() const;
	bool hasHandleArguments() const;

private:
	std::string mName;
	const Base *mReturns;
	const Base *mContext;
	const Location* mLocation;
	bool mIsExtern;
	std::vector<const Argument*> mArguments;

	// gcc-xml attribute.. used to describe how to calculate the amount of memory that is used from a pointer, in this case the return value.
	std::string mRangeExpression;
};

#endif // _FUNCTION_H_