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

#ifndef _FIFO_H_
#define _FIFO_H_

#include <helpers/helpers.h>
#include <helpers/CriticalSection.h>

using namespace MoSyncError;

//A FIFO Queue implemented using a non-resizable circular buffer
template<class T, int size> class CircularFifo {
public:
	CircularFifo() : mReadPos(0), mWritePos(0) { InitializeCriticalSection(&criticalSection); }

	void put(const T& t) {
		CriticalSectionHandler ccect(&criticalSection);
		mWritePos++;
		if(mWritePos == size)
			mWritePos = 0;
		//prevent overruns; lose the data being put, but not the rest.
		//max count becomes capacity - 1.
		if(mWritePos == mReadPos) {
			//mWritePos--;
			BIG_PHAT_ERROR(ERR_FIFO_OVERRUN);
		} else {
			mBuf[mWritePos] = t;
		}
	}
	const T& get() {
		CriticalSectionHandler ccect(&criticalSection);
		DEBUG_ASSERT(count() != 0);
		mReadPos++;
		if(mReadPos == size)
			mReadPos = 0;
		const T& t = mBuf[mReadPos];
		return t;
	}
	size_t count() {
		CriticalSectionHandler ccect(&criticalSection);
		if(mWritePos >= mReadPos)
			return mWritePos - mReadPos;
		else	//(mWritePos < mReadPos)
			return size - mReadPos + mWritePos;
	}
	void clear() {
		CriticalSectionHandler ccect(&criticalSection);
		mReadPos = mWritePos = 0;
	}
private:
	CRITICAL_SECTION criticalSection;

	T mBuf[size];
	size_t mReadPos, mWritePos;
};

#endif
