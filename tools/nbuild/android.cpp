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
#include <algorithm>
#include <cstring>

#define TEMP_BUILD_DIR "--android-build-dir"

using namespace std;

int buildAndroidNative(Arguments* params) {
	int result = 1;
	if (!params->isFlagSet("--android-execute-makefile")) {
		result = generateMakefiles(params);
	}
	if (!params->isFlagSet("--android-generate-makefile")) {
		result = executeNdkBuild(params);
	}
	return result;
}

/**
 * Returns the temp build dir, given a config, or
 * the root temp build dir if the config is empty.
 */
string getTempBuildDir(Arguments* params, string* cfg) {
	string tmpDir = params->getSwitchValue(TEMP_BUILD_DIR);
	if (tmpDir.empty()) {
		tmpDir = params->getSwitchValue(PROJECT_DIR);
		toDir(tmpDir);
		tmpDir += "temp/";
	} else {
		toDir(tmpDir);
	}
	if (cfg) {
		tmpDir = tmpDir + (*cfg);
		toDir(tmpDir);
	}
	toOSSlashes(tmpDir);
	return tmpDir;
}

int generateMakefiles(Arguments* params) {
	vector<string> configNames;
	split(configNames, require(params, CONFIGURATION), ",");
	int result = 0;
	string tmpRoot = getTempBuildDir(params);
	_mkdir(tmpRoot.c_str());
	for (size_t i = 0; i < configNames.size(); i++) {
		string configName = configNames[i];
		result += generateMakefile(params, configName);
	}
	return result;
}

int generateMakefile(Arguments* params, string& configName) {
	string tmpBuildDir = getTempBuildDir(params, &configName);
	_mkdir(tmpBuildDir.c_str());

	vector<string> bootstrapModules;
	split(bootstrapModules, params->getSwitchValue(BOOT_MODULE_LIST), ",");
	if (bootstrapModules.empty()) {
		// Default
		bootstrapModules.push_back("mosync");
		bootstrapModules.push_back("mosynclib");
	} else if (bootstrapModules[0] == ".") {
		bootstrapModules.clear();
	}

	vector<string> modules(bootstrapModules);
	split(modules, params->getSwitchValue(MODULE_LIST), ",");

	DefaultContext rootCtx(NULL);
	for (size_t i = 0; i < modules.size(); i++) {
		DefaultContext* moduleCtx = new DefaultContext(&rootCtx);
		// TODO: Clean up
		moduleCtx->setParameter("name", modules[i]);
		rootCtx.addChild("modules", moduleCtx);
	}
	vector<string> sourceFiles = getSourceFiles(params);
	string sourceFileList;
	bool isVerbose = params->isFlagSet(VERBOSE);
	for (size_t i = 0; i < sourceFiles.size(); i++) {
		sourceFileList += sourceFiles[i];
		sourceFileList += " ";
		if (isVerbose) {
			printf("Adding source file: %s.\n", sourceFiles[i].c_str());
		}
	}

	if (sourceFiles.empty()) {
		error("No source files!");
	}
	rootCtx.setParameter("source-files", sourceFileList);
	vector<string> compilerDefines = params->getPrefixedList(MACRO_DEFINES, true);
	rootCtx.setParameter("compiler-defines", delim(compilerDefines, " "));
	rootCtx.setParameter("additional-compiler-switches", params->getSwitchValue("--compiler-switches"));
	vector<string> additionalIncludeList = params->getPrefixedList(ADDITIONAL_INCLUDES, false);
	string additionalIncludes;
	for (size_t i = 0; i < additionalIncludeList.size(); i++) {
		if (i > 0) {
			additionalIncludes += " ";
		}
		additionalIncludes += toMakefileFile(additionalIncludeList[i]);
	}
	rootCtx.setParameter("additional-includes", additionalIncludes);
	if (bootstrapModules.empty()) {
		rootCtx.setParameter("no-default-includes", "true");
	}

	bool useStatic = "static" == params->getSwitchValue("--android-lib-type");
	if (useStatic) {
		rootCtx.setParameter("static-lib", "true");
	}

	MustacheParser parser(true);
	ProfileDB db;
	string androidProfilesDir = db.profilesdir("Android");
	string androidMkTemplate = androidProfilesDir + "/Android.mk.template";

	string androidMkOutput = tmpBuildDir + "/Android.mk";
	stringstream output;

	DefaultParserCallback cb(&rootCtx, output);
	parser.parseFile(androidMkTemplate, &cb);

	output.flush();
	string mf = output.str();

	bool different = true;
	ifstream cmp(androidMkOutput.c_str(), ifstream::in);
	if (cmp.good()) {
		int len = mf.size() * sizeof(char);
		char* cmpStr = (char*) malloc(len);
		cmp.read(cmpStr, len);
		different = cmp.fail();
		different |= (mf != string(cmpStr));
		free(cmpStr);
	}

	if (different) {
		ofstream outputFile(androidMkOutput.c_str());
		if (output.good()) {
			outputFile << mf;
			outputFile.flush();
			outputFile.close();
			string androidAppMkOutput = tmpBuildDir + "/Application.mk";
			string androidAppMkOriginal = androidProfilesDir + "/Application.mk";
			toOSSlashes(androidAppMkOutput);
			toOSSlashes(androidAppMkOriginal);
			copyFile(androidAppMkOutput.c_str(), androidAppMkOriginal.c_str());
			return 0;
		}
		return 1;
	}
	// Failed!
	return 2;
}

string toMakefileFile(string file) {
#ifdef WIN32
	// We assume cygwin
	string result(file);
	size_t ixColon = result.find(':');
	if (ixColon != string::npos) {
		// Device is whatever is before the colon;
		// if no colon we assume the path is relative
		string device = result.substr(0, ixColon);
		transform(device.begin(), device.end(), device.begin(), ::tolower);
		result = "/cygdrive/" + device + result.substr(ixColon + 1);
	}
	toSlashes(result);
	return result;
#else
	// We are already good to go!
	return file;
#endif
}

string getNdkBuildCommand(string commandLine) {
#ifdef WIN32
	bool exists;
	string cygpath = getCygpath(exists);
	if (!cygpath.empty()) {
		toDir(cygpath);
	}
	if (exists) {
		return cygpath + "bash.exe -c \"" + commandLine + "\"";
	} else {
		error("No cygwin found! Please set CYGPATH or add the cygwin bin directory to PATH.");
	}
#endif
	return commandLine;
}

#ifdef WIN32
string getCygpath(bool& exists) {
	char* envCygpath = getenv("CYGPATH");
	if (envCygpath && strlen(envCygpath) > 0) {
		exists = true;
		return envCygpath;
	}
	exists = !sh("bash.exe -c pwd", false, "", false);
	return "";
}
#endif

int executeNdkBuild(Arguments* params) {
	vector<string> configNames;
	vector<string> libVariants;
	split(configNames, require(params, CONFIGURATION), ",");
	split(libVariants, require(params, BINARY_TYPE), ",");

	string ndkbuildCmd = getNdkBuildScript(params);
	string projectPath = toMakefileFile(require(params, PROJECT_DIR));
	toSlashes(projectPath);
	string moduleName = require(params, NAME);
	string outputDir = require(params, OUTPUT_DIR);
	toDir(outputDir);

	if (configNames.size() != libVariants.size()) {
		error("--config and --lib-variants must have a the same number of args", 2);
	}

	vector<string> archs;
	archs.push_back("armeabi");
	archs.push_back("armeabi-v7a");

	for (size_t i = 0; i < configNames.size(); i++) {
		string validatedConfig = validateConfiguration(configNames[i]);
		if (!validatedConfig.empty()) {
			error(validatedConfig.c_str(), 2);
		}
	}

	for (size_t i = 0; i < configNames.size(); i++) {
		for (size_t j = 0; j < archs.size(); j++) {
			string configName = configNames[i];
			string libVariant = libVariants[i];
			string arch = archs[j];

			string tmpBuildDir = getTempBuildDir(params, &configName);
			_mkdir(tmpBuildDir.c_str());

			bool isDebug = libVariant == "debug";
			bool isVerbose = params->isFlagSet(VERBOSE);
			bool doClean = params->isFlagSet(CLEAN);

			ostringstream cmd;
			string ndkBuildCmd = getNdkBuildScript(params);
			cmd << ndkBuildCmd << " -C " << toMakefileFile(file(tmpBuildDir)) << " ";
			if (isVerbose) {
				cmd << "V=1 ";
			}
			if (isDebug) {
				cmd << "NDK_DEBUG=1 ";
			}
			if (doClean) {
				cmd << "-B ";
			}
			string libDir = string(mosyncdir()) + "/lib";
			toSlashes(libDir);
			string moduleDir = string(mosyncdir()) + "/modules";
			toSlashes(moduleDir);
			string makeFile = toMakefileFile(tmpBuildDir + "Android.mk");
			cmd << arg("MOSYNC_MODULES=" + moduleDir) << " ";
			cmd << arg("APP_MODULES=" + moduleName) << " ";
			cmd << arg("MOSYNC_LIBS=" + libDir) << " ";
			cmd << arg("PROJECT_DIR=" + projectPath) << " ";
			cmd << arg("APP_BUILD_SCRIPT=" + makeFile) << " ";
			cmd << arg("MOSYNC_CONFIG=" + configName) << " ";
			cmd << arg("MOSYNC_MODULE_NAME=" + moduleName) << " ";
			string platform = params->getSwitchValue("--platform");
			cmd << arg("MOSYNC_PLATFORM=" + platform) << " ";
			cmd << arg("MOSYNC_LIB_VARIANT=" + libVariant) << " ";
			cmd << arg("NDK_PROJECT_PATH=.") << " ";
			cmd << arg("APP_ABI=" + arch) << " ";
			string androidVersion = getAppPlatform(params);
			cmd << arg("APP_PLATFORM=android-" + androidVersion) << " ";
			cmd << arg("MOSYNCDIR=" + toMakefileFile(mosyncdir())) << " ";

//			cmd << arg("APP_STL=stlport_static") << " ";

			bool useStatic = "static" == params->getSwitchValue("--android-lib-type");
			string fullOutputDir = outputDir + "android_" + arch + "_" + libVariant + "/";
			_mkdir(fullOutputDir.c_str());

			string libFile = useStatic ?
					tmpBuildDir + "obj/local/" + arch + "/lib" + moduleName + ".a" :
					tmpBuildDir + "libs/" + arch + "/lib" + moduleName + ".so";
			string libExt = useStatic ? ".a" : ".so";
			string fullOutputFile = fullOutputDir + "lib" + moduleName + libExt;

			file(libFile.c_str());
			string fullCmd = getNdkBuildCommand(cmd.str());
			if (sh(fullCmd.c_str(), !isVerbose)) {
				error("NDK build command failed!\n", 1);
			}

			copyFile(fullOutputFile.c_str(), libFile.c_str());
		}
	}
	return 0;
}

string getNdkBuildScript(Arguments* params) {
	string ndkDir = require(params, "--android-ndk-location");
	toDir(ndkDir);
	string buildCmdRel = params->getSwitchValue("--android-ndkbuild-cmd");
	if (buildCmdRel.empty()) {
		buildCmdRel = "ndk-build";
	}
	string buildCmdAbs = ndkDir + buildCmdRel;
	return toMakefileFile(buildCmdAbs);
}

string getAppPlatform(Arguments* params) {
	string ndkDir = require(params, "--android-ndk-location");
	toDir(ndkDir);
	string androidVersion = require(params, "--android-version");
	string platformDir = ndkDir + "/platforms/android-" + androidVersion;
	toOSSlashes(platformDir);
	if (!existsDir(platformDir.c_str())) {
		error("Could not find android platform version given by --android-version, set another!", 2);
	}
	return androidVersion;
}
