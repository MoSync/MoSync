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
#include <stdio.h>

#include "config.h"
#include "helpers/log.h"

#include "CoreCommon.h"
#include "sld.h"

#include "StubConnection.h"
#include "helpers.h"
#include "commandInterface.h"
#include "cmd_break.h"
#include "globals.h"
#include "opHandler.h"

using namespace std;

void exec_continue(const string& args);
void exec_step_instruction(const string& args);
void exec_step(const string& args);
void exec_next_instruction(const string& args);
void exec_next(const string& args);
void exec_interrupt(const string& args);
void test_wait(const string& args);

//******************************************************************************
// misc
//******************************************************************************
#define NOARGS if(args.size() > 0) { error("Too many arguments"); return; }

//returns the length, in bytes, of the call instruction at the given PC.
//returns 0 if the instruction is not a call.
static int callLen(int pc);

//problem: op may be async.
//solution: centralize Register retrieval, so it's always available.
static bool nextInstrIsBreak();
static bool nextInstrIsCall() { ASSERT_REG; return callLen(r.pc) > 0; }

static void skipBreak();
static void skipCallBreak(int address);

static OpHandler op;
OpHandler* OpHandler::mActiveHandler = NULL;

static int sStepLine;
static string sStepFile;

static bool sRunning = false;

//******************************************************************************
// ops
//******************************************************************************
static bool stubContinue();	//is done as soon as stub reports "started"
static bool stubRun();	//is done only when stub reported "stopped" somehow
static bool stubStep();	//is done only when stub reported "stopped" somehow
static bool runPrologue();
static bool sourceStepi();
static bool writeBreakedInstr(int address);
static bool writeBreak(int address);
static bool setTempBreak(int address);
static bool removeTempBreak(int address);
static bool sourceNexti();
static bool reportRunning();
static bool stepStop();

namespace Callback {
	static void opDone();
	static void opRunning();
	static void tempBreakpointHit();
}

//******************************************************************************
// continue
//******************************************************************************

void exec_continue(const string& args) {
	NOARGS;
	CHECK_REG;
	if(nextInstrIsBreak()) {
		skipBreak();
	}
	op(stubContinue);
	op(reportRunning);
}

//******************************************************************************
// step
//******************************************************************************

void exec_step_instruction(const string& args) {
	NOARGS;
	CHECK_REG;
	if(nextInstrIsBreak()) {
		skipBreak();
	} else {
		op(stubStep);
	}
	op(stepStop);
}

void exec_step(const string& args) {
	NOARGS;
	NEED_REG;
	if(!mapIp(r.pc, sStepLine, sStepFile)) {
		exec_step_instruction(args);
		return;
	}
	//skip breakpoint only on the first instruction
	if(nextInstrIsBreak()) {
		skipBreak();
		if(nextInstrIsCall()) {
			op(runPrologue);
			return;
		}
	}
	op(sourceStepi);
	op(stepStop);
}

static bool sourceStepi() {
	int curLine;
	string curFile;
	ASSERT_REG;
	if(!mapIp(r.pc, curLine, curFile)) {
		return true;
	}
	if(!(sStepLine == curLine && sStepFile == curFile)) {
		return true;
	}
	if(nextInstrIsCall()) {
		op(stubStep);
		op(runPrologue);
	} else {
		op(stubStep);
		op(sourceStepi);	//delayed recursive
	}
	return true;
}

void skipBreak() {
	NEED_REG;
	op(writeBreakedInstr, r.pc);
	op(stubStep);
	op(writeBreak, r.pc);
}

//******************************************************************************
// next
//******************************************************************************

void exec_next_instruction(const string& args) {
	//NOARGS; 
	//eclipse seems to send 1 as an argument...
	if(args.size() && args!=string("1")) {
		error("Wrong arguments");
		return;
	}

	NEED_REG;
	int len = callLen(r.pc);
	if(len > 0) {
		skipCallBreak(r.pc + len);
	} else {
		op(stubStep);
	}
	op(stepStop);
}

static void skipCall(int address) {
	op(setTempBreak, address);
	op(stubRun);
	op(removeTempBreak, address);
}

static void skipCallBreak(int address) {
	NEED_REG;
	if(nextInstrIsBreak())
		op(writeBreakedInstr, r.pc);
	skipCall(address);
	if(nextInstrIsBreak())
		op(writeBreak, r.pc);
}

void exec_next(const string& args) {
	NOARGS;
	NEED_REG;
	if(!mapIp(r.pc, sStepLine, sStepFile)) {
		exec_next_instruction(args);
		return;
	}
	//skip breakpoint only on the first instruction
	int len = callLen(r.pc);
	if(len > 0) {
		skipCallBreak(r.pc + len);
	} else if(nextInstrIsBreak()) {
		skipBreak();
	}
	op(sourceNexti);
	op(stepStop);
}

static bool sourceNexti() {
	int curLine;
	string curFile;
	ASSERT_REG;
	if(!mapIp(r.pc, curLine, curFile)) {
		return true;
	}
	if(!(sStepLine == curLine && sStepFile == curFile)) {
		return true;
	}
	int len = callLen(r.pc);
	if(len > 0) {
		skipCall(r.pc + len);
	} else {
		op(stubStep);
	}
	op(sourceNexti);	//delayed recursive
	return true;
}

//******************************************************************************
// interrupt
//******************************************************************************

void exec_interrupt(const string& args) {
	NOARGS;
	//TODO: error if inferior isn't running
	StubConnection::interrupt();
	oprintDoneLn();
	commandComplete();
}

void StubConnection::interruptHit() {
	LOG("interruptHit\n");
	ASSERT_REG;
	oprintf("*stopped,signal-name=\"SIGINT\",signal-meaning=\"Interrupt\",frame={");
	oprintFrame(r.pc);
	oprintf("\n" GDB_PROMPT);
	fflush(stdout);
	abortIfRunning();
}

//******************************************************************************
// misc
//******************************************************************************
	
static int callLen(int pc) {
	if(gMemCs[pc] == _CALL) {
		return 2;
	} else if(gMemCs[pc] == _CALLI) {
		return 3;
	} else if(gMemCs[pc] == _FAR && gMemCs[pc + 1] == _CALLI) {
		return 5;
	} else {
		return 0;
	}
}

static bool nextInstrIsBreak() {
	ASSERT_REG;
	InstructionMap::const_iterator itr = sInstructions.find(r.pc);
	return itr != sInstructions.end();
}

void StubConnection::stepHit() {
	LOG("stepHit\n");
	op.done();
}

void abortIfRunning() {
	if(sRunning) {
		sRunning = false;
		op.abort();
	}
}

//******************************************************************************
// exitHit
//******************************************************************************

void StubConnection::exitHit(int code) {
	ASSERT_REG;
	oprintf("*stopped,reason=\"exited\",exit-code=\"%x\",frame={", code);
	oprintFrame(r.pc);
	oprintf("\n" GDB_PROMPT);
	fflush(stdout);
	abortIfRunning();
}

//******************************************************************************
// test_wait
//******************************************************************************

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

//******************************************************************************
// helper ops
//******************************************************************************

static bool writeBreakedInstr(int address) {
	StubConnection::writeCodeMemory(address, &gMemCs[address], 1, &Callback::opDone);
	return false;
}

static bool writeBreak(int address) {
	StubConnection::writeCodeMemory(address, &BREAKPOINT_OPCODE, 1, &Callback::opDone);
	return false;
}

static bool setTempBreak(int address) {
	_ASSERT(gTempBreakpoint.callback == NULL);
	gTempBreakpoint.address = address;
	gTempBreakpoint.orig = gMemCs[address];
	gTempBreakpoint.callback = Callback::tempBreakpointHit;
	StubConnection::writeCodeMemory(address, &BREAKPOINT_OPCODE, 1, &Callback::opDone);
	return false;
}

static bool removeTempBreak(int address) {
	_ASSERT(gTempBreakpoint.callback != NULL && gTempBreakpoint.address == (uint)address);
	StubConnection::writeCodeMemory(address, &gMemCs[address], 1, &Callback::opDone);
	gTempBreakpoint.callback = NULL;
	return false;
}

static bool stubContinue() {
	StubConnection::execContinue(&Callback::opDone);
	return false;
}

static bool stubRun() {
	StubConnection::execContinue(&Callback::opRunning);
	return false;
}

static bool stubStep() {
	StubConnection::step(&Callback::opRunning);
	return false;
}

static bool runPrologue() {
	ASSERT_REG;
	int a = nextSldEntry(r.pc);
	if(a < 0) {
		error("Broken function call");
		return false;
	}
	op(setTempBreak, a);
	op(stubRun);
	op(removeTempBreak, a);
	op(stepStop);
	return true;
}

static bool reportRunning() {
	if(sRunning)
		return true;
	oprintToken();
	oprintf("^running\n");
	commandComplete();
	return true;
}

static bool stepStop() {
	ASSERT_REG;
	oprintf("*stopped,reason=\"end-stepping-range\",frame={");
	oprintFrame(r.pc);
	oprintf("\n" GDB_PROMPT);
	fflush(stdout);
	sRunning = false;
	return true;
}

//******************************************************************************
// helper's callbacks
//******************************************************************************

static void Callback::opDone() {
	op.done();
}

static void Callback::opRunning() {
	reportRunning();
	sRunning = true;
}

static void Callback::tempBreakpointHit() {
	_ASSERT(sRunning);
	op.done();
}
