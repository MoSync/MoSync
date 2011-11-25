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
#include <stdlib.h>
#include "packagers.h"
#include "util.h"
#include "tools.h"
#include "helpers/mkdir.h"
#include "filelist/copyFiles.h"

#if defined (__MACH__) || defined(__APPLE__)
#define PLATFORM_OSX
#endif

using namespace std;

void packageWindowsPhone(const SETTINGS& s, const RuntimeInfo& ri) {
	testDst(s);
	testName(s);
	testVendor(s);
	testVersion(s);
	//testIOSCert(s);
	//testCppOutputDir(s);

	std::ostringstream generateCmd;
	std::ostringstream buildCmd;

	printf("Building: %s\n", s.model);

#if 0
	// super hack
	bool rebuild = false;
	if(std::string(s.model).find("WindowsPhoneRebuilt") != string::npos)
		rebuild = true;
#endif

	string dst = s.dst;
	string templateLocation = string(ri.path) + "/template";
	string templateFileLocation = string(ri.path) + "/template/mosync.csproj";
	string csprojOutputFile = dst + "/project/mosync.csproj";
	string csprojOutput = dst + "/project";

	/*
	generateCmd << getBinary("iphone-builder") << " generate -project-name " <<
		arg(s.name) << " -version " << s.version << " -company-name " <<
		arg(s.vendor) << " -cert " << arg(s.iOSCert) << " -input " << file(templateLocation) <<
		" -output " << file(xcodeprojOutput);
	*/

	_mkdir(csprojOutput.c_str());
	copyFilesRecursively(templateLocation.c_str(), csprojOutput.c_str());
	generateCmd << getBinary("winphone-builder") << " -output-type interpreted -input-file " << file(templateFileLocation) <<
		" -output-file " << file(csprojOutputFile);

	sh(generateCmd.str().c_str(), s.silent);

	// Copy program files to xcode template
	//copyFile((xcodeprojOutput + "/Classes/rebuild.build.cpp").c_str(), (src + "/rebuild.build.cpp").c_str());
	copyFile((csprojOutput + "/program").c_str(), s.program);

	string resourceFileCopy = csprojOutput + "/resources";
	if(s.resource) {
		copyFile(resourceFileCopy.c_str(), s.resource);
	} else {
		ofstream empty(resourceFileCopy.c_str());
		empty.close();
	}

	// Icons!
	/*
	if (s.icon) {
		const string sizes[3] = { "57x57", "72x72", "114x114" };
		const string filenames[3] = { "Icon.png", "Icon-72.png", "Icon@2x.png" };

		for(int i = 0; i < 3; i++) {
			std::ostringstream iconInjectCmd;
			string size = sizes[i];
			string outputIcon = xcodeprojOutput + "/" + filenames[i];
			injectIcon("iOS", size.c_str(), s.icon, outputIcon.c_str(), s.silent);
		}
	}
	*/

	/*
	if (!s.iOSgenerateOnly) {
#ifdef PLATFORM_OSX
		testIOSSdk(s);
		chdir(xcodeprojOutput.c_str());
		buildCmd << "xcodebuild -project " << arg(string(s.name) + ".xcodeproj");
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
	*/

}
