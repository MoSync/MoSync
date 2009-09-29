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

#include <string>

#include "config.h"
#include "helpers/log.h"

#include "CoreCommon.h"
#include "sld.h"

#include "StubConnection.h"
#include "helpers.h"
#include "commandInterface.h"
#include "cmd_break.h"
#include "globals.h"

using namespace std;

void exec_continue(const string& args);
void exec_step_instruction(const string& args);
void exec_step(const string& args);
void exec_next_instruction(const string& args);
void exec_next(const string& args);
void exec_interrupt(const string& args);
void test_wait(const string& args);

namespace Callback {
	static void stepGetRegisters(const Registers&);
	static void stepBpWriteOrig();
	static void stepBpStep();
	static void stepStop(const Registers&);
	static void stepStep();
	static void stepRunning();

	static void continueStepDone();
	static void running();

	static void stepInstDone();

	static void stepSourceGetRegisters(const Registers&);
	static void stepSourceDone();
	static void stepSourceGetRegisters2(const Registers&);
	static void stepSourceCall();
	static void stepSourceCallReg(const Registers&);

	static void interruptRegs(const Registers& r);
	static void exitRegs(const Registers& r);

	static void nextGetRegisters(const Registers& r);
	static void stepTempBreakpoint(const Registers& r);

	static void wait();
}

static int sContinueAddress;
static void (*sStepDoneCallback)();
static void (*sStepHitCallback)();
static void (*sExecFunction)();
static void (*sStepSourceRegisterCallback)(const Registers& r);
static void (*sTempBreakpointCallback)();

static int sStepSourceLine;
static string sStepSourceFile;
static bool sStepRunning;

static int sExitCode;

//******************************************************************************
// continue
//******************************************************************************

void exec_continue(const string& args) {
	if(args.size() > 0) {
		error("Too many arguments");
		return;
	}
	sStepDoneCallback = Callback::continueStepDone;
	sExecFunction = Callback::continueStepDone;
	sStepRunning = false;
	StubConnection::getRegisters(Callback::stepGetRegisters);
}

static void Callback::continueStepDone() {
	StubConnection::execContinue(Callback::stepRunning);
}

//******************************************************************************
// step, shared
//******************************************************************************

static void Callback::stepGetRegisters(const Registers& r) {
	InstructionMap::const_iterator itr = sInstructions.find(r.pc);
	if(itr == sInstructions.end()) {
		//no breakpoint at this position; we can continue.
		sExecFunction();
		sStepHitCallback = sStepDoneCallback;
	} else {
		//we've got a breakpoint.
		//we have to temprarily replace it with the original instruction
		//while we step past it.
		//sequence: write orig, step, write bp, continue.
		sContinueAddress = r.pc;
		sStepHitCallback = stepBpStep;
		StubConnection::writeMemory(sContinueAddress + INSTRUCTION_MEMORY_START,
			&(itr->second.orig), 1, Callback::stepBpWriteOrig);
	}
}

static void Callback::stepBpWriteOrig() {
	StubConnection::step(Callback::stepRunning);
}

static void Callback::stepBpStep() {
	StubConnection::writeMemory(sContinueAddress + INSTRUCTION_MEMORY_START,
		&BREAKPOINT_INSTRUCTION, 1, sStepDoneCallback);
}

static void Callback::stepStep() {
	StubConnection::step(Callback::stepRunning);
}

static void Callback::stepRunning() {
	//only print "running" if we haven't done it before, for this step command.
	if(!sStepRunning) {
		sStepRunning = true;
		oprintToken();
		oprintf("^running\n");
		commandComplete();
	}
}

static void Callback::stepStop(const Registers& r) {
	oprintf("*stopped,reason=\"end-stepping-range\",frame={");
	oprintFrame(r.pc);
	oprintf("\n");
	fflush(stdout);
}

void StubConnection::stepHit() {
	sStepHitCallback();
}

//returns the length, in bytes, of the call instruction at the given PC.
//returns 0 if the instruction is not a call.
static int callLen(int pc) {
	if(gMemCs[pc] == _CALL) {
		return 2;
	} else if(gMemCs[pc] == _CALLI) {
		return 3;
	} else if(gMemCs[pc] == _FAR && gMemCs[pc + 1] == _CALLI) {
		return 4;
	} else {
		return 0;
	}
}

//******************************************************************************
// step-instruction
//******************************************************************************

// step-instruction
//new command flow:
// getRegisters
// if(breakpoint) {
//  writeOrig
//  step -> ack and async stepHit
//  writeBp
// } else {
//  step -> a&a
// }
// commandComplete

//async: stepHit

void exec_step_instruction(const string& args) {
	if(args.size() > 0) {
		error("Too many arguments");
		return;
	}
	sStepDoneCallback = Callback::stepInstDone;
	sExecFunction = Callback::stepStep;
	sStepRunning = false;
	StubConnection::getRegisters(Callback::stepGetRegisters);
}

static void Callback::stepInstDone() {
	StubConnection::getRegisters(Callback::stepStop);
}

//******************************************************************************
// step-source
//******************************************************************************

//This one is supposed to step past the prologue of a function call,
//but it's not very simple. For example, you must know if the next
//instruction is a call (see Callback::nextGetRegisters()), and if so,
//set a temporary breakpoint at the end of the prologue. Use nextSldEntry()
//to find that point. In yet another twist, that breakpoint should NOT be set if
//it's on the same source line as the starting instruction for this gdb command.

void exec_step(const string& args) {
	if(args.size() > 0) {
		error("Too many arguments");
		return;
	}
	//step instructions until we're no longer on this source line.
	//if we're not on a known source line, just step one instruction.
	//TODO: maybe we should keep stepping automatically until we arrive a known
	//source line, any line.
	sStepRunning = false;
	sStepSourceRegisterCallback = Callback::stepGetRegisters;
	StubConnection::getRegisters(Callback::stepSourceGetRegisters);
}

static void Callback::stepSourceGetRegisters(const Registers& r) {
	int line;
	string file;
	sExecFunction = Callback::stepStep;
	if(mapIp(r.pc, line, file)) {
		sStepSourceLine = line;
		sStepSourceFile = file;
		if(callLen(r.pc)) {
			sStepDoneCallback = Callback::stepSourceCall;
		} else {
			sStepDoneCallback = Callback::stepSourceDone;
		}
	} else {
		sStepDoneCallback = Callback::stepInstDone;
	}
	sStepSourceRegisterCallback(r);
}

static void Callback::stepSourceDone() {
	StubConnection::getRegisters(Callback::stepSourceGetRegisters2);
}

static void Callback::stepSourceGetRegisters2(const Registers& r) {
	int line;
	string file;
	if(mapIp(r.pc, line, file)) {
		if(!(line == sStepSourceLine && file == sStepSourceFile)) {
			stepStop(r);
		} else {
			sStepSourceRegisterCallback(r);
		}
	} else {
		stepStop(r);
	}
}

static void Callback::stepSourceCall() {
	StubConnection::getRegisters(Callback::stepSourceCallReg);
}

static void Callback::stepSourceCallReg(const Registers& r) {
	//we have now jumped into the function whose prologue we should run.
	int line;
	string file;
	if(mapIp(r.pc, line, file)) {
		if(line == sStepSourceLine && file == sStepSourceFile) {
			sStepDoneCallback = Callback::stepSourceDone;
			sStepSourceRegisterCallback(r);
		}
	}
	int a = nextSldEntry(r.pc);
	if(a < 0) {
		error("Broken function call");
		return;
	}
	gTempBreakpoint.address = a;
	gTempBreakpoint.orig = gMemCs[a];
	gTempBreakpoint.callback = Callback::stepTempBreakpoint;
	sStepDoneCallback = Callback::continueStepDone;
	sExecFunction = Callback::continueStepDone;
	StubConnection::writeMemory(gTempBreakpoint.address + INSTRUCTION_MEMORY_START,
		&BREAKPOINT_INSTRUCTION, 1, Callback::continueStepDone);
}

//******************************************************************************
// next-instruction
//******************************************************************************

// if(isCall(nextInstr)) {
//  setTempBreak(nextInstr);
//  continue();
// } else {
//  step();
// }

void exec_next_instruction(const string& args) {
	if(args.size() > 0) {
		error("Too many arguments");
		return;
	}
	sStepRunning = false;
	sTempBreakpointCallback = Callback::stepInstDone;
	sStepDoneCallback = Callback::stepInstDone;	//may be overwritten; that's cool.
	StubConnection::getRegisters(Callback::nextGetRegisters);
}

static void Callback::nextGetRegisters(const Registers& r) {
	int len = callLen(r.pc);
	if(len > 0) {
		gTempBreakpoint.address = r.pc + len;
		gTempBreakpoint.orig = gMemCs[gTempBreakpoint.address];
		gTempBreakpoint.callback = Callback::stepTempBreakpoint;
		sStepDoneCallback = Callback::continueStepDone;
		sExecFunction = Callback::continueStepDone;
		StubConnection::writeMemory(gTempBreakpoint.address + INSTRUCTION_MEMORY_START,
			&BREAKPOINT_INSTRUCTION, 1, continueStepDone);
	} else {
		//sStepDoneCallback = Callback::stepInstDone;
		sExecFunction = Callback::stepStep;
		stepGetRegisters(r);
	}
}

static void Callback::stepTempBreakpoint(const Registers& r) {
	gTempBreakpoint.callback = NULL;
	StubConnection::writeMemory(gTempBreakpoint.address + INSTRUCTION_MEMORY_START,
		&gTempBreakpoint.orig, 1, sTempBreakpointCallback);
}

//******************************************************************************
// next-source
//******************************************************************************

void exec_next(const string& args) {
	if(args.size() > 0) {
		error("Too many arguments");
		return;
	}
	sStepRunning = false;
	sStepSourceRegisterCallback = Callback::nextGetRegisters;
	sTempBreakpointCallback = Callback::stepSourceDone;
	sStepDoneCallback = Callback::stepSourceDone;
	StubConnection::getRegisters(Callback::stepSourceGetRegisters);
}

//******************************************************************************
// interrupt
//******************************************************************************

void exec_interrupt(const string& args) {
	if(args.size() > 0) {
		error("Too many arguments");
		return;
	}
	//TODO: error if inferior isn't running
	StubConnection::interrupt();
	oprintDoneLn();
	commandComplete();
}

void StubConnection::interruptHit() {
	LOG("interruptHit\n");
	StubConnection::getRegisters(Callback::interruptRegs);
}

static void Callback::interruptRegs(const Registers& r) {
	oprintf("*stopped,signal-name=\"SIGINT\",signal-meaning=\"Interrupt\",frame={");
	oprintFrame(r.pc);
	oprintf("\n" GDB_PROMPT);
	fflush(stdout);
}

//******************************************************************************
// exitHit
//******************************************************************************

void StubConnection::exitHit(int code) {
	sExitCode = code;
	StubConnection::getRegisters(Callback::exitRegs);
}

static void Callback::exitRegs(const Registers& r) {
	oprintf("*stopped,reason=\"exited\",exit-code=\"%x\",frame={", sExitCode);
	oprintFrame(r.pc);
	oprintf("\n" GDB_PROMPT);
	fflush(stdout);
}

//******************************************************************************
// test_wait
//******************************************************************************

static bool sWaiting = false;

void test_wait(const string& args) {
	if(args.size() > 0) {
		error("Too many arguments");
		return;
	}
	if(!StubConnection::isRunning()) {
		oprintDoneLn();
		commandComplete();
		return;
	}
	gTestWaiting = true;
}
