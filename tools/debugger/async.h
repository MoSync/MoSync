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
 * Closes the event system, any subsequent calls to getEvent or putEvent will
 * fail.
 */
void closeEventSystem();

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
