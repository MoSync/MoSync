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
#include <iterator>

#define LLVM_GCC_LOC "/Platforms/iPhoneOS.platform/Developer/usr/bin/llvm-gcc"
#define OUTPUT_FILE_NAME "userCode.o"

using namespace std;

int compileIOSUserCode(Arguments* params) {
	int result = 1;

	string xcodeLocation = require(params, IOS_XCODE_LOCATION);
	string sdkVersion = require(params, IOS_SDK_VERSION);
	string projectDir = require(params, PROJECT_DIR);

	//Special header folder for the native ios build, which does not include headers
	//that already exist in the iphone SDK
	string nativeHeaderDir = string(mosyncdir()) + "/include/MAStdNative";

	typedef vector<string>::const_iterator vctrStrCIter;

	//Gather the source files in a string
	vector<string> sourceFiles = getSourceFiles(params);
	stringstream sourceFileList;
	if (!sourceFiles.empty()) {
		copy(sourceFiles.begin(), sourceFiles.end(), ostream_iterator<string>(sourceFileList," "));
	}
	else {
		error("No source files!\n");
	}

	vector<string> compilerDefines = params->getPrefixedList(MACRO_DEFINES, true);
	stringstream compilerDefinesList;
	copy(compilerDefines.begin(), compilerDefines.end(), ostream_iterator<string>(compilerDefinesList," "));

	vector<string> additionalIncludes = params->getPrefixedList(ADDITIONAL_INCLUDES, true);
	stringstream additionalIncludesList;
	copy(additionalIncludes.begin(), additionalIncludes.end(), ostream_iterator<string>(additionalIncludesList," "));

	//Not used yet
	vector<string> additionalLibPaths = params->getPrefixedList(ADDITIONAL_LIB_PATHS, true);
	stringstream additionalLibPathsList;
	copy(additionalLibPaths.begin(), additionalLibPaths.end(), ostream_iterator<string>(additionalLibPathsList," "));


	vector<string> configNames;
	vector<string> libVariants;
	split(configNames, require(params, CONFIGURATION), ",");
	split(libVariants, require(params, BINARY_TYPE), ",");

	string moduleName = require(params, NAME);

	//Recursively create the output dir
	string outputDir = require(params, OUTPUT_DIR);
	size_t slashPos = 0;
	do {
		slashPos = outputDir.find('/', slashPos + 1);
		_mkdir(outputDir.substr(0,slashPos).c_str());
	}while(slashPos != string::npos);

	if (configNames.size() != libVariants.size()) {
		error("--config and --lib-variants must have a the same number of args", 2);
	}

	bool isVerbose = params->isFlagSet(VERBOSE);
	string compilerSwitches = params->getSwitchValue(COMPILER_SWITCHES);

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
			string archSDKName = (arch == "i386")?"iPhoneSimulator":"iPhoneOS";

			string outputFile = outputDir + string("/userCode") + arch + ".o";
			sh((string("rm -f ") + outputFile).c_str());
			oFileList += outputFile + " ";

			ostringstream cmd; //The build command
			cmd << xcodeLocation << LLVM_GCC_LOC << " "; //path to llvm-gcc
			if (isVerbose) {
				cmd << "-v "; //verbose output
			}
			if (isDebug) {
				cmd << "-g -DDEBUG=1 "; //debug mode
			}

			cmd << "-arch " + arch << " "; //The architecture to build
			cmd << "-Xlinker -r "; //Tell the linker to just bundle all the .o files into one .o instead of linking/assembling
			cmd << "-nostdlib "; //Don't link with stdlib, xcode will do that
			cmd << "-isysroot " << xcodeLocation << "/Platforms/" << archSDKName << ".platform/Developer/SDKs/" << archSDKName << sdkVersion << ".sdk ";
			cmd << "-miphoneos-version-min=" << sdkVersion << " ";
			cmd << "-D__IOS__ -DUSE_NEWLIB -DMOSYNC_NATIVE" << " ";
			cmd << compilerSwitches << " ";
			cmd << "-I" << nativeHeaderDir << " ";
			cmd << "-I" << outputDir << " ";
			cmd << sourceFileList.str() << " ";
			cmd << additionalIncludesList.str() << " ";
			cmd << additionalLibPathsList.str() << " ";
			cmd << compilerDefinesList.str() << " ";
			cmd << "-o " << outputFile << " ";
			sh(cmd.str().c_str(), !isVerbose);
		}
		ostringstream cmd;
		sh((string("rm -f ") + outputDir + "/" + OUTPUT_FILE_NAME).c_str());
		//Combine all architecture files into one universal .o file
		cmd << "lipo " << oFileList << " -create -output " + outputDir + "/" + OUTPUT_FILE_NAME;
		sh(cmd.str().c_str(), !isVerbose);
	}
	return result;
}

int prepareIOSFramework(Arguments* params) {

	return 1;
}

int buildIOSNative(Arguments* params) {
	int result = compileIOSUserCode(params);
	if(!result) {
		return result;
	}

	result = prepareIOSFramework(params);
	return result;
}