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

	last_mangled_name = mangled_name;

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
