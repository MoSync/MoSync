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

#ifndef _ARRAY_TYPE_H_
#define _ARRAY_TYPE_H_

#include "Base.h"

class ArrayType : public Base {
public:
	ArrayType();
	void fromParseNode(const ParseNode& node);
	const Base* getType() const;
	int getMin() const;
	int getMax() const;
	int getAlign() const;
	std::string toString() const;

private: 
	std::string mMaxString;
	int mMin, mMax, mAlign;
	const Base* mType;
};

#endif // _ARRAY_TYPE_H_