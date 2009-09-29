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

//seems no symbol commands are implemented in GDB
//thus no frontend can use them
#if 0
#include <string>

#include "config.h"
#include "helpers/log.h"

#include "stabs/stabs.h"

//#include "sld.h"
#include "helpers.h"
#include "commandInterface.h"

using namespace std;

//******************************************************************************
// statics
//******************************************************************************

namespace Callback {
	static void listFunc(const Symbol*);
	static void listVar(const Symbol*);
}

static bool sEnumFirst;

//******************************************************************************
// unimplemented ones
//******************************************************************************

#define UNIMPL error("Unimplemented MI command: " __FUNCTION__)

//not implemented in GDB!?!
void symbol_info_address(const string& args) {
	Symbol* s = stabsFindSymbolByName(args);
}

//not implemented in GDB!?!
void symbol_type(const string& args) {
	Type* type = stabsFindTypeByName(args);
	if(!type) {
		error("Couldn't find type");
		return;
	}
	oprintDone();
	oprintf(",%s={id=(%i,%i),",
		type->name.c_str(), type->id.a, type->id.b);
	type->type->printMI(oprintf);
}

//******************************************************************************
// list symbols
//******************************************************************************

//not implemented in GDB!
void symbol_list_functions(const string& args) {
	oprintDone();
	oprintf(",functions=[");
	sEnumFirst = true;
	stabsEnumerateSymbols(Callback::listFunc);
	oprintf("]\n");
}
static void Callback::listFunc(const Symbol* s) {
	if(s->type != eFunction)
		return;
	if(sEnumFirst)
		sEnumFirst = false;
	else
		oprintf(",");
	//Function* f = (Function*)s;
	oprintf("{name=\"%s\"}", s->name.c_str());
	//todo: expand
}

//not implemented in GDB!
void symbol_list_variables(const string& args) {
	oprintDone();
	oprintf(",variables=[");
	sEnumFirst = true;
	stabsEnumerateSymbols(Callback::listVar);
	oprintf("]\n");
}
static void Callback::listVar(const Symbol* s) {
	if(s->type != eVariable)
		return;
	if(sEnumFirst)
		sEnumFirst = false;
	else
		oprintf(",");
	//Variable* v = (Variable*)s;
	oprintf("{name=\"%s\"}", s->name.c_str());
	//todo: expand
}
#endif
