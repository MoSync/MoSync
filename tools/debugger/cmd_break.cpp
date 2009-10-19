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

#include <queue>

#include "config.h"
#include "helpers/log.h"

#include "CoreCommon.h"
#include "sld.h"
#include "stabs/stabs.h"

#include "cmd_break.h"
#include "StubConnection.h"
#include "helpers.h"
#include "commandInterface.h"

#include "globals.h"

using namespace std;

void break_insert(const string& args);
void break_delete(const string& args);
void break_list(const string& args);
void break_disable(const string& args);
void break_enable(const string& args);

//******************************************************************************
// globals
//******************************************************************************
InstructionMap sInstructions;
BreakpointMap sBreakpoints;
BreakpointAddressMap sBreakpointAddresses;
TempBreakpoint gTempBreakpoint = { NULL, -1, 0 };

//******************************************************************************
// statics
//******************************************************************************
static int sNextBpNumber = 1;
static Breakpoint sInsertingBreakpoint;
static queue<int> sBpRestoreQueue;
static void (*sInsertBpInstructionCallback)();

//returns false if an error has occured.
//otherwise, a valid address will be stored.
//location will be modified during parsing.
static bool parseLocation(string& location, int& address);

static void oprintBreakpoint(int number, const Breakpoint& bp);

//returns false on error.
//bi is guaranteed to be valid.
static bool breakMulti(const string& args, void (*cb)(BreakpointMap::iterator));

static void insertBpInstruction(int address, void (*cb)());

//******************************************************************************
// callbacks
//******************************************************************************
namespace Callback {
	static void insert_done();
	static void breakpointRegs(const Registers&);
	static void bpRestore();
	static void bpStore();
	static void bpDelete(BreakpointMap::iterator);
	static void bpDisable(BreakpointMap::iterator);
	static void bpEnable(BreakpointMap::iterator);
}

//******************************************************************************
// insert
//******************************************************************************
void break_insert(const string& args) {
	vector<string> argv;
	splitArgs(args, argv);
	if(argv.size() == 0) {
		error("Too few arguments");
		return;
	}

	int address;
	for(size_t i=0; i<argv.size(); i++) {
		string& a(argv[i]);
		_ASSERT(a.size() != 0);
		if(a[0] == '-') {	//parameter
			switch(a[1]) {
			case 'f':	//a pending breakpoint is requested. ignore; we don't do that.
				break;
			default:
				error("Unsupported parameter");
				return;
			}
		} else {	//location
			if(!parseLocation(a, address))
				return;
		}
	}

	sInsertingBreakpoint.address = address;
	sInsertingBreakpoint.enabled = true;
	sInsertingBreakpoint.keep = true;
	sInsertingBreakpoint.times = 0;
	const char* functionName = mapFunction(address);
	if(functionName)
		sInsertingBreakpoint.func = functionName;
	if(!mapIp(address, sInsertingBreakpoint.line, sInsertingBreakpoint.path)) {
		error("Internal error: Reverse mapping not found");
		return;
	}
	size_t lastSlash = sInsertingBreakpoint.path.find_last_of('/');
	size_t filenameIndex;
	if(lastSlash == string::npos)
		filenameIndex = 0;
	else
		filenameIndex = lastSlash + 1;
	sInsertingBreakpoint.file = sInsertingBreakpoint.path.substr(filenameIndex);

	insertBpInstruction(address, Callback::insert_done);
}

//returns the first safe breakpoint address in the named function,
//or <0 on failure.
static int mapFunctionBreakpoint(const char* name) {
	int address = mapFunction(name);
	if(address < 0)
		return address;
	return nextSldEntry(address);
}

static bool parseLocation(string& location, int& address) {
	_ASSERT(location.size() != 0);
	if(location[0] == '*') {	//hex address
		bool okFormat = false;
		do {
			if(location.size() != 11)
				break;
			if(location.substr(1, 2) != "0x")
				break;
			okFormat = true;
			for(size_t i=3; i<location.size(); i++) {
				if(!isxdigit(location[i])) {
					okFormat = false;
					break;
				}
			}
		} while(0);
		if(!okFormat) {
			error("Invalid address format");
			return false;
		}
		int res = sscanf(location.c_str() + 3, "%x", &address);
		_ASSERT(res == 1);
	}
	else	//file or function
	{
		size_t colonIndex = location.find_last_of(':');
		if(colonIndex != string::npos) {	//filename:<linenum | function>
			int fli = colonIndex + 1;
			if(isdigit(location[fli])) {	//linenum, since function names may not begin with a digit.
				for(size_t i=fli; i<location.size(); i++) {
					if(!isdigit(location[i])) {
						error("Invalid linenum format");
						return false;
					}
				}
				int linenum;
				int res = sscanf(location.c_str() + fli, "%i", &linenum);
				_ASSERT(res == 1);
				//Then what? Use SDL table to map to address.
				location[colonIndex] = 0;
				address = mapFileLine(location.c_str(), linenum);
				switch(address) {
				case ERR_NOFILE:
					error("No such file");
					break;
				case ERR_NOLINE:
					error("Bad line number");
					break;
				case ERR_NOMAP:
					error("Program map not loaded");
					break;
				}
				if(address < 0) {
					return false;
				}
			}
			else if(iscsym(location[fli])) {	//function name (static)
				address = mapFunctionBreakpoint(location.c_str() + fli);
			} else {
				error("Invalid location format");
				return false;
			}
		}
		else	//function without file (global?)
		{
			address = mapFunctionBreakpoint(location.c_str());
		}
	}

	if(address < 0) {
		error("Address not found");
		return false;
	}
	return true;
}

static void insertBpInstruction(int address, void (*cb)()) {
	sInsertBpInstructionCallback = cb;
	if(sInstructions[address].refCount == 0) {
		sInstructions[address].orig = gMemCs[address];
	}
	sInstructions[sInsertingBreakpoint.address].refCount++;
	StubConnection::writeCodeMemory(sInsertingBreakpoint.address,
		&BREAKPOINT_OPCODE, 1, sInsertBpInstructionCallback);
}

void Callback::insert_done() {
	sBreakpoints[sNextBpNumber] = sInsertingBreakpoint;
	sBreakpointAddresses[sInsertingBreakpoint.address] = sNextBpNumber;
	oprintDone();
	oprintf(",");
	oprintBreakpoint(sNextBpNumber, sInsertingBreakpoint);
	oprintf("\n");
	sNextBpNumber++;
	commandComplete();
}

static void oprintBreakpoint(int number, const Breakpoint& bp) {
	oprintf("bkpt={number=\"%i\",type=\"breakpoint\",disp=\"%s\","
		"enabled=\"%c\",addr=\"0x%X\",func=\"%s\",file=\"%s\","
		"fullname=\"%s\",line=\"%i\",times=\"%i\"}",
		number, bp.keep ? "keep" : "nokeep",
		bp.enabled ? 'y' : 'n', bp.address, bp.func.c_str(), bp.file.c_str(),
		bp.path.c_str(), bp.line, bp.times);
}

//******************************************************************************
// hit
//******************************************************************************
void StubConnection::breakpointHit() {
	//oprintf("*stopped,reason=\"breakpoint-hit\"\n");
	LOG("breakpointHit\n");
	//find the breakpoint and extract frame information
	ASSERT_REG;

	if(gTempBreakpoint.callback != NULL && r.pc == gTempBreakpoint.address) {
		gTempBreakpoint.callback();
		return;
	}

	//todo: problematic. what if multiple breakpoints point to this address?
	//maybe we shouldn't allow that. check gcc's behaviour.
	BreakpointAddressMap::const_iterator itr = sBreakpointAddresses.find(r.pc);
	oprintf("*stopped,reason=\"breakpoint-hit\",bkptno=\"");
	if(itr == sBreakpointAddresses.end()) {
		oprintf("unknown");
	} else {
		oprintf("%i", itr->second);
	}
	oprintf("\",frame={addr=\"0x%X\"", r.pc);
	if(itr != sBreakpointAddresses.end()) {
		Breakpoint& bp(sBreakpoints[itr->second]);
		bp.times++;
		oprintf(",func=\"%s\",file=\"%s\",fullname=\"%s\",line=\"%i\"",
			bp.func.c_str(), bp.file.c_str(), bp.path.c_str(), bp.line);
	}
	oprintf("}\n" GDB_PROMPT);
	fflush(stdout);
}

//******************************************************************************
// delete
//******************************************************************************

static bool breakMulti(const string& args, void (*cb)(BreakpointMap::iterator bi)) {
	vector<string> argv;
	splitArgs(args, argv);
	if(argv.size() == 0) {
		error("Too few arguments");
		return false;
	}

	vector<int> nums(argv.size());
	for(size_t i=0; i<argv.size(); i++) {
		const string& a(argv[i]);
		for(size_t j=0; j<a.size(); j++) {
			if(!isdigit(a[j])) {
				error("Bad argument format");
				return false;
			}
		}
		if(sscanf(a.c_str(), "%i", &nums[i]) != 1) {
			error("Cannot parse argument");
			return false;
		}
		//have to do this check before deleting anything,
		//as an error should leave everything unchanged.
		if(sBreakpoints.find(nums[i]) == sBreakpoints.end()) {
			error("Cannot find breakpoint");
			return false;
		}
		//todo: check for dupes
	}

	_ASSERT(sBpRestoreQueue.empty());
	for(size_t i=0; i<nums.size(); i++) {
		BreakpointMap::iterator bi = sBreakpoints.find(nums[i]);
		_ASSERT(bi != sBreakpoints.end());
		cb(bi);
	}
	return true;
}

void break_delete(const string& args) {
	if(!breakMulti(args, Callback::bpDelete))
		return;

	Callback::bpRestore();
}

static void Callback::bpDelete(BreakpointMap::iterator bi) {
	bpDisable(bi);
	sBreakpoints.erase(bi);
}

static void Callback::bpRestore() {
	if(sBpRestoreQueue.empty()) {
		oprintDoneLn();
		commandComplete();
		return;
	}
	int address = sBpRestoreQueue.front();
	sBpRestoreQueue.pop();
	InstructionMap::iterator ii = sInstructions.find(address);
	_ASSERT(ii != sInstructions.end());
	StubConnection::writeCodeMemory(address,
		&ii->second.orig, 1, Callback::bpRestore);
	sInstructions.erase(ii);
}

//******************************************************************************
// list
//******************************************************************************

void break_list(const string& args) {
	oprintDone();
	oprintf(",BreakpointTable={nr_rows=\"2\",nr_cols=\"6\","
		"hdr=[{width=\"3\",alignment=\"-1\",col_name=\"number\",colhdr=\"Num\"},"
		"{width=\"14\",alignment=\"-1\",col_name=\"type\",colhdr=\"Type\"},"
		"{width=\"4\",alignment=\"-1\",col_name=\"disp\",colhdr=\"Disp\"},"
		"{width=\"3\",alignment=\"-1\",col_name=\"enabled\",colhdr=\"Enb\"},"
		"{width=\"10\",alignment=\"-1\",col_name=\"addr\",colhdr=\"Address\"},"
		"{width=\"40\",alignment=\"2\",col_name=\"what\",colhdr=\"What\"}],"
		"body=[");
	BreakpointMap::iterator itr = sBreakpoints.begin();
	while(itr != sBreakpoints.end()) {
		oprintBreakpoint(itr->first, itr->second);
		itr++;
	}
	oprintf("]}\n");
	commandComplete();
}

//******************************************************************************
// disable
//******************************************************************************

void break_disable(const string& args) {
	if(!breakMulti(args, Callback::bpDisable))
		return;

	Callback::bpRestore();
}

static void Callback::bpDisable(BreakpointMap::iterator bi) {
	Breakpoint& bp(bi->second);
	if(bp.enabled) {
		InstructionMap::iterator ii = sInstructions.find(bp.address);
		_ASSERT(ii != sInstructions.end());
		Instruction& inst(ii->second);
		if(inst.refCount == 1) {
			//we need to restore the instruction
			sBpRestoreQueue.push(bp.address);
		} else if(inst.refCount > 1) {
			inst.refCount--;
		}
		bp.enabled = false;
	}
}

//******************************************************************************
// enable
//******************************************************************************

void break_enable(const string& args) {
	if(!breakMulti(args, Callback::bpEnable))
		return;

	Callback::bpStore();
}

static void Callback::bpEnable(BreakpointMap::iterator bi) {
	Breakpoint& bp(bi->second);
	if(!bp.enabled) {
		InstructionMap::iterator ii = sInstructions.find(bp.address);
		if(ii == sInstructions.end()) {
			//we need to store the instruction
			sBpRestoreQueue.push(bp.address);
		} else {
			ii->second.refCount++;
		}
		bp.enabled = true;
	}
}

static void Callback::bpStore() {
	if(sBpRestoreQueue.empty()) {
		oprintDoneLn();
		commandComplete();
		return;
	}
	int address = sBpRestoreQueue.front();
	sBpRestoreQueue.pop();
	sInsertingBreakpoint.address = address;
	insertBpInstruction(address, bpStore);
}
