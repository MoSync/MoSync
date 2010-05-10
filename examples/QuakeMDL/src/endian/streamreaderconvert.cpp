/*
 * streamreaderconvert.cpp
 *
 *  Created on: 2009-jun-09
 *      Author: Ali Mosavian
 */
#include "endian/streamreaderconvert.hpp"


/**
 * Constructor
 *
 * @param h 	MAHandle to binary stream
 * @param offs	Starting offset
 */
StreamReaderConvert::StreamReaderConvert ( MAHandle h,
										   uint32 offs )
: StreamReader( h, offs )
{
}


/**
 * Reads two unsigned bytes and increments position
 *
 */
uint16 StreamReaderConvert::readUInt16 ( void )
{
	uint8 t, s[2];

	maReadData( m_stream, &s, m_pos, 2 );
	m_pos += 2;

	t = s[1];
	s[1] = s[0];
	s[0] = t;

	return *((uint16 *)s);
}

/**
 * Reads two signed bytes and increments position
 *
 */
sint16 StreamReaderConvert::readSInt16 ( void )
{
	uint8 t, s[2];

	maReadData( m_stream, &s, m_pos, 2 );
	m_pos += 2;

	t = s[1];
	s[1] = s[0];
	s[0] = t;

	return *((sint16 *)s);
}

/**
 * Reads four unsigned bytes and increments position
 *
 */
uint32 StreamReaderConvert::readUInt32 ( void )
{
	uint8 t, s[4];

	maReadData( m_stream, &s, m_pos, 4 );
	m_pos += 4;

	t = s[3];
	s[3] = s[0];
	s[0] = t;
	t = s[2];
	s[2] = s[1];
	s[1] = t;

	return *((uint32 *)s);

}


/**
 * Reads four signed bytes and increments position
 *
 */
sint32 StreamReaderConvert::readSInt32 ( void )
{
	uint8 t, s[4];

	maReadData( m_stream, &s, m_pos, 4 );
	m_pos += 4;

	t = s[3];
	s[3] = s[0];
	s[0] = t;
	t = s[2];
	s[2] = s[1];
	s[1] = t;

	return *((sint32 *)s);
}
