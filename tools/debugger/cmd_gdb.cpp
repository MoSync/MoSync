/* Copyright 2013 David Axmark

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

	http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

#include <string>
#include <stdlib.h>

#include "commandInterface.h"
#include "helpers.h"
#include "StubConnLow.h"

using namespace std;

void gdb_exit(const string& args) GCCATTRIB(noreturn);
void gdb_version(const string& args);
void interpreter_exec(const string& args);
void gdb_set(const string& args);
void gdb_show(const string& args);

void gdb_exit(const string& args) {
	oprintToken();
	oprintf("^exit\n");
	StubConnLow::sendPacket("e", NULL);
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


// we don't take any gdb internal variables into account yet..
void gdb_set(const string& args) {
	oprintDoneLn();
	commandComplete();
}

void gdb_show(const string& args) {
	oprintDoneLn();
	commandComplete();
}
