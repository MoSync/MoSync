#include "nbuild.h"
#include "android.h"
#include "ios.h"
#include "args.h"
#include "fileset.h"

#include <string>
#include <map>
#include <set>
#include <vector>
#include <stdio.h>
#include <stdlib.h>

#include <filelist/filelist.h>

using namespace std;

int main(int argc, const char** argv) {
	Arguments args(argc, argv);
	string platform = require(&args, "--platform");
	if (platform == "Android") {
		buildAndroidNative(&args);
	} else if (platform == "iOS") {
		buildIOSNative(&args);
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

vector<string> getSourceFiles(Arguments* args) {
	string projectPath = require(args, PROJECT_DIR);

	vector<string> argList = args->getPrefixedList("-", false);

	FileSetList fs;

	vector<string> cExtensions;
	cExtensions.push_back("cpp");
	cExtensions.push_back("c");
	ExtensionFileFilter filter(cExtensions);

	for (size_t i = 0; i < argList.size(); i++) {
		string arg = argList[i];
		bool isExclusion = arg[0] == 'X';
		if (arg[0] == 'S' || isExclusion) {
			string file = arg.substr(1);
			string fullPath = projectPath + "/" + file;
			// TODO: delete dfs!
			DefaultFileSet* dfs = new DefaultFileSet(projectPath, file, isExclusion, &filter);
			fs.addFileSet(dfs);
		}
	}

	vector<string> result;
	fs.listFiles(result);
	return result;
}

string validateConfiguration(string& configName) {
	for (size_t i = 0; i < configName.size(); i++) {
		char ch = configName[i];
		if (ch == ' ') {
			return "Spaces are not allowed in configuration names!";
		}
		if (!isalnum(ch) && ch != '-' && ch != '_') {
			return "Invalid character in configuration name!"
				" Only letters, digits, hyphen and underscore allowed.";
		}
	}
	return "";
}

bool isSTL(string& moduleName) {
	return "stlport" == moduleName;
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

int sh(const char* cmd, bool hideOutput, const char* shownCmdLine, bool exitOnError) {
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
	if(res != 0 && exitOnError) {
		printf("System error %i\n", res);
		exit(res);
	}
	return res;
}

void error(const char* msg, int errorCode) {
	puts(msg);
	puts("\n");
	exit(errorCode);
}
