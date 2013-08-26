/* Copyright 2013 David Axmark

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

	http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

#include <string>
#include <list>
#include <sstream>
#include <fstream>
#include <cstring>
#include <stdlib.h>
#include "packagers.h"
#include "util.h"
#include "tools.h"
#include "mustache/mustache.h"
#include "helpers/mkdir.h"
#include "profiledb/profiledb.h"

#if defined (__MACH__) || defined(__APPLE__)
#define PLATFORM_OSX
#endif

using namespace std;

string filterWhiteSpace(const string& str);

void packageIOS(const SETTINGS& s, const RuntimeInfo& ri) {
	testDst(s);
	testName(s);
	testVendor(s);
	testVersion(s);
	testIOSCert(s);
	testCppOutputDir(s);

	std::ostringstream generateCmd;
	std::ostringstream buildCmd;

	bool isNative = !strcmp("native", s.outputType);

	string dst = s.dst;
	string src = s.cppOutputDir;
	string templateLocation = string(ri.path) + "/template";
	string xcodeprojOutput = dst + "/xcode-proj";
	// If no bundle identifier, use the default one (used until 3.0.2) that is RFC1034-"ish"
	string bundleIdArg = s.iOSBundleId ? s.iOSBundleId : (string("com.") + s.vendor + "." + s.name);

	generateCmd << getBinary("iphone-builder") << " generate -project-name " <<
		arg(s.name) << " -version " << s.version << " -company-name " <<
		arg(s.vendor) << " -bundle-identifier " << arg(bundleIdArg) <<
		" -cert " << arg(s.iOSCert) << " -input " << file(templateLocation) <<
		" -output " << file(xcodeprojOutput);

	sh(generateCmd.str().c_str(), s.silent);

	if(!isNative) {
		// Copy program files to xcode template
		copyFile((xcodeprojOutput + "/Classes/rebuild.build.cpp").c_str(), (src + "/rebuild.build.cpp").c_str());
		copyFile((xcodeprojOutput + "/data_section.bin").c_str(), (src + "/data_section.bin").c_str());
	}
	else {
		copyFile((xcodeprojOutput + "/userCode.o").c_str(), (src + "/userCode.o").c_str());
		copyFile((xcodeprojOutput + "/MoSyncLibs.a").c_str(), (src + "/MoSyncLibs.a").c_str());
	}

	ProfileDB db;
	string iOSProfilesDir = db.profilesdir("iOS") + "/config.xml";
	copyFile((xcodeprojOutput + "/config.xml").c_str(), iOSProfilesDir.c_str());

	string resourceFileCopy = xcodeprojOutput + "/resources";
	if(s.resource) {
		copyFile(resourceFileCopy.c_str(), s.resource);
	} else {
		ofstream empty(resourceFileCopy.c_str());
		empty.close();
	}

	// Icons!
	if (s.icon) {
		const string sizes[3] = { "57x57", "72x72", "114x114" };
		const string filenames[3] = { "Icon.png", "Icon-72.png", "Icon@2x.png" };

		for(int i = 0; i < 3; i++) {
			std::ostringstream iconInjectCmd;
			string size = sizes[i];
			string outputIcon = xcodeprojOutput + "/" + filenames[i];
			injectIcon("iOS", size.c_str(), s.icon, outputIcon.c_str(), s.silent, true);
		}
	}

	if (!s.iOSgenerateOnly) {
#ifdef PLATFORM_OSX
		testIOSSdk(s);
		chdir(xcodeprojOutput.c_str());
		string filteredProjName = filterWhiteSpace(string(s.name));
		buildCmd << "xcodebuild -project " << arg(filteredProjName) << ".xcodeproj";
		buildCmd << " -target " << filteredProjName;
		if (isNative) {
			buildCmd << "Native";
		}
		if (s.iOSSdk) {
			buildCmd << " -sdk " << arg(s.iOSSdk);
		}
		if (s.iOSXcodeTarget) {
			buildCmd << " -configuration " << arg(s.iOSXcodeTarget);
		}
		sh(buildCmd.str().c_str(), s.silent);
#else
		printf("Building Xcode projects is only supported on Mac OS X");
		exit(1);
#endif
	}

}

string filterWhiteSpace(const string& str) {
	string newString = "";
	for(size_t i = 0; i < str.length(); i++) {
		unsigned char s = str[i];
		if(s > 32 && s < 127) {
			newString += str[i];
		} else {
			newString += "";
		}
	}
	return newString;
}
