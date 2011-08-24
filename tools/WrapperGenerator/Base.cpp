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
