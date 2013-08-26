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

#ifndef MOSYNC_BASE_PIM_H
#define MOSYNC_BASE_PIM_H

#include "helpers/cpp_defs.h"

class PimItem {
public:
	PimItem(MAHandle list) : pimList(list) {}
	virtual ~PimItem() {}

	virtual int count() const = 0;
	virtual int fieldId(int index) const = 0;
	virtual int fieldCount(int field) const = 0;
	virtual int attr(int field, int index) const = 0;
	virtual int getValue(int field, int index, void* buf, int bufSize) const = 0;
	virtual int setValue(int field, int index, void* buf, int bufSize,
		int attributes) = 0;
	virtual int addValue(int field, void* buf, int bufSize, int attributes) = 0;
	virtual int removeValue(int field, int index) = 0;
	virtual void close() = 0;

	const MAHandle pimList;
};

class PimList {
public:
	virtual ~PimList() {}

	//returns a new PimItem, which must be deleted later.
	//or NULL, if there are no more items.
	virtual PimItem* next() = 0;

	virtual int type(int field) const = 0;

	// \a list must be the handle to this list.
	// also adds the item to this list.
	virtual PimItem* createItem() = 0;

	// panics if the item is not a member of this list.
	virtual int removeItem(PimItem*) = 0;
};

int pimContactFieldType(int field);

#endif	//MOSYNC_BASE_PIM_H
