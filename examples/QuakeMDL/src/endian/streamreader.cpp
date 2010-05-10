/*
 * streamreader.cpp
 *
 *  Created on: 2009-jun-09
 *      Author: Ali Mosavian
 */
#include <conprint.h>
#include "endian/streamreader.hpp"
#include "endian/streamreadersame.hpp"
#include "endian/streamreaderconvert.hpp"

/**
 * Constructor
 *
 * @param h 	MAHandle to binary stream
 * @param o		Starting offset
 */
StreamReader::StreamReader ( MAHandle h,
							 uint32 o )
{
	m_pos = o;
	m_beg = o;
	m_stream = h;
}


/**
 * Reads unsigned byte and increments position
 *
 */
uint8 StreamReader::readUInt8 ( void )
{
	uint8 b;

	maReadData( m_stream, &b, m_pos, 1 );
	m_pos++;
	return b;
}

/**
 *s Reads signed byte and increments position
 *
 */
sint8 StreamReader::readSInt8 ( void )
{
	sint8 b;

	maReadData( m_stream, &b, m_pos, 1 );
	m_pos++;
	return b;
}


/**
 * Reads a float
 *
 */
float32 StreamReader::readFloat32 ( void )
{
	float32 f;

	maReadData( m_stream, &f, m_pos, 4 );
	m_pos += 4;
	return f;
}


/**
 * Reads a double
 *
 */
float64 StreamReader::readFloat64 ( void )
{
	float64 f;

	maReadData( m_stream, &f, m_pos, 8 );
	m_pos += 8;
	return f;
}


/**
 * Reads a continuous stream of bytes
 *
 */
void StreamReader::read ( void *dst,
							   uint32 size )
{
	maReadData( m_stream, dst, m_pos, size );
	m_pos += size;
}


/**
 * Returns position pointer
 *
 */
uint32 StreamReader::getPos ( void )
{
	return m_pos;
}

/**
 * Seeks to a position within stream
 *
 */
void StreamReader::seek ( uint32 pos )
{
	m_pos = pos;
}


/**
 * Creates a StreamReader object which can be used to read
 * streams independent of the streams and the platforms
 * endianness.
 *
 * @param e		Endianness of the binary stream
 * @param h 	MAHandle to binary stream
 * @param o		Starting offset
 */

StreamReader* StreamReader::createStreamReader ( ENDIAN e,
												 MAHandle h,
												 uint32 o = 0 )
{
	uint32 	magic = 0xa0000000;
	ENDIAN	native= (*((char *)&magic) == 0xA) ? BIG : LITTLE;

	char *text[] = {(char*)"LITTLE", (char*)"BIG"};
	printf( "Endianness, source %s", text[e] );
	printf( "Endianness, native: %s", text[native] );
	if ( native == LITTLE )
	{
		if ( e == LITTLE )
			return new StreamReaderSame( h, o );
		else
			return new StreamReaderConvert( h, o );
	}
	else
	{
		if ( e == BIG )
			return new StreamReaderSame( h, o );
		else
			return new StreamReaderConvert( h, o );
	}
}

