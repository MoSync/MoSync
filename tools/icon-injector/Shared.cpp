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

#include "Shared.h"
#include "Icon.h"

using namespace std;

namespace MoSync {

void errorExit(const string& reason) {
	printf("%s", reason.c_str());
	exit(1);
}

string getExtension(const string& str) {
	char ext[16];
	char *extd = ext;
	const char *fd = &str[str.length()-1];
	while(*fd && *fd != '.') {
		*extd++ = *fd--;
	}
	if(*fd == '.') {
		return ext;
	} else {
		return "";
	}
}

string getAbsolutePath(const string& path) {
	size_t i = path.length()-1;
	while(path[i]!='/'&&path[i]!='\\'&&i!=0) i--;
	return path.substr(0, i);
}

bool convertInstanceToImageFormat(const IconInstance *iconInstance, const char *dstFilename, const string& size, const string& format) {
	
	const char *mosyncdir_c = getenv("MOSYNCDIR");
	if(!mosyncdir_c) errorExit("MOSYNCDIR missing");
	string mosyncdir = mosyncdir_c;
	string magick = 
		mosyncdir+"\\bin\\ImageMagick\\convert " + iconInstance->filename + 
		" -resize " + size + " " + string(dstFilename);

	if(system(magick.c_str()) != 0) errorExit("Image Magick failed.");

	/*
	string ext = getExtension(iconInstance->filename);
	if(ext == "svg") {
	} else {
	}
	*/
	return true;
}
}
