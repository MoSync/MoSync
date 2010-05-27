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
#elif defined(LINUX) || defined(DARWIN)
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
#else
#error Unsupported platform
#endif

#endif	//MKDIR_H
