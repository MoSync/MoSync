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

#include "filelist.h"

int scanDirectory(const char* path, FileListCallback cb) {
	glob_t g;
	size_t i;
	int res = glob(path, 0, 0, &g);
	if(res != 0) {
		globfree(&g);
		return res;
	}
	for(i=0; i<g.gl_pathc; i++) {
		cb(g.gl_pathv[i]);
	}
	return 0;
}

int isDirectory(const char* filename) {
	struct stat s;
	int res = stat(filename, &s);
	if(res != 0)
		return -1;
	return S_ISDIR(s.st_mode);
}
