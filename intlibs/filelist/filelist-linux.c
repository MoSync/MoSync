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
