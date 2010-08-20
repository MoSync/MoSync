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

/**
 * The debugger is event-driven and responds to the following events:
 *   eRecv - indicates that there are packets available from MoRE.
 *   eUserInput - a GDB MI command from the GDB session.
 *   eReadMemory - a request for MDB to read memory from MoRE.
 *   eLocateSymbol - a request for MDB to find a symbol with the given name.
 *   eExpressionEvaluated - indicates that the result of an expression
 *                         evaluation is ready.
 */
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

/**
 * Initializes the event system, must be called before any other function
 * in this module is called.
 */
void initEventSystem();

/**
 * Waits until there is an event and points *pde to it. 
 *
 * @param pde A pointer to a pointer that will point to the event upon return.
 *            The user is responsible for deallocating the memory for the 
 *            event.
 */
void getEvent(DebuggerEvent** pde);

/**
 * Adds the given event to the event queue.
 *
 * @param de The event to put in the queue. The allocated memory for de will be
 *        deallocated.
 */
void putEvent(DebuggerEvent* de);

#endif	//ASYNC_H
