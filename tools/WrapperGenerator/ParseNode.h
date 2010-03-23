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

#ifndef _PARSE_NODE_H_
#define _PARSE_NODE_H_

#include <string>
#include <map>
#include <vector>

class Base;

struct ParseNode {
	ParseNode();
	std::string getAttr(const std::string& name, bool force=true) const;

	int getIntAttr(const std::string& name, bool force=true) const;

	std::string name;
	std::map<std::string, std::string> attributes;
	std::vector<const ParseNode*> children;

	ParseNode *parent;

	Base* base;

	int lineNumber;
};

#endif // _PARSE_NODE_H_