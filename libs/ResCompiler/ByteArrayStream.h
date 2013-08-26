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

/**
 * @file ByteArrayStream.h
 * @author Florin Leu
 * @date 22 Nov 2011
 *
 * @brief Class that implements a byte array stream.
 *
 **/

#ifndef __BYTEARRAYSTREAM_H__
#define __BYTEARRAYSTREAM_H__

#include <ma.h>

#include "rescompdefines.h"

namespace ResourceCompiler
{
	class ByteArrayStream
	{
	public:
		ByteArrayStream();
		~ByteArrayStream();
		ByteArrayStream(int n);
		byte readByte();
		short readShort();
		int readInt();
		void writeData(MAHandle placeHolder);

	private:
		byte*	buffer;

		//buffer size
		int		size;

		//pointer position in buffer
		int		pos;
	};
}

#endif //__BYTEARRAYSTREAM_H__
