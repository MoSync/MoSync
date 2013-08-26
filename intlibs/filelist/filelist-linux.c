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

#include <glob.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>

#include "filelist.h"

int scanDirectory(const char* path, FileListCallback cb) {
	glob_t g;
	size_t i;
	int res = glob(path, 0, 0, &g);
	if(res != 0) {
		globfree(&g);
		if(res == GLOB_NOMATCH)
			return 0;
		return res;
	}
	for(i=0; i<g.gl_pathc; i++) {
		const char* pathv = g.gl_pathv[i];
		const char* lastSlash = strrchr(pathv, '/');
		const char* filename = (lastSlash == NULL) ? pathv : lastSlash + 1;
		cb(filename);
	}
	return 0;
}

int isDirectory(const char* filename) {
	struct stat s;
	int res = stat(filename, &s);
	if(res == -1)
		return -1;
	return S_ISDIR(s.st_mode);
}

int compareTime(const char* file1, const char* file2) {
	struct stat s1, s2;
	time_t t1, t2;
	int res1, res2;

	res1 = stat(file1, &s1);
	t1 = res1 ? 0 : s1.st_mtime;
	res2 = stat(file2, &s2);
	t2 = res2 ? 0 : s2.st_mtime;

	if (t1 == t2) {
		return 0;
	}

	return t1 - t2;
}

char* fullpath(const char* name) {
	return realpath(name, NULL);
}
