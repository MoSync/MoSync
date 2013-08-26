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

#ifndef _FILE_H_
#define _FILE_H_

#include "Base.h"

class File : public Base {
public:
	File();
	void fromParseNode(const ParseNode& node);
	const std::string& getName() const;
	const std::string& getId() const;
	std::string toString() const;

private:
	std::string mName;
	std::string mId;
};

#endif // _FILE_H_
