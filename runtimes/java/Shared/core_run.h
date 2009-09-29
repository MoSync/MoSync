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

//include in Core.jpp
//define EXEC_NAME and EXEC_LOOP


//***************************************************************************
//Instruction executor
//***************************************************************************
final int EXEC_NAME() throws Exception {
	int IP = mIP;
	//byte op;
	int rd,rs;
	int imm32;
	int[] regs = mRegs;
	int[] mem_ds = mMem_ds;
	byte[] mem_cs = mMem_cs;
	int[] mem_cp = mMem_cp;

	mVM_Yield = false;
	//	printf("VM IP at %d\n",(int) IP - (int) mem_cs);

	EXEC_LOOP {
#ifndef PHONE_RELEASE
		mInstCount++;
#endif
#ifdef UPDATE_IP
		mIP = IP;
#endif
		//op = FETCH_CODEBYTE_FAST;
		switch(FETCH_CODEBYTE_FAST) {
			OPC(ADD)	FETCH_RD_RS	ARITH(RD, +, RS);	EOP;
			OPC(ADDI)	FETCH_RD_CONST	ARITH(RD, +, IMM);	EOP;
			OPC(SUB)	FETCH_RD_RS	ARITH(RD, -, RS);	EOP;
			OPC(SUBI)	FETCH_RD_CONST	ARITH(RD, -, IMM);	EOP;
			OPC(MUL)	FETCH_RD_RS	ARITH(RD, *, RS);	EOP;
			OPC(MULI)	FETCH_RD_CONST	ARITH(RD, *, IMM);	EOP;
			OPC(AND)	FETCH_RD_RS	ARITH(RD, &, RS);	EOP;
			OPC(ANDI)	FETCH_RD_CONST	ARITH(RD, &, IMM);	EOP;
			OPC(OR)		FETCH_RD_RS	ARITH(RD, |, RS);	EOP;
			OPC(ORI)	FETCH_RD_CONST	ARITH(RD, |, IMM);	EOP;
			OPC(XOR)	FETCH_RD_RS	ARITH(RD, ^, RS);	EOP;
			OPC(XORI)	FETCH_RD_CONST	ARITH(RD, ^, IMM);	EOP;
			OPC(DIVU)	FETCH_RD_RS	DIVIDEU(RD, RS);	EOP;
			OPC(DIVUI)	FETCH_RD_CONST	DIVIDEU(RD, IMM);	EOP;
			OPC(DIV)	FETCH_RD_RS	DIVIDE(RD, RS);		EOP;
			OPC(DIVI)	FETCH_RD_CONST	DIVIDE(RD, IMM);	EOP;
			OPC(SLL)	FETCH_RD_RS	ARITH(RD, <<, RS);	EOP;
			OPC(SLLI)	FETCH_RD_IMM8	ARITH(RD, <<, IMM);	EOP;
			OPC(SRA)	FETCH_RD_RS	ARITH(RD, >>, RS);	EOP;
			OPC(SRAI)	FETCH_RD_IMM8	ARITH(RD, >>, IMM);	EOP;
			OPC(SRL)	FETCH_RD_RS	ARITH(RD, >>>, RS);	EOP;
			OPC(SRLI)	FETCH_RD_IMM8	ARITH(RD, >>>, IMM);	EOP;

			OPC(NOT)	FETCH_RD_RS	RD = ~RS;	EOP;
			OPC(NEG)	FETCH_RD_RS	RD = -RS;	EOP;

			OPC(PUSH)	FETCH_RD_IMM8
			{
				int r = rd;
				int n = IMM;
				if(rd < 2 || ((int)rd) + n > 32)
					BIG_PHAT_ERROR;

				do {
					REG(REG_sp) -= 4;
					WINT(REG(REG_sp), REG(r));
					DEBUG("\t0x"+Integer.toHexString(REG(r)));
					r++;
				} while(--n != 0);
			}
			EOP;

			OPC(POP) FETCH_RD_IMM8
			{
				int r = rd;
				int n = IMM;
				if(rd > 31 || ((int)rd) - n < 1)
					BIG_PHAT_ERROR;

				do {
					REG(r) = RINT(REG(REG_sp));
					REG(REG_sp) += 4;
					DEBUG("\t0x"+Integer.toHexString(REG(r)));
					r--;
				} while(--n != 0);
			}
			EOP;

			OPC(CALL)	FETCH_RD	CALL_RD		EOP;
			OPC(CALLI)	FETCH_IMM16	CALL_IMM	EOP;

			OPC(LDB) FETCH_RD_RS_CONST  RBYTE(RS + IMM, RD); DEBUG("\t"+RD);	EOP;
			OPC(LDH) FETCH_RD_RS_CONST  RSHORT(RS + IMM, RD); DEBUG("\t"+RD);	EOP;
			OPC(LDW) FETCH_RD_RS_CONST  RD = RINT(RS + IMM); DEBUG("\t"+RD);	EOP;

			OPC(STB) FETCH_RD_RS_CONST  WBYTE(RD + IMM, (byte)RS);  EOP;
			OPC(STH) FETCH_RD_RS_CONST  WSHORT(RD + IMM, (short)RS);	EOP;
			OPC(STW) FETCH_RD_RS_CONST  WINT(RD + IMM, RS); EOP;

			OPC(LDI)	FETCH_RD_CONST	RD = IMM;	EOP;
			OPC(LDR)	FETCH_RD_RS	RD = RS;	EOP;

			OPC(RET)
				IP = REG(REG_rt);
			EOP;

			OPC(JC_EQ) 	FETCH_RD_RS_ADDR	if (RD == RS)	{ JMP_IMM; } 	EOP;
			OPC(JC_NE)	FETCH_RD_RS_ADDR	if (RD != RS)	{ JMP_IMM; }	EOP;
			OPC(JC_GE)	FETCH_RD_RS_ADDR	if (RD >= RS)	{ JMP_IMM; }	EOP;
			OPC(JC_GT)	FETCH_RD_RS_ADDR	if (RD >  RS)	{ JMP_IMM; }	EOP;
			OPC(JC_LE)	FETCH_RD_RS_ADDR	if (RD <= RS)	{ JMP_IMM; }	EOP;
			OPC(JC_LT)	FETCH_RD_RS_ADDR	if (RD <  RS)	{ JMP_IMM; }	EOP;

			OPC(JC_LTU)	FETCH_RD_RS_ADDR	if(OPU(RD, <, RS))	{ JMP_IMM; }	EOP;
			OPC(JC_GEU)	FETCH_RD_RS_ADDR	if(OPU(RD, >=, RS))	{ JMP_IMM; }	EOP;
			OPC(JC_GTU)	FETCH_RD_RS_ADDR	if(OPU(RD, >, RS))	{ JMP_IMM; }	EOP;
			OPC(JC_LEU)	FETCH_RD_RS_ADDR	if(OPU(RD, <=, RS))	{ JMP_IMM; }	EOP;

			OPC(JPI)	FETCH_IMM16		JMP_IMM		EOP;
			OPC(JPR)	FETCH_RD		JMP_RD		EOP;

			// !!>> Added ARH - pls check 16-01-2007

			OPC(FAR)
			{
				//op = FETCH_CODEBYTE_FAST;
				switch(FETCH_CODEBYTE_FAST)
				{
					OPC(CALLI)	FETCH_IMM24	CALL_IMM	EOP;

					OPC(JC_EQ) 	FETCH_RD_RS_ADDR24	if (RD == RS)	{ JMP_IMM; } 	EOP;
					OPC(JC_NE)	FETCH_RD_RS_ADDR24	if (RD != RS)	{ JMP_IMM; }	EOP;
					OPC(JC_GE)	FETCH_RD_RS_ADDR24	if (RD >= RS)	{ JMP_IMM; }	EOP;
					OPC(JC_GT)	FETCH_RD_RS_ADDR24	if (RD >  RS)	{ JMP_IMM; }	EOP;
					OPC(JC_LE)	FETCH_RD_RS_ADDR24	if (RD <= RS)	{ JMP_IMM; }	EOP;
					OPC(JC_LT)	FETCH_RD_RS_ADDR24	if (RD <  RS)	{ JMP_IMM; }	EOP;

					OPC(JC_LTU)	FETCH_RD_RS_ADDR24	if(OPU(RD, <, RS))	{ JMP_IMM; }	EOP;
					OPC(JC_GEU)	FETCH_RD_RS_ADDR24	if(OPU(RD, >=, RS))	{ JMP_IMM; }	EOP;
					OPC(JC_GTU)	FETCH_RD_RS_ADDR24	if(OPU(RD, >, RS))	{ JMP_IMM; }	EOP;
					OPC(JC_LEU)	FETCH_RD_RS_ADDR24	if(OPU(RD, <=, RS))	{ JMP_IMM; }	EOP;

					OPC(JPI)	FETCH_IMM24		JMP_IMM		EOP;

				default:
					DEBUG_ALWAYS("\tIllegal far instruction 0x" +
						Integer.toHexString(REG(mem_cs[IP-1])) + ", 0x" +
						Integer.toHexString(REG(IP-1)));
					BIG_PHAT_ERROR;
				}
			} EOP;

			OPC(XB)		FETCH_RD_RS		RD = (int)((byte) RS);	EOP;
			OPC(XH)		FETCH_RD_RS		RD = (int)((short) RS);	EOP;

			OPC(SYSCALL) {
				InvokeSysCall(FETCH_CODEBYTE_FAST);
#ifdef PUBLIC_DEBUG
				mLastSyscall = INVALID_SYSCALL_NUMBER;
#endif
				if(mVM_Yield)
					return IP;
			} EOP;

#ifdef GDB_DEBUG
			OPC(DBG_STAB) {
				IP--;
				mIP = IP;
				mGdbStub.exceptionHandler(0);
				IP = mIP;
			} EOP;
#else
			OPC(ENDOP) {	//something of a hack
				mVM_Yield = true;
				return IP;
			} //EOP;	//unreachable statement
#endif

			OPC(CASE) FETCH_RD; FETCH_IMM24; {
				int CaseStart = mem_ds[imm32];
				//DDUMP(CaseStart);
				int CaseLength = mem_ds[imm32 + 1];
				//DDUMP(CaseLength);
				int index = RD - CaseStart;
				//DDUMP(index);
				if(index <= CaseLength && index >= 0) {
					int tableAddress = imm32 + 3;
					//DDUMP(tableAddress);
					IP = mem_ds[tableAddress + index];
				} else {
					int DefaultCaseAddress = mem_ds[imm32 + 2];
					//DDUMP(DefaultCaseAddress);
					IP = DefaultCaseAddress;
				}
			} EOP;

			default:
				//VM_State = -3;				// Bad instruction
				DEBUG_ALWAYS("Illegal instruction 0x"+Integer.toHexString(mem_cs[IP-1])+
					" @ 0x"+Integer.toHexString(IP-1)+"\n");
				BIG_PHAT_ERROR;
		}
	}
	EXEC_RETURN
}
