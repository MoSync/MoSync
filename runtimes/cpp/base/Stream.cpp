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
