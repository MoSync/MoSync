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

void setMemSize(int size);

// all bits are belong to us
void clearMemoryCacheBits();

// will fill all bits between src and src+len with ones
// i.e. you have to download memory between src+len if memory
// isn't cached.
bool isMemoryCached(int src, int len);

#endif /* _MEMORY_H_ */