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

	int reg_used_as_src = 0;
	int reg_used_as_dst = 0;
	int reg_assigned = 0;
	int reg_uninit = 0;
	
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
		reg_assigned |=  REGBIT(REG_i0 + n);
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

		// Check for a dest reg
		
		if (thisOp.flags & fetch_d)
		{
			if (thisOp.rd < 32)
			{
				// Since this is a dst regs we say its initialized
				reg_assigned |=  REGBIT(thisOp.rd);
	
				// Say this reg was used as a dst reg
				reg_used_as_dst |= REGBIT(thisOp.rd);
			}
		}
		
		// Check for a source reg
		
		if (thisOp.flags & fetch_s)
		{
			if (thisOp.rs < 32)
			{
				// check if this reg was assigned previously, if it was'nt it is uninitialized before use
			
				int is_assigned = reg_assigned & REGBIT(thisOp.rs);
			
				if (!is_assigned)
					reg_uninit |= REGBIT(thisOp.rs);
			
				reg_used_as_src |= REGBIT(thisOp.rs);
			}
		}

		//-----------------------------------------
		//		Deal with immediate calls
		//-----------------------------------------
		
		// Add the call parameters to the used list

		if (thisOp.op == _CALLI)
		{
			SYMBOL *CallSym;
			
			CallSym = (SYMBOL *) ArrayGet(&CallArray, thisOp.rip);

			if (CallSym)
			{
				int p;
				params = CallSym->Params;
	
				for (p=0;p<params;p++)
				{
					// check if this reg was assigned previously, if it was'nt it is uninitialized before use
			
					int is_assigned = reg_assigned & REGBIT(REG_i0 + p);
			
					if (!is_assigned)
						reg_uninit |= REGBIT(REG_i0 + p);
			
					reg_used_as_src |= REGBIT(REG_i0 + p);
				}

				// Check what the function returns
				// Do nothing for void
				
				if (CallSym->RetType == RET_int   ||
					CallSym->RetType == RET_float ||
					CallSym->RetType == RET_int)
					reg_used_as_dst |= REGBIT(REG_r14);

				if (CallSym->RetType == RET_double)
					reg_used_as_dst |= REGBIT(REG_r14) | REGBIT(REG_r15);
			}

		}

		//-----------------------------------------
		//		Deal with register calls
		//-----------------------------------------
		
		// Add the call parameters to the used list

		if (thisOp.op == _CALL)
		{
				int p;
	
				for (p=0;p<4;p++)
				{
					// check if this reg was assigned previously, if it was'nt it is uninitialized before use
			
					int is_assigned = reg_assigned & REGBIT(REG_i0 + p);
			
					if (!is_assigned)
						reg_uninit |= REGBIT(REG_i0 + p);
			
					reg_used_as_src |= REGBIT(REG_i0 + p);
				}

				reg_used_as_dst |= REGBIT(REG_r14) | REGBIT(REG_r15);
		}

	}


/*
	int reg_used_as_src = 0;
	int reg_used_as_dst = 0;
	int reg_assigned = 0;
	int reg_uninit = 0;
*/

#ifdef AFDEBUG

	printf("\n");

	printf("                  rrrrrrrrrrrrrrrriiiiddddddddfrsz\n");
	printf("                  fedcba9876543210321076543210rtpr\n");

	printf("reg_used_as_src = %s\n", Bin32(reg_used_as_src));
	printf("reg_used_as_dst = %s\n", Bin32(reg_used_as_dst));
	printf("reg_assigned    = %s\n", Bin32(reg_assigned));
	printf("reg_uninit      = %s\n", Bin32(reg_uninit));

	if (reg_uninit)
		printf("");

#endif

	fp->src_reg		= reg_used_as_src;
	fp->dst_reg		= reg_used_as_dst;
	fp->assign_reg	= reg_assigned;
	fp->uninit_reg	= reg_uninit;
	fp->reg_used	= reg_used_as_src | reg_used_as_dst;

	return fp->reg_used;
}

