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

#include "config_platform.h"
#include <helpers/helpers.h>

#include "MemStream.h"

#ifdef _android
#include <jni.h>
#endif

//******************************************************************************
//MemStreamC
//******************************************************************************

using namespace Base;

MemStreamC::MemStreamC(const void* src, int size) : mSrc(src), mSize(size), mPos(0) {}
bool MemStreamC::isOpen() const {
	return mSrc != NULL;
}
bool MemStreamC::read(void* dst, int size) {
	TEST(isOpen());
	if(mPos + size > mSize) {
		FAIL;
	}
	memcpy(dst, (char*)mSrc + mPos, size);
	mPos += size;
	return true;
}

bool MemStreamC::length(int& aLength) const {
	TEST(isOpen());
	aLength = mSize;
	return true;
}
bool MemStreamC::seek(Seek::Enum mode, int offset) {
	TEST(isOpen());
	int newpos;
	switch(mode) {
	case Seek::Start: newpos = offset; break;
	case Seek::Current: newpos = mPos + offset; break;
	case Seek::End: newpos = mSize + offset; break;
	default:
		FAIL;
	}
	if(newpos > mSize || newpos < 0) {
		FAIL;
	}
	mPos = newpos;
	return true;
}
bool MemStreamC::tell(int& aPos) const {
	TEST(isOpen());
	aPos = mPos;
	return true;
}

#ifndef _android
Stream* MemStreamC::createLimitedCopy(int size) const {
#else
Stream* MemStreamC::createLimitedCopy(int size, JNIEnv* jNIEnv, jobject jThis) const {
#endif
	if(size < 0)
		size = mSize - mPos;
	else if(mPos + size > mSize) {
		FAIL;
	}
	return new MemStreamC(CBP mSrc + mPos, size);
}

Stream* MemStreamC::createCopy() const {
	return new MemStreamC(CBP mSrc, mSize);
}


//******************************************************************************
//MemStream
//******************************************************************************
MemStream::MemStream(int _size) : MemStreamC(open(_size), _size) {
#ifdef _android
	externalAllocated = false;
#endif
}
MemStream::MemStream(char* buf, int _size) : MemStreamC(buf, _size) {
	mBuffer = buf;
#ifdef _android
	externalAllocated = true;
#endif
}

MemStream::~MemStream() {
#ifdef _android
	if(false == externalAllocated)
#endif
	delete mBuffer;
}
char* MemStream::open(int size) {
	return mBuffer = new char[size];
	//return mBuffer = (char*) malloc(size);
}
bool MemStream::write(const void* src, int size) {
	TEST(isOpen());
	if(mPos + size > mSize) {
		FAIL;
	}
	memcpy(mBuffer + mPos, src, size);
	mPos += size;
	return true;
}
