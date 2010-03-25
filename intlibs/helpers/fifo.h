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
