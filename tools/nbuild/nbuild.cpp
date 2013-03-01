#include "nbuild.h"
#include "android.h"
#include "args.h"

#include <string>
#include <map>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

int main(int argc, const char** argv) {
	Arguments args(argc, argv);
	string platform = require(&args, "--platform");
	if (platform == "Android") {
		buildAndroidNative(&args);
	} else {
		printf("Unknown platform: %s\n", platform.c_str());
		exit(1);
	}
}

string require(Arguments* args, string key) {
	string value = args->getSwitchValue(key);
	if (value.empty()) {
		printf("Required argument missing: %s\n", key.c_str());
		exit(1);
	}
	return value;
}

const char* mosyncdir() {
	// COPY & PASTED FROM PACKAGE.CPP
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

void sh(const char* cmd, bool hideOutput, const char* shownCmdLine) {
	// COPY & PASTED FROM PACKAGE.CPP
	if (!shownCmdLine) {
		printf("%s\n", cmd);
	} else {
		printf("%s\n", shownCmdLine);
	}

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
