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
//				       			MoSync Code Tools API
//*********************************************************************************************

#include "compile.h"

//****************************************
//		Analyse Function
//****************************************
/*
typedef struct
{
	int src_reg;
	int dst_reg;
	int assign_reg;
	int uninit_reg;
	int reg_used;
} FuncProp;
*/

#define REGBIT(bit) (1 << (bit))

//#define AFDEBUG

int FunctionRegAnalyse(SYMBOL *sym, FuncProp *fp)
{
	OpcodeInfo thisOp;
	uchar *ip, *ip_end;
	int params,n;

	fp->src_reg = 0;
	fp->dst_reg = 0;
	fp->assign_reg = 0;
	fp->uninit_reg = 0;

	// Make sure we have a valid symbol

	if (!sym)
		return -1;

	// Make sure the symbol is a function

	if (sym->Type != SECT_code)
		return -1;

	// Get the start and end of the function in the code array

	ip_end = (uchar *) ArrayPtr(&CodeMemArray, sym->EndIP);
	ip = (uchar *) ArrayPtr(&CodeMemArray, sym->Value);

	// Set up the parameter regs

	params = sym->Params;

	for (n=0;n<params;n++)
	{
		fp->assign_reg |=  REGBIT(REG_i0 + n);
	}

	// Scan the function

#ifdef AFDEBUG
	printf("\n");
#endif

	while(1)
	{
		if (ip > ip_end)
			break;

#ifdef AFDEBUG
		{
			char buf[2560];
			buf[0] = 0;
			DisassembleFromSource(ip - CodeMemArray.array, buf);
			printf("%s\n", buf);
		}
#endif

		ip = DecodeOpcode(&thisOp, ip);

		// Ignor push and pop

		if (thisOp.op == _PUSH)
			continue;

		if (thisOp.op == _POP)
			continue;

		//-----------------------------------------
		// Deal with syscalls
		// for some strange reason rd is the syscallId which will
		// tag unused registers as used for void functions
		//-----------------------------------------

		if(thisOp.op == _SYSCALL)
		{
			FunctionReg_Syscall(&thisOp, fp);
		}

		// Check for a dest reg
		else if (thisOp.flags & fetch_d)
		{
			if (thisOp.rd < 32)
			{
				// Since this is a dst regs we say its initialized
				fp->assign_reg |=  REGBIT(thisOp.rd);

				// Say this reg was used as a dst reg
				fp->dst_reg |= REGBIT(thisOp.rd);
			}
		}

		// Check for a source reg

		if (thisOp.flags & fetch_s)
		{
			if (thisOp.rs < 32)
			{
				// check if this reg was assigned previously, if it was'nt it is uninitialized before use

				int is_assigned = fp->assign_reg & REGBIT(thisOp.rs);

				if (!is_assigned)
					fp->uninit_reg |= REGBIT(thisOp.rs);

				fp->src_reg |= REGBIT(thisOp.rs);
			}
		}

		//-----------------------------------------
		//		Deal with immediate calls
		//-----------------------------------------

		// Add the call parameters to the used list

		if (thisOp.op == _CALLI)
		{

			FunctionReg_Calli(&thisOp, fp);
		}

		//-----------------------------------------
		//		Deal with register calls
		//-----------------------------------------

		// Add the call parameters to the used list

		if (thisOp.op == _CALL)
		{
			FunctionReg_CallReg(&thisOp, fp);
		}
	}


#ifdef AFDEBUG

	printf("\n");

	printf("                  rrrrrrrrrrrrrrrriiiiddddddddfrsz\n");
	printf("                  fedcba9876543210321076543210rtpr\n");

	printf("src_reg = %s\n", Bin32(fp->src_reg));
	printf("dst_reg = %s\n", Bin32(fp->dst_reg));
	printf("assign_reg    = %s\n", Bin32(fp->assign_reg));
	printf("uninit_reg      = %s\n", Bin32(fp->uninit_reg));

	if (fp->uninit_reg)
		printf("");

#endif

	// Make sure zr is never uninitialized

	fp->uninit_reg &= ~REGBIT(REG_zero);

	// Make a composit of which reg's were used

	fp->reg_used = fp->src_reg | fp->dst_reg;

	return fp->reg_used;
}

//****************************************
//		Deal with syscalls
//****************************************

// Add the call parameters to the used list

void FunctionReg_Syscall(OpcodeInfo *thisOp, FuncProp *fp)
{
	SYMBOL* CallSym = FindSysCall(thisOp->imm);

	if (CallSym)
	{
		int params = CallSym->Params;
		int p;

		for (p=0;p<params;p++)
		{
			// check if this reg was assigned previously, if it was'nt it is uninitialized before use

			int is_assigned = fp->assign_reg & REGBIT(REG_i0 + p);

			if (!is_assigned)
				fp->uninit_reg |= REGBIT(REG_i0 + p);

			fp->src_reg |= REGBIT(REG_i0 + p);
		}

		// Check what the function returns
		// Do nothing for void

		if (CallSym->RetType == RET_int   ||
			CallSym->RetType == RET_float)
			fp->dst_reg |= REGBIT(REG_r14);

		if (CallSym->RetType == RET_double)
			fp->dst_reg |= REGBIT(REG_r14) | REGBIT(REG_r15);
	}
}

//****************************************
//		Deal with immediate calls
//****************************************

// Add the call parameters to the used list

void FunctionReg_Calli(OpcodeInfo *thisOp, FuncProp *fp)
{
	SYMBOL *CallSym;

	CallSym = (SYMBOL *) ArrayGet(&CallArray, thisOp->rip);

	if (CallSym)
	{
		int params = CallSym->Params;
		int p;

		for (p=0;p<params;p++)
		{
			// check if this reg was assigned previously, if it was'nt it is uninitialized before use

			int is_assigned = fp->assign_reg & REGBIT(REG_i0 + p);

			if (!is_assigned)
				fp->uninit_reg |= REGBIT(REG_i0 + p);

			fp->src_reg |= REGBIT(REG_i0 + p);
		}

		// Check what the function returns
		// Do nothing for void

		if (CallSym->RetType == RET_int   ||
			CallSym->RetType == RET_float)
			fp->dst_reg |= REGBIT(REG_r14);

		if (CallSym->RetType == RET_double)
			fp->dst_reg |= REGBIT(REG_r14) | REGBIT(REG_r15);
	}
}

//****************************************
//		Deal with immediate calls
//****************************************

// Add the call parameters to the used list

void FunctionReg_CallReg(OpcodeInfo *thisOp, FuncProp *fp)
{
#if !defined(_MSC_VER) && !defined(__GNUC__)
#pragma unused(thisOp)
#endif

	int p;

	for (p=0;p<4;p++)
	{
		// check if this reg was assigned previously, if it was'nt it is uninitialized before use

		int is_assigned = fp->assign_reg & REGBIT(REG_i0 + p);

		if (!is_assigned)
			fp->uninit_reg |= REGBIT(REG_i0 + p);

		fp->src_reg |= REGBIT(REG_i0 + p);
	}

	fp->dst_reg |= REGBIT(REG_r14) | REGBIT(REG_r15);
}
