/*
 * endianwrap.hpp
 *
 *  Created on: 2009-jun-09
 *      Author: Ali Mosavian
 */

#ifndef __STREAMREADER_HPP__
#define __STREAMREADER_HPP__

#include <maapi.h>
#include "../common.h"



/**
 * Endian independent stream reader
 *
 */
class StreamReader
{
protected:
	uint32	m_pos;
	uint32	m_beg;
	MAHandle	m_stream;

	/**
	 * Constructor
	 *
	 * @param h 	MAHandle to binary stream
	 * @param offs	Starting offset, default is 0
	 */
	StreamReader ( MAHandle h, uint32 offs );

public:
	enum ENDIAN
	{
		LITTLE,
		BIG
	};

	/**
	 * Reads unsigned byte and increments position
	 *
	 */
	uint8 readUInt8 ( void );


	/**
	 * Reads signed byte and increments position
	 *
	 */
	sint8 readSInt8 ( void );


	/**
	 * Reads two unsigned bytes and increments position
	 *
	 */
	virtual uint16 readUInt16 ( void ) = 0;


	/**
	 * Reads two signed bytes and increments position
	 *
	 */
	virtual sint16 readSInt16 ( void ) = 0;


	/**
	 * Reads four unsigned bytes and increments position
	 *
	 */
	virtual uint32 readUInt32 ( void ) = 0;


	/**
	 * Reads four signed bytes and increments position
	 *
	 */
	virtual sint32 readSInt32 ( void ) = 0;


	/**
	 * Reads a float
	 *
	 */
	float32 readFloat32 ( void );


	/**
	 * Reads a double
	 *
	 */
	float64 readFloat64 ( void );


	/**
	 * Reads a continuous stream of bytes
	 *
	 */
	void read ( void *dst, uint32 bytes );


	/**
	 * Get stream position
	 *
	 */
	uint32 getPos ( void );


	/**
	 * Seeks to a position within stream
	 *
	 */
	void seek ( uint32 pos );



	/**
	 * Creates a StreamReader object which can be used to read
	 * streams independent of the streams and the platforms
	 * endianness.
	 *
	 * @param e		Endianness of the binary stream
	 * @param h 	MAHandle to binary stream
	 * @param o		Starting offset
	 */
	static StreamReader* createStreamReader ( ENDIAN e,
											  MAHandle h,
											  uint32 offs );
};


#endif /* ENDIANWRAP_HPP_ */
