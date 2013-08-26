/* Copyright 2013 David Axmark

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

	http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
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
