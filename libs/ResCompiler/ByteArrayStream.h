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
		int		size;
		int		pos; //pointer position in buffer
	};
}

#endif //__BYTEARRAYSTREAM_H__
