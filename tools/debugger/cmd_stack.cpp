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
#include <limits.h>

#include "config.h"
#include "helpers/helpers.h"

#include "stabs/stabs.h"

#include "sld.h"
#include "CoreCommon.h"

#include "StubConnection.h"
#include "helpers.h"
#include "commandInterface.h"
#include "stab_helpers.h"
#include "cmd_stack.h"

//#include "old_expr.h"
#include "expression.h"

#include "cmd_data.h"

using namespace std;

void stack_info_depth(const string& args);
void stack_list_frames(const string& args);
void stack_list_arguments(const string& args);
void stack_list_locals(const string& args);
void stack_select_frame(const string& args);
void stack_info_frame(const string& args);
void data_evaluate_expression(const string& args);
void thread_info(const string& args);
void console_whatis(const string& args);
void console_ptype(const string& args);

namespace Callback {
	static void lsGetRegisters(const Registers&);
	static void lsReadMem();
	static void sid();
	static void slf();
	static void sif();
	static void sla();
	static void sll();
	static void ssf();
	static void dee(const Value* value, const char *err);
	static void threadInfo(const Registers&);
	static void print_type(const Value* value, const char *err);
}

static void oprintFrameArgs(const FRAME& frame);

static void oprintLocalVariable(const LocalVariable* lv, const FRAME& frame);


//static int sMemSize = 0;
//char* sMemBuf = NULL;
static Registers sReg;
static size_t sCurrentFrameIndex;	//todo: should be invalidated when program runs.
static vector<FRAME> sFrames;
static void (*sLsCallback)();
static size_t sFrameRequest;
static PrintValueSimplicity sPrintValueSimplicity;
static int sInfoDepthMax;
static int sLowFrame, sHighFrame;
static string sToken;

const size_t& gCurrentFrameIndex(sCurrentFrameIndex);
const std::vector<FRAME>& gFrames(sFrames);
//const CharP& gMemBuf(gMemBuf);
const Registers& gReg(sReg);

//******************************************************************************
// continue/stop
//******************************************************************************

void stackContinued() {
	sFrames.clear();
}

//******************************************************************************
// loadStack
//******************************************************************************

void loadStack(void (*cb)()) {
	if(sFrames.size() > 0) {
		cb();
		return;
	}
	sLsCallback = cb;
	StubConnection::getRegisters(Callback::lsGetRegisters);
}

static void Callback::lsGetRegisters(const Registers& r) {
	sReg = r;
	u32 stackLow = r.gpr[REG_fr] - 8;
	u32 stackHi = gMemSize;
	if(stackLow >= stackHi) {	//strictly insufficient. todo: improve.
		LOG("stackLow: 0x%08x\n", stackLow);
		LOG("stackHi: 0x%08x\n", stackHi);
		LOG("Frame pointer out of bounds\n");
		//let's try to give it one frame at least.
		FRAME frame;
		frame.pc = r.pc;
		frame.pointer = r.gpr[REG_fr];
		sFrames.push_back(frame);
		sLsCallback();
		return;
	}
	int offset = MIN(stackLow, r.gpr[REG_sp]);
	StubConnection::readMemory(gMemBuf + offset, offset, stackHi - offset,
		Callback::lsReadMem);
}

static void Callback::lsReadMem() {
	u32 stackHi = gMemSize;
	u32 stackLow = sReg.gpr[REG_fr] - 8;
	u32 sp = sReg.gpr[REG_sp];
	u32 framePtr = sReg.gpr[REG_fr];
	u32 retAddr = sReg.gpr[REG_rt];	//initial value is ignored
	int offset = MIN(stackLow, sp);
	int pc = sReg.pc;
	uint level = 0;

	const int nWords = (stackHi - stackLow) / 4;
	LOG("framePtr: 0x%08x\n", framePtr);
	LOG("retAddr: 0x%08x\n", retAddr);
	LOG("pc: 0x%08x\n", pc);
	LOG("Stack: %i bytes (%i words) @ 0x%08x\n", stackHi - offset, nWords, sp);
	for(int i=0; i<nWords; ) {
		LOG("%08X:", offset + i*4);
		for(int j=0; j<4 && i<nWords; j++, i++) {
			LOG(" %08X", *(int*)(gMemBuf + offset + i*4));
		}
		LOG("\n");
	}

	sFrames.clear();
	do {
		if(framePtr == 0) {
			break;
		}
		FRAME frame;
		frame.pointer = framePtr;
		frame.pc = pc;
		sFrames.push_back(frame);

		//step to next frame
		if(framePtr >= stackHi || (framePtr - 8) < stackLow) {	//strictly insufficient. todo: improve.
			LOG("framePtr: 0x%08x\n", framePtr);
			error("Frame pointer out of bounds");
			return;
		}
		pc = retAddr = *(u32*)&gMemBuf[framePtr - 4];
		framePtr = *(int*)&gMemBuf[framePtr - 8];
		level++;
	} while(true);
	if(level != sFrames.size()) {
		LOG("Warning: stack broken at level %i (%i)\n", level, sFrames.size());
	}
	sLsCallback();
}

//******************************************************************************
// info-depth
//******************************************************************************

void stack_info_depth(const string& args) {
	sInfoDepthMax = INT_MAX;
	if(args.size() > 0) {
		if(!parseArgInt(args, &sInfoDepthMax))
			return;
	}
	loadStack(Callback::sid);
}

static void Callback::sid() {
	oprintDone();
	oprintf(",depth=\"%i\"\n", MIN((int)sFrames.size(), sInfoDepthMax));
	commandComplete();
}

//******************************************************************************
// list-frames
//******************************************************************************

static bool parseLhfArgs(const string& args) {
	if(args.size() == 0) {
		sLowFrame = 0;
		sHighFrame = INT_MAX;
		return true;
	}
	int len;
	int res = sscanf(args.c_str(), "%i %i%n", &sLowFrame, &sHighFrame, &len);
	if(res != 2 || len != (int)args.size()) {
		error("Bad arguments");
		return false;
	}
	if(sLowFrame < 0 || sHighFrame < 0) {
		error("Bad arguments");
		return false;
	}
	return true;
}

void stack_list_frames(const string& args) {
	if(!parseLhfArgs(args))
		return;

	loadStack(Callback::slf);
}

static void Callback::slf() {
	if(sLowFrame >= (int)sFrames.size()) {
		error("LowFrame out of bounds");
		return;
	}
	int level = sLowFrame;
	oprintDone();
	oprintf(",stack=[");
	do {
		if(level != sLowFrame)
			oprintf(",");
		oprintf("frame={level=\"%i\",", level);
		oprintFrame(sFrames[level].pc);
		level++;
	} while(level <= MIN(sHighFrame, (int)sFrames.size()-1));
	oprintf("]\n");
	commandComplete();
}

#define UNIMPL error("Unimplemented MI command: %s", __FUNCTION__)

//******************************************************************************
// list-arguments
//******************************************************************************
static void stack_list_base(const string& args, void (*cb)()) {
	CHECK_STABS;
	if(args.size() != 1) {
		error("Bad arguments");
		return;
	}
	if(!parseArgPVS(args, &sPrintValueSimplicity))
		return;
	loadStack(cb);
}

void stack_list_arguments(const string& args) {
	int len;
	int dummy;
	int res = sscanf(args.c_str(), "%i%n", &dummy, &len);
	if(res != 1) {
		error("Bad arguments");
		return;
	}
	if(!parseLhfArgs(args.substr(len)))
		return;
	stack_list_base(args.substr(0, len), Callback::sla);
}

void Callback::sla() {
	if(sLowFrame >= (int)sFrames.size()) {
		error("LowFrame out of bounds");
		return;
	}
	oprintDone();
	oprintf(",stack-args=[");
	for(int i=sLowFrame; i<=MIN(sHighFrame, (int)sFrames.size()-1); i++) {
		if(i != sLowFrame)
			oprintf(",");
		oprintf("frame={level=\"%i\",args=[", i);
		oprintFrameArgs(sFrames[i]);
		oprintf("]}");
	}
	oprintf("]\n");
	commandComplete();
}

static void oprintFrameArgs(const FRAME& frame) {
	const Function* f = stabsFindFunctionByInsideAddress(frame.pc);
	if(!f)	//probably a thunk or something
		return;
	for(size_t i=0; i<f->params.size(); i++) {
		if(i != 0)
			oprintf(",");
		oprintLocalVariable(f->params[i], frame);
	}
}

static void oprintLocalVariable(const LocalVariable* lv, const FRAME& frame) {
	if(sPrintValueSimplicity != eNameOnly)
		oprintf("{");
	oprintf("name=\"%s\"", lv->name.c_str());
	switch(sPrintValueSimplicity) {
	case eNameOnly:
		break;
	case eSimpleValues:
		oprintf(",type=\"");
		oprintf("%s", getType(lv->dataType, false).c_str());
		oprintf("\"");
		if(!lv->dataType->isSimpleValue)
			break;
	case eAllValues:
		{
		oprintf(",value=\"", lv->name.c_str());
		StringPrintFunctor spf;
		if(lv->storageClass == eStack) {
			const StackVariable* sv = (StackVariable*)lv;
			oprintf("%s", getValue(sv->dataType, &gMemBuf[frame.pointer + sv->offset],
				TypeBase::eNatural).c_str());
		} else if(lv->storageClass == eRegister) {
			const RegisterVariable* rv = (RegisterVariable*)lv;
			oprintf("%s", getValue(rv->dataType, &sReg.gpr[rv->reg],
				TypeBase::eNatural).c_str());
		} else if(lv->storageClass == eStatic) {
			DEBIG_PHAT_ERROR;	//todo
		} else {
			DEBIG_PHAT_ERROR;
		}
		oprintf("\"");
		}
	}
	if(sPrintValueSimplicity != eNameOnly)
		oprintf("}");
}

//******************************************************************************
// list-locals
//******************************************************************************
void stack_list_locals(const string& args) {
	stack_list_base(args, Callback::sll);
}

void Callback::sll() {
	const FRAME& frame(sFrames[sCurrentFrameIndex]);
	const Function* f = stabsFindFunctionByInsideAddress(frame.pc);
	if(!f) {
		error("No debug info for frame");
		return;
	}
	oprintDone();
	oprintf(",locals=[");
	bool first = true;
	int offset = frame.pc - f->address;
	for(size_t i=0; i<f->locals.size(); i++) {
		const ScopedVariable& sv(f->locals[i]);
		if(!sv.contains(offset))
			continue;
		if(first)
			first = false;
		else
			oprintf(",");
		oprintLocalVariable(sv.v, frame);
	}
	oprintf("]\n");
	commandComplete();
}

//******************************************************************************
// select-frame
//******************************************************************************
void stack_select_frame(const string& args) {
	if(sscanf(args.c_str(), "%i", &sFrameRequest) != 1) {
		error("Not enough arg");
		return;
	}
	loadStack(Callback::ssf);
}

void Callback::ssf() {
	if(sFrameRequest > sFrames.size()) {
		error("no such frame");
		return;
	}
	sCurrentFrameIndex = sFrameRequest;
	oprintDoneLn();
	commandComplete();
}

//******************************************************************************
// info-frame
//******************************************************************************
void stack_info_frame(const string& args) {
	loadStack(Callback::sif);
}

void Callback::sif() {
	int pc;
	if(sCurrentFrameIndex == 0) {
		pc = sReg.pc;
	} else if(sCurrentFrameIndex >= sFrames.size()) {
		error("Stack broken, cannot see selected frame.");
		return;
	} else {
		pc = sFrames[sCurrentFrameIndex].pc;
	}
	oprintDone();
	oprintf(",frame={level=\"%i\",", sCurrentFrameIndex);
	oprintFrame(pc);
	oprintf("\n");
	commandComplete();
}

//******************************************************************************
// data_evaluate_expression	//here because it needs stack frames
//******************************************************************************

static void Callback::dee(const Value* value, const char *err) {
	const SYM& sym = value->getSymbol();
	if(!err) {
		oprintDone();
		oprintf(",value=\"");
		StringPrintFunctor spf;
		sym.type->printMI(spf, value->getDataAddress(), TypeBase::eNatural);	
		oprintf("%s", spf.getString());
		oprintf("\"\n");
		commandComplete();
	} else {
		error("%s", err);
	}
}

void data_evaluate_expression(const string& args) {
	stackEvaluateExpression(args, -1, Callback::dee);
}

//******************************************************************************
// thread_info	//here for lack of a better place
//******************************************************************************

static void thread_info_print_start() {
	oprintDone();
	oprintf(",threads=[{id=\"1\",target-id=\"The One\",state=\"");
}

static void thread_info_print_end() {
	oprintf("}],current-thread-id=\"1\"\n");
}

void thread_info(const string& args) {
	if(StubConnection::isRunning()) {
		thread_info_print_start();
		oprintf("running\"");
		thread_info_print_end();
		commandComplete();
	} else {
		StubConnection::getRegisters(Callback::threadInfo);
	}
}

static void Callback::threadInfo(const Registers& r) {
	thread_info_print_start();
	oprintf("stopped\",frame={");
	oprintFrame(r.pc);
	thread_info_print_end();
	commandComplete();
}

//******************************************************************************
// whatis & ptype	//here for lack of a better place
//******************************************************************************

static bool sComplex;

static void console_print_type(const string& args, bool complex) {
	sComplex = complex;
	stackEvaluateExpression(args, -1, Callback::print_type);
}

static void Callback::print_type(const Value* value, const char *err) {
	//handle EOL in type string. See GDB. Replace with "\"\n~\"".
	if(err) {
		error("%s", err);
		return;
	} 
	string type = getType(value->getSymbol().type, sComplex);
	oprintf("~\"type = ");//%s\"\n", type);
	const char* t = type.c_str();
	while(*t) {
		if(*t == '\n')
			oprintf("\\n\"\n~\"");
		else
			oputc(*t);
		t++;
	}
	oprintf("\\n\"\n");
	oprintDoneLn();
	commandComplete();
}

void console_whatis(const string& args) {
	console_print_type(args, false);
}

void console_ptype(const string& args) {
	console_print_type(args, true);
}
