/* Copyright (C) 2010 MoSync AB

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

#include <android/log.h>
//#include "wce_helpers.h"

//#include <SyscallImpl.h>

#include <unistd.h>

namespace Base {
#if 0
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
		if(filename == 0)
			return;
		
		mFile = NULL;//fopen(filename, "rb");
	}
	
	FileStream::FileStream(FILE* file) : mFile(file) {
	
		mFilename = "";
		__android_log_write(ANDROID_LOG_INFO, "FileStream constructor", "1");
		
		if(!isOpen())
		{
			__android_log_write(ANDROID_LOG_INFO, "FileStream constructor", "2");
		}
		__android_log_write(ANDROID_LOG_INFO, "FileStream constructor", "3");
	
	}
	
	bool FileStream::isOpen() const {
		return mFile != NULL;
	}
	
	FileStream::~FileStream() {
		if(isOpen())
			fclose(mFile);
	}
	
	bool FileStream::read(void* dst, int size) {
		TEST(isOpen());
		int res = fread(dst, 1, size, mFile);
		
		// todo: disable in release mode
		char b[200];
		sprintf(b, "size: %i res: %i (read: %x)", size, res, dst);
		__android_log_write(ANDROID_LOG_INFO, "FileStream::read", b);
		
		return res == size;
	}
	
	bool FileStream::length(int& aLength) const {
		TEST(isOpen());
		int oldpos = ftell(mFile);

		if(fseek(mFile, 0, SEEK_END) != 0) {
			FAIL;
		}
		aLength = ftell(mFile);

		fseek(mFile, oldpos, SEEK_SET);
		return true;
	}
	
	bool FileStream::seek(Seek::Enum mode, int offset) {
		TEST(isOpen());
		if(mode == Seek::Start) {
			if(fseek(mFile, offset, SEEK_SET) != 0) {
				FAIL;
			}
		} else if(mode == Seek::Current) {
			int oldpos = ftell(mFile);
			if(fseek(mFile, offset, SEEK_CUR) != 0) {
				FAIL;
			}
		} else {	//unsupported mode
			FAIL;
		}
		return true;
	}
	
	bool FileStream::tell(int& aPos) const {
		TEST(isOpen());
		aPos = ftell(mFile);
		return true;
	}
#endif
	//******************************************************************************
	//LimitedFileStream
	//******************************************************************************
	LimitedFileStream::LimitedFileStream(const char* filename, int offset, int len, JNIEnv* jNIEnv, jobject jThis)
		: FileStream(filename) , mStartPos(offset), mEndPos(offset + len)
	{
		__android_log_write(ANDROID_LOG_INFO, "LimitedFileStream constructor", "1");
		
		mJNIEnv = jNIEnv;
		mJThis = jThis;
		
		
		// get a file handle for the namned filename file from Dalvik

		jclass cls = mJNIEnv->GetObjectClass(mJThis);
		jmethodID methodID = mJNIEnv->GetMethodID(cls, "getResourceFileDesriptor", "()Ljava/io/FileDescriptor;");
		if (methodID == 0) return;
		jobject jo = jNIEnv->CallObjectMethod(mJThis, methodID);
		
		methodID = jNIEnv->GetMethodID(cls, "getResourceStartOffset", "()I");
		if (methodID == 0) return;
		jint startOffset = mJNIEnv->CallIntMethod(mJThis, methodID);
		
		offset += startOffset;
		
		jNIEnv->DeleteLocalRef(cls);
		
		
		jclass fdClass = mJNIEnv->FindClass("java/io/FileDescriptor");
		if (fdClass != NULL) 
		{
			jclass fdPrgClassRef = (jclass) mJNIEnv->NewGlobalRef(fdClass); 
			jfieldID fdClassDescriptorFieldID = mJNIEnv->GetFieldID(fdPrgClassRef, "descriptor", "I");
			
			if (fdClassDescriptorFieldID != NULL && jo != NULL) 
			{			
				jint fd = mJNIEnv->GetIntField(jo, fdClassDescriptorFieldID);	
				mFd = dup(fd);
			}
		}
		
		if(!_open()) {
			__android_log_write(ANDROID_LOG_INFO, "LimitedFileStream constructor", "2");
			close(mFd);
			__android_log_write(ANDROID_LOG_INFO, "LimitedFileStream constructor", "3");
			mFd = -1;
		}	
		__android_log_write(ANDROID_LOG_INFO, "LimitedFileStream constructor", "4");
	}
#if 0
	//******************************************************************************
	//WriteFileStream
	//******************************************************************************

	WriteFileStream::WriteFileStream(const char* filename, bool append, bool exist) {
/*		
		char temp[256];	
		getWorkingDirectory(temp, 256);
		strcat(temp, "\\");
		strcat(temp, filename);
*/		
		const char* mode = append ? "ab+" : (exist ? "rb+" : "wb+");
		mFile = fopen(filename, mode);

		if(!mFile)
		{
			LOG("Could not open file for writing\n");
			//LOG(SDL_GetError());
		}
	}
	bool WriteFileStream::write(const void* src, int size) {
		TEST(isOpen());
		int res = fwrite(src, 1, size, mFile);
		return res == size;
	}
#endif
}	//namespace Base
