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

#include "config_platform.h"
#include "FileStream.h"

namespace Base {
	//******************************************************************************
	//FileStream
	//******************************************************************************
	const char* FileStream::getFilename() const {
		return mFilename.c_str();
	}
	FileStream::FileStream() {}
	FileStream::FileStream(const char* filename) : mFilename(filename) {
		rwops = SDL_RWFromFile(filename, "rb");
		if(!rwops)
		{
			LOG("SDL_RWFromFile(%s, rb) = %p\n", filename, rwops);
			LOG("%s\n", SDL_GetError());
		}
	}
	bool FileStream::isOpen() const {
		return rwops != NULL;
	}
	FileStream::~FileStream() {
		if(isOpen())
			SDL_RWclose(rwops);
			//SDL_FreeRW(rwops);
	}
	bool FileStream::read(void* dst, int size) {
		TEST(isOpen());
		int res = SDL_RWread(rwops, dst, 1, size);
		return res == size;
	}
	bool FileStream::length(int& aLength) const {
		TEST(isOpen());
		int oldpos = SDL_RWtell(rwops);
		aLength = SDL_RWseek(rwops, 0, SEEK_END);
		if(aLength < 0) {
			FAIL;
		}
		SDL_RWseek(rwops, oldpos, SEEK_SET);
		return true;
	}
	bool FileStream::seek(Seek::Enum mode, int offset) {
		TEST(isOpen());
		if(mode == Seek::Start) {
			if(SDL_RWseek(rwops, offset, SEEK_SET) != offset) {
				FAIL;
			}
		} else if(mode == Seek::Current) {
			int oldpos = SDL_RWtell(rwops);
			int newpos = SDL_RWseek(rwops, offset, SEEK_CUR);
			if(newpos != oldpos + offset) {
				FAIL;
			}
		} else {	//unsupported mode
			FAIL;
		}
		return true;
	}
	bool FileStream::tell(int& aPos) const {
		TEST(isOpen());
		aPos = SDL_RWtell(rwops);
		return true;
	}

	//******************************************************************************
	//LimitedFileStream
	//******************************************************************************
	LimitedFileStream::LimitedFileStream(const char* filename, int offset, int len)
		: FileStream(filename), mStartPos(offset), mEndPos(offset + len)
	{
		if(!_open()) {
			SDL_RWclose(rwops);
			rwops = NULL;
		}
	}

	//******************************************************************************
	//WriteFileStream
	//******************************************************************************

	WriteFileStream::WriteFileStream(const char* filename, bool append) {
		const char* mode = append ? "ab" : "wb";
		rwops = SDL_RWFromFile(filename, mode);
		if(!rwops)
		{
			LOG("SDL_RWFromFile(%s, %s) = %p\n", filename, mode, rwops);
			LOG("%s\n", SDL_GetError());
		}
	}
	bool WriteFileStream::write(const void* src, int size) {
		TEST(isOpen());
		int res = SDL_RWwrite(rwops, src, 1, size);
		return res == size;
	}

};
