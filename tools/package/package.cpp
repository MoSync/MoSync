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

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <string>

#include "package.h"

static const char* sUsage =
"This program turns compiled MoSync programs into packages that can be\n"
"installed on various portable devices.\n"
"\n"
"Usage: package <OPTIONS>\n"
"\n"
"Options:\n"
" -h, --help                   Print this information.\n"
" -p, --program <file>         Input: compiled program file.\n"
" -r, --resource <file>        Input: compiled resource file.\n"
" -i, --icon <file>            Input: MoSync icon definition file (XML).\n"
" -m, --model <vendor>/<model> Output: target model.\n"
" -d, --dst <path>             Output: target directory.\n"
" -n, --name <name>            Output: application name.\n"
"     --vendor <name>          Output: application vendor's name.\n"
"     --version <version>      Output: application version. Format: major[.minor][.micro][.qualifier]\n"
"                              Some parts may be ignored by some platforms.\n"
"     --debug                  Output: use debug runtime.\n"
"     --s60v3uid <8-digit hex> Output: Symbian UID, 3rd edition.\n"
"     --s60v2uid <8-digit hex> Output: Symbian UID, 2nd edition.\n"
" -s, --silent                 Output: Supress additional output, e.g. warnings."
"\n"
"Environment variables used:\n"
" MOSYNCDIR                    Path to the MoSync installation directory.\n"
;

static bool streq(const char* a, const char* b) {
	return strcmp(a, b) == 0;
}

static void setString(int& i, int argc, const char** argv, const char*& dst) {
	i++;
	if(i == argc) {
		printf("Too few arguments!\n");
		exit(1);
	}
	dst = argv[i];
}

static void onExit() {
	printf("onExit\n");
}

int main(int argc, const char** argv) {
	SETTINGS s;
	memset(&s, 0, sizeof(s));

	atexit(&onExit);

	if(argc == 1) {
		printf("%s", sUsage);
		return 1;
	}

	for(int i=1; i<argc; i++) {
		if(streq(argv[i], "-h") || streq(argv[i], "--help")) {
			printf("%s", sUsage);
			return 1;
		} else if(streq(argv[i], "-p") || streq(argv[i], "--program")) {
			setString(i, argc, argv, s.program);
		} else if(streq(argv[i], "-r") || streq(argv[i], "--resource")) {
			setString(i, argc, argv, s.resource);
		} else if(streq(argv[i], "-i") || streq(argv[i], "--icon")) {
			setString(i, argc, argv, s.icon);
		} else if(streq(argv[i], "-m") || streq(argv[i], "--model")) {
			setString(i, argc, argv, s.model);
		} else if(streq(argv[i], "-d") || streq(argv[i], "--dst")) {
			setString(i, argc, argv, s.dst);
		} else if(streq(argv[i], "-n") || streq(argv[i], "--name")) {
			setString(i, argc, argv, s.name);
		} else if(streq(argv[i], "--vendor")) {
			setString(i, argc, argv, s.vendor);
		} else if(streq(argv[i], "--version")) {
			setString(i, argc, argv, s.version);
		} else if(streq(argv[i], "--permissions")) {
			setString(i, argc, argv, s.permissions);
		} else if(streq(argv[i], "--s60v3uid")) {
			setString(i, argc, argv, s.s60v3uid);
		} else if(streq(argv[i], "--s60v2uid")) {
			setString(i, argc, argv, s.s60v2uid);
		} else if(streq(argv[i], "--debug")) {
			s.debug = true;
		} else if(streq(argv[i], "--ios-cert")) { // iOS specific
			setString(i, argc, argv, s.iOSCert);
		} else if(streq(argv[i], "--ios-sdk")) { // iOS specific
			setString(i, argc, argv, s.iOSSdk);
		} else if(streq(argv[i], "--project-only")) { // iOS specific
			s.iOSgenerateOnly = true;
		} else if(streq(argv[i], "-s") || streq(argv[i], "--silent")) {
			s.silent = true;
		} else {
			printf("Unknown argument: '%s'\n", argv[i]);
			return 1;
		}
	}

	package(s);
	return 0;
}

void testProgram(const SETTINGS& s) {
	if(!s.program) {
		printf("Must specify program!\n");
		exit(1);
	}
}
void testModel(const SETTINGS& s) {
	if(!s.model) {
		printf("Must specify model!\n");
		exit(1);
	}
}
void testDst(const SETTINGS& s) {
	if(!s.dst) {
		printf("Must specify target directory!\n");
		exit(1);
	}
}
void testName(const SETTINGS& s) {
	if(!s.name) {
		printf("Must specify name!\n");
		exit(1);
	}
}
void testVendor(const SETTINGS& s) {
	if(!s.vendor) {
		printf("Must specify vendor!\n");
		exit(1);
	}
}
void testVersion(const SETTINGS& s) {
	if (!s.version) {
		printf("Must specify version!\n");
		exit(1);
	}
}
static void testUid(const char* uid) {
	if(strlen(uid) == 8) {
		bool good = true;
		for(int i=0; i<8; i++) {
			if(!isxdigit(uid[i]))
				good = false;
		}
		if(good)
			return;
	}
	printf("Invalid UID format!\n");
	exit(1);
}
void testS60v3Uid(const SETTINGS& s) {
	if(!s.s60v3uid) {
		printf("Must specify s60v3uid!\n");
		exit(1);
	}
	testUid(s.s60v3uid);
}
void testS60v2Uid(const SETTINGS& s) {
	if(!s.s60v2uid) {
		printf("Must specify s60v2uid!\n");
		exit(1);
	}
	testUid(s.s60v2uid);
}
void testIOSCert(const SETTINGS& s) {
	if(!s.iOSCert) {
		printf("Must specify iOS certificate!\n");
		exit(1);
	}
}
void testIOSSdk(const SETTINGS& s) {
	if(!s.iOSSdk) {
		printf("Must specify iOS SDK!\n");
		exit(1);
	}
}

const char* mosyncdir() {
	static const char* md = NULL;
	if(!md) {
		md	= getenv("MOSYNCDIR");
		if(!md) {
			printf("MOSYNCDIR is not set!\n");
			exit(1);
		}
	}
	return md;
}

void sh(const char* cmd, bool hideOutput) {
	printf("%s\n", cmd);
	fflush(stdout);
	fflush(stderr);

	std::string finalCmd(cmd);
	if(hideOutput) {
		// Supress output from stdout and stderr
#ifdef WIN32
		finalCmd += "> nul 2>&1";
#else
		finalCmd += "> /dev/null 2>&1";
#endif
	}

	int res = system(finalCmd.c_str());

	fflush(stdout);
	fflush(stderr);
	if(res != 0) {
		printf("System error %i\n", res);
		exit(res);
	}
}
