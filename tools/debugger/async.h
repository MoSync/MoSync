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

#ifndef ASYNC_H
#define ASYNC_H

#include <string>
#include "old_expr.h"
#include "expression.h"

typedef void (*AckCallback)();

struct DebuggerEvent {
	enum Type { eRecv, eUserInput, eReadMemory, eLocateSymbol, eExpressionEvaluated };

	Type type;

	std::string str;	//valid if type == eUserInput || eLocateSymbol
	union {
		int result;	//valid if type == eRecv
		struct {	//valid if type == eReadMemory
			int src, len;
			AckCallback rmcb;
		};
		struct {	//valid if type == eExpressionEvaluated
			const Value* erv;
			ExpressionCallback ecb;
			const char* err;
		};
		SeeCallback lscb;	//valid if type == eLocateSymbol
	};
};

void initEventSystem();

//waits until there is an event.
//*pde must be deleted.
void getEvent(DebuggerEvent** pde);

//takes a new DebuggerEvent. It will be deleted when Gotten.
void putEvent(DebuggerEvent* de);

#endif	//ASYNC_H
