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

#include "demangle.h"

#ifndef __GNUC__
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

#include "compile.h"

int demangle_system(const char* mangled_name, const char* filename, char* buffer);

char* my_demangle(const char* mangled_name) {
	const char* filename = tmpnam(NULL);
	char buffer[1024];
	char* newbuf;
	int size, res;

	g_last_mangled_name = mangled_name;

	res = demangle_system(mangled_name, filename, buffer);
	remove(filename);
	if(!res)
		return NULL;

	size = strlen(buffer) + 1;
	newbuf = NewPtr(size); //malloc(size);
	if(!newbuf)
		return NULL;
	memcpy(newbuf, buffer, size);
	return newbuf;
}

int demangle_system(const char* mangled_name, const char* filename, char* buffer) {
	int res;
	FILE* file;

#ifdef __GNUC__
	const char* path = "c++filt";
#else
	char path[1024];
	char* pathEnd;

	if(!GetModuleFileName(NULL, path, sizeof(path)))
		return 0;

	pathEnd = strrchr(path, '\\');
	if(!pathEnd)
		return 0;

	pathEnd++;
	strcpy(pathEnd, "c++filt");
#endif

	sprintf(buffer, "%s %s > %s", path, mangled_name, filename);
	if(system(buffer) < 0)
		return 0;

	file = fopen(filename, "r");
	if(!file)
		return 0;
	res = fscanf(file, "%[^\n]", buffer);
	fclose(file);

	return res;
}
