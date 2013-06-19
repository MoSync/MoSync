/* Copyright (C) 2010 MoSync AB

 This program is free software; you can redistribute it and/or modify it under
 the terms of the GNU General Public License, version 2, as published by
 the Free Software Foundation.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 for more details.

 You should have received a copy of the GNU General Public License
 along with this program; see the file COPYING.  If not, write to the Free
 Software Foundation, 59 Temple Place - Suite 330, Boston, MA
 02111-1307, USA.
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
