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

#ifndef PHONE_RELEASE
#ifdef __SYMBIAN32__
#include <e32base.h>
#else
#include <stdio.h>
#include <stdarg.h>
#endif

typedef unsigned char byte;
typedef unsigned int uint;

//****************************************
//		 Instruction descripters
//****************************************
enum
{
	_NUL = 0,
	_PUSH,
	_POP,
	_CALL,
	_CALLI,
	_LDB,
	_STB,
	_LDH,
	_STH,
	_LDW,
	_STW,
	_LDI,
	_LDR,
	_ADD,
	_ADDI,
	_MUL,
	_MULI,
	_SUB,
	_SUBI,
	_AND,
	_ANDI,
	_OR,
	_ORI,
	_XOR,
	_XORI,
	_DIVU,
	_DIVUI,
	_DIV,
	_DIVI,
	_SLL,
	_SLLI,
	_SRA,
	_SRAI,
	_SRL,
	_SRLI,
	_NOT,
	_NEG,
	_RET,	
	_JC_EQ,
	_JC_NE,
	_JC_GE,
	_JC_GEU,
	_JC_GT,
	_JC_GTU,
	_JC_LE,
	_JC_LEU,
	_JC_LT,
	_JC_LTU,
	_JPI,
	_JPR,
	_XB,
	_XH,
	_SYSCALL,
	_CASE,
	_FAR,
	_ENDOP
};

enum
{
	REG_zero,
	REG_sp,
	REG_rt,
	REG_fr,
	REG_d0,
	REG_d1,
	REG_d2,
	REG_d3,
	REG_d4,
	REG_d5,
	REG_d6,
	REG_d7,
	REG_i0,
	REG_i1,
	REG_i2,
	REG_i3,

	REG_r0,
	REG_r1,
	REG_r2,
	REG_r3,
	REG_r4,
	REG_r5,
	REG_r6,
	REG_r7,
	REG_r8,
	REG_r9,
	REG_r10,
	REG_r11,
	REG_r12,
	REG_r13,
	REG_r14,
	REG_r15
};

#define _DBG_OP _ENDOP

#ifdef __SYMBIAN32__
#define WRITE(argv...) if(buf) buf += write(buf, argv)
__inline int write(char* buf, const char* fmt, ...) {
	//LOG("write 0x%08x \"%s\"\n", buf, fmt);
	VA_LIST argptr;
	VA_START(argptr, fmt);
	TPtr8 ptr((byte*) buf, 1024);
	TPtrC8 fmtPtr((byte*) fmt);
	ptr.FormatList(fmtPtr, argptr);
	//LOG("writeResult: %x \"%s\"\n", ptr.Length(), buf);
	buf[ptr.Length()] = 0;
	return ptr.Length();
}
#else
static char* sPtr;
__inline void WRITE(const char* fmt, ...) {
	if(!sPtr) return;
	va_list argptr;
	va_start(argptr, fmt);
	sPtr += vsprintf(sPtr, fmt, argptr);
}
#endif	//__SYMBIAN32__


#define IB ((int)(*ip++))

#define OPC(opcode)	case _##opcode: WRITE("%x: %i %s", ip - mem_cs - 1, _##opcode, #opcode);
#define EOP	WRITE("\n"); break;

#define FETCH_RD	rd = IB; WRITE(" rd%i", rd);
#define FETCH_RS	rs = IB; WRITE(" rs%i", rs);
#define FETCH_CONST	imm32 = IB; if(imm32>127) {imm32=((imm32&127)<<8)+IB;}\
	WRITE(" c[%i]", imm32); imm32=mem_cp[imm32]; WRITE("(%i)", imm32);
#define FETCH_INT	imm32 = IB; if(imm32>127) {imm32=((imm32&127)<<8)+IB;}\
	WRITE(" i%i", imm32);
#define FETCH_IMM8	imm32 = IB; WRITE(" n%i", imm32);
#define FETCH_IMM16	imm32 = IB << 8; imm32 += IB; WRITE(" m%i(0x%x)", imm32, imm32);
#define FETCH_IMM24	imm32 = IB << 16; imm32 += IB << 8; imm32 += IB;\
	WRITE(" i%i(0x%x)", imm32, imm32);

#define ARITH(a, oper, b)
#define DIVIDE(a, b)
#define CALL_IMM
#define CALL_RD
#define JMP_IMM
#define JMP_RD
#define JMP_GENERIC(a)
#define REG(i) (i)
#define MEM(type, addr) (addr)

#define FETCH_RD_RS		FETCH_RD FETCH_RS
#define FETCH_RD_CONST		FETCH_RD FETCH_CONST
#define FETCH_RD_RS_CONST	FETCH_RD FETCH_RS FETCH_CONST
#define FETCH_RD_RS_ADDR16	FETCH_RD FETCH_RS FETCH_IMM16
#define FETCH_RD_RS_ADDR24	FETCH_RD FETCH_RS FETCH_IMM24
#define FETCH_RD_IMM8		FETCH_RD FETCH_IMM8



/**
 * Disassembles one MoSync IL instruction
 *
 * @param ip      [in] Pointer to the instruction
 * @param mem_cs  [in] Pointer to data section
 * @param mem_cp  [in] Pointer to constant section
 * @param buff    [in] Pointer to char buffer which can hold one
 *                     dissassembled instruction.
 * @param op      [out] Opcode
 * @param op2     [out] Second opcode, incase op was a prefix opcode
 * @param rd      [out] Destination register
 * @param rs      [out] Source register
 * @param imm32   [out] Immediate
 *
 * @return Size of the dissassembled instruction in bytes
 */
int disassemble_one ( const byte* ip, 
                      const byte* mem_cs, 
                      const int* mem_cp, 
                      char* buf, 
                      byte& op, 
                      byte& op2, 
                      byte &rd, 
                      byte &rs, 
                      int &imm32 )
{
	const byte* startIp = ip;
#ifndef __SYMBIAN32__
	sPtr = buf;
#endif
	op = *ip++;

	int RD=0, RS=0, RDU=0, RSU=0;

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

		OPC(PUSH)	FETCH_RD_IMM8		EOP;

		OPC(POP) FETCH_RD_IMM8 EOP;

		OPC(LDB)	FETCH_RD_RS_CONST EOP;

		OPC(LDH) FETCH_RD_RS_CONST EOP;

		OPC(LDW) FETCH_RD_RS_CONST EOP;

		OPC(STB) FETCH_RD_RS_CONST EOP;

		OPC(STH) FETCH_RD_RS_CONST EOP;

		OPC(STW) FETCH_RD_RS_CONST EOP;

		OPC(LDI)	FETCH_RD_CONST	EOP;
		OPC(LDR)	FETCH_RD_RS	EOP;

		OPC(RET)
		JMP_GENERIC(REG(REG_rt));
		EOP;

		OPC(CALL)
			FETCH_RD
			CALL_RD
		EOP;
		OPC(CALLI)
			FETCH_IMM16
			CALL_IMM
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

		OPC(FAR) op2 = *ip++; switch(op2) {
			OPC(CALLI)
				FETCH_IMM24
				CALL_IMM
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
				WRITE("Illegal far instruction 0x%02X @ 0x%04X\n", op, (ip - mem_cs) - 1);
				//BIG_PHAT_ERROR(ERR_ILLEGAL_INSTRUCTION);
		} EOP;

		OPC(XB)		FETCH_RD_RS		RD = (int)((char) RS);	EOP;
		OPC(XH)		FETCH_RD_RS		RD = (int)((short) RS);	EOP;

		OPC(SYSCALL)
		{
			int syscallNumber = imm32 = IB;
			WRITE("%i", syscallNumber);
		}
		EOP;

		OPC(CASE) FETCH_RD; FETCH_IMM24; {
			int imm32temp = imm32 << 2;
			uint CaseStart = MEM(int, imm32temp);
			uint CaseLength = MEM(int, imm32temp + 1*sizeof(int));
			WRITE("cs 0x%x, cl 0x%x", CaseStart, CaseLength);
		} EOP;

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
		}
#endif

	default:
		WRITE("Illegal instruction 0x%02X @ 0x%04X\n", op, (ip - mem_cs) - 1);
		//BIG_PHAT_ERROR(ERR_ILLEGAL_INSTRUCTION);
	}

	return (int)(ip-startIp);
}


/**
 * Disassembles one MoSync IL instruction
 *
 * @param ip      [in] Pointer to the instruction
 * @param mem_cs  [in] Pointer to data section
 * @param mem_cp  [in] Pointer to constant section
 * @param buff    [in] Pointer to char buffer which can hold one
 *                     dissassembled instruction.
 *
 * @return Size of the dissassembled instruction in bytes
 */
int disassemble_one ( const byte* ip, 
                      const byte* mem_cs, 
                      const int* mem_cp, 
                      char* buf ) 
{
	byte rd=0, rs=0;
	byte op, op2;
	int imm32;
	return disassemble_one(ip, mem_cs, mem_cp, buf, op, op2, rd, rs, imm32);
}

#endif	//PHONE_RELEASE
