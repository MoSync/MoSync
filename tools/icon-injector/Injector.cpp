/* Copyright (C) 2009 Mobile Sorcery AB

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
			batik << "java -jar \""<<mosyncdir<<"/bin/Batik/batik-rasterizer.jar\""
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
