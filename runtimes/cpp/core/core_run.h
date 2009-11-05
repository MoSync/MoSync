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

#ifdef COUNT_INSTRUCTION_USE
struct InstructionBucket {
	int id;
	int count;
	const char *opName;
};

std::vector<InstructionBucket> gInstructionUseCount; 

void countInstructionUse(const char* opName, byte x) {
	gInstructionUseCount[x].count++;
	gInstructionUseCount[x].opName = opName;
}

void initInstructionUseCount() {
	gInstructionUseCount.resize(_ENDOP);
	for(int i = 0; i < _ENDOP; i++) {
		gInstructionUseCount[i].id = i;
		gInstructionUseCount[i].count = 0;
		gInstructionUseCount[i].opName = "UNUSED";
	}
}

static bool UDgreater ( InstructionBucket& elem1, InstructionBucket& elem2 )
{
   return elem1.count > elem2.count;
}


void logInstructionUse() {
	std::sort(gInstructionUseCount.begin(),
		gInstructionUseCount.end(), &Core::VMCoreInt::UDgreater);

	char temp[1024];
	Base::WriteFileStream use("instruction_use.txt", false);	
	for(int i = 0; i < _ENDOP; i++) {
		int len = sprintf(temp, "inst %s: %d\n", gInstructionUseCount[i].opName, gInstructionUseCount[i].count);
		use.write(temp, len);
	}
}
#else
#define countInstruction(x)
#endif

byte* RUN_NAME(byte* ip) {
	byte op,rd,rs;
	unsigned long imm32;

	VM_Yield = 0;

#ifdef _WIN32
	static LARGE_INTEGER iCounterFreq;
	static double lastTime;
	LARGE_INTEGER iCounter;
	QueryPerformanceFrequency(&iCounterFreq);
	QueryPerformanceCounter(&iCounter);
	lastTime = (double)iCounter.QuadPart * 1000.0 / (double)iCounterFreq.QuadPart;	
#endif	//_WIN32

	//	printf("VM IP at %d\n",(long) ip - (long) mem_cs);

#ifndef CORE_DEBUGGING_MODE
VMLOOP_LABEL
#endif

#ifdef UPDATE_IP
	IP = uint(ip - mShared.mem_cs);
	//LOG("IP 0x%04X\n", IP);
#endif

#ifdef MEMORY_DEBUG
	InstCount++;
	if(uint(ip - mShared.mem_cs) >= (mShared.CODE_SEGMENT_SIZE - 4)) {
		uint currentIP = uint(ip - mShared.mem_cs);
		DUMPHEX(IP);
		DUMPHEX(currentIP);
		DUMPINT(InstCount);
		IP = currentIP;
		BIG_PHAT_ERROR(ERR_IMEM_OOB);
	}
#endif	//MEMORY_DEBUG
#if defined(INSTRUCTION_PROFILING) && defined(UPDATE_IP) && defined(MEMORY_DEBUG)
	instruction_count[IP]++;
#endif

#ifdef USE_DELAY
#ifdef _WIN32
	if(InstCount>DELAY_BATCH) {
		InstCount = 0;
		QueryPerformanceCounter(&iCounter);
		double time = (double)iCounter.QuadPart * 1000.0 / (double)iCounterFreq.QuadPart;

		while((time-lastTime)<DELAY_BATCH_TIME) {
			double diff = time-lastTime;
			double a = DELAY_BATCH_TIME;
			QueryPerformanceCounter(&iCounter);
			time = (double)iCounter.QuadPart * 1000.0 / (double)iCounterFreq.QuadPart;				
		}

		lastTime = time;
	}
#endif	//_WIN32
#endif	//USE_DELAY

#ifdef LOG_STATE_CHANGE
	logStateChange((int)(ip-mem_cs));
#endif
	op = *ip++;

	switch (op)
	{
		OPC(ADD)	FETCH_RD_RS	ARITH(RD, +, RS);	EOP;
		OPC(ADDI)	FETCH_RD_CONST	ARITH(RD, +, IMM);	EOP;
		OPC(SUB)	FETCH_RD_RS	ARITH(RD, -, RS);	EOP;
		OPC(SUBI)	FETCH_RD_CONST	ARITH(RD, -, IMM);	EOP;
		OPC(MUL)	FETCH_RD_RS	ARITH(RD, *, RS);	EOP;
		OPC(MULI)	FETCH_RD_CONST	ARITH(RD, *, IMM);	EOP;
		OPC(AND)	FETCH_RD_RS	ARITH(RD, &, RS);	EOP;
		OPC(ANDI)	FETCH_RD_CONST	ARITH(RD, &, IMM);	EOP;
		OPC(OR)	FETCH_RD_RS	ARITH(RD, |, RS);	EOP;
		OPC(ORI)	FETCH_RD_CONST	ARITH(RD, |, IMM);	EOP;
		OPC(XOR)	FETCH_RD_RS	ARITH(RD, ^, RS);	EOP;
		OPC(XORI)	FETCH_RD_CONST	ARITH(RD, ^, IMM);	EOP;
		OPC(DIVU)	FETCH_RD_RS	DIVIDE(RDU, RSU);	EOP;
		OPC(DIVUI)	FETCH_RD_CONST	DIVIDE(RDU, IMMU);	EOP;
		OPC(DIV)	FETCH_RD_RS	DIVIDE(RD, RS);		EOP;
		OPC(DIVI)	FETCH_RD_CONST	DIVIDE(RD, IMM);	EOP;
		OPC(SLL)	FETCH_RD_RS	ARITH(RDU, <<, RSU);	EOP;
		OPC(SLLI)	FETCH_RD_IMM8	ARITH(RDU, <<, IMMU);	EOP;
		OPC(SRA)	FETCH_RD_RS	ARITH(RD, >>, RS);	EOP;
		OPC(SRAI)	FETCH_RD_IMM8	ARITH(RD, >>, IMM);	EOP;
		OPC(SRL)	FETCH_RD_RS	ARITH(RDU, >>, RSU);	EOP;
		OPC(SRLI)	FETCH_RD_IMM8	ARITH(RDU, >>, IMMU);	EOP;

		OPC(NOT)	FETCH_RD_RS	RD = ~RS;	EOP;
		OPC(NEG)	FETCH_RD_RS	RD = -RS;	EOP;

		OPC(PUSH)	FETCH_RD_IMM8
		{
			byte r = rd;
			unsigned n = imm32;
			if(rd < 2 || int(rd) + n > 32) {
				DUMPINT(rd);
				DUMPINT(n);
				BIG_PHAT_ERROR(ERR_ILLEGAL_INSTRUCTION_FORM); //raise hell
			}

			do {
				REG(REG_sp) -= 4;
				MEM(long, REG(REG_sp), WRITE) = REG(r);
				LOGC("\t0x%x", REG(r));
				r++;
			} while(--n);
		}
		EOP;

		OPC(POP) FETCH_RD_IMM8
		{
			byte r = rd;
			unsigned n = imm32;
			if(rd > 31 || int(rd) - n < 1)
				BIG_PHAT_ERROR(ERR_ILLEGAL_INSTRUCTION_FORM); //raise hell

			do {
				REG(r) = MEM(long, REG(REG_sp), READ);
				REG(REG_sp) += 4;
				LOGC("\t0x%x", REG(r));
				r--;
			} while(--n);
		}
		EOP;

		OPC(LDB)
		{
			FETCH_RD_RS_CONST
				RD = MEM(char, RS + IMM, READ);
			LOGC("\t%i", RD);
		}
		EOP;

		OPC(LDH)
		{
			FETCH_RD_RS_CONST
				RD = MEM(short, RS + IMM, READ);
			LOGC("\t%i", RD);
		}
		EOP;

		OPC(LDW)
		{
			FETCH_RD_RS_CONST
				RD = MEM(long, RS + IMM, READ);
			LOGC("\t%i", RD);
		}
		EOP;

		OPC(STB)
		{
			FETCH_RD_RS_CONST
				MEM(byte, RD + IMM, WRITE) = RS;
		}
		EOP;

		OPC(STH)
		{
			FETCH_RD_RS_CONST
				MEM(unsigned short, RD + IMM, WRITE) = RS;
		}
		EOP;

		OPC(STW)
		{
			FETCH_RD_RS_CONST
				MEM(unsigned long, RD + IMM, WRITE) = RS;
		}
		EOP;

		OPC(LDI)	FETCH_RD_CONST	RD = IMM;	EOP;
		OPC(LDR)	FETCH_RD_RS	RD = RS;	EOP;

		OPC(RET)
			fakePop();
		JMP_GENERIC(REG(REG_rt));
		EOP;

		OPC(CALL)
			FETCH_RD
			CALL_RD
			fakePush(REG(REG_rt), RD);
		EOP;
		OPC(CALLI)
			FETCH_IMM16
			CALL_IMM
			fakePush(REG(REG_rt), IMM);
		EOP;

		OPC(JC_EQ) 	FETCH_RD_RS_ADDR16	if (RD == RS)	{ JMP_IMM; } 	EOP;
		OPC(JC_NE)	FETCH_RD_RS_ADDR16	if (RD != RS)	{ JMP_IMM; }	EOP;
		OPC(JC_GE)	FETCH_RD_RS_ADDR16	if (RD >= RS)	{ JMP_IMM; }	EOP;
		OPC(JC_GT)	FETCH_RD_RS_ADDR16	if (RD >  RS)	{ JMP_IMM; }	EOP;
		OPC(JC_LE)	FETCH_RD_RS_ADDR16	if (RD <= RS)	{ JMP_IMM; }	EOP;
		OPC(JC_LT)	FETCH_RD_RS_ADDR16	if (RD <  RS)	{ JMP_IMM; }	EOP;

		OPC(JC_LTU)	FETCH_RD_RS_ADDR16	if (RDU <  RSU)	{ JMP_IMM; }	EOP;
		OPC(JC_GEU)	FETCH_RD_RS_ADDR16	if (RDU >= RSU)	{ JMP_IMM; }	EOP;
		OPC(JC_GTU)	FETCH_RD_RS_ADDR16	if (RDU >  RSU)	{ JMP_IMM; }	EOP;
		OPC(JC_LEU)	FETCH_RD_RS_ADDR16	if (RDU <= RSU)	{ JMP_IMM; }	EOP;

		OPC(JPI)		FETCH_IMM16		JMP_IMM		EOP;
		OPC(JPR)		FETCH_RD		JMP_RD		EOP;

		OPC(FAR) op = *ip++; switch(op) {
			OPC(CALLI)
				FETCH_IMM24
				CALL_IMM
				fakePush(REG(REG_rt), IMM);
			EOP;

			OPC(JC_EQ) 	FETCH_RD_RS_ADDR24	if (RD == RS)	{ JMP_IMM; } 	EOP;
			OPC(JC_NE)		FETCH_RD_RS_ADDR24	if (RD != RS)	{ JMP_IMM; }	EOP;
			OPC(JC_GE)		FETCH_RD_RS_ADDR24	if (RD >= RS)	{ JMP_IMM; }	EOP;
			OPC(JC_GT)		FETCH_RD_RS_ADDR24	if (RD >  RS)	{ JMP_IMM; }	EOP;
			OPC(JC_LE)		FETCH_RD_RS_ADDR24	if (RD <= RS)	{ JMP_IMM; }	EOP;
			OPC(JC_LT)		FETCH_RD_RS_ADDR24	if (RD <  RS)	{ JMP_IMM; }	EOP;

			OPC(JC_LTU)	FETCH_RD_RS_ADDR24	if (RDU <  RSU)	{ JMP_IMM; }	EOP;
			OPC(JC_GEU)	FETCH_RD_RS_ADDR24	if (RDU >= RSU)	{ JMP_IMM; }	EOP;
			OPC(JC_GTU)	FETCH_RD_RS_ADDR24	if (RDU >  RSU)	{ JMP_IMM; }	EOP;
			OPC(JC_LEU)	FETCH_RD_RS_ADDR24	if (RDU <= RSU)	{ JMP_IMM; }	EOP;

			OPC(JPI)		FETCH_IMM24		JMP_IMM		EOP;
		default:
			LOG("Illegal far instruction 0x%02X @ 0x%04X\n", op, (ip - mShared.mem_cs) - 1);
			BIG_PHAT_ERROR(ERR_ILLEGAL_INSTRUCTION);
			return ip;
		} EOP;

		//OPC(XB)		FETCH_RD_RS		RD = (int)((char) RS); EOP;
		OPC(XB)	FETCH_RD_RS	RD = ((RS & 0x80) == 0) ? (RS & 0xFF) : (RS | ~0xFF); EOP;
		//OPC(XH)		FETCH_RD_RS		RD = (int)((short) RS);	EOP;
		OPC(XH)	FETCH_RD_RS	RD = ((RS & 0x8000) == 0) ? (RS & 0xFFFF) : (RS | ~0xFFFF); EOP;

		OPC(SYSCALL)
		{
			int syscallNumber = IB;
			fakePush((long) (ip - mShared.mem_cs), -syscallNumber);
			InvokeSysCall(syscallNumber);
			fakePop();
			if (VM_Yield)
				return ip;
		}
		EOP;

		OPC(CASE) FETCH_RD; FETCH_IMM24; {
			imm32 <<= 2;
			uint CaseStart = MEM(int, imm32, READ);
			uint CaseLength = MEM(int, imm32 + 1*sizeof(int), READ);
			uint index = RD - CaseStart;
			if(index <= CaseLength) {
				int tableAddress = imm32 + 3*sizeof(int);
				JMP_GENERIC(MEM(int, tableAddress + index*sizeof(int), READ));
			} else {
				int DefaultCaseAddress = MEM(int, imm32 + 2*sizeof(int), READ);
				JMP_GENERIC(DefaultCaseAddress);
			}
		} EOP;

#if 0
#ifdef ENABLE_DEBUGGER
		OPC(DBG_OP) {
			int pc = IP;
			switch(Debugger::brk()) {
	case Debugger::BRK_CONTINUE:
	default:
		BIG_PHAT_ERROR(0);
			}
			if(pc!=IP) {
				ip = mem_cs+IP;
			}
		} EOP;
#endif
#else
#ifdef GDB_DEBUG
		OPC(DBG_OP) {
			ip--;
			if(mGdbOn && mGdbSignal != eStep) {
				mGdbSignal = eBreakpoint;
			} else {
				//if mGdbOn, the debugger tried to step on a breakpoint.
				//it shouldn't do that, because the core can't handle it.
				//it should have overwritten the breakpoint opcode with the original.
				DEBIG_PHAT_ERROR;
			}
		} EOP;
#endif
#endif

	default:
		//VM_State = -3;				// Bad instruction
		LOG("Illegal instruction 0x%02X @ 0x%04X\n", op, (ip - mShared.mem_cs) - 1);
		BIG_PHAT_ERROR(ERR_ILLEGAL_INSTRUCTION);
		return ip;
	}
	RUN_LOOP;
}
