/*
Copyright (C) 2011 MoSync AB

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

/*
 * streamreadersame.cpp
 *
 *  Created on: 2009-jun-09
 *      Author: Ali Mosavian
 */
#include "endian/streamreadersame.hpp"


/**
 * Constructor
 *
 * @param h 	MAHandle to binary stream
 * @param offs	Starting offset
 */
StreamReaderSame::StreamReaderSame ( MAHandle h,
									 uint32 offs )
: StreamReader( h, offs )
{
}


/**
 * Reads two unsigned bytes and increments position
 *
 */
uint16 StreamReaderSame::readUInt16 ( void )
{
	uint16 s;

	maReadData( m_stream, &s, m_pos, 2 );
	m_pos += 2;
	return s;
}

/**
 * Reads two signed bytes and increments position
 *
 */
sint16 StreamReaderSame::readSInt16 ( void )
{
	sint16 s;

	maReadData( m_stream, &s, m_pos, 2 );
	m_pos += 2;
	return s;
}

/**
 * Reads four unsigned bytes and increments position
 *
 */
uint32 StreamReaderSame::readUInt32 ( void )
{
	uint32 i;

	maReadData( m_stream, &i, m_pos, 4 );
	m_pos += 4;
	return i;
}


/**
 * Reads four signed bytes and increments position
 *
 */
sint32 StreamReaderSame::readSInt32 ( void )
{
	sint32 i;

	maReadData( m_stream, &i, m_pos, 4 );
	m_pos += 4;
	return i;
}


