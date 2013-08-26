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

// should probably add a vector to the breakpoint.
typedef std::multimap<int, Breakpoint> BreakpointMap;	//key: bp-number

//todo: make value into a vector. maybe combine with InstructionMap.
typedef std::map<int, int> BreakpointAddressMap;	//key: address. value: bp-number

static const byte BREAKPOINT_OPCODE = 55;	//warning for hardcode

struct TempBreakpoint {
	void (*callback)();	//NULL if invalid.
	uint address;
	byte orig;
};

//******************************************************************************
// globals
//******************************************************************************

extern InstructionMap sInstructions;
extern BreakpointMap sBreakpoints;
extern BreakpointAddressMap sBreakpointAddresses;
extern TempBreakpoint gTempBreakpoint;

//******************************************************************************
// functions
//******************************************************************************

void abortIfRunning();

#endif	//CMD_BREAK_H
