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

#ifndef _WRAPPER_GENERATOR_H_
#define _WRAPPER_GENERATOR_H_

#include <stdarg.h>
#include <string>
#include <vector>
#include <map>
#include "helpers/attribute.h"

namespace System {
	std::string toUpperCase(const std::string& str);
	void error(const char* fmt, ...) PRINTF_ATTRIB(1, 2) GCCATTRIB(noreturn);
	std::string genstr(const char * fmt, ...) PRINTF_ATTRIB(1, 2);
	void split(const std::string& str, const std::string& delim, std::vector<std::string>& output);
	void parseAttributes(const std::string& str, std::map<std::string, std::string>& attr);
} // namespace System

#define SASSERT(truthiness) if(!(truthiness)) {\
	System::error("Assertion failure (" #truthiness ") on line %i in %s", __LINE__, __FILE__); }

#endif // _WRAPPER_GENERATOR_H_