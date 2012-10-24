/* Copyright (C) 2011 Mobile Sorcery AB

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

/** \file mastack.h
* \brief Call stack access
*/

#ifndef MASTACK_H
#define MASTACK_H

#ifdef __cplusplus
extern "C" {
#endif

#include "ma.h"

/// A MoSync call stack frame.
typedef struct MA_STACK_FRAME {
	/// Don't use this value.
	void* _nextFrame;
	/// The return instruction address.
	void* retAddr;
} MA_STACK_FRAME;

/// Returns the topmost call stack frame.
struct MA_STACK_FRAME* getStackTop(void);

/// Returns the next stack frame. Will return NULL if this is the last stack frame.
__inline struct MA_STACK_FRAME* nextFrame(struct MA_STACK_FRAME* frame);
__inline struct MA_STACK_FRAME* nextFrame(struct MA_STACK_FRAME* frame) {
	char* bp = (char*)(frame->_nextFrame);
	if(!bp)
		return NULL;
	else
		return (struct MA_STACK_FRAME*)(bp - 8);
}

#ifdef __cplusplus
}	//extern "C"
#endif

#endif /* MASTACK_H */
