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

#ifndef _PARSER_H_
#define _PARSER_H_

#include "ParseNode.h"
#include <string>
#include <map>

const ParseNode* getParseNodeFromId(const std::string& name);
const Base* parseType(const ParseNode& node,  bool& isconst);

namespace Parser {
	void parse(const std::string& input, std::multimap<std::string, const Base*>& output);
};

#endif // _PARSER_H_