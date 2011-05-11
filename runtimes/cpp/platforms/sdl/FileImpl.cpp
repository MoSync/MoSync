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

#ifdef _MSC_VER
#include <io.h>
#else
#include <unistd.h>
#endif
#include <fcntl.h>
#include <sys/stat.h>
#include <errno.h>

enum MyOpenMode {
	eRead, eOverwrite, eAppend, eWriteExisting
};

#define LTEST(func) { int _res = (func); if(_res < 0) {\
	LOG("errno: %i(%s)\n", errno, strerror(errno)); FAIL; } }

static int myOpen(const char* filename, MyOpenMode mode) {
	// use low-level io.
	// parse mode.
#ifdef WIN32
	int flags = O_BINARY;
#else
	int flags = 0;
#endif
	switch(mode) {
		case eRead: flags |= O_RDONLY; break;
		case eOverwrite: flags |= O_CREAT | O_TRUNC | O_RDWR; break;
		case eAppend: flags |= O_CREAT | O_APPEND | O_RDWR; break;
		case eWriteExisting: flags |= O_RDWR; break;
	}

	// open file descriptor
	int fd = ::open(filename, flags, S_IREAD | S_IWRITE);
	if(fd < 0) {
		LOG("open(%s, 0x%x) failed: %i(%s)\n", filename, flags, errno, strerror(errno));
	}
	return fd;
}

namespace Base {
	//******************************************************************************
	//FileStream
	//******************************************************************************
	const char* FileStream::getFilename() const {
		return mFilename ? mFilename : "";
	}
	FileStream::FileStream() : mFilename(NULL) {}
	FileStream::FileStream(const char* filename) {
		int size = strlen(filename) + 1;
		mFilename = (char*)malloc(size);
		memcpy(mFilename, filename, size);
		
		// SDL_RWFromFile on Windows locks files,
		// but the glibc test suite requires that they be shared.
		//rwops = SDL_RWFromFile(filename, "rb");
		mFd = myOpen(filename, eRead);
	}
	bool FileStream::isOpen() const {
		return mFd > 0;
	}
	FileStream::~FileStream() {
		if(mFilename) {
			free(mFilename);
		}
		if(isOpen()) {
			::close(mFd);
		}
	}
	bool FileStream::read(void* dst, int size) {
		TEST(isOpen());
		byte* pos = (byte*)dst;
		byte* end = pos + size;
		while(pos != end) {
			int len = end - pos;
			int res = ::read(mFd, pos, len);
			if(res == 0) {
				LOG("Unexpected EOF.\n");
				FAIL;
			}
			LTEST(res);
			DEBUG_ASSERT(res <= len);
			pos += res;
		}
		return true;
	}
	bool FileStream::length(int& aLength) const {
		TEST(isOpen());
		int oldpos;
		LTEST(oldpos = lseek(mFd, 0, SEEK_CUR));
		LTEST(aLength = lseek(mFd, 0, SEEK_END));
		LTEST(lseek(mFd, oldpos, SEEK_SET));
		return true;
	}
	bool FileStream::seek(Seek::Enum mode, int offset) {
		TEST(isOpen());
		int lm;
		switch(mode) {
			case Seek::Start: lm = SEEK_SET; break;
			case Seek::Current: lm = SEEK_CUR; break;
			case Seek::End: lm = SEEK_END; break;
			default: DEBIG_PHAT_ERROR;
		}
		LTEST(lseek(mFd, offset, lm));
		return true;
	}
	bool FileStream::tell(int& aPos) const {
		TEST(isOpen());
		LTEST(aPos = lseek(mFd, 0, SEEK_CUR));
		return true;
	}
	bool FileStream::mTime(time_t& t) const {
		TEST(isOpen());
		struct stat s;
		LTEST(fstat(mFd, &s));
		t = s.st_mtime;
		return true;
	};

#ifdef _android
	FileStream::FileStream(int fd) : mFilename(NULL) {
		mFd = fd;
	}
#endif

#ifndef _android
	//******************************************************************************
	//LimitedFileStream
	//******************************************************************************
	LimitedFileStream::LimitedFileStream(const char* filename, int offset, int len)
		: FileStream(filename), mStartPos(offset), mEndPos(offset + len)
	{
		if(!_open()) {
			::close(mFd);
			mFd = -1;
		}
	}
#endif	//_android

	//******************************************************************************
	//WriteFileStream
	//******************************************************************************

	WriteFileStream::WriteFileStream(const char* filename, bool append, bool exist) {
		MyOpenMode mode = append ? eAppend : (exist ? eWriteExisting : eOverwrite);
		mFd = myOpen(filename, mode);
	}
	bool WriteFileStream::write(const void* src, int size) {
		TEST(isOpen());
		const byte* pos = (const byte*)src;
		const byte* end = pos + size;
		while(pos != end) {
			int len = end - pos;
			int res = ::write(mFd, pos, len);
			if(res == 0) {
				LOG("Unexpected EOF.\n");
				FAIL;
			}
			LTEST(res);
			DEBUG_ASSERT(res <= len);
			pos += res;
		}
		return true;
	}
#ifdef _MSC_VER
#define ftruncate _chsize
#endif
	bool WriteFileStream::truncate(int size) {
		TEST(isOpen());
		LTEST(ftruncate(mFd, size));
		return true;
	}

};
