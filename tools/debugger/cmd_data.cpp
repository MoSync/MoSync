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

#include <limits.h>
#include <stdlib.h>

#include "config.h"
#include "helpers/log.h"
#include "helpers/array.h"
#undef NUL

#include "sld.h"
#include "CoreCommon.h"
#include "disas.h"

#include "cmd_data.h"
#include "globals.h"
#include "StubConnection.h"
#include "helpers.h"
#include "commandInterface.h"

using namespace std;

void data_list_register_values(const string& args);
void data_read_memory(const string& args);
void data_list_register_names(const string& args);
void data_list_changed_registers(const string& args);

//called during disassembly of CALL instructions.
//should return the demangled name of the function that starts at the specified address.
typedef const char* (*functionLookupCallback)(int address);

//called after each instruction has been disassembled.
//\param inst The text representing the instruction.
//\param ip The address of the instruction.
typedef void (*drCallback)(const char* inst, const byte* ip);

static void disassemble_range(const byte* ip, int maxLen, int maxLines,
	const byte* mem_cs, const int* mem_cp, drCallback, functionLookupCallback);

namespace Callback {
	static void register_values(const Registers&);
	static void read_memory();
	static void changed_registers(const Registers&);
	static void disasm(const char* inst, const byte* ip);
}

static struct READ_MEM_PARAMS {
	int address;
	int wordSize;
	int nRows;
	int nCols;
} sRMP;
static Array<byte> sReadMemBuf(0);

static Registers sRegData;
static bool sListVal[N_GPR];
static bool sDisasmFirst, sDisasmMix;
static bool sDisasmUnfinished;

const char *gRegsyms[] = {
	"zr","sp","rt","fr","d0","d1","d2","d3",
	"d4","d5","d6","d7","i0","i1","i2","i3",
	"r0","r1","r2","r3","r4","r5","r6","r7",
	"r8","r9","r10","r11","r12","r13","r14","r15"
};

//******************************************************************************
// data_list_register_values
//******************************************************************************

void data_list_register_values(const string& args) {
	vector<string> argv;
	splitArgs(args, argv);
	if(argv.size() == 0) {
		error("Too few arguments");
		return;
	}
	if(argv.size() == 1) {
		//print all registers
		memset(sListVal, 1, sizeof(sListVal));
	} else {
		memset(sListVal, 0, sizeof(sListVal));
		//a list of zero-based register numbers follow
		const char* ptr = args.c_str() + 1;
		int num = 0;
		while(true) {
			int len, reg;
			int res = sscanf(ptr, " %i%n", &reg, &len);
			if(res != 1)
				break;
			if(reg >= N_GPR || reg < 0) {
				error("Bad argument");
				return;
			}
			sListVal[reg] = 1;
			num++;
			ptr += len;
		}
		if(num == 0) {
			error("Bad arguments");
			return;
		}
	}
	const string& fmt(argv[0]);
	if(fmt != "x") {
		error("Unsupported format");
		return;
	}
	StubConnection::getRegisters(Callback::register_values);
}

static void Callback::register_values(const Registers& r) {
	oprintDone();
	oprintf(",register-values=[");
	bool first = true;
	for(int i=0; i<N_GPR; i++) {
		if(sListVal[i]) {
			if(first) {
				first = false;
			} else {
				oprintf(",");
			}
			oprintf("{number=\"%i\",value=\"0x%X\"}", i, r.gpr[i]);
		}
	}
	oprintf("]\n");
	commandComplete();
}

//******************************************************************************
// data_read_memory
//******************************************************************************

void data_read_memory(const string& args) {
	vector<string> argv;
	splitArgs(args, argv);
	EFAIL(argv.size() != 5, "Unsupported arguments");

	EFAIL(sscanf(argv[0].c_str(), "%i", &sRMP.address) != 1, "Bad format");
	EFAIL(argv[1] != "x", "Unsupported format");
	EFAIL(sscanf(argv[2].c_str(), "%i", &sRMP.wordSize) != 1, "Bad format");
	EFAIL(sscanf(argv[3].c_str(), "%i", &sRMP.nRows) != 1, "Bad format");
	EFAIL(sscanf(argv[4].c_str(), "%i", &sRMP.nCols) != 1, "Bad format");

	int totalBytes = sRMP.wordSize * sRMP.nRows * sRMP.nCols;
	sReadMemBuf.resize(totalBytes);

	StubConnection::readMemory(sReadMemBuf.p(), sRMP.address, totalBytes, Callback::read_memory);
}

static void Callback::read_memory() {
	int next = sRMP.address + sReadMemBuf.size();
	int rowSize = sRMP.wordSize * sRMP.nCols;
	oprintDone();
	oprintf(",addr=\"0x%X\",nr-bytes=\"%i\",total-bytes=\"%i\","
		"next-row=\"0x%X\",prev-row=\"0x%X\",next-page=\"0x%X\",prev-page=\"0x%X\","
		"memory=[",
		sRMP.address, sReadMemBuf.size(), sReadMemBuf.size(),
		next, sRMP.address - rowSize, next, sRMP.address - sReadMemBuf.size());
	int offset = 0;
	const byte* ptr = sReadMemBuf.p();
	for(int row=0; row<sRMP.nRows; row++) {
		if(row != 0)
			oprintf(",");
		oprintf("{addr=\"0x%X\",data=[", sRMP.address + offset);
		for(int col=0; col<sRMP.nCols; col++) {
			if(col != 0)
				oprintf(",");
			oprintf("\"0x");
			for(int i=0; i<sRMP.wordSize; i++) {
				oprintf("%02X", ptr[offset++]);
			}
			oprintf("\"");
		}
		oprintf("]}");
	}
	oprintf("\n");
	commandComplete();
}

//******************************************************************************
// data_list_register_names
//******************************************************************************

void data_list_register_names(const string& args) {
	if(args.size() != 0) {
		error("Unsupported arguments");
		return;
	}
	oprintDone();
	oprintf(",register-names=[");
	for(int i=0; i<32; i++) {
		if(i != 0)
			oprintf(",");
		oprintf("\"%s\"", gRegsyms[i]);
	}
	for(int i=0; i<N_GPR-32; i++) {
		oprintf(",\"k%i\"", i);
	}
	oprintf("]\n");
	commandComplete();
}

//******************************************************************************
// data_list_changed_registers
//******************************************************************************

//lists the numbers of the registers whose content has changed since the last
//call to this function.
void data_list_changed_registers(const string& args) {
	StubConnection::getRegisters(Callback::changed_registers);
}

static void Callback::changed_registers(const Registers& r) {
	oprintDone();
	oprintf(",register-names=[");
	int num = 0;
	for(int i=0; i<N_GPR; i++) {
		if(sRegData.gpr[i] != r.gpr[i]) {
			sRegData.gpr[i] = r.gpr[i];
			if(num != 0)
				oprintf(",");
			oprintf("\"%i\"", i);
			num++;
		}
	}
	oprintf("]\n");
	commandComplete();
}

//******************************************************************************
// data_disassemble
//******************************************************************************

//-data-disassemble {{ -s start-addr -e end-addr } | { -f filename -l linenum }} [ -n lines ] -- mode
#define ETEST(func) if(!(func)) return

void data_disassemble(const string& args) {
	int startAddress, endAddress, maxLines = INT_MAX;

	//parse args
	vector<string> argv;
	splitArgs(args, argv);
	EFAIL(argv.size() < 4 + 2, "Too few arguments");
	if(argv[0] == "-s") {
		ETEST(parseArgInt(argv[1], &startAddress));
		EFAIL(argv[2] != "-e", "Bad arguments");
		ETEST(parseArgInt(argv[3], &endAddress));
	} else if(argv[0] == "-f") {
		EFAIL(argv[2] != "-l", "Bad arguments");
		int line;
		ETEST(parseArgInt(argv[3], &line));
		startAddress = mapFileLine(argv[1].c_str(), line);
		//if startAddress is part of a function, stop at the end of a function.
		const FuncMapping* pfm = mapFunctionEx(startAddress);
		if(pfm != NULL) {
			endAddress = pfm->stop;
		} else {
			endAddress = gHead.CodeLen - 1;
		}
	} else {
		error("Bad arguments");
		return;
	}

	int i = 4;
	if(argv[i] == "-n") {
		i++;
		ETEST(parseArgInt(argv[i], &maxLines));
		i++;
	}

	EFAIL(argv[i] != "--", "Bad arguments");
	i++;
	if(argv[i] == "0") {
		sDisasmMix = false;
	} else if(argv[i] == "1") {
		sDisasmMix = true;
	} else {
		error("Bad argument");
		return;
	}

	EFAIL(startAddress < 0, "Bad start address");
	EFAIL(endAddress < 0, "Bad end address");
	EFAIL(endAddress < startAddress, "Bad addresses");

	sDisasmFirst = true;
	sDisasmUnfinished = false;
	oprintDone();
	oprintf(",asm_insns=[");
	disassemble_range(gMemCs + startAddress, endAddress - startAddress, maxLines,
		gMemCs, gMemCp, Callback::disasm, mapFunction);
	if(sDisasmUnfinished) {
		oprintf("]}");
	}
	oprintf("]\n");
	commandComplete();
}

static void Callback::disasm(const char* inst, const byte* ip) {
	static int oldLine;
	static string oldFile;
	if(sDisasmFirst) {
		oldLine = -1;
	}

	int address = ip - gMemCs;

	//a bit suboptimal.
	//would be faster if we could access the SLD array directly.
	int newLine;
	string newFile;
	bool hasMap = sDisasmMix ? mapIp(address, newLine, newFile) : false;
	if(!hasMap) {
		newLine = -1;
	}
	if(oldLine > 0 && newLine != oldLine) {
		oprintf("]}");
		sDisasmUnfinished = false;
	}
	if(sDisasmFirst) {
		sDisasmFirst = false;
	} else {
		oprintf(",");
	}
	if(newLine > 0 && newLine != oldLine) {
		oprintf("src_and_asm_line={line=\"%i\",file=\"%s\",line_asm_insn=[",
			newLine, newFile.c_str());
		sDisasmUnfinished = true;
	}
	oprintf("{address=\"0x%08x\"", address);
	//todo: add function name and offset
	oprintf(",inst=\"%s\"}", inst);
	oldLine = newLine;
	oldFile = newFile;
}

static void disassemble_range(const byte* ip, int maxLen, int maxLines,
	const byte* mem_cs, const int* mem_cp, drCallback cb, functionLookupCallback flc)
{
	DisasInit((char*)mem_cs, (int*)mem_cp);
	int lines = 0;
	const byte* end = ip + maxLen;
	char buf[128];
	while(ip <= end && lines < maxLines) {
		const byte* oldIp = ip;
		DisasInfo di;
		ip = DisasDecode(&di, ip);
		DisasString(&di, buf);
		cb(buf, oldIp);
	}
}
