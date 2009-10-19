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

#ifndef CMD_BREAK_H
#define CMD_BREAK_H

#include <string>
#include <map>

#include "helpers/types.h"
#include "StubConnection.h"

//******************************************************************************
// types
//******************************************************************************

struct Instruction {
	Instruction() : refCount(0) {}
	int refCount;
	byte orig;
};

struct Breakpoint {
	int address;	//may become a vector later
	bool keep;
	bool enabled;
	std::string func, file, path;
	int line;
	int times;
};

typedef std::map<int, Instruction> InstructionMap;	//key: address
typedef std::map<int, Breakpoint> BreakpointMap;	//key: bp-number

//todo: make value into a vector. maybe combine with InstructionMap.
typedef std::map<int, int> BreakpointAddressMap;	//key: address. value: bp-number

static const byte BREAKPOINT_OPCODE = 55;	//warning for hardcode

struct TempBreakpoint {
	void (*callback)();	//NULL if invalid.
	int address;
	byte orig;
};

//******************************************************************************
// globals
//******************************************************************************

extern InstructionMap sInstructions;
extern BreakpointMap sBreakpoints;
extern BreakpointAddressMap sBreakpointAddresses;
extern TempBreakpoint gTempBreakpoint;

#endif	//CMD_BREAK_H
