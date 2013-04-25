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

#define LLVM_GCC_LOC "/Platforms/iPhoneOS.platform/Developer/usr/bin/llvm-gcc"

using namespace std;

int buildIOSNative(Arguments* params) {
	int result = 1;

	string xcodeLocation = "/Applications/Xcode.app/Contents/Developer/"; //Should be taked from the command options eventually
	string sdkVersion = "6.1"; //Should be taked from the command options eventually
	string projectDir = require(params, "--project");
	string finalOutputFileName = "userCode.o"; //Should be taked from the command options eventually

	//Special header folder for the native ios build, which does not include headers
	//that already exist in the iphone SDK
	string nativeHeaderDir = string(mosyncdir()) + "/include/MAStdNative";

	//Gather the source files in a string
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

	//Recursively create the output dir
	string outputDir = require(params, "--dst");
	size_t slashPos = 0;
	do {
		slashPos = outputDir.find('/', slashPos + 1);
		_mkdir(outputDir.substr(0,slashPos).c_str());
	}while(slashPos != string::npos);

	if (configNames.size() != libVariants.size()) {
		error("--config and --lib-variants must have a the same number of args", 2);
	}

	bool isVerbose = params->isFlagSet(VERBOSE);

	//List of architectures to build
	vector<string> archs;
	archs.push_back("armv7");
	archs.push_back("armv7s");
	archs.push_back("i386"); //That's the simulator

	for (size_t i = 0; i < configNames.size(); i++) {
		string configName = configNames[i];
		string libVariant = libVariants[i];
		string libDir = string(mosyncdir()) + "/lib/ios_" + libVariant;
		bool isDebug = libVariant == "debug";

		//Copy the MoSync libraries. It's not used by this tool (yet), but needed for the packaging step.
		copyFile((outputDir + "/MoSyncLibs.a").c_str(), (libDir + "/MoSyncLibs.a").c_str() );

		//List of output files to be passed to the lipo tool
		string oFileList = "";

		//We make a build for each architecture
		for (size_t j = 0; j < archs.size(); j++) {
			string arch = archs[j];

			//Simulator uses a different sdk
			string archSDKName = (archs[j] == "i386")?"iPhoneSimulator":"iPhoneOS";

			string outputFile = outputDir + string("/userCode") + arch + ".o";
			oFileList += outputFile + " ";

			ostringstream cmd; //The build command
			cmd << xcodeLocation << LLVM_GCC_LOC << " "; //path to llvm-gcc
			if (isVerbose) {
				cmd << "-v "; //verbose output
			}
			if (isDebug) {
				cmd << "-g -O0 -DDEBUG=1 "; //debug mode
			}
			else
			{
				cmd << "-Os ";
			}

			cmd << "-arch " + arch << " "; //The architecture to build
			cmd << "-Xlinker -r "; //Tell the linker to just bundle all the .o files into one .o instead of linking/assembling
			cmd << " -nostdlib "; //Don't link with stdlib, xcode will do that
			cmd << "-isysroot /Applications/Xcode.app/Contents/Developer/Platforms/" << archSDKName << ".platform/Developer/SDKs/" << archSDKName << sdkVersion << ".sdk ";
			cmd << "-miphoneos-version-min=" << sdkVersion << " ";
			cmd << "-D__IOS__ -DUSE_NEWLIB -DMOSYNC_NATIVE" << " ";
			cmd << "-I" << nativeHeaderDir << " ";
			cmd << "-I" << outputDir << " ";
			cmd << sourceFileList << " ";
			cmd << "-o " << outputFile << " ";
			sh(cmd.str().c_str(), !isVerbose);
		}
		ostringstream cmd;
		//Combine all architecture files into one universal .o file
		cmd << "lipo " << oFileList << " -create -output " + outputDir + "/" + finalOutputFileName;
		sh(cmd.str().c_str(), !isVerbose);
	}
	return result;
}