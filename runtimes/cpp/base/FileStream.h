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

#ifndef _BASE_FILE_STREAM_H_
#define _BASE_FILE_STREAM_H_

#include "Platform.h"

#include "Stream.h"

#include <time.h>

// platform specific

#ifdef _android
#include <jni.h>
#endif

namespace Base {

	class FileStream : public Stream {	//read-only
	public:
		FileStream(const char* filename);
#ifdef _android
		explicit FileStream(int fd);
#endif
		virtual ~FileStream();
		virtual bool isOpen() const;
		virtual bool read(void* dst, int size);
		virtual bool write(const void*, int) { FAIL; }

		virtual bool length(int& aLength) const;
		virtual bool seek(Seek::Enum mode, int offset);
		virtual bool tell(int& aPos) const;
		virtual bool mTime(time_t&) const;

#ifndef _android
		virtual Stream* createLimitedCopy(int size) const;
#else
		virtual Stream* createLimitedCopy(int size, JNIEnv* jniEnv, jobject jthis) const;
#endif
		virtual Stream* createCopy() const;

		const char* getFilename() const;
		
		virtual bool truncate(int size) { FAIL; }

#include "FileImpl.h"
	};

	class WriteFileStream : public FileStream {
	public:
		// if append: open an existing file, or create a new one if needed.
		// else if exist: open an existing file with write access.
		// else: create a file. overwrite any existing file.
		WriteFileStream(const char* filename, bool append=false, bool exist=false);
		bool write(const void* src, int size);
		Stream* createLimitedCopy(int /*size*/) const { FAIL; }
		Stream* createCopy() const { FAIL; }
		bool truncate(int size);
	};

	class LimitedFileStream : public FileStream {	//read-only
	public:
#ifndef _android
		LimitedFileStream(const char* filename, int offset, int len);
#else
		LimitedFileStream(const char* filename, int offset, int len, JNIEnv* jNIEnv, jobject jThis);
#endif
		bool read(void* dst, int size);

		bool length(int& aLength) const;
		bool seek(Seek::Enum mode, int offset);
		bool tell(int& aPos) const;

		Stream* createLimitedCopy(int size) const;
		Stream* createCopy() const;
	protected:
		const int mStartPos, mEndPos;
		bool _open();
#ifdef _android
		JNIEnv* mJNIEnv;
		jobject mJThis;
#endif
	};

} // namespace Base

#endif // _BASE_FILE_STREAM_H_
