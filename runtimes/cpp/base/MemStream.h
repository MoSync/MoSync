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

		Stream* createLimitedCopy(int size) const;
		Stream* createCopy() const;
	protected:
		const void* mSrc;
		int mSize;
		int mPos;
	};

	class MemStream : public MemStreamC {
	public:
		MemStream(int _size);	//allocates memory
		virtual ~MemStream();	//frees memory
		bool write(const void* src, int size);
		void* ptr() { return mBuffer; }
	protected:
		char* open(int size);

		char* mBuffer;
	};


} // namespace Base

#endif // _BASE_MEM_STREAM_H_
