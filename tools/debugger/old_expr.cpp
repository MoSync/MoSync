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
#include "helpers/helpers.h"

#include "stabs/stabs.h"

#include "cmd_stack.h"
#include "helpers.h"
#include "old_expr.h"
#include "stab_helpers.h"

#include "expression.h"
#include "helpers.h"
#include "commandInterface.h"

using namespace std;

namespace Callback {
	static void see();
	static void seeSymbol(const SYM& sym);
	static void seeAddressOf(const SYM& sym);
	static void seeDeref(const SYM& sym);
	static void seeSizeof(const SYM& sym);
	static void seeReadMem();
}

//static int sFrameAddr;
static string sExpr;
static EECallback sEECallback;
static SeeCallback sSeeCallback;
static SYM sSeeSym;
static char* sPtr;
static int sPos;
static const int BUFSIZE=1024;
static bool sComplex;

//******************************************************************************
// stackEvaluateExpression
//******************************************************************************

/*
void stackEvaluateExpression(const std::string& expr, int frameAddr,
	EECallback cb, bool complex)
{
	if(frameAddr >= 0) {
		//todo: search the stack for a matching frame.
		// if no matching frame is found, the address can be considered faulty.
		error("Don't know how to handle specific frame address");
		return;
	}
	CHECK_STABS;
	sExpr = expr;
	sEECallback = cb;
	sComplex = complex;
	loadStack(Callback::see);
}
*/

void Callback::see() {
	SeeCallback cb;

	//custom hack-filtering
	if(sExpr == "*((this)+0)@1") {
		sExpr = "*this";
	}

	if(sExpr[0] == '&') {	//address of
		cb = Callback::seeAddressOf;
		sExpr.erase(0, 1);
	} else if(sExpr[0] == '*') {	//dereference
		cb = Callback::seeDeref;
		sExpr.erase(0, 1);
	} else if(sExpr[0] == '$') {	//named register
		int ri;
		if(!parseArgRegName(sExpr.substr(1), &ri))
			return;
		char buf[32];
		ASSERT_REG;
		sprintf(buf, "%i", r.gpr[ri]);
		sEECallback("int", buf, true);
		return;
	} else if(beginsWith(sExpr, "sizeof(")) {	//sizeof
		size_t end = sExpr.find(')', 7);
		if(end == string::npos) {
			error("Syntax error");
			return;
		}
		sExpr = sExpr.substr(7, end - 7);
		cb = Callback::seeSizeof;
	} else if(iscsym(sExpr[0])) {	//symbol
		cb = Callback::seeSymbol;
	} else {
		error("Cannot evaluate symbol '%s'", sExpr.c_str());
		return;
	}
	locate_symbol(sExpr, cb);
}

static void handle_local(const LocalVariable* lv, const FRAME& frame, SeeCallback cb) {
	SYM sym;
	if(lv->storageClass == eStack) {
		const StackVariable* sv = (StackVariable*)lv;
		sym.type = sv->dataType;
		sym.address = &gMemBuf[frame.pointer + sv->offset];
	} else if(lv->storageClass == eRegister) {
		const RegisterVariable* rv = (RegisterVariable*)lv;
		sym.type = rv->dataType;
		ASSERT_REG;
		sym.address = &r.gpr[rv->reg];
	} else if(lv->storageClass == eStatic) {
		DEBIG_PHAT_ERROR;	//todo
	} else {
		DEBIG_PHAT_ERROR;
	}
	sym.storageClass = lv->storageClass;
	cb(sym);
}

//returns true if a symbol was found and cb called
static bool handle_locals(const vector<ScopedVariable>& locals, int offset,
	const FRAME& frame, const string& name, SeeCallback cb)
{
	//find the last local in scope. that should be the top scope.
	for(size_t i=locals.size()-1; i<locals.size(); i--) {
		const ScopedVariable& sv(locals[i]);
		if(!sv.contains(offset))
			continue;
		if(sv.v->name != name)
			continue;
		handle_local(sv.v, frame, cb);
		return true;
	}
	return false;
}

//returns true if a symbol was found and cb called
static bool handle_params(const vector<LocalVariable*>& params,
	const FRAME& frame, const string& name, SeeCallback cb)
{
	for(size_t i=0; i<params.size(); i++) {
		const LocalVariable* lv = params[i];
		if(lv->name != name)
			continue;
		handle_local(lv, frame, cb);
		return true;
	}
	return false;
}

void dummy(const SYM& sym) {
}

bool handleLocalsAndArguments(const string& name, const FRAME& frame, const Function* f, SeeCallback cb) {
	if(!f) {
		error("No debugging information for current function");
		return false;
	}
	int offset = frame.pc - f->address;

	if(handle_locals(f->locals, offset, frame, name, cb))
		return true;
	if(handle_params(f->params, frame, name, cb))
		return true;

	return false;	
}

bool isLocal(const string& name) {
	//locals
	const FRAME& frame(gFrames[gCurrentFrameIndex]);
	const Function* f = stabsFindFunctionByInsideAddress(frame.pc);
	return handleLocalsAndArguments(name, frame, f, dummy);
}

void locate_symbol(const string& name, SeeCallback cb) {
	//first search locals in the current frame, including any function parameters.
	//then search static symbols in the current frame's file scope.
	//finally search global symbols (scope 0).

	SYM sym;
	sym.address = NULL;
	sym.type = NULL;

	//locals
	const FRAME& frame(gFrames[gCurrentFrameIndex]);
	const Function* f = stabsFindFunctionByInsideAddress(frame.pc);
	if(handleLocalsAndArguments(name, frame, f, cb))
		return;

	//statics and globals
	const Symbol* s = stabsGetSymbolByScopeAndName(f->fileScope, name);
	if(!s)
		s = stabsGetSymbolByScopeAndName(0, name);
	if(!s) {
		//cb(sym);
		error("cannot find symbol '%s'", name.c_str());
		return;
	}

	sym.address = gMemBuf + s->address;	//requires loading that part of memory...
	sym.symType = s->type;
	if(s->type == eFunction) {
		const Function* sf = (Function*)s;
		sym.type = sf->type;
		sym.address = (void*)s->address;	//hack. see FunctionType::printMI().
		cb(sym);
	} else if(s->type == eVariable) {
		const StaticVariable* sv = (StaticVariable*)s;
		sym.type = sv->dataType;
		sSeeCallback = cb;
		sSeeSym = sym;
		StubConnection::readMemory(gMemBuf + sv->address, sv->address, sv->dataType->size(),
			Callback::seeReadMem);
	} else {
		cb(sym);
	}
}

static void Callback::seeReadMem() {
	sSeeCallback(sSeeSym);
}

static int seePrintf(const char* fmt, ...) {
	if(sPos >= BUFSIZE)
		return -1;
	va_list argptr;
	va_start(argptr, fmt);
	int left = BUFSIZE - sPos;
	int len = vsnprintf(sPtr + sPos, left, fmt, argptr);
	if(len >= left) {
		sPos = BUFSIZE;
		error("Buffer overrun");
		return -1;
	}
	sPos += len;
	return len;
}

static void Callback::seeSymbol(const SYM& sym) {
	StringPrintFunctor type, value;
	sym.type->printTypeMI(type, sComplex);	
	sym.type->printMI(value, sym.address, TypeBase::eNatural);
	sEECallback(type.getString(), value.getString(), sym.type->isSimpleValue());
}

static void Callback::seeAddressOf(const SYM& sym) {
	StringPrintFunctor type, value;
	value("0x%x", (unsigned int)sym.address - (unsigned int)gMemBuf);
	sym.type->printTypeMI(type, sComplex);
	type("%s", "*");
	sEECallback(type.getString(), value.getString(), sym.type->isSimpleValue());
}

static void Callback::seeDeref(const SYM& sym) {
	const TypeBase* target = sym.type->deref();
	if(target == NULL) {
		error("Symbol '%s' is not a pointer", sExpr.c_str());
		return;
	}
	sSeeCallback = seeSymbol;
	sSeeSym.type = target;
	//int address = (char*)sym.address - gMemBuf;
	int address = *(int*)sym.address;
	sSeeSym.address = gMemBuf + address;
	if(sSeeSym.type->size() <= 0) {
		error("Symbol '%s' has no size", sExpr.c_str());
		return;
	}
	StubConnection::readMemory(gMemBuf + address, address, sSeeSym.type->size(),
		Callback::seeReadMem);
}

static void Callback::seeSizeof(const SYM& sym) {
	char valBuf[BUFSIZE];
	sprintf(valBuf, "%i", sym.type->size());
	sEECallback("size_t", valBuf, true);
}
