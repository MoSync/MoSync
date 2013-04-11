#include "android.h"
#include "util.h"
#include "nbuild.h"
#include "mustache/mustache.h"
#include "profiledb/profiledb.h"
#include "helpers/mkdir.h"
#include <map>
#include <vector>
#include <sstream>
#include <fstream>

#define TEMP_BUILD_DIR "--ios-build-dir"
#define XCODE_LOC_ARG "--ios-xcode-location"
#define LLVM_GCC_LOC "/Platforms/iPhoneOS.platform/Developer/usr/bin/llvm-gcc"

using namespace std;

int buildIOSNative(Arguments* params) {
	int result = 1;

	vector<string> sourceFiles = getSourceFiles(params);
	string sourceFileList;
	for (size_t i = 0; i < sourceFiles.size(); i++) {
		sourceFileList += sourceFiles[i];
		sourceFileList += " ";
	}
	if (sourceFiles.empty()) {
		error("No source files!\n");
	}

	vector<string> configNames;
	vector<string> libVariants;
	split(configNames, require(params, CONFIGURATION), ",");
	split(libVariants, require(params, BINARY_TYPE), ",");

	string moduleName = require(params, NAME);
	string outputDir = require(params, OUTPUT_DIR);
	toDir(outputDir);

	if (configNames.size() != libVariants.size()) {
		error("--config and --lib-variants must have a the same number of args", 2);
	}

	vector<string> archs;
	archs.push_back("armv7");
	archs.push_back("i386");

	for (size_t i = 0; i < configNames.size(); i++) {
		for (size_t j = 0; j < archs.size(); j++) {
			string configName = configNames[i];
			string libVariant = libVariants[i];
			string arch = archs[j];

			string tmpBuildDir = getTempBuildDir(params);

			bool isDebug = libVariant == "debug";
			bool isVerbose = params->isFlagSet(VERBOSE);
			bool doClean = params->isFlagSet(CLEAN);

			ostringstream cmd;
			cmd << require(params, XCODE_LOC_ARG) << LLVM_GCC_LOC << " ";

			if (isVerbose) {
				cmd << "-v ";
			}
			if (isDebug) {
				cmd << "-O0 -g ";
			}
			else {
				cmd << "-O2 ";
			}

			string libDir = string(mosyncdir()) + "/lib";

			cmd << "-arch " + archs[j] << " ";
			cmd << "-fmessage-length=0 -pipe -std=gnu99 -Wno-trigraphs -fpascal-strings -gdwarf-2 -mthumb -miphoneos-version-min=6.1" << " ";
			cmd << sourceFileList << " ";
			cmd << "-o userCode.a" << " ";

			sh(cmd.str().c_str(), !isVerbose);

			/*
			string fullOutputDir = outputDir + "android_" + arch + "_" + libVariant + "/";
			_mkdir(fullOutputDir.c_str());
			// TODO: Static libs
			string libFile = tmpBuildDir + "libs/" + arch + "/lib" + moduleName + ".so";
			copyFile((fullOutputDir + "lib" + moduleName + ".so").c_str(), libFile.c_str());
			*/
		}
	}
	return result;
}

string getTempBuildDir(Arguments* params) {
	string tmpDir = params->getSwitchValue(PROJECT_DIR);
	if (tmpDir.empty()) {
		tmpDir = params->getSwitchValue(TEMP_BUILD_DIR);
		toDir(tmpDir);
	} else {
		toDir(tmpDir);
		tmpDir += "temp/";
	}
	return tmpDir;
}