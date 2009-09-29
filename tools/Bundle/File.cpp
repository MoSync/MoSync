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

/** Copyright (c) Mobile Sorcery AB 2005-2006 **/

#include "File.h"
#include <windows.h>

#ifdef _MSC_VER
#pragma warning(disable:4800)
#endif

File::File(String path) : file(NULL), path(path) {
}

File::~File() {
	close();
}

Bool File::isDirectory() {
	DWORD attributes = GetFileAttributesA((path).c_str());
	return attributes & FILE_ATTRIBUTE_DIRECTORY;
}
void File::open(String path) {
	this->path = path;
	file = NULL;
}

void File::close() {
	if(!file) return;
	FindClose(file);
}

Bool File::first(File *firstFile, int flags) {
	if(file) FindClose(file);
	file = FindFirstFileA((path + "\\" + "*").c_str(), &fd);
	if(file==INVALID_HANDLE_VALUE) return false;

	if((flags&FSF_FILE)&&(flags&FSF_DIR)) {
		firstFile->close();
		firstFile->open(path + "\\" + fd.cFileName);
		return true;
	}

	if((flags&FSF_FILE)&&(!(flags&FSF_DIR))) {
		while((fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
		{
			Bool ret = FindNextFileA(file, &fd);
			if(ret == false) return false;
		}
	}

	if(!(flags&FSF_FILE)&&(flags&FSF_DIR)) {
		while(!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
		{
			Bool ret = FindNextFileA(file, &fd);
			if(ret == false) return false;
		}
	}

	firstFile->close();
	firstFile->open(path + "\\" + fd.cFileName);

	return true;
}

Bool File::next(File *nextFile, int flags) {
	Bool ret = FindNextFileA(file, &fd);
	if(ret == false) return false;

	if((flags&FSF_FILE)&&(flags&FSF_DIR)) {
		nextFile->close();
		nextFile->open(path + "\\" + fd.cFileName);
		return true;
	}

	if((flags&FSF_FILE)&&(!(flags&FSF_DIR))) {
		while((fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
		{
			Bool ret = FindNextFileA(file, &fd);
			if(ret == false) return false;
		}
	}

	if(!(flags&FSF_FILE)&&(flags&FSF_DIR)) {
		while(!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
		{
			Bool ret = FindNextFileA(file, &fd);
			if(ret == false) return false;
		}
	}

	nextFile->close();
	nextFile->open(path + "\\" + fd.cFileName);

	return true;
}

const String& File::getAbsolutePath() {
	return (path);
}

String temp = "";

const String& File::getName() {
	int i = path.find_last_of("\\");
	if(i==String::npos) temp = "";
	//printf("FOUND '%s'\n", &path[i+1]);
	temp = String(&path[i+1]);

	return temp;
}