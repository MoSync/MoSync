#include <stdio.h>
#include <string.h>
#include <stdlib.h>
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
" -m, --model <vendor>/<model> Output: target model.\n"
" -d, --dst <path>             Output: target directory.\n"
" -n, --name <name>            Output: application name.\n"
"     --vendor <name>          Output: application vendor's name.\n"
"     --debug                  Output: use debug runtime.\n"
"     --uid <8-digit hex>      Output: Symbian UID.\n"
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

int main(int argc, const char** argv) {
	SETTINGS s;
	memset(&s, 0, sizeof(s));

	for(int i=1; i<argc; i++) {
		if(streq(argv[i], "-h") || streq(argv[i], "--help")) {
			printf("%s", sUsage);
			return 1;
		} else if(streq(argv[i], "-p") || streq(argv[i], "--program")) {
			setString(i, argc, argv, s.program);
		} else if(streq(argv[i], "-r") || streq(argv[i], "--resource")) {
			setString(i, argc, argv, s.resource);
		} else if(streq(argv[i], "-m") || streq(argv[i], "--model")) {
			setString(i, argc, argv, s.model);
		} else if(streq(argv[i], "-d") || streq(argv[i], "--dst")) {
			setString(i, argc, argv, s.dst);
		} else if(streq(argv[i], "-n") || streq(argv[i], "--name")) {
			setString(i, argc, argv, s.name);
		} else if(streq(argv[i], "--vendor")) {
			setString(i, argc, argv, s.vendor);
		} else if(streq(argv[i], "--uid")) {
			setString(i, argc, argv, s.uid);
		} else if(streq(argv[i], "--debug")) {
			s.debug = true;
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
void testUid(const SETTINGS& s) {
	if(!s.uid) {
		printf("Must specify uid!\n");
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

void sh(const char* cmd) {
	printf("%s\n", cmd);
	fflush(stdout);
	fflush(stderr);
	int res = system(cmd);
	fflush(stdout);
	fflush(stderr);
	if(res != 0) {
		printf("System error %i\n", res);
		exit(res);
	}
}
