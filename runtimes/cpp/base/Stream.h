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

#ifndef _BASE_STREAM_H_	
#define _BASE_STREAM_H_	

#include <helpers/helpers.h>

#ifdef _android
#include <jni.h>
#endif

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
#ifndef _android
		virtual Stream* createLimitedCopy(int size = -1) const = 0;
#else
		virtual Stream* createLimitedCopy(int size, JNIEnv* jniEnv, jobject jthis) const = 0;
#endif
		//Creates a [new] read-only copy of this stream, with the position reset.
		//May fail if a stream is singular by nature (like a Connection stream),
		//or if the system is out of memory.
		//Must be [deleted].
		virtual Stream* createCopy() const = 0;

		virtual ~Stream() {}
	};

} // namespace Base

#endif // _BASE_STREAM_H_	
