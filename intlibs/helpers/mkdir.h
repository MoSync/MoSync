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

#ifndef MKDIR_H
#define MKDIR_H

// returns 0 on success.

#ifdef WIN32
#ifdef _WIN32_WCE
#include <winbase.h>
inline int _wmkdir(const wchar_t* name) {
	BOOL res = CreateDirectoryW(name, NULL);
	return res != 0 ? 0 : -1;
}
inline int _wrmdir(const wchar_t* name) {
	BOOL res = RemoveDirectoryW(name);
	return res != 0 ? 0 : -1;
}
inline int wremove(const wchar_t* name) {
	BOOL res = DeleteFileW(name);
	return res != 0 ? 0 : -1;
}
#else
#include <direct.h>
#endif	//_WIN32_WCE
#elif defined(LINUX) || defined(DARWIN) || defined(__IPHONE__)
#include <unistd.h>
#include <sys/stat.h>
int _mkdir(const char* name);
inline int _mkdir(const char* name) {
	return mkdir(name, 0755);
}
int _rmdir(const char* name);
inline int _rmdir(const char* name) {
	return rmdir(name);
}
int _chdir(const char* name);
inline int _chdir(const char* name) {
	return chdir(name);
}
#else
#error Unsupported platform
#endif

#endif	//MKDIR_H
