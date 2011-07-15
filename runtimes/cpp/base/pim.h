/* Copyright (C) 2010 Mobile Sorcery AB

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License, version 2, as published by
the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

You should have received a copy of the GNU General Public License
along with this program; see the file COPYING.  If not, write to the Free
Software Foundation, 59 Temple Place - Suite 330, Boston, MA
02111-1307, USA.
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
	virtual void setValue(int field, int index, void* buf, int bufSize,
		int attributes) = 0;
	virtual int addValue(int field, void* buf, int bufSize, int attributes) = 0;
	virtual void removeValue(int field, int index) = 0;

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
	virtual PimItem* createItem(MAHandle list) = 0;

	// panics if the item is not a member of this list.
	virtual void removeItem(PimItem*) = 0;
};

int pimContactFieldType(int field);

#endif	//MOSYNC_BASE_PIM_H
