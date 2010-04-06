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

private:
	std::string mName;
	const Base* mType;
	const Base* mContext;
	const Location *mLocation;
};

#endif // _TYPEDEF_H_