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

#include "Base.h"
#include "Location.h"
#include "File.h"

using namespace std;

Base::Base(BaseType type) : mGroup(""), mBaseType(type) {
}

string Base::toString() const {
	return "";
}

Base::BaseType Base::getBaseType() const {
	return mBaseType;
}

void Base::setGroup(const std::string& group) {
	mGroup = group;
}

const std::string& Base::getGroup() const {
	return mGroup;
}


bool BaseLocationSortPredicate(const Base* d1, const Base* d2)
{
	const Location* l1 = d1->getLocation();
	const Location* l2 = d2->getLocation();
	if(!l1||!l2) return false;
	int l1Line = l1->getLine();
	int l2Line = l2->getLine();
	const File* file1 = l1->getFile();
	const File* file2 = l2->getFile();
	if(!file1||!file2) return false;

	if(d1->getGroup() > d2->getGroup())
		return true;
	else if(d1->getGroup() < d2->getGroup())
		return false;
	else if(file1->getId() > file2->getId())
		return true;
	else if(file1->getId() < file2->getId())
		return false;
	else
		return (l1Line < l2Line);
}
