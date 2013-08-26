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

#include "Injector.h"
#include "Shared.h"
#include "filelist/filelist.h"
#include <stdio.h>
#include <stdlib.h>
#include <sstream>

using namespace std;

namespace MoSync {
	string Injector::verifyParameter(
		const map<string, string>& params, 
		const string& name)
	{
		map<string, string>::const_iterator i = params.find(name);
		if(i == params.end()) errorExit("Parameter " + name + " missing.");
		return (*i).second;
	}

	void Injector::setForce(bool force) {
		fForce = force;
	}

	bool Injector::convertInstanceToImageFormat(const IconInstance *iconInstance,
		const char *dstFilename, const string& size, const string& format) {
		const char *mosyncdir_c = getenv("MOSYNCDIR");
		if(!mosyncdir_c) errorExit("MOSYNCDIR missing");
		string mosyncdir = mosyncdir_c;

		string src = iconInstance->filename;
		string dst = string(dstFilename);

		string extension = getExtension(src);

		if (!fForce && compareTime(src.c_str(), dst.c_str()) < 0) {
			printf("No need to generate icon, use -force switch to override.\n");
			return true;
		}

		if(extension == "svg") {
			ostringstream batik;
			batik << "java -Djava.awt.headless=true -jar \""<<mosyncdir<<"/bin/Batik/batik-rasterizer.jar\""
				" \""<<src<<"\" -d \""<<dst<<"\"";
			if(size != "default") {
				unsigned int n;
				unsigned w, h;
				int res = sscanf(size.c_str(), "%ux%u%n", &w, &h, &n);
				if(res != 2 || n != size.size())
					errorExit("Broken parameter: size.");
				batik << " -w "<<w<<" -h "<<h;
			}
			int res = run(batik.str().c_str());
			if(res != 0)
				errorExit("Batik failed.");
			return true;
		}

		string resizeFlag;
		if(size != "default")
			resizeFlag = " -resize " + size;
	#if defined(WIN32)
		string magick =
			"\""+mosyncdir+"\\bin\\ImageMagick\\convert.exe\" \"" + src + "\"" +
			resizeFlag + " \"" + dst + "\"";
	#else
		string magick =
		"\""+mosyncdir+"/bin/ImageMagick/convert\" \"" + src + "\"" +
		resizeFlag + " \"" + dst + "\"";
	#endif

		if(run(magick.c_str()) != 0) errorExit("Image Magick failed.");

		return true;
	}
}
