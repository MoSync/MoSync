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

#ifndef _MEMORY_H_
#define _MEMORY_H_

extern int gMemSize;
extern char* gMemBuf;

/**
 * Sets the size of the memory being cached and initializes the memory module.
 *
 * @param size Size of the memory.
 */
void setMemSize(int size);

/**
 * Clears the all cached memory locations.
 */
void clearMemoryCacheBits();

/**
 * Checks if the given memory locations are cached. After this call the 
 * specified memory locations will be marked as cached, i.e. assuming a call
 * to chache them will be made after this call.
 *
 * @param src Address of the beginning of the cached locations.
 * @param len The range of the bytes being cached.
 * @return True if the specified memory locations was cached, false otherwise.
 */
bool isMemoryCached(int src, int len);

#endif /* _MEMORY_H_ */
