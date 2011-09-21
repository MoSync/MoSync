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
#include "util.h"

using namespace std;

static string parseBlackberryVersion(const string& v) {
	unsigned major, minor;
	int res = sscanf(v.c_str(), "%u.%u", &major, &minor);
	bool bad = false;
	bad |= (res != 2);
	bad |= major < 4;
	bad |= major > 6;
	if(bad) {
		printf("Error: bad Blackberry version (%s)\n", v.c_str());
		exit(1);
	}
	switch(major) {
	case 4:
		if(minor < 3)
			return "40";
		else
			return "47";
	case 5:
		return "500";
	case 6:
		return "600";
	default:
		exit(42);
	}
}

// reads JAD and JAR from cwd, outputs COD file to s.dst.
void packageBlackberry(const SETTINGS& s, const RuntimeInfo& ri) {
	testDst(s);
	testName(s);
	std::stringstream cmd;
	string bbdir = mosyncdir()+string("/bin/bb")+parseBlackberryVersion(ri.blackberryVersion);
	cmd << "java -Xmx256m -classpath \""<<bbdir<<"/rapc.jar\""
		" net.rim.tools.compiler.Compiler"
		" \"import="<<bbdir<<"/net_rim_api.jar\""
		" \"codename="<<s.name<<"\" -midlet"
		" \"jad="<<s.name<<".jad\" \""<<s.name<<".jar\"";
	sh(cmd.str().c_str());

	std::string codName(s.name);
	codName += ".cod";
	std::string newName = s.dst + ("/" + codName);
	remove(newName.c_str());
	renameFile(newName, codName);
}
