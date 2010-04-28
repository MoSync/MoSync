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

#ifndef _FUNCTION_H_
#define _FUNCTION_H_

#include "Base.h"
#include "Location.h"

class Argument {
public:
	void fromParseNode(const ParseNode& node);
	const std::string& getName() const;
	const Base* getType() const;
	const Location* getLocation() const;
	std::string toString() const;
	bool isEllipsis() const;

protected:
	bool mIsEllipsis;
	std::string mName;
	const Base* mType;
	Location *mLocation;
};

class Function : public Base {
public:
	Function();
	void fromParseNode(const ParseNode& node);
	const std::string& getName() const;
	const Base* getContext() const;
	const Base* getReturnType() const;
	const Location* getLocation() const;
	bool isExtern() const;
	const std::vector<const Argument*>& getArguments() const;
	std::string toString() const;

private:
	std::string mName;
	const Base *mReturns;
	std::string mRangeExpression;
	const Base *mContext;
	const Location* mLocation;
	bool mIsExtern;
	std::vector<const Argument*> mArguments;
};

#endif // _FUNCTION_H_