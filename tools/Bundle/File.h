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

#ifndef _SE_MSAB_FILE_H_
#define _SE_MSAB_FILE_H_

#include <string>

typedef std::string String;
typedef bool Bool;

#define WIN32_LEAN_AND_MEAN
#include <windows.h>


	enum eFileSearchFlag {
		FSF_FILE=1,
		FSF_DIR=2
	};

	class File {
	public:


		File(String path="");
		~File();
		Bool isDirectory();
		void open(String path);
		void close();

		// only when File object is a directory
		Bool first(File*, int flags);
		Bool next(File*, int flags);

		const String& getAbsolutePath();
		const String& getName();

	protected:
		HANDLE file;
		WIN32_FIND_DATAA fd;
		String path;
	};
#endif //_SE_MSAB_FILE_H_
