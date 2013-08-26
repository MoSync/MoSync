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
