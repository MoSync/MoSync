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

#ifndef FILELIST_H
#define FILELIST_H

#ifdef __cplusplus
extern "C" {
#endif

//filename contains only filename, not its path.
typedef void (*FileListCallback)(const char* filename);

//Calls \a cb once per file found in \a path.
//\a path must contain wildcards (* or ?) if you want to find more than one file.
//Returns non-zero on failure.
int scanDirectory(const char* path, FileListCallback cb);

//Returns 1 if the file is a directory.
//Returns 0 if the file is not a directory.
//Returns <0 if the file does not exist or is inaccessible.
int isDirectory(const char* filename);

// Returns a malloc'd string containing the absolute path of the file
// referred to by \a name.
// \a name is a relative filename.
// It is the responsibility of the caller to free() the returned string.
// If the path indicated by \name does not exist, this function returns NULL.
// The file itself need not exist, but the directory must.
// On error, errno is set.
char* fullpath(const char* name);

// Compares the timestamp (last modified) of two files.
// Returns a positive value if the first file is newer,
// a negative value if the second file is and zero if
// they are equal. A non-existing file counts as the lowest
// possible timestamp, so an existing file will always
// be considered 'newer' than a non-existing file.
// \file1 One file
// \file2 Another file
int compareTime(const char* file1, const char* file2);

#ifdef __cplusplus
}
#endif

#endif	//FILELIST_H
