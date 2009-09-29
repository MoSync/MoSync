/* REminiscence - Flashback interpreter
 * Copyright (C) 2005-2007 Gregory Montoir
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

#ifndef __FILE_H__
#define __FILE_H__

#include "intern.h"

struct File_impl;

struct File {
	File(bool gzipped = false);
	~File();

	File_impl *_impl;

	bool open(const char *filename, const char *directory, const char *mode);
	void close();
	bool ioErr() const;
	uint32 size();
	void seek(int32 off);
	void read(void *ptr, uint32 len);
	uint8 readByte();
	uint16 readUint16LE();
	uint32 readUint32LE();
	uint16 readUint16BE();
	uint32 readUint32BE();
	void write(void *ptr, uint32 size);
	void writeByte(uint8 b);
	void writeUint16BE(uint16 n);
	void writeUint32BE(uint32 n);
};

#endif // __FILE_H__
