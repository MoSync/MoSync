/* Copyright (C) 2009 Mobile Sorcery AB

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

#include <stdlib.h>
#include <stdio.h>

#include "idl-common.h"

class idl_exception : public exception {
public:
	idl_exception(const string& str) throw() : mStr(str) { }

	virtual ~idl_exception() throw() {}
	virtual const char* what() const throw() { return mStr.c_str(); }
protected:
	string mStr;
};

void throwException(const string& msg) {
	throw idl_exception(msg.c_str());
}

void runCommand(const string& cmd) {
	printf("%s\n", cmd.c_str());
	int res = system(cmd.c_str());
	if(res != 0) {
		throwException("External error!");
	}
}
