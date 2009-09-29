/* Copyright (C) 2009 Mobile Sorcery AB

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License, version 2, as published by
the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

You should have received a copy of the GNU General Public License
along with this program; see the file COPYING.  If not, write to the Free
Software Foundation, 59 Temple Place - Suite 330, Boston, MA
02111-1307, USA.
*/

#ifndef _STREAM_HELPERS_H_
#define _STREAM_HELPERS_H_

//#include "Stream.h"

inline int readLEInt(Base::Stream *stream) {
	unsigned char data[4];
	stream->read(data, 4);
	return ((unsigned int)data[0]<<0)|((unsigned int)data[1]<<8)|((unsigned int)data[2]<<16)|((unsigned int)data[3]<<24);
}

inline unsigned int readLEUnsignedInt(Base::Stream *stream) {
	unsigned char data[4];
	stream->read(data, 4);
	return ((unsigned int)data[0]<<0)|((unsigned int)data[1]<<8)|((unsigned int)data[2]<<16)|((unsigned int)data[3]<<24);
}

inline short readLEShort(Base::Stream *stream) {
	unsigned char data[2];
	stream->read(data, 2);
	return ((unsigned short)data[0]<<0)|((unsigned short)data[1]<<8);
}

inline unsigned short readLEUnsignedShort(Base::Stream *stream) {
	unsigned char data[2];
	stream->read(data, 2);
	return ((unsigned short)data[0]<<0)|((unsigned short)data[1]<<8);
}

inline int readBEInt(Base::Stream *stream) {
	unsigned char data[4];
	stream->read(data, 4);
	return ((unsigned int)data[0]<<24)|((unsigned int)data[1]<<16)|((unsigned int)data[2]<<8)|((unsigned int)data[3]<<0);
}

inline unsigned int readBEUnsignedInt(Base::Stream *stream) {
	unsigned char data[4];
	stream->read(data, 4);
	return ((unsigned int)data[0]<<24)|((unsigned int)data[1]<<16)|((unsigned int)data[2]<<8)|((unsigned int)data[3]<<0);
}

inline short readBEShort(Base::Stream *stream) {
	unsigned char data[2];
	stream->read(data, 2);
	return ((unsigned short)data[0]<<8)|((unsigned short)data[1]<<0);
}

inline unsigned short readBEUnsignedShort(Base::Stream *stream) {
	unsigned char data[2];
	stream->read(data, 2);
	return ((unsigned short)data[0]<<8)|((unsigned short)data[1]<<0);
}

#endif /* _STREAM_HELPERS_H_ */
