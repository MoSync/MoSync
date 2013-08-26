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
 * @file ByteArrayStream.cpp
 * @author Florin Leu
 * @date 22 Nov 2011
 **/

#include "ByteArrayStream.h"

namespace ResourceCompiler
{
	ByteArrayStream::ByteArrayStream():
	buffer(0),
	size(0),
	pos(0)
	{
	}

	ByteArrayStream::~ByteArrayStream()
	{
		DELETE(buffer);
	}

	ByteArrayStream::ByteArrayStream(int n)
	{
		buffer = new byte[n];
		size = n;
		pos = 0;
	}

	byte ByteArrayStream::readByte()
	{
		if (pos >= size)
		{
			return 0;
		}
		return buffer[pos++];
	}

	short ByteArrayStream::readShort()
	{
		if (pos >= size - 1)
		{
			return 0;
		}
		short ret = buffer[pos] + (buffer[pos + 1] << 8);
		pos += 2;
		return ret;
	}

	int ByteArrayStream::readInt()
	{
		if (pos >= size - 3)
		{
			return 0;
		}
		short ret = buffer[pos] + (buffer[pos + 1] << 8) + (buffer[pos + 2] << 16) + (buffer[pos + 3] << 24);
		pos += 4;
		return ret;
	}

	void ByteArrayStream::writeData(MAHandle placeHolder)
	{
		if (buffer != NULL)
		{
			maReadData(placeHolder, buffer, 0, size);
		}
	}
}
