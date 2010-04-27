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

#include <config_platform.h>
#include "FileStream.h"
#include <stdio.h>
//#include "wce_helpers.h"

namespace Base {
	//******************************************************************************
	//FileStream
	//******************************************************************************
	const char* FileStream::getFilename() const {
		return mFilename;
	}
	FileStream::FileStream() {}
	FileStream::FileStream(const char* filename) : mFilename(filename) {
/*		
		char temp[256];
		getWorkingDirectory(temp, 256);
		strcat(temp, "\\");
		strcat(temp, filename);
		file = fopen(temp, "rb");
*/
		file = fopen(filename, "rb");
	}



	bool FileStream::isOpen() const {
		return file != NULL;
	}
	FileStream::~FileStream() {
		if(isOpen())
			fclose(file);
	}
	bool FileStream::read(void* dst, int size) {
		TEST(isOpen());
		int res = fread(dst, 1, size, file);
		return res == size;
	}
	bool FileStream::length(int& aLength) const {
		TEST(isOpen());
		int oldpos = ftell(file);

		if(fseek(file, 0, SEEK_END) != 0) {
			FAIL;
		}
		aLength = ftell(file);

		fseek(file, oldpos, SEEK_SET);
		return true;
	}
	bool FileStream::seek(Seek::Enum mode, int offset) {
		TEST(isOpen());
		if(mode == Seek::Start) {
			if(fseek(file, offset, SEEK_SET) != 0) {
				FAIL;
			}
		} else if(mode == Seek::Current) {
			int oldpos = ftell(file);
			if(fseek(file, offset, SEEK_CUR) != 0) {
				FAIL;
			}
		} else {	//unsupported mode
			FAIL;
		}
		return true;
	}
	bool FileStream::tell(int& aPos) const {
		TEST(isOpen());
		aPos = ftell(file);
		return true;
	}

	//******************************************************************************
	//LimitedFileStream
	//******************************************************************************
	LimitedFileStream::LimitedFileStream(const char* filename, int offset, int len)
		: FileStream(filename), mStartPos(offset), mEndPos(offset + len)
	{
		if(!_open()) {
			fclose(file);
			file = NULL;
		}
	}

	//******************************************************************************
	//WriteFileStream
	//******************************************************************************

	WriteFileStream::WriteFileStream(const char* filename, bool append) {
/*		
		char temp[256];	
		getWorkingDirectory(temp, 256);
		strcat(temp, "\\");
		strcat(temp, filename);
*/		
		if(append) {
			file = fopen(filename, "ab");
		} else {
			file = fopen(filename, "wb");
		}

		if(!file)
		{
			LOG("Could not open file for writing\n");
			//LOG(SDL_GetError());
		}
	}
	bool WriteFileStream::write(const void* src, int size) {
		TEST(isOpen());
		int res = fwrite(src, 1, size, file);
		return res == size;
	}

};
