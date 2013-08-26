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

#ifndef _BASE_MEM_STREAM_H_
#define _BASE_MEM_STREAM_H_

#include "Stream.h"

namespace Base {

	class MemStreamC : public Stream {
	public:
		//does not own the memory
		MemStreamC(const void* _src, int _size);
		virtual bool isOpen() const;
		virtual bool read(void* dst, int size);
		virtual bool write(const void*, int) { FAIL; }
		const void* ptr() const { return mSrc; }

		bool length(int& aLength) const;
		bool seek(Seek::Enum mode, int offset);
		bool tell(int& aPos) const;

		const void* ptrc() { return mSrc; }
		virtual void* ptr() { FAIL; }
#ifndef _android
		Stream* createLimitedCopy(int size) const;
#else
		Stream* createLimitedCopy(int size, JNIEnv* jNIEnv, jobject jThis) const;
#endif
		Stream* createCopy() const;
	protected:
		const void* mSrc;
		int mSize;
		int mPos;
	};

	class MemStream : public MemStreamC {
	public:
		MemStream(int _size);	//allocates memory
#ifdef _android
		MemStream(char* _src, int _size);
#endif
		virtual ~MemStream();	//frees memory

		bool write(const void* src, int size);
		void* ptr() { return mBuffer; }
	protected:
		char* open(int size);
#ifndef _android
		MemStream(char* buf, int size);
#else
		bool externalAllocated;
#endif
		char* mBuffer;
	};


} // namespace Base

#endif // _BASE_MEM_STREAM_H_
