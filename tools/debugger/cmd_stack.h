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

#ifndef CMD_STACK_H
#define CMD_STACK_H

#include <vector>

#include "StubConnection.h"
#include "memory.h"

void stackContinued();
void stackSetMemSize(int size);

void console_whatis(const std::string& args, const std::string& token);

//populates gFrames and sets gCurrentFrameIndex, then calls cb.
//on failure, calls error() and not the callback.
void loadStack(void (*cb)());
void assertStack();
#define ASSERT_STACK assertStack()

struct FRAME {
	int pointer, pc;
};

typedef char* CharP;

extern const size_t& gCurrentFrameIndex;
extern const std::vector<FRAME>& gFrames;

#endif	//CMD_STACK_H
