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
#include <cstring>
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

	string dst;
	string templateLocation;
	string templateFileLocation;
	string templateLibProjectLocation;
	string outputLibFile;
	string csprojOutputFile;
	string libProjOutputFolder;

	string appManifestInputFile;
	string appManifestOutputFile;

	string csprojOutput;
	string outputType;

	bool isNative = !strcmp("native", s.outputType);

	if(isNative)
	{
		dst = s.dst;
		templateLocation = string(ri.path) + "/template_WP8";
		templateFileLocation = string(ri.path) + "/template_WP8/mosync_WP8/mosync_WP8.csproj";
		templateLibProjectLocation = string(ri.path) + "/template_WP8/MoSyncLibrary/MoSyncLibrary.vcxproj";
		csprojOutputFile = dst + "/project/mosync_WP8/mosync_WP8.csproj";
		libProjOutputFolder = dst + "/project/MoSyncLibrary";
		outputLibFile = dst + "/project/MoSyncLibrary/MoSyncLibrary.vcxproj";
		appManifestInputFile = string(ri.path) + "/template_WP8/mosync_WP8/Properties/WMAppManifest.xml";
		appManifestOutputFile = dst + "/project/mosync_WP8/Properties/WMAppManifest.xml";

		csprojOutput = dst + "/project";
		outputType = "native";
	}
	else
	{
		dst = s.dst;
		templateLocation = string(ri.path) + "/template";
		templateFileLocation = string(ri.path) + "/template/mosync.csproj";
		csprojOutputFile = dst + "/project/mosync.csproj";

		appManifestInputFile = string(ri.path) + "/template/Properties/WMAppManifest.xml";
		appManifestOutputFile = dst + "/project/Properties/WMAppManifest.xml";

		csprojOutput = dst + "/project";
		outputType = s.outputType ? string(s.outputType) : string("interpreted");
	}

	if(s.csOutputDir && !isNative)
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

	if(isNative)
	{
		generateCmd << " -mosync-project-path " << file(s.mosyncProjectPath) <<
					   " -output-file-library-project " << file(outputLibFile) <<
					   " -input-file-library-project " << file(templateLibProjectLocation);

		if(s.WPMacroDefines != NULL && strcmp(s.WPMacroDefines, "") != 0)
			generateCmd << " -macro-defines " << s.WPMacroDefines;

		if(strcmp(s.WPIncludePaths, "") != 0)
			generateCmd << " -include-paths \"" << s.WPIncludePaths << "\"";
	}

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
	} else if(outputType != "native"){
		copyFile((csprojOutput + "/program").c_str(), s.program);
	}

	string resourceFileCopy;
	if(isNative)
	{
		resourceFileCopy = csprojOutput + "/mosync_WP8/resources";
	}
	else
	{
		resourceFileCopy = csprojOutput + "/resources";
	}

	if(s.resource) {
		copyFile(resourceFileCopy.c_str(), s.resource);
	} else {
		//ofstream empty(resourceFileCopy.c_str());
		//empty.close();
	}

	string maHeaderFileCopy = csprojOutput + "/MoSyncLibrary/MAHeaders.h";
	if(isNative && s.resource) //copy MAHeader.h
	{
		int pos = dst.find("package");
		string maHeaderSource = dst.substr(0, pos) + "MAHeaders.h";
		copyFile(maHeaderFileCopy.c_str(), maHeaderSource.c_str());
	}

	// Icons!
	if (s.icon) {
		const string sizes[2] = { "62x62", "173x173"};
		const string filenames[2] = { "ApplicationIcon.png", "Background.png"};

		for(int i = 0; i < 2; i++) {
			std::ostringstream iconInjectCmd;
			string size = sizes[i];
			string outputIcon;
			if(outputType == "native")
			{
				outputIcon = csprojOutput + "/mosync_WP8/" + filenames[i];
			}
			else
			{
				outputIcon = csprojOutput + "/" + filenames[i];
			}
			injectIcon("Windows Phone", size.c_str(), s.icon, outputIcon.c_str(), s.silent, true);
		}
	}

	if (!s.WPgenerateOnly) {
#ifdef PLATFORM_WIN32
		//testIOSSdk(s);

		// todo: find this programatically...
		//std::string vsBuildPath =
		//	"/Windows/Microsoft.NET/Framework/v4.0.30319/MSBuild.exe";
		if(outputType != "native")
		{
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
		}
		else
		{
			string outputDir = csprojOutput + "\\mosync_WP8";
			_chdir(outputDir.c_str());
			buildCmd << s.WPvsBuildPath << " mosync_WP8_solution.sln";
			buildCmd << " /p:OutputPath=\"..\\Bin\\" << s.WPconfig << "\"";
			printf("%s", buildCmd.str().c_str());
		}

		sh(buildCmd.str().c_str(), s.silent);

#else
		printf("Building Visual Studio projects is only supported on Windows");
		exit(1);
#endif
	}
}
