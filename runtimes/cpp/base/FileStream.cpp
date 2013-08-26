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

#include "FileStream.h"

namespace Base {

	Stream* FileStream::createCopy() const {
		return new FileStream(getFilename());
	}
#ifndef _android
	Stream* FileStream::createLimitedCopy(int size) const {
#else
	Stream* FileStream::createLimitedCopy(int size, JNIEnv* jniEnv, jobject jthis) const {
#endif
		int curPos;
		TEST(tell(curPos));
		int src_size;
		TEST(length(src_size));
		if(size < 0)
			size = src_size - curPos;
		else if(curPos + size > src_size) {
			FAIL;
		}
#ifndef _android
		return new LimitedFileStream(getFilename(), curPos, size);
#else
		return new LimitedFileStream(getFilename(), curPos, size, jniEnv, jthis);
#endif
	}

	bool LimitedFileStream::_open() {
		//make sure it's big enough
		TEST(FileStream::seek(Seek::Start, mEndPos));
		//seek to our starting point
		TEST(FileStream::seek(Seek::Start, mStartPos));
		return true;
	}

	bool LimitedFileStream::read(void* dst, int size) {
		int curPos;
		TEST(FileStream::tell(curPos));
		if(curPos + size > mEndPos) {
			FAIL;
		}
		TEST(FileStream::read(dst, size));
		return true;
	}

	bool LimitedFileStream::length(int& aLength) const {
		TEST(isOpen());
		aLength = mEndPos - mStartPos;
		return true;
	}

	bool LimitedFileStream::tell(int& aPos) const {
		TEST(FileStream::tell(aPos));
		aPos -= mStartPos;
		return true;
	}

	bool LimitedFileStream::seek(Seek::Enum mode, int offset) {
		int newPos;
		if(mode == Seek::Start) {
			newPos = mStartPos + offset;
		} else if(mode == Seek::Current) {
			int curPos;
			TEST(FileStream::tell(curPos));
			newPos = curPos + offset;
		} else if(mode == Seek::End) {
			newPos = mEndPos + offset;
		} else {	//unsupported mode
			FAIL;
		}
		if(newPos < mStartPos || newPos > mEndPos) {
			FAIL;
		}
		TEST(FileStream::seek(Seek::Start, newPos));
		return true;
	}

	Stream* LimitedFileStream::createLimitedCopy(int size) const {
		int curPos;
		TEST(FileStream::tell(curPos));
		if(size < 0)
			size = mEndPos - curPos;
		else if(curPos + size > mEndPos) {
			FAIL;
		}

#ifndef _android
		return new LimitedFileStream(getFilename(), curPos, mEndPos - curPos);
#else
		return new LimitedFileStream(getFilename(), curPos, mEndPos - curPos, mJNIEnv, mJThis);
#endif
	}

	Stream* LimitedFileStream::createCopy() const {
		int curPos;
		TEST(FileStream::tell(curPos));

#ifndef _android
		return new LimitedFileStream(getFilename(), mStartPos, mEndPos - mStartPos);
#else
		return new LimitedFileStream(getFilename(), mStartPos, mEndPos - mStartPos, mJNIEnv, mJThis);
#endif
	}

}
