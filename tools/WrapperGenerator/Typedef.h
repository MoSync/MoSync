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

#ifndef _TYPEDEF_H_
#define _TYPEDEF_H_

#include "Base.h"
#include "Location.h"

class Typedef : public Base {
public:
	Typedef();
	void fromParseNode(const ParseNode& node);
	const std::string& getName() const;
	const Base* getType() const;
	const Base* getContext() const;
	const Location* getLocation() const;
	std::string toString() const;

	const Base* resolveFully() const;

private:
	std::string mName;
	const Base* mType;
	const Base* mContext;
	const Location *mLocation;
};

#endif // _TYPEDEF_H_