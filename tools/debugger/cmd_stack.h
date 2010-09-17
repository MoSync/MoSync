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
