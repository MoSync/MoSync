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

#if defined (__MACH__) || defined(__APPLE__)
#define PLATFORM_OSX
#endif

using namespace std;

void packageIOS(const SETTINGS& s, const RuntimeInfo& ri) {
	testDst(s);
	testName(s);
	testVendor(s);
	testVersion(s);
	testIOSCert(s);
	testCppOutputDir(s);

	std::ostringstream generateCmd;
	std::ostringstream buildCmd;

	string dst = s.dst;
	string src = s.cppOutputDir;
	string templateLocation = string(ri.path) + "/template";
	string xcodeprojOutput = dst + "/xcode-proj";

	generateCmd << mosyncdir() << "/bin/iphone-builder generate -project-name \"" <<
		s.name << "\" -version \"" << s.version << "\" -company-name \"" <<
		s.vendor << "\" -cert \"" << s.iOSCert << "\" -input \"" << templateLocation <<
		"\" -output \"" << xcodeprojOutput << "\"";

	sh(generateCmd.str().c_str(), s.silent);

	// Copy program files to xcode template
	copyFile((xcodeprojOutput + "/Classes/rebuild.build.cpp").c_str(), (src + "/rebuild.build.cpp").c_str());
	copyFile((xcodeprojOutput + "/data_section.bin").c_str(), (src + "/data_section.bin").c_str());

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
			injectIcon("iOS", size.c_str(), s.icon, outputIcon.c_str(), s.silent);
		}
	}

	if (!s.iOSgenerateOnly) {
#ifdef PLATFORM_OSX
		testIOSSdk(s);
		chdir(xcodeprojOutput.c_str());
		buildCmd << "xcodebuild -project \"" << s.name << ".xcodeproj\"";
		if (s.iOSSdk) {
			buildCmd << " -sdk \"" << s.iOSSdk << "\"";
		}
		sh(buildCmd.str().c_str(), s.silent);
#else
		printf("Building Xcode projects is only supported on Mac OS X");
		exit(1);
#endif
	}

}
