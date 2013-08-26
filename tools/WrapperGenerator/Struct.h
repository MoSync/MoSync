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

#ifndef _STRUCT_H_
#define _STRUCT_H_

#include "Namespace.h"
#include "Location.h"

class Struct : public Namespace {
public:
	Struct();

	void fromParseNode(const ParseNode& node);
	const Location* getLocation() const;

private:
	std::vector<const Base*> mBases;
	const Location *mLocation;
	int mSize, mAlign;
	bool mIsArtificial;
	bool mIncomplete;

};

#endif // _STRUCT_H_