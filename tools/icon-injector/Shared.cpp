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
#ifdef WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif
#include <stdlib.h>

using namespace std;

namespace MoSync {

void errorExit(const string& reason) {
	printf("%s\n", reason.c_str());
	exit(1);
}

string getExtension(const string& str) {
	size_t i = str.find_last_of('.');
	if(i == string::npos)
		return "";
	else
		return str.substr(i + 1);
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
	string resizeFlag;
	if(size != "default")
		resizeFlag = " -resize " + size;
	string magick = 
		"\""+mosyncdir+"\\bin\\ImageMagick\\convert.exe\" \"" + iconInstance->filename + "\"" +
		resizeFlag + " \"" + string(dstFilename) + "\"";

	if(run(magick.c_str()) != 0) errorExit("Image Magick failed.");

	/*
	string ext = getExtension(iconInstance->filename);
	if(ext == "svg") {
	} else {
	}
	*/
	return true;
}

void sizeString(const std::string& size, int* w, int* h) {
	size_t ofs;
	if((ofs=size.find("x"))!=std::string::npos) {
		*w = atoi(size.substr(0, ofs).c_str());
		*h = atoi(size.substr(ofs+1, size.length()-(ofs+1)).c_str());
	} else {
		errorExit("Invalid size string.");
	}
}

int run(const char* cmd) {
	printf("%s\n", cmd);
#ifdef WIN32
#define GLE(e, func, args) { BOOL res = (func args); if(res == e) { int err = GetLastError();\
	printf(#func " error %i\n", err); return err; } }

	// hopefully this method should be more stable than the system() function.
	PROCESS_INFORMATION pi;
	STARTUPINFO si;
	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	GLE(0, CreateProcess, (NULL, (char*)cmd, NULL, NULL, false, 0, NULL, NULL, &si, &pi));
	GLE(0, CloseHandle, (pi.hThread));
	GLE(WAIT_FAILED, WaitForSingleObject, (pi.hProcess, INFINITE));
	DWORD exitCode;
	GLE(0, GetExitCodeProcess, (pi.hProcess, &exitCode));
	if(exitCode != 0) {
		printf("exitCode: %i\n", exitCode);
	}
	return exitCode;
#else
	int res = system(cmd);
	if(res != 0) {
		printf("Error %i\n", res);
	}
	return res;
#endif
}

}
