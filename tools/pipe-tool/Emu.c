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

//*********************************************************************************************
//					   VMGP (TM) PIP II Execution Engine
// 						   Written by A.R.Hartley
//		    This code is based on the GameTasker v2.0 PIP Engine
//*********************************************************************************************

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdarg.h>

#include "compile.h"
#include "PipOp.h"

int EmuRunning;

extern unsigned char *reg;

//*********************************************************************************************
//									Emulator Verions 1.0
//*********************************************************************************************

//****************************************
//			 Type Definitions
//****************************************

typedef signed 		char	s1;
typedef signed 		short	s2;
typedef signed 		int	s4;

typedef unsigned	char	u1;
typedef unsigned	short	u2;
typedef unsigned	int	u4;

//****************************************
//				Macroes
//****************************************

#define	RD		*((s4*) (&reg[IP->rd]))
#define	RS		*((s4*) (&reg[IP->rs]))
#define	RT		*((s4*) (&reg[IP->rt]))

#define	RDU		*((u4*) (&reg[IP->rd]))
#define	RSU		*((u4*) (&reg[IP->rs]))
#define	RTU		*((u4*) (&reg[IP->rt]))

#define	RDV		IP->rd
#define	RSV		IP->rs
#define	RTV		IP->rt

#define REG(nn) *((u4*) (&reg[4*nn]))

#define	IMM		(s4) IP->imm
#define	IMMU	(u4) IP->imm

#define	JMP_IMM		IP = (InstImm *) IMM;	goto Loop0
#define	JMP_RD		IP = (InstImm *) RD;	goto Loop0

#define	CALL_IMM	REG(REG_ra) = (int) IP + 8; JMP_IMM;
#define	CALL_RD		REG(REG_ra) = (int) IP + 4; JMP_RD;

#define IC(opcode)	case opcode:

#define ENDOP		goto Loop4
#define ENDOPI		goto Loop8

//****************************************
//			  Run emulator
//****************************************

#define EMULOOPING	1

#define MCHECK(addr,align) if (CheckDataBounds((int) addr,align)) goto Err0

int Emulate(register InstImm *IP,register char *reg)
{

EmuLoop:

	if (CheckCodeBounds((int) IP,3))
		return (int) IP;
	
	switch (IP->op)
	{

	// Arith 3 imm

	IC(_ADD)	RD = RS + RT;			ENDOP;

	IC(_ADDI)	RD = RS + IMM;			ENDOPI;

	IC(_AND)	RD = RS & RT;			ENDOP;
	IC(_ANDI)	RD = RS & IMM;			ENDOPI;

	IC(_OR)		RD = RS | RT;			ENDOP;
	IC(_ORI)	RD = RS | IMM;			ENDOPI;

	IC(_XOR)	RD = RS ^ RT;			ENDOP;
	IC(_XORI)	RD = RS ^ IMM;			ENDOPI;

	IC(_SUB)	RD = RSU - RTU;			ENDOP;
	IC(_SUBI)	RD = RS - IMM;			ENDOPI;
	
	IC(_SLL)	RD = RSU << RTU;		ENDOP;
	IC(_SLLI)	RD = RSU << IMMU;		ENDOPI;

	IC(_SRA)	RD = RS >> RT;			ENDOP;
	IC(_SRAI)	RD = RS >> IMM;			ENDOPI;	// Shift >> with xtend

	IC(_SRL)	RD = RSU >> RTU;		ENDOP;
	IC(_SRLI)	RD = RSU >> IMMU;		ENDOPI;		// Shift >> with 0's

	IC(_MUL)	RD = RS  * RT;			ENDOP;
	IC(_MULU)	RD = RSU * RTU;			ENDOP;
	IC(_MULI)	RD = RS  * IMM;			ENDOPI;
	IC(_MULUI)	RD = RSU * IMMU;		ENDOPI;

	IC(_DIV)	if (RS && RT)	RD = RS / RT;			ENDOP;
	IC(_DIVU)	if (RS && RT)	RD = RSU / RTU;			ENDOP;
	IC(_DIVI)	if (RS && IMM)	RD = RS / IMM;			ENDOPI;
	IC(_DIVUI)	if (RS && IMM)	RD = RSU / IMMU;		ENDOPI;

	IC(_NOT)	RD = ~RS;			ENDOP;
	IC(_NEG)	RD = -RSU;			ENDOP;
	IC(_MOVE)	RD = RS;			ENDOP;

	IC(_EXSB)	RD = (char) (RS & 0xff);			ENDOP;
	IC(_EXSH)	RD = (short)(RS & 0xffff);			ENDOP;

	IC(_STORE)
	{
		int *sp = (int *) REG(REG_sp);
		int r = IP->rd >> 2;
		int n = IP->rs >> 2;

		do
		{
#if 0
			printf("STORE %s (0x%08x)\n",
                               regnames[r], REG(r));
#endif
			// check bounds

			if (CheckDataBounds((int) sp-4,3))
			{
				EmuError("Stack memory protection error SP = 0x%lx\n",(int) sp-4);
				return (int) IP;
			}

			*--sp = REG(r);
			r++;
		}
		while(--n);

		REG(REG_sp) = (int) sp;
		ENDOP;
	}
	
	
	IC(_RESTORE)
	{
		int *sp = (int *) REG(REG_sp);
		int r = IP->rd >> 2;
		int n = IP->rs >> 2;

		do
		{
			if (CheckDataBounds((int) sp,3))
			{
				EmuError("Stack memory protection error SP = 0x%lx\n",(int) sp);
				return (int) IP;
			}

			REG(r) = *sp++;
#if 0
			printf("RESTORE %s (0x%08x)\n",
                               regnames[r], REG(r));
#endif
			r--;
		}
		while(--n);

		REG(REG_sp) = (int) sp;
		ENDOP;
	}

	// Base load

	IC(_LDI)	RD = IMM;			ENDOPI;

	IC(_STB)
	{
		register char *addr;

		addr = (char *) RS + IMM;
		MCHECK(addr,0);
		*addr = RD;
		ENDOPI;
	}

	IC(_STH)
	{
		register char *addr;

		addr = (char *) RS + IMM;	
		MCHECK(addr,1);
		*((short *) addr) = RD;
		ENDOPI;
	}

	IC(_STW)
	{
		register char *addr;

		addr = (char *) RS + IMM;
		MCHECK(addr,3);
		*((int *) addr) = RD;
		ENDOPI;
	}

	IC(_LDB)
	{
		register char *addr;

		addr = (char *) RS + IMM;
		MCHECK(addr,0);
		RD = *addr;
		ENDOPI;
	}

	IC(_LDBU)
	{
		register char *addr;

		addr = (char *) RS + IMM;
		MCHECK(addr,0);
		RDU = *((unsigned char *) addr);
		ENDOPI;
	}

	IC(_LDH)
	{
		register char *addr;

		addr = (char *) RS + IMM;	
		MCHECK(addr,1);
		RD = *((short *) addr);
		ENDOPI;
	}

	IC(_LDHU)
	{
		register char *addr;

		addr = (char *) RS + IMM;
		MCHECK(addr,1);
		RDU = *((unsigned short *) addr);
		ENDOPI;
	}

	IC(_LDW)
	{
		register char *addr;

		addr = (char *) RS + IMM;
		MCHECK(addr,3);
		RD = *((int *) addr);
		ENDOPI;
	}

	// jump/call
	
	IC(_JP)		JMP_RD;					// Dont need ENDOP's
	IC(_JPI)	JMP_IMM;
	IC(_CALL)	CALL_RD;
	IC(_CALLI)	CALL_IMM;

	// jump cond imm 
	
	IC(_BEQ)	if (RD == RS)	{ JMP_IMM; }		ENDOPI;
	IC(_BNE)	if (RD != RS)	{ JMP_IMM; }		ENDOPI;
	IC(_BGE)	if (RD >= RS)	{ JMP_IMM; }		ENDOPI;
	IC(_BGEU)	if (RDU >= RSU)	{ JMP_IMM; }		ENDOPI;
	IC(_BGT)	if (RD > RS)	{ JMP_IMM; }		ENDOPI;
	IC(_BGTU)	if (RDU > RSU)	{ JMP_IMM; }		ENDOPI;
	IC(_BLE)	if (RD <= RS)	{ JMP_IMM; }		ENDOPI;
	IC(_BLEU)	if (RDU <= RSU)	{ JMP_IMM; }		ENDOPI;
	IC(_BLT)	if (RD < RS)	{ JMP_IMM; }		ENDOPI;
	IC(_BLTU)	if (RDU < RSU)	{ JMP_IMM; }		ENDOPI;

	IC(_NATIVE)
	{
		int *SP = (int *) REG(REG_sp);

		MCHECK(SP,3);
		
		REG(REG_p0) = ((int (*)(int,...)) IP->imm)(REG(REG_p0),REG(REG_p1),REG(REG_p2),REG(REG_p3),SP[0],SP[1]);
		ENDOPI;
	}

	IC(_SYSEX)
		((int (*)(int,...)) IP->imm)(RD,RS,RT);

		if (!EmuRunning)
			return (int) IP + 8;

		ENDOPI;

	IC(_SLEEP)
		return (int) IP + 8;

//	IC(_KILLTASK)
//		KillTask();
//		return NULL;
		
	default:
		EmuError("Bad instruction %d\n",IP->op);
		return (int) IP;

	}

Loop0:

	#if EMULOOPING == 0
		return (int) IP;
	#else
		goto EmuLoop;
	#endif

Loop4:

	((char *) IP) += 4;

	#if EMULOOPING == 0
		return (int) IP;
	#else
		goto EmuLoop;
	#endif

Loop8:

	((char *) IP) += 8;

	#if EMULOOPING == 0
		return (int) IP;
	#else
		goto EmuLoop;
	#endif

Err0:
	return (int) IP;
}

//*********************************************************************************************
//									Emulator Support Routines
//*********************************************************************************************

//****************************************
//		    Emulator Error Print
//****************************************

void EmuError(char *Template, ...)
{
	char 			tbuf[128];
	va_list 		args;

	va_start(args,Template);
	vsprintf(tbuf,Template,args);
	va_end(args);

	printf("%s",tbuf);

	EmuRunning = 0;
}

//****************************************
// 			Is Emu Running
//****************************************

short IsEmuRunning()
{
	return EmuRunning;
}

//****************************************
// 			Is Emu Running
//****************************************

void SetEmu(int run)
{
	EmuRunning = run;
}

