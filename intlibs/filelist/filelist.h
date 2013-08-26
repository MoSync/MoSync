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
