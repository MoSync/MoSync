#include "android.h"
#include "ios.h"
#include "util.h"
#include "nbuild.h"
#include "mustache/mustache.h"
#include "profiledb/profiledb.h"
#include "helpers/mkdir.h"
#include <map>
#include <vector>
#include <sstream>
#include <fstream>

#define XCODE_LOC_ARG "--ios-xcode-location"
#define LLVM_GCC_LOC "/Platforms/iPhoneOS.platform/Developer/usr/bin/llvm-gcc"
//#define LLVM_GCC_LOC "/Toolchains/XcodeDefault.xctoolchain/usr/bin/clang"

using namespace std;

int buildIOSNative(Arguments* params) {
	int result = 1;

	string projectDir = require(params, "--project");
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
	string outputDir = require(params, "--dst");
	unsigned int slashPos = 0;
	do {
		slashPos = outputDir.find('/', slashPos + 1);
		_mkdir(outputDir.substr(0,slashPos).c_str());
	}while(slashPos != string::npos);

	//toDir(outputDir);

	if (configNames.size() != libVariants.size()) {
		error("--config and --lib-variants must have a the same number of args", 2);
	}

	bool isVerbose = params->isFlagSet(VERBOSE);

	vector<string> archs;
	archs.push_back("armv7");
	archs.push_back("i386");

	string oFileList;

	for (size_t i = 0; i < configNames.size(); i++) {
		string configName = configNames[i];
		string libVariant = libVariants[i];
		string libDir = string(mosyncdir()) + "/lib/ios_" + libVariant;
		bool isDebug = libVariant == "debug";
		copyFile((outputDir + "/MoSyncLibs.a").c_str(), (libDir + "/MoSyncLibs.a").c_str() );

		for (size_t j = 0; j < archs.size(); j++) {
			string arch = archs[j];

			string tmpBuildDir = getTempBuildDir(params);

			//bool doClean = params->isFlagSet(CLEAN);

			string archSDKName = (archs[j] == "i386")?"iPhoneSimulator":"iPhoneOS";
			string sdkVersion = "6.1";

			string runtimeDir = string(mosyncdir()) + "/profiles/runtimes/iphoneos/1/template/libs/" + configNames[i] + "-" + archSDKName;
			string headerDir = string(mosyncdir()) + "/include";
			string nativeHeaderDir = headerDir + "/MAStdNative";

			//for (size_t k = 0; k < sourceFiles.size(); k++) {
			ostringstream cmd;
			//cmd << require(params, XCODE_LOC_ARG) << LLVM_GCC_LOC << " ";
			cmd << "/Applications/Xcode.app/Contents/Developer" << LLVM_GCC_LOC << " ";
			if (isVerbose) {
				cmd << "-v ";
			}
			if (isDebug) {
				cmd << "-g ";
			}

			string outputFile = string("userCode") + arch + ".o";
			oFileList += outputDir + "/" + outputFile + " ";

			cmd << "-arch " + archs[j] << " ";


			cmd << "-Xlinker -r -nostdlib -isysroot /Applications/Xcode.app/Contents/Developer/Platforms/" << archSDKName << ".platform/Developer/SDKs/" << archSDKName << sdkVersion << ".sdk -miphoneos-version-min=" << sdkVersion << " ";
			cmd << "-D__IOS__ -DUSE_NEWLIB -DMOSYNC_NATIVE" << " ";
			cmd << "-I" << nativeHeaderDir << " ";
			//cmd << "-I" << headerDir << " ";
			cmd << "-I" << outputDir << " ";
			cmd << sourceFileList << " ";
			//cmd <<  libDir + "/MoSyncLibs.a" << " ";
			//cmd <<  runtimeDir + "/libMoSyncLib.a" << " ";
			cmd << "-o " << outputDir << "/" << outputFile << " ";
			//cmd << sourceFiles[k] << " ";
			//cmd << "-o " << outputDir << "/" << sourceFiles[k] << ".o" << " ";
			sh(cmd.str().c_str(), !isVerbose);
			//}



			/*
			string fullOutputDir = outputDir + "android_" + arch + "_" + libVariant + "/";
			_mkdir(fullOutputDir.c_str());
			// TODO: Static libs
			string libFile = tmpBuildDir + "libs/" + arch + "/lib" + moduleName + ".so";
			copyFile((fullOutputDir + "lib" + moduleName + ".so").c_str(), libFile.c_str());
			*/
		}
	}

	ostringstream cmd;
	cmd << "lipo " << oFileList << " -create -output " + outputDir + "/" + "userCode.o";
	sh(cmd.str().c_str(), !isVerbose);
	return result;
}