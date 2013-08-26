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

#include "Symbian9Injector.h"
#include "Shared.h"
#include <stdlib.h>
#include <stdio.h>

using namespace std;

namespace MoSync {
	void Symbian9Injector::inject(const Icon* icon, const std::map<std::string,
		std::string>& params)
	{
		string size = verifyParameter(params, "size");
		string dst = verifyParameter(params, "dst");
		const IconInstance* iconInst = icon->findBestInstance(size, "svg");
		if(!iconInst) errorExit("Couldn't find an SVG icon instance.");

		string ext = getExtension(iconInst->filename);

		// Bitmaps on s60v3/v5 phones turn invisible or
		// make the entire program menu invisible, respectively.
		// Disabled for now.
#if 0
		string mifconvSrc;
		if(ext == "svg") {
			//preserve
			mifconvSrc = iconInst->filename;
		}
		else {
			//convert to bmp
			mifconvSrc = "temp.bmp";
			if(!convertInstanceToImageFormat(iconInst, mifconvSrc.c_str(), size, "bmp")) 
				errorExit("Java icon conversion failed.");
		}
#else
		string mifconvSrc = iconInst->filename;
#endif
		char buf[2048];
		const char *mosyncdir_c = getenv("MOSYNCDIR");
		if(!mosyncdir_c) errorExit("MOSYNCDIR missing");
#ifdef WIN32
		sprintf(buf, "\"%s\\bin\\mifconv.exe\" \"%s\" \"%s\"", mosyncdir_c, dst.c_str(), mifconvSrc.c_str());
#else
		sprintf(buf, "\"%s/bin/mifconv\" \"%s\" \"%s\"", mosyncdir_c, dst.c_str(), mifconvSrc.c_str());
#endif
		int res = run(buf);
		if(res != 0)
			errorExit("mifconf failed");
	}
}
