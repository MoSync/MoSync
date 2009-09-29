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

#include <string>
#include <stdlib.h>

#include "commandInterface.h"
#include "helpers.h"

using namespace std;

void gdb_exit(const string& args);
void gdb_version(const string& args);
void interpreter_exec(const string& args);
void gdb_set(const string& args);

void gdb_exit(const string& args) {
	oprintToken();
	oprintf("^exit\n");
	exit(0);
}
void gdb_version(const string& args) {
	oprintf(
		"~Mobile Sorcery mdb 0.1\\n\n"
		"~Copyright 2009 Mobile Sorcery AB\\n\n");
	oprintDoneLn();
	commandComplete();
}

void interpreter_exec(const string& args) {
	static const string whatisString("whatis ");

	vector<string> argv;
	splitArgs(args, argv);
	if(argv.size() != 2) {
		error("Need 2 arguments");
		return;
	}
	if(argv[0] != "console") {
		error("Unknown interpreter");
		return;
	}
	if(argv[1] == "set unwindonsignal on") {
		//ignored, because mdb doesn't support calling program functions yet.
	} else if(argv[1] == "set breakpoint pending on") {
		//ignored, because MoSync doesn't have dynamic loading. maLoadProgram() doesn't count.
	} else if(argv[1] == "show endian") {
		//copy from GDB, reformatted for interpreter-exec.
		oprintf("~The target endianness is set automatically (currently little endian)\\n\n");
	} else if(beginsWith(argv[1], whatisString)) {
		string what = argv[1].substr(whatisString.length());
		oprintf("~No symbol \"%s\" in current context.\\n\n", what.c_str());
	} else {
		error("Unknown console command");
		return;
	}
	oprintDoneLn();
	commandComplete();
}

void gdb_set(const string& args) {
	oprintDoneLn();
	commandComplete();
}
