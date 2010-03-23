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