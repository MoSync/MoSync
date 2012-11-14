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
#include "filelist/copyfiles.h"

#if defined (WIN32) || defined(_WIN32)
#define PLATFORM_WIN32
#endif

using namespace std;

void packageWindowsPhone(const SETTINGS& s, const RuntimeInfo& ri) {
	testDst(s);
	testName(s);
	testVendor(s);
	testVersion(s);
	testVsBuildPath(s);

	std::ostringstream generateCmd;
	std::ostringstream buildCmd;

	printf("Building: %s\n", s.model);

	string dst = s.dst;
	string templateLocation = string(ri.path) + "/template";
	string templateFileLocation = string(ri.path) + "/template/mosync.csproj";
	string csprojOutputFile = dst + "/project/mosync.csproj";

	string appManifestInputFile = string(ri.path) + "/template/Properties/WMAppManifest.xml";
	string appManifestOutputFile = dst + "/project/Properties/WMAppManifest.xml";

	string csprojOutput = dst + "/project";
	string outputType = s.outputType ? string(s.outputType) : string("interpreted");
	if(s.csOutputDir)
	{
		outputType = "rebuilt";
	}

	_mkdir(csprojOutput.c_str());
	copyFilesRecursively(templateLocation.c_str(), csprojOutput.c_str());

	generateCmd << getBinary("winphone-builder") <<
		" -version " << arg(s.version) <<
		" -company-name " << arg(s.vendor) <<
		" -input-app-manifest-file " << file(appManifestInputFile) <<
		" -output-app-manifest-file " << file(appManifestOutputFile) <<
		" -project-name " << arg(s.name) <<
		" -output-type " << arg(outputType) <<
		" -input-file " << file(templateFileLocation) <<
		" -output-file " << file(csprojOutputFile);

	if(s.WPguid)
	{
		generateCmd << " -guid " << s.WPguid;
	}

	if(!s.resource)
	{
		generateCmd << " -exclude-resource-file";
	}

	sh(generateCmd.str().c_str(), s.silent);

	// Copy program files to xcode template
	if(outputType == "rebuilt")
	{
		testCsOutputDir(s);
		copyFile((csprojOutput + "/RebuildData/data_section.bin").c_str(), (s.csOutputDir + string("/data_section.bin")).c_str());
		copyFile((csprojOutput + "/RebuildData/rebuild.build.cs").c_str(), (s.csOutputDir + string("/rebuild.build.cs")).c_str());
	} else {
		copyFile((csprojOutput + "/program").c_str(), s.program);
	}

	string resourceFileCopy = csprojOutput + "/resources";
	if(s.resource) {
		copyFile(resourceFileCopy.c_str(), s.resource);
	} else {
		//ofstream empty(resourceFileCopy.c_str());
		//empty.close();
	}

	// Icons!
	if (s.icon) {
		const string sizes[2] = { "62x62", "173x173"};
		const string filenames[2] = { "ApplicationIcon.png", "Background.png"};

		for(int i = 0; i < 2; i++) {
			std::ostringstream iconInjectCmd;
			string size = sizes[i];
			string outputIcon = csprojOutput + "/" + filenames[i];
			injectIcon("Windows Phone", size.c_str(), s.icon, outputIcon.c_str(), s.silent);
		}
	}

	if (!s.WPgenerateOnly) {
#ifdef PLATFORM_WIN32
		//testIOSSdk(s);

		// todo: find this programatically...
		//std::string vsBuildPath =
		//	"/Windows/Microsoft.NET/Framework/v4.0.30319/MSBuild.exe";

		_chdir(csprojOutput.c_str());

		buildCmd << s.WPvsBuildPath << " mosync.csproj";

		// Set our configuration.
		buildCmd << " /p:Configuration=" << s.WPconfig;

		// Target rebuild. Means always rebuild.
		buildCmd << " /t:Rebuild";

		// We compile for any cpu.
		buildCmd << " /p:Platform=\"Any CPU\"";

		// Doesn't seem to get the DefineConstants from the configurations??
		if(outputType == "rebuilt")
			buildCmd << " /p:DefineConstants=\"REBUILD\"";

		// Output path will be defined by the config name.
		buildCmd << " /p:OutputPath=\"Bin\\" << s.WPconfig << "\"";

		sh(buildCmd.str().c_str(), s.silent);

#else
		printf("Building Visual Studio projects is only supported on Windows");
		exit(1);
#endif
	}
}
