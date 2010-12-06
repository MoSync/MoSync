/* Copyright (C) 2010 MoSync AB

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

#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include "packagers.h"

// reads JAD and JAR from cwd, outputs COD file to s.dst.
void packageBlackberry(const SETTINGS& s, const RuntimeInfo& ri) {
	testDst(s);
	testName(s);
	std::stringstream cmd;
	cmd << "java -Xmx256m -classpath \""<<mosyncdir()<<"/bin/bb40/rapc.jar\""
		" net.rim.tools.compiler.Compiler"
		" \"import="<<mosyncdir()<<"/bin/bb40/net_rim_api.jar\""
		" \"codename="<<s.name<<"\" -midlet"
		" \"jad="<<s.name<<".jad\" \""<<s.name<<".jar\"";
	sh(cmd.str().c_str());

	std::string codName(s.name);
	codName += ".cod";
	int res = rename(codName.c_str(), (s.dst + ("/" + codName)).c_str());
	if(res != 0) {
		printf("rename error %i, %i\n", res, errno);
		exit(1);
	}
}
