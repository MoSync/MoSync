/*
 * streamreaderconvert.hpp
 *
 *  Created on: 2009-jun-09
 *      Author: Ali Mosavian
 */

#ifndef __STREAMREADERCONVERT_HPP__
#define __STREAMREADERCONVERT_HPP__

#include "streamreader.hpp"



/**
 * A StreamReader which does not convert the byte order.
 * Used when the endian is same.
 *
 */
class StreamReaderConvert
: public StreamReader
{
public:
	/**
	 * Constructor
	 *
	 * @param h 	MAHandle to binary stream
	 * @param offs	Starting offset
	 */
	StreamReaderConvert ( MAHandle h,
						  uint32 offs );

	/**
	 * Reads two unsigned bytes and increments position
	 *
	 */
	virtual uint16 readUInt16 ( void );


	/**
	 * Reads two signed bytes and increments position
	 *
	 */
	virtual sint16 readSInt16 ( void );


	/**
	 * Reads four unsigned bytes and increments position
	 *
	 */
	virtual uint32 readUInt32 ( void );


	/**
	 * Reads four signed bytes and increments position
	 *
	 */
	virtual sint32 readSInt32 ( void );

};

#endif /* STREAMREADERCONVERT_HPP_ */
