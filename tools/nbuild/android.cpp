#include "android.h"
#include "util.h"
#include "nbuild.h"
#include "mustache/mustache.h"
#include "profiledb/profiledb.h"
#include <map>
#include <vector>
#include <sstream>
#include <fstream>

using namespace std;

int buildAndroidNative(Arguments* params) {
	int result = generateMakefile(params);
	if (!params->isFlagSet("--android-generate-makefile")) {
		result = executeNdkBuild(params);
	}
	return result;
}

int generateMakefile(Arguments* params) {
	string outputDir = require(params, "--dst");

	vector<string> modules;
	split(modules, params->getSwitchValue("--modules"), ",");
	if (modules.empty()) {
		// Default
		modules.push_back("mosync");
		modules.push_back("mosynclib");
	}

	DefaultContext rootCtx(NULL);
	for (size_t i = 0; i < modules.size(); i++) {
		DefaultContext* moduleCtx = new DefaultContext(&rootCtx);
		// TODO: Clean up
		moduleCtx->setParameter("name", modules[i]);
		rootCtx.addChild("modules", moduleCtx);
	}
	rootCtx.setParameter("source-files", require(params, "--source-files"));
	vector<string> compilerDefines = params->getPrefixedList("-D", true);
	rootCtx.setParameter("compiler-defines", delim(compilerDefines, " "));
	rootCtx.setParameter("additional-compiler-switches", params->getSwitchValue("--compiler-switches"));

	MustacheParser parser(true);
	ProfileDB db;
	string androidProfilesDir = db.profilesdir("Android");
	string androidMkTemplate = androidProfilesDir + "/Android.mk.template";

	string androidMkOutput = outputDir + "/Android.mk";
	ofstream output(androidMkOutput.c_str());
	if (output.good()) {
		DefaultParserCallback cb(&rootCtx, output);
		parser.parseFile(androidMkTemplate, &cb);
		output.flush();
		output.close();
	} else {
		return 1;
	}

	string androidAppMkOutput = outputDir + "/Application.mk";
	string androidAppMkOriginal = androidProfilesDir + "/Application.mk";
	copyFile(androidAppMkOutput.c_str(), androidAppMkOriginal.c_str());

	return 0;
}


int executeNdkBuild(Arguments* params) {
	string ndkbuildCmd = require(params, "--android-ndkbuild-cmd");
	string projectPath = require(params, "--project");
	string moduleName = require(params, "--name");
	toSlashes(projectPath);
	string configName = require(params, "--config");
	string outputDir = require(params, "--dst");
	toSlashes(outputDir);
	string libVariant = require(params, "--lib-variant");
	bool isDebug = libVariant == "debug";
	bool isVerbose = params->isFlagSet("--verbose");
	bool doClean = params->isFlagSet("--clean");

	ostringstream cmd;
	cmd << file(ndkbuildCmd) << " ";
	cmd << "-C " << file(outputDir) << " ";
	if (isVerbose) {
		cmd << "V=1 ";
	}
	if (isDebug) {
		cmd << "NDK_DEBUG=1 ";
	}
	if (doClean) {
		cmd << "-B ";
	}
	string moduleDir = string(mosyncdir()) + "/modules";
	toSlashes(moduleDir);
	string makeFile = outputDir + "/Android.mk";
	toSlashes(makeFile);
	cmd << arg("MOSYNC_MODULES=" + moduleDir) << " ";
	cmd << arg("PROJECT_DIR=" + projectPath) << " ";
	cmd << arg("APP_BUILD_SCRIPT=" + makeFile) << " ";
	cmd << arg("MOSYNC_CONFIG=" + configName) << " ";
	cmd << arg("MOSYNC_MODULE_NAME=" + moduleName) << " ";
	cmd << arg("MOSYNC_PLATFORM=" + params->getSwitchValue("--platform")) << " ";
	cmd << arg("MOSYNC_LIB_VARIANT=" + libVariant) << " ";
	cmd << arg("NDK_PROJECT_PATH=.") << " ";
	cmd << arg("APP_PLATFORM=android-14");

	sh(cmd.str().c_str(), !isVerbose);

	return 0;
}
