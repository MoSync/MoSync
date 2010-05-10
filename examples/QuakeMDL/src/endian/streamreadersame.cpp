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


