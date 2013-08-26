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
