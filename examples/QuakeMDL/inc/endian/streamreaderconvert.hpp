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
