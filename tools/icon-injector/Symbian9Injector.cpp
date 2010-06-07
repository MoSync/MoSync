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

#include "Symbian9Injector.h"
#include "Shared.h"
#include <stdlib.h>

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
		sprintf(buf, "\"%s\\bin\\mifconv.exe\" \"%s\" \"%s\"", mosyncdir_c, dst.c_str(), mifconvSrc.c_str());
		int res = run(buf);
		if(res != 0)
			errorExit("mifconf failed");
	}
}
