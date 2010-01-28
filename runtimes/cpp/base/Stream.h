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

#ifndef _BASE_STREAM_H_	
#define _BASE_STREAM_H_	

#include <helpers/helpers.h>


namespace Base {

	namespace Seek {
		enum Enum {
			Start, Current, End
		};
	}

//Declare And Read
#define DAR_USHORT(name) ushort name; TEST(file.readUnsignedShort(name))
#define DAR_SHORT(name) short name; TEST(file.readShort(name))
#define DAR_UBYTE(name) byte name; TEST(file.readByte(name))
#define DAR_UVINT(name) int name; TEST(file.readUnsignedVarInt(name))
#define DAR_SVINT(name) int name; TEST(file.readSignedVarInt(name))


	class MemStream;

	class Stream {	//A read-write, seekable stream interface
	public:
		virtual bool isOpen() const = 0;
		virtual bool read(void* dst, int size) = 0;
		virtual bool write(const void* src, int size) = 0;

		bool readByte(byte& b);
		bool readUnsignedVarInt(int& res);
		bool readSignedVarInt(int& res);
		bool readUnsignedShort(ushort& res);
		bool readShort(short& res);

		template<class T> bool readObject(T& obj) {
			return read(&obj, sizeof(T));
		}

		//reads from this stream into all of dst. ignores dst's position.
		bool readFully(MemStream& dst);

		//writes all of src to this stream. requires src to have length.
		//ignores src's position. src's position is reset to 0 after this operation.
		bool writeFully(Stream& src);

		//writes some of src to this stream.
		//respects both src's and this stream's position.
		bool writeStream(Stream& src, int size);

		//supported by all so far, except connection streams.
		virtual bool length(int& aLength) const = 0;
		virtual bool seek(Seek::Enum mode, int offset) = 0;
		virtual bool tell(int& aPos) const = 0;

		//supported only by memory streams.
		//returns a pointer to the entire buffer.
		virtual const void* ptrc() { return NULL; }
		virtual void* ptr() { return NULL; }

		//Creates a copy of this stream, with the current position as the copy's starting point
		//and the specified size. The default size, < 0, means that (src_size - pos) will be used.
		//Returns NULL on failure.
		virtual Stream* createLimitedCopy(int size = -1) const = 0;

		//Creates a [new] read-only copy of this stream, with the position reset.
		//May fail if a stream is singular by nature (like a Connection stream),
		//or if the system is out of memory.
		//Must be [deleted].
		virtual Stream* createCopy() const = 0;

		virtual ~Stream() {}
	};

} // namespace Base

#endif // _BASE_STREAM_H_	
