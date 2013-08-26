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

#include <config_platform.h>
#include "FileStream.h"
#include <stdio.h>
#include "wce_helpers.h"

namespace Base {
	//******************************************************************************
	//FileStream
	//******************************************************************************
	const char* FileStream::getFilename() const {
		return mFilename.c_str();
	}
	FileStream::FileStream() {}
	FileStream::FileStream(const char* filename) : mFilename(filename) {
		char temp[256];
		getWorkingDirectory(temp, 256);
		strcat(temp, "\\");
		strcat(temp, filename);
		file = fopen(temp, "rb");
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
	bool FileStream::mTime(time_t& t) const {
		// TODO
		FAIL;
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

	WriteFileStream::WriteFileStream(const char* filename, bool append, bool exist) {
		char temp[256];
		getWorkingDirectory(temp, 256);
		strcat(temp, "\\");
		strcat(temp, filename);

		if(append) {
			file = fopen(temp, "ab+");
		} else if(exist) {
			file = fopen(temp, "rb+");
		} else {
			file = fopen(temp, "wb+");
		}

		if(!file)
		{
			LOG("Could not open file for writing\n");
		}
	}
	bool WriteFileStream::write(const void* src, int size) {
		TEST(isOpen());
		int res = fwrite(src, 1, size, file);
		return res == size;
	}
	bool WriteFileStream::truncate(int size) {
		// TODO. Will probably require rewrite of this entire file to use Win32 functions.
		FAIL;
	}
};
