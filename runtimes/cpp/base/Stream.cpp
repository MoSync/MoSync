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
#include <helpers/helpers.h>
#include "Stream.h"
#include "MemStream.h"
#include <helpers/smartie.h>

namespace Base {

	//******************************************************************************
	//Stream
	//******************************************************************************

	bool Stream::readByte(byte& b) {
		TEST(read(&b, 1));
		return true;
	}
	bool Stream::readUnsignedVarInt(int& res) {
		int nBytes=0;
		res = 0;
		while(true) {
			byte b;
			TEST(readByte(b));
			res |= ((int)(b & 0x7f)) << (nBytes*7);
			if(b > 0x7f)
				break;
			nBytes++;
			if(nBytes >= 4) {
				FAIL;
			}
		}
		return true;
	}
	bool Stream::readSignedVarInt(int& res) {
		int nBytes=0;
		byte b;
		res = 0;
		TEST(readByte(b));
		res |= (b & 0x7e) >> 1;
		bool sign = (b & 1) != 0;
		nBytes++;
		while(b <= 0x7f) {
			TEST(readByte(b));
			res |= ((int)(b & 0x7f)) << ((nBytes*7) - 1);
			if(b > 0x7f)
				break;
			nBytes++;
			if(nBytes >= 4) {
				FAIL;
			}
		}
		if(sign)
			res = -res;
		return true;
	}
	bool Stream::readUnsignedShort(ushort& res) {
		Stream& file = *this; //to placate the macros
		DAR_UBYTE(d);
		DAR_UBYTE(c);
		res = (((ushort(c) & 0xff) << 8) | (ushort(d) & 0xff));
		return true;
	}
	bool Stream::readShort(short& res) {
		Stream& file = *this; //to placate the macros
		DAR_UBYTE(d);
		DAR_UBYTE(c);
		res = (((ushort(c) & 0xff) << 8) | (ushort(d) & 0xff));
		return true;
	}
	bool Stream::readFully(MemStream& dst) {
		int len;
		TEST(dst.length(len));
		TEST(read(dst.ptr(), len));
		return true;
	}

	bool Stream::writeFully(Stream& src) {
		int len;
		TEST(src.length(len));
#if 0
		const void* psrc = src.ptrc();
		if(psrc) {	//memory source stream
			TEST(this->write(psrc, len));
		} else {
			TEST(src.seek(Seek::Start, 0));
			void* pdst = this->ptr();
			if(pdst) {	//memory destination stream
				TEST(src.read(pdst, len));
			} else {
				Smartie<char> temp(new char[len]);
				TEST(temp);
				TEST(src.read(temp(), len));
				TEST(this->write(temp(), len));
			}
		}
		TEST(src.seek(Seek::Start, 0));
#else
		TEST(src.seek(Seek::Start, 0));
		TEST(writeStream(src, len));
		TEST(src.seek(Seek::Start, 0));
#endif
		return true;
	}

	bool Stream::writeStream(Stream& src, int size) {
		const void* psrc = src.ptrc();
		if(psrc) {	//memory source stream
			int pos;
			TEST(src.tell(pos));
			int srcSize;
			TEST(src.length(srcSize));
			TEST(pos + size <= srcSize);
			TEST(this->write((char*)psrc + pos, size));
		} else {
			void* pdst = this->ptr();
			if(pdst) {	//memory destination stream
				TEST(src.read(pdst, size));
			} else {
				Smartie<char> temp(new char[size]);
				TEST(temp);
				TEST(src.read(temp(), size));
				TEST(this->write(temp(), size));
			}
		}
		return true;
	}

}
