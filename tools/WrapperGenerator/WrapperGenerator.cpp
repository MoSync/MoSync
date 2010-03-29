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

#include <map>
#include <string>
#include <vector>
#include <stack>
#include <stdarg.h>
#include <stdio.h>
#include <fstream>

#include "Parser.h"
#include "IDLBackend.h"

using namespace std;

namespace System  {
	void error(const char* fmt, ...) {
		va_list argptr;
		va_start(argptr, fmt);
		int res = vprintf(fmt, argptr);
		va_end(argptr);

		exit(1);
	}

	string genstr(const char * fmt, ...) {
		va_list argptr;
		va_start(argptr, fmt);
		char str[1024];
		int res = vsprintf(str, fmt, argptr);
		va_end(argptr);
		return str;
	}
}

int main(int argc, char **argv) {

	Backend *backend = new IDLBackend();

	Backend::BasesMap bases;

	/*
	Parser::parse("tests/sqlite.xml", bases);
	backend->generate(bases, "tests/sqlite.idl");
*/
	Parser::parse("tests/gl.xml", bases);
	backend->generate(bases, "tests/gl.idl");
}
