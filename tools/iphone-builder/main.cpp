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
#include <stdarg.h>

using namespace std;

#if defined (__MACH__) || defined(__APPLE__) 
#define PLATFORM_OSX
#endif

static const char sInfo[] =
" iphone-builder\n"
" author: Romain Chalant and Niklas Nummelin\n"
"\n"
" usage:\n"
" iphone-builder <mode> <mode-specific-options>\n"
"\n"
" modes:\n"
"   generate        generates an xcode project.\n"
"   build           builds an xcode project using xcode (mac only).\n"
"   run             runs an iphone application built using xcode (mac only).\n"
"\n"
" generate options:\n"
"   -input          specifies the input template.\n"
"   -project-name   the name of the project.\n"
"   -output         specifies the filename of the xcode project.\n"
"\n"
" build options:\n"
"   -input          specifies the filename of the xcode project.\n"
"   -output         specifies where to put the output.\n"
"\n"
" run options:\n"
"   -input          specifies where the application is.\n";

static map<string, string> sArguments;

void error(const char* fmt, ...) {
	va_list argptr;
	va_start(argptr, fmt);
	vprintf(fmt, argptr);
	va_end(argptr);

	exit(1);
}


void generate() {
}

#ifdef PLATFORM_OSX
void build() {
}

void run() {
}
#else
void build() {
	error("Incompatible platform for mode 'build'.\n");
}
void run() {
	error("Incompatible platform for mode 'run'.\n");
}
#endif


int main(int argc, char **argv) {
	if(argc<2) {
		error("%s", sInfo);
	}

	// parse arguments
	for(int i = 2; i < argc; i++) {
		if(argv[i][0] == '-') {
			i++;
			if(i>=argc) error("Missing value for option %s", argv[i-1]);
			sArguments[&argv[i-1][1]] = argv[i];
		} else {
			error("Invalid option %s", argv[i]);
		}
	}

	// get mode
	string mode;
	mode = argv[1];

	if(mode == "generate") {
		generate();
	} else if(mode == "build") {
		build();
	} else if(mode == "run") {
		run();
	}


}