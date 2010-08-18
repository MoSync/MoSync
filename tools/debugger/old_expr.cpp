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
#include "sld.h"

#include "cmd_stack.h"
#include "helpers.h"
#include "old_expr.h"
#include "stab_helpers.h"

#include "expression.h"
#include "helpers.h"
#include "commandInterface.h"

using namespace std;

namespace Callback {
	static void seeReadMem();
}

static string sExpr;
static SeeCallback sSeeCallback;
static SYM sSeeSym;
static const int BUFSIZE=1024;
static SYM::Scope sScope;


static void handle_local(const LocalVariable* lv, const FRAME& frame, SeeCallback cb) {
	SYM sym;
	if(lv->storageClass == eStack) {
		const StackVariable* sv = (StackVariable*)lv;
		sym.type = sv->dataType->resolve();
		sym.address = &gMemBuf[frame.pointer + sv->offset];

	} else if(lv->storageClass == eRegister) {
		const RegisterVariable* rv = (RegisterVariable*)lv;
		sym.type = rv->dataType->resolve();
		ASSERT_REG;
		sym.address = &r.gpr[rv->reg];
	} else if(lv->storageClass == eStatic) {
		//DEBIG_PHAT_ERROR;	//todo
		const StaticLocal* sl = (StaticLocal*)lv;
		sym.address = &gMemBuf[sl->address];
		sym.type = sl->dataType->resolve();
	} else {
		DEBIG_PHAT_ERROR;
	}
	sym.storageClass = lv->storageClass;
	sym.scope = sScope;
	cb(sym);
}

//returns true if a symbol was found and cb called
static bool handle_locals(const Function* f, const vector<ScopedVariable>& locals, int offset,
	const FRAME& frame, const string& name, SeeCallback cb)
{
	//find the last local in scope. that should be the top scope.
	for(size_t i=locals.size()-1; i<locals.size(); i--) {
		const ScopedVariable& sv(locals[i]);
		if(!sv.contains(offset))
			continue;
		if(sv.v->name != name)
			continue;
		sScope.type = SYM::Scope::eLocal;
		sScope.start = sv.start+f->address;
		sScope.end = sv.end+f->address;
		handle_local(sv.v, frame, cb);
		return true;
	}
	return false;
}

//returns true if a symbol was found and cb called
static bool handle_params(const Function* f, const vector<LocalVariable*>& params,
	const FRAME& frame, const string& name, SeeCallback cb)
{
	for(size_t i=0; i<params.size(); i++) {
		const LocalVariable* lv = params[i];
		if(lv->name != name)
			continue;
		const FuncMapping *fm = mapFunctionEx(f->address);
		sScope.type = SYM::Scope::eLocal;
		sScope.start = fm->start;
		sScope.end = fm->stop;
		handle_local(lv, frame, cb);
		return true;
	}
	return false;
}

static void dummy(const SYM& sym) {
}

static bool handleLocalsAndArguments(const string& name, const FRAME& frame, const Function* f, SeeCallback cb) {
	if(!f) {
		error("No debugging information for current function");
		return false;
	}
	int offset = frame.pc - f->address;

	if(handle_locals(f, f->locals, offset, frame, name, cb))
		return true;
	if(handle_params(f, f->params, frame, name, cb))
		return true;

	return false;	
}

bool isLocalGlobalOrStatic(const string& name) {
	//locals
	const FRAME& frame(gFrames[gCurrentFrameIndex]);
	const Function* f = stabsFindFunctionByInsideAddress(frame.pc);
	if(!f) return false;
	if(handleLocalsAndArguments(name, frame, f, dummy)) return true;
	const Symbol* s = stabsGetSymbolByScopeAndName(f->fileScope, name);
	if(!s)
		s = stabsGetSymbolByScopeAndName(0, name);
	return s != NULL;
}

static SeeCallback sSeeCallbackThis = NULL;
static bool sWasThis = false;
static string sThisName ="";
void thisHandler(const SYM& sym) {
	const TypeBase* resolved = sym.type->resolve();
	if(resolved->type() != TypeBase::eConst) return;
	const ConstType* thisType = (const ConstType*) resolved;
	resolved = thisType->deref();
	if(!resolved) return;
	resolved = resolved->resolve();

	if(resolved && resolved->type() == TypeBase::eStruct) {
		const StructType *type = ((const StructType*)resolved);
		
		// cannot be c++ (if we are in a method, the this pointer must have methods naturally.)
		if(type->getMethods().size()==0) return;

		DotNode::SearchResult res;
		DotNode::recursiveSearch(sThisName, type, &res);
		if(res.found) {
			sSeeSym = sym;
			int thisPointer = *(int*)sSeeSym.address;
			int ptr = thisPointer + (res.offsetBits>>3);
			sSeeSym.address = gMemBuf + ptr;
			sSeeSym.type = res.type;
			sWasThis = true;
			sSeeSym.scope = sScope;
			sSeeCallback = sSeeCallbackThis;
			StubConnection::readMemory(gMemBuf + ptr, ptr, res.type->size(),
				Callback::seeReadMem);
			}
	}
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

	if(!f) { error("Function doesn't exist"); return; }

	if(handleLocalsAndArguments(name, frame, f, cb))
		return;

	// this
	sWasThis = false;
	sThisName = name;
	sSeeCallbackThis = cb;
	if(handleLocalsAndArguments("this", frame, f, thisHandler)) {
		if(sWasThis) return;
	}

	//statics and globals
	const Symbol* s = stabsGetSymbolByScopeAndName(f->fileScope, name);
	if(!s)
		s = stabsGetSymbolByScopeAndName(0, name);
	if(!s) {
		//cb(sym);
		error("cannot find symbol '%s'", name.c_str());
		return;
	}

	sScope.fileScope = s->fileScope;

	sym.address = gMemBuf + s->address;	//requires loading that part of memory...
	sym.symType = s->type;
	if(s->type == eFunction) {
		const Function* sf = (Function*)s;
		sym.type = sf->type->resolve();
		sym.address = (void*)s->address;	//hack. see FunctionType::printMI().
		sym.scope = sScope;
		cb(sym);
	} else if(s->type == eVariable) {
		const StaticVariable* sv = (StaticVariable*)s;
		sym.type = sv->dataType->resolve();
		sym.scope = sScope;
		sSeeCallback = cb;
		sSeeSym = sym;
		StubConnection::readMemory(gMemBuf + sv->address, sv->address, sv->dataType->size(),
			Callback::seeReadMem);
	} else {
		sScope.type = SYM::Scope::eGlobal;
		sym.scope = sScope;
		cb(sym);
	}
}

static void Callback::seeReadMem() {
	sSeeCallback(sSeeSym);
}
