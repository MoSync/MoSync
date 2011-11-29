/* Copyright (C) 2011 MoSync AB

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License,
version 2, as published by the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
MA 02110-1301, USA.
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
