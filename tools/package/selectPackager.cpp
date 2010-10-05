#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string>
#include <fstream>
#include "package.h"
#include "packagers.h"
#include "util.h"

using namespace std;

static bool parseRuntimeTxt(const char* filename, string& path, string& name);
static bool parseProfileHeader(const char* filename, bool& isBlackberry);

void package(const SETTINGS& s) {
	// Read runtime.txt and maprofile.h to find which runtime to use.
	// Then call the appropriate packager.

	testModel(s);

	string modelDir(mosyncdir());
	modelDir += "/profiles/vendors/";
	modelDir += s.model;
	string runtimeTxtPath = modelDir + "/runtime.txt";
	toSlashes(runtimeTxtPath);
	string headerPath = modelDir + "/maprofile.h";
	toSlashes(headerPath);

	string runtimePath, runtimeName;
	if(!parseRuntimeTxt(runtimeTxtPath.c_str(), runtimePath, runtimeName)) {
		printf("runtime.txt parse error\n");
		exit(1);
	}

	toDir(runtimePath);

	if(runtimeName == "JavaME") {
		bool isBlackberry;
		parseProfileHeader(headerPath.c_str(), isBlackberry);

		packageJavaME(s, runtimePath, isBlackberry);
		if(isBlackberry) {
			packageBlackberry(s);
		}
	} else if(runtimeName == "s60v2") {
		packageS60v2(s, runtimePath);
	} else if(runtimeName == "s60v3") {
		packageS60v3(s, runtimePath);
	} else if(runtimeName == "s60v5") {
		packageS60v3(s, runtimePath);
	} else if(runtimeName == "sp2003") {
		packageWM(s, runtimePath);
	} else if(runtimeName == "wm5") {
		packageWM(s, runtimePath);
	} else if(runtimeName == "wm6") {
		packageWM(s, runtimePath);
	} else if(runtimeName == "wm6pro") {
		packageWM(s, runtimePath);
	} else if(runtimeName == "moblin") {
		packageMoblin(s, runtimePath);
	} else if(runtimeName == "android") {
		packageAndroid(s, runtimePath);
	} else {
		printf("Error: unknown runtime '%s'\n", runtimeName.c_str());
		exit(1);
	}
}

void toSlashes(string& str) {
	for(size_t i=0; i<str.size(); i++) {
		str[i] = (str[i] == '\\' ? '/' : str[i]);
	}
}

static bool parseRuntimeTxt(const char* filename, string& path, string& name) {
	// read file
	ifstream file(filename);
	if(!file.good())
		return false;
	string line;
	getline(file, line);
	if(line[line.length()-1] == '\r')
		line = line.erase(line.length()-1, 1);
	if(!file.good())
		return false;

	// swap backslashes
	toSlashes(line);

	// set path
	path = mosyncdir() + ("/" + line);

	// check runtime number
	size_t lastSlash = line.find_last_of('/');
	if(lastSlash == string::npos)
		return false;

	string number = line.substr(lastSlash + 1);
	for(size_t i=0; i<number.size(); i++) {
		if(!isdigit(number[i]))
			return false;
	}

	// find runtime name
	size_t nameSlash = line.find_last_of('/', lastSlash - 1);
	if(nameSlash == string::npos)
		return false;
	name = line.substr(nameSlash + 1, (lastSlash - nameSlash) - 1);
	if(name.length() == 0)
		return false;

	return true;
}

static bool parseProfileHeader(const char* filename, bool& isBlackberry) {
	ifstream file(filename);
	if(!file.good())
		return false;
	isBlackberry = false;
	while(file.good()) {
		string line;
		getline(file, line);
		if(line.find("#define MA_PROF_SUPPORT_BLACKBERRY") == 0) {
			isBlackberry = true;
		}
	}
	return true;
}

void packageWM(const SETTINGS&, const std::string& runtimePath) {
	printf("not implemented\n");
	exit(1);
}
void packageMoblin(const SETTINGS&, const std::string& runtimePath) {
	printf("not implemented\n");
	exit(1);
}
void packageAndroid(const SETTINGS&, const std::string& runtimePath) {
	printf("not implemented\n");
	exit(1);
}
