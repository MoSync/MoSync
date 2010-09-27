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

#define DISAS_DEBUG 0

//****************************************
//			Build Fetch Table
//****************************************

#undef INSTR_MACRO
#define INSTR_MACRO(op_far, op_num, op_str, op_jstr, op_fetch, op_extra)	op_fetch,

int OpcodeFetch[256] =
{
	#include "InstTable.h"
};

//****************************************
// 		Build Opcode Strings Table
//****************************************

#undef INSTR_MACRO
#define INSTR_MACRO(op_far, op_num, op_str, op_jstr ,op_fetch, op_extra)	op_str,

char *OpcodeStrings[256] =
{
	#include "InstTable.h"
};

//****************************************
//		   Decode an opcode
//****************************************

#define FRED_HACK

uchar * DecodeOpcode(OpcodeInfo *thisOpcode, uchar *code_ip)
{
	uchar thisOp;
	uchar thisRD=0,thisRS=0;
	uint thisIMM=0, FetchedImm=0;
	uint rip = (int) code_ip - (int) ArrayPtr(&CodeMemArray, 0);
	
	uchar *start_code_ip = code_ip;		// Make a copy
	
	int farflag = 0;
	int flags;

	CaseRef = 0;

	thisOp = *code_ip++;

	if (thisOp > _FAR)
		ErrorOnIP(Error_Fatal, rip, "DecodeOpcode: Illegal instruction > _FAR (1)");
//		return 0;

	// Decode the op
	
	if (thisOp == _FAR)
	{
		thisOp = *code_ip++;
		farflag = 1;
	}

	if (thisOp >= _FAR)
		ErrorOnIP(Error_Fatal, rip, "DecodeOpcode: Illegal instruction > _FAR (2)");
//		return 0;
	
	flags = OpcodeFetch[thisOp];
		
	// Fetch register info first
	
	if (flags & fetch_d)
		thisRD = *code_ip++;

	if (flags & fetch_s)
		thisRS = *code_ip++;

	// rest is either or

	if (flags & fetch_a)
	{
		if (farflag)
		{
			thisIMM = (*code_ip++ << 16);
			thisIMM += (*code_ip++ << 8);
			thisIMM += *code_ip++;
			FetchedImm++;
		}
		else
		{
			thisIMM = *code_ip++ << 8;
			thisIMM += *code_ip++;
			FetchedImm++;
		}
	}

	if (flags & fetch_c)
	{
		thisIMM = (*code_ip++ << 16);
		thisIMM += (*code_ip++ << 8);
		thisIMM += *code_ip++;
		FetchedImm++;
	}

	if (flags & (fetch_j | fetch_k))
	{
		thisIMM = *code_ip++;
		FetchedImm++;
	}
	
	if (flags & fetch_i)
	{
		thisIMM = *code_ip++;
		
		if(thisIMM > 127)
		{
			thisIMM = ((thisIMM & 127) << 8) + *code_ip++;
		}

		FetchedImm++;
	}

	if (flags & fetch_i)
	{
		thisIMM = GetVarPoolEntry(thisIMM);
		FetchedImm++;
	}
	
	// ** Special case for SysCalls **
	// which marks that they use R14
	
	if (thisOp == _SYSCALL)
	{
		flags |= fetch_d;
		thisRD = REG_r14;
	}

	// !! ARH note remember this could be dangerous !!
	// !! Expand RS registers > 32 into constants in imm field 
	
	if ((flags & fetch_s) && (thisRS >= 32))
	{
		// Only do this if we have'nt used the imm field

		if (FetchedImm == 0)
			thisIMM = ConstRegValue(thisRS);
	}
	
	// !!

	// Save info

	thisOpcode->flags	= flags;
	thisOpcode->farflag	= farflag;
	thisOpcode->op		= thisOp;
	thisOpcode->rd		= thisRD;
	thisOpcode->rs		= thisRS;
	thisOpcode->imm		= thisIMM;
	thisOpcode->rip		= rip;
	thisOpcode->str		= OpcodeStrings[thisOp];
	thisOpcode->len		= code_ip - start_code_ip;
	
	return code_ip;
}

//****************************************
//		   Decode an opcode
//****************************************

int DecodeOpcodeIP(OpcodeInfo *thisOpcode, int code_ip)
{
	uchar *ip = (uchar *) ArrayPtr(&CodeMemArray, code_ip);
	uchar *sip = ip;

	ip = DecodeOpcode(thisOpcode, ip);
	
	code_ip += (int) (ip - sip);

	if (code_ip < 0)
		Error(Error_Fatal, "DecodeOpcodeIP: code_ip < 0 (Contact MoSync)");

	return code_ip;
}

//***************************************
//        print to disassembler
//***************************************

void DecodeAsmEmit(char *out, char *Template, ...)
{
	char 	Str[1280];
	va_list args;

	va_start(args,Template);
	vsprintf(Str,Template,args);
	va_end(args);

	strcat_cond(out,Str);
}

//****************************************
//			Get Reg Name
//****************************************

char str_cond_table[128];

void strcat_cond(char *in_dst, char *src)
{		
	int dstlen = strlen(in_dst);
	char c;
	int cond;

	char *dst = &in_dst[dstlen];
	
	while(1)
	{
		c = *src++;
		
		if (c == 0)
			break;

		if (c != '<')
		{
			*dst++ = c;
			continue;
		}

		// Get condition delimeter

		c = *src++;

		// Make sure that the delimeter is there, otherwise error

		if (c != '@')
			Error(Error_Fatal, "Missing '@' delimeter in conditional string");

		// Get condition variable char

		c = *src++;
		cond  = str_cond_table[c & 127];
		
		// Copy everything if the condition is true
		
		while(1)
		{
			// Get the next char

			c = *src++;

			if (c == 0)
				Error(Error_Fatal, "Missing '>' closure in output");

			// if we have reached the end marker, skip it then keep going

			if (c == '>')
				break;

			// If condition is true copy the char

			if (cond)
				*dst++ = c;
			
		}
	}

	*dst = 0;
}

//****************************************
//		 Set string condition
//****************************************

void SetStrCondition(int v, int c)
{
	str_cond_table[v&127] = c;
}

//****************************************
//			Get Reg Name
//****************************************

char * DecodeRegName(int reg, int use_zero)
{
	regstr[0] = 0;

	if (reg & 0xffffff00)
		return regstr;

	if (use_zero && reg == 0)
	{
		sprintf(regstr, "#0");
		return regstr;
	}

	if (reg < 32)
	{
		strcpy(regstr, regsyms[reg]);
		return regstr;
	}

	sprintf(regstr, "#0x%x", ConstTable[reg] );
	return regstr;
}

//****************************************
//		Disassemble Case
//****************************************

void DecodeCase(char *out, OpcodeInfo *thisOpcode)
{
	SYMBOL *ref, *labref;
	int addr;
	int data_ip;

	data_ip = thisOpcode->imm;

	// Write start

	DecodeAsmEmit(out, "#0x%x,", GetDataMemLong(data_ip));
	data_ip++;

	// Write len

	DecodeAsmEmit(out, "#0x%x,", GetDataMemLong(data_ip));
	data_ip++;

	// Write table address

	ref = (SYMBOL *) ArrayGet(&DataAccessArray, thisOpcode->rip);
			
	if (!ref)
		ErrorOnIP(Error_Fatal,thisOpcode->rip, "Bad case table reference");

	CaseRef = ref;
	
	DecodeAsmEmit(out, "#%s<@1_%d>,", ref->Name, ref->LocalScope);

	// Write default address
	
	ref = (SYMBOL *) ArrayGet(&CallArray, thisOpcode->rip);
	
	// Check if what it points to in lablearry and use that
			
	if (!ref)
		ErrorOnIP(Error_Fatal,thisOpcode->rip, "Bad default case reference");

	addr = ref->Value;
	
	labref = (SYMBOL *) ArrayGet(&CodeLabelArray, addr);

	if (!labref)
		ErrorOnIP(Error_Fatal, thisOpcode->rip, "Bad default case label reference");
	
	ref = labref;

	DecodeAsmEmit(out, "#%s<@1_%d>", ref->Name, ref->LocalScope);	
}

//****************************************
//		Disassemble to string
//****************************************

int DecodeDataAccessImm(char *out, OpcodeInfo *thisOpcode)
{
	SYMBOL *ref;
	int d=0;

	ref = (SYMBOL *) ArrayGet(&DataAccessArray, thisOpcode->rip);
			
	if (!ref)
		return 0;

	if (DISAS_DEBUG) DecodeAsmEmit(out, "*DDAI*");

	if (ref->Type == SECT_data)
		d = thisOpcode->imm - ref->Value;
	else if (ref->Type == SECT_bss)
		d = thisOpcode->imm - (ref->Value + MaxDataIP);
	else if (ref->Type == SECT_code)
	{
		// !!! BEWARE: test this code !!!

		//DecodeAsmEmit(out, ">>>");
		d = thisOpcode->imm - ref->Value;
	}
	else
		ErrorOnIP(Error_Fatal,thisOpcode->rip, "(DecodeDataAccessImm) illegal section reference %d", ref->Type);
	
	if (d == 0)
	{
		DecodeAsmEmit(out, "&%s<@1_%d>", ref->Name, ref->LocalScope);
		return 1;
	}

	if (d > 0)
	{
	
		if (ref->EndIP)				// Get rid of 0 lenght reports
		if (d >= ref->EndIP)
			ErrorOnIP(Error_Warning, thisOpcode->rip, "Index '%s+%d': may be out of bounds (%d length)", ref->Name, d, ref->EndIP);
	
		DecodeAsmEmit(out, "&%s<@1_%d>+%d", ref->Name, ref->LocalScope, d);
		return 1;
	}

	//FIX ARH 2008-11-03
	// Some time we got a double negative on a index from variable
	// so now invert d so that it becomes positive
	
	d = -d;

	DecodeAsmEmit(out, "&%s<@1_%d>-%d", ref->Name, ref->LocalScope, d);

	if (ref->EndIP)				// Get rid of 0 lenght reports
	ErrorOnIP(Error_Warning, thisOpcode->rip, "Index '%s-%d': negative index may be out of bounds (%d length)", ref->Name, d, ref->EndIP);
	return 1;
}

//****************************************
//		Disassemble to string
//****************************************

int DecodeIndexAccessImm(char *out, OpcodeInfo *thisOpcode)
{
	SYMBOL *ref;
	int d=0;

	ref = (SYMBOL *) ArrayGet(&DataAccessArray, thisOpcode->rip);
			
	if (!ref)
		return 0;

	if (DISAS_DEBUG) DecodeAsmEmit(out, "*DIAI*");

	if (ref->Type == SECT_data)
		d = thisOpcode->imm - ref->Value;
	else if (ref->Type == SECT_bss)
		d = thisOpcode->imm - (ref->Value + MaxDataIP);
	else
		ErrorOnIP(Error_Fatal,thisOpcode->rip,"(DecodeIndexAccessImm) illegal section reference");
	
	if (d == 0)
	{
		DecodeAsmEmit(out, "%s<@1_%d>", ref->Name, ref->LocalScope);
		return 1;
	}

	if (d > 0)
	{
		if (ref->EndIP)				// Get rid of 0 lenght reports
		if (d >= ref->EndIP)
			ErrorOnIP(Error_Warning, thisOpcode->rip, "Index '%s+%d': may be out of bounds (%d length)", ref->Name, d, ref->EndIP);
	
		DecodeAsmEmit(out, "%s<@1_%d>+%d", ref->Name, ref->LocalScope, d);
		return 1;
	}

	//FIX ARH 2008-11-03
	// Some time we got a double negative on a index from variable
	// so now invert d so that it becomes positive
	
	d = -d;

	DecodeAsmEmit(out, "%s<@1_%d>-%d", ref->Name, ref->LocalScope, d);

	if (ref->EndIP)				// Get rid of 0 lenght reports
	ErrorOnIP(Error_Warning, thisOpcode->rip, "Index '%s-%d': negative index may be out of bounds (%d length)", ref->Name, d, ref->EndIP);
	return 1;
}


//****************************************
/*
int DecodeDataAccess(char *out, OpcodeInfo *thisOpcode)
{
	SYMBOL *ref;

	ref = (SYMBOL *) ArrayGet(&DataAccessArray, thisOpcode->rip);
			
	if (!ref)
		return 0;

	if (DISAS_DEBUG) DecodeAsmEmit(out, "*DDA*");

	DecodeAsmEmit(out, "&%s<@1_%d>", ref->Name, ref->LocalScope);
	return 1;
}
*/

int DecodeDataAccess(char *out, OpcodeInfo *thisOpcode)
{
	SYMBOL *ref;
	int d=0;

	ref = (SYMBOL *) ArrayGet(&DataAccessArray, thisOpcode->rip);
			
	if (!ref)
		return 0;

	if (DISAS_DEBUG) DecodeAsmEmit(out, "*DDA*");

// !imm

	if (ref->Type == SECT_data)
		d = thisOpcode->imm - ref->Value;
	else if (ref->Type == SECT_bss)
		d = thisOpcode->imm - (ref->Value + MaxDataIP);
	else
		ErrorOnIP(Error_Fatal,thisOpcode->rip,"(DecodeDataAccess) illegal section reference");


	DecodeAsmEmit(out, "&%s<@1_%d>(+%d)", ref->Name, ref->LocalScope,d);
	return 1;
}

//****************************************

int DecodeCallArray(char *out, OpcodeInfo *thisOpcode)
{
	SYMBOL *ref, *labref;
	int addr;
	
	ref = (SYMBOL *) ArrayGet(&CallArray, thisOpcode->rip);
	
	// !! Check if what it points to in lablearry and use that !!
			
	if (!ref)
		return 0;

	if (DISAS_DEBUG) DecodeAsmEmit(out, "*DCA*");

	addr = ref->Value;
	
	labref = (SYMBOL *) ArrayGet(&CodeLabelArray, addr);

	if (!labref)
		return 0;
	
	ref = labref;

	DecodeAsmEmit(out, "&%s<@1_%d>", ref->Name, ref->LocalScope);
	return 1;
}

//****************************************
//		Disassemble to string
//****************************************

void DecodeAsmString(OpcodeInfo *thisOpcode, char *out, int add_scope)
{
	short n,len;
	char c;
	char *oldFilePtr = FilePtr;			// Save FilePtr

	out[0] = 0;							// Terminate string first

	SetStrCondition('1', add_scope);

	SetFilePtrFromIP(thisOpcode->rip);

	CaseRef = 0;

	len = strlen(thisOpcode->str);	

	
	for(n=0;n<len;n++)
	{
		c = thisOpcode->str[n];
	
		switch(c)
		{	
			case 'd': 		// Reg rd
			{
				if (DISAS_DEBUG) DecodeAsmEmit(out,"(d)");
				DecodeAsmEmit(out,"%s",DecodeRegName(thisOpcode->rd, 1));
			}
			break;

			case 's': 		// Reg rs
			{
				if (DISAS_DEBUG) DecodeAsmEmit(out,"(s)");
				DecodeAsmEmit(out, "%s",DecodeRegName(thisOpcode->rs, 1));
			}
			break;		

			case 'q': 		// Reg rs or DataAccess
			{
				if (DISAS_DEBUG) DecodeAsmEmit(out,"(q)");

//!! A copy
//				if (DecodeDataAccess(out, thisOpcode))
//					break;
// !!


// !! *ARH* Note New Solution Carefull about this !!

				if (thisOpcode->rs >= 32)
				{
					if (DecodeCallArray(out, thisOpcode))
						break;
			
					if (DecodeDataAccessImm(out, thisOpcode))
						break;
				}
// !!

				DecodeAsmEmit(out, "%s",DecodeRegName(thisOpcode->rs, 1));
			}
			break;		


			case 'i': 		// Immediate const
			case 'a': 		// Immediate address
			{
				if (DISAS_DEBUG) DecodeAsmEmit(out,"(ia)");

				if (DecodeCallArray(out, thisOpcode))
					break;
		
				if (DecodeDataAccessImm(out, thisOpcode))
					break;

				DecodeAsmEmit(out,"#0x%x",thisOpcode->imm);
			}
			break;

			case 'c': 		// Immediate address
				if (DISAS_DEBUG) DecodeAsmEmit(out,"(c)");

				DecodeCase(out, thisOpcode);
			break;


			case 'j': 		// Immediate 8 bits
			{
				if (DISAS_DEBUG) DecodeAsmEmit(out,"(j)");

				DecodeAsmEmit(out,"#0x%x",thisOpcode->imm);
			}
			break;

			case 'k': 		// Syscalls
			{
#if 1
				DecodeAsmEmit(out,"%d",thisOpcode->imm);
#else
				{
					SYMBOL *syscall =  FindSysCall(thisOpcode->imm);			

					if (!syscall)
					{
						Error(Error_System, "Could'nt locate syscall\n");
						return;
					}
					
					DecodeAsmEmit(out,"#%s",syscall->Name);
				}
#endif
				
			}		
			break;

			case 'm': 		// rs+imm	
			{

				if (DISAS_DEBUG)
					DecodeAsmEmit(out,"(m)");

				if (thisOpcode->imm)
				{
//					if (thisOpcode->rs)
						DecodeAsmEmit(out,"%s,",DecodeRegName(thisOpcode->rs, 0));
//					else
//						DecodeAsmEmit(out,"#");


					if (DecodeIndexAccessImm(out, thisOpcode))
						break;

					DecodeAsmEmit(out,"%d", thisOpcode->imm);
				}
				else // 0 case
				{
					DecodeAsmEmit(out,"%s",DecodeRegName(thisOpcode->rs, 0));

					if (DecodeIndexAccessImm(out, thisOpcode))
						break;
				}

			}		
			break;

			case 'n': 		// rd+imm	
			{
				if (DISAS_DEBUG) DecodeAsmEmit(out,"(n)");

				if (thisOpcode->imm)
				{
//					if (thisOpcode->rd)
						DecodeAsmEmit(out,"%s,",DecodeRegName(thisOpcode->rd, 0));
//					else
//						DecodeAsmEmit(out,"#");

					if (DecodeIndexAccessImm(out, thisOpcode))
						break;

					DecodeAsmEmit(out,"%d", thisOpcode->imm);
				}
				else
				{
					DecodeAsmEmit(out,"%s",DecodeRegName(thisOpcode->rd, 0));

					if (DecodeIndexAccessImm(out, thisOpcode))
						break;
				}

			}		
			break;

			case 'x': 		// Push
			{
				if (DISAS_DEBUG) DecodeAsmEmit(out,"(x)");
				DecodeAsmEmit(out,"%s,",DecodeRegName(thisOpcode->rd, 0));
				DecodeAsmEmit(out,"%s",DecodeRegName(thisOpcode->rd + thisOpcode->rs - 1, 0));
			}
			break;

			case 'y': 		// Pop
			{
				if (DISAS_DEBUG) DecodeAsmEmit(out,"(y)");
				DecodeAsmEmit(out,"%s,", DecodeRegName(thisOpcode->rd - thisOpcode->rs + 1, 0));
				DecodeAsmEmit(out,"%s",  DecodeRegName(thisOpcode->rd, 0));
			}
			break;

			case  32: 		// Space
			case  9: 		// Space
			{
				DecodeAsmEmit(out," ");
			}
			break;

			default:
				if (isalpha(c))
					DecodeAsmEmit(out,"%c",tolower(c));
				else
					DecodeAsmEmit(out,"%c",c);

		}

	}

	FilePtr = oldFilePtr;		// Restore FilePtr
}

//****************************************
//		Disassemble Labels
//****************************************

void DecodeAsmLabel(int ip, char *out)
{
	SYMBOL *ref;

	out[0] = 0;
	
	ref = (SYMBOL *) ArrayGet(&CodeLabelArray, ip);

	if (!ref)
		return;

	if (ref->LabelType == label_Local)
		DecodeAsmEmit(out,"%s<@1_%d>", ref->Name, ref->LocalScope);
}

//****************************************
//		Disassemble Function
//****************************************

void DisassembleFunc(char *func, int showHex)
{
	OpcodeInfo thisOp;
	SYMBOL *sym, *ref;
	uchar *ip, *ip_last;
	int base, n, len;
	char str[256];

	sym = GetGlobalSym(func);

	if (!sym)
	{
		printf("Symbol '%s' does not exist, or non-global\n", func);
		return;
	}

	if (sym->Type != SECT_code)
	{
		//printf("Only functions can be disassembled\n");

		DisassembleData(sym);
		return;
	}
		
	printf("Function '%s'\n\n", sym->Name);

	ip = (uchar *) ArrayPtr(&CodeMemArray, sym->Value);

	base	= sym->Value;

	while(1)
	{
		ip_last = ip;
		
		ip = DecodeOpcode(&thisOp, ip);
		DecodeAsmString(&thisOp, str, 1);			// scoped variable on

		len = ip - ip_last;

		printf("%-4x: %-32s\t",thisOp.rip, str);

		if (showHex)
		{
			for (n=0;n<len;n++)
				printf("%s ", Hex8(GetCodeMem(thisOp.rip + n)));
		}


//		printf("%-4x: %-32s\t",base - sym->Value, str);
//		printf("%-4x: %-32s\t",thisOp.rip, str);

		// Print CodeLabelArray
		
		ref = (SYMBOL *) ArrayGet(&CodeLabelArray, base);
		
		if (ref)
		{
			if (sym->Value != base)
			if (sym->LabelType >= label_Function)
				break;
				
			printf("<CLA '%s'>", ref->Name);
		}
		
		// Print CallArray
		
		ref = (SYMBOL *) ArrayGet(&CallArray, base);
		
		if (ref)
			printf("<CA '%s'>", ref->Name);

		// Print DataAccessArray
		
		ref = (SYMBOL *) ArrayGet(&DataAccessArray, base);
		
		if (ref)
			printf("<DA '%s'>", ref->Name);

		printf("\n");

		str[0] = 0;
		DisassembleFromSource(thisOp.rip, str);
		printf("%-4x: %-32s\n",thisOp.rip, str);


		base += len;	
	}

	printf("\n");

}

//****************************************
//
//****************************************

void DisassembleData(SYMBOL *sym)
{
	SYMBOL *thisSym;

	uint ip;
	uint n, v;

	if (!sym)
		Error(Error_System, "DisassembleMemory null symbol\n");

	printf("Data '%s' Addr 0x%x Len %d\n\n", sym->Name, sym->Value, sym->EndIP);

	if (sym->Type == SECT_code)
		return;

	ip = sym->Value;

	if (sym->Type == SECT_bss)
		ip += MaxDataIP;

	// Byte search of memory

	for (n=ip;n<LabelArray.hi;n++)
	{
		printf("0x%x: %s\t", n, Hex8(GetDataMem(n)));

		// Check DataArray

		v = ArrayGet(&DataArray, n);

		if (v)
		{
			thisSym = (SYMBOL *) v;
									
			if (thisSym->Type == SECT_data)
				printf( "<DP '%s'>", thisSym->Name);

			if (thisSym->Type == SECT_code)
				printf("<FP '%s'>", thisSym->Name);
		}

		printf("\n");
		
		// Check if the data symbol is finished, i.e it bumps
		// into the next symbol
		
		if (n != ip)
		{
			v = ArrayGet(&LabelArray, n);

			if (v)
				break;
		}

		// Next ip
	}
}

//****************************************
//		Disassemble Function
//****************************************

int FunctionRegUsage(SYMBOL *sym)
{
	OpcodeInfo thisOp;
	uchar *ip, *ip_end;
	int regmask = 0;
	
	if (!sym)
		return -1;

	if (sym->Type != SECT_code)
		return -1;

	ip_end = (uchar *) ArrayPtr(&CodeMemArray, sym->EndIP);
	ip = (uchar *) ArrayPtr(&CodeMemArray, sym->Value);

	while(1)
	{		
		if (ip > ip_end)
			break;
	
		ip = DecodeOpcode(&thisOp, ip);

		// Ignor push and pop
		
		if (thisOp.op == _PUSH)
			continue;

		if (thisOp.op == _POP)
			continue;

		// Check for a dest reg
		
		if (thisOp.flags & fetch_d)
			if (thisOp.rd < 32)
				regmask |= (1 << thisOp.rd);

		// Check for a source reg
		
		if (thisOp.flags & fetch_s)
			if (thisOp.rs < 32)
				regmask |= (1 << thisOp.rs);
	}

	return regmask;
}

//****************************************
// Enumerate the labels of a Function
//****************************************

void EnumerateFunctionLabels(SYMBOL *sym)
{
	SYMBOL *ref;
	
	uchar *ip, *ip_end;
	
	int real_ip;
	int Count;
	
	if (!sym)
		return;

	ip_end = (uchar *) ArrayPtr(&CodeMemArray, sym->EndIP);
	ip = (uchar *) ArrayPtr(&CodeMemArray, sym->Value);	
	
	real_ip	= sym->Value;
	Count = 1;
		
	while(1)
	{		
		if (ip > ip_end)
			break;

		// Print labels
		
		ref = (SYMBOL *) ArrayGet(&CodeLabelArray, real_ip);

		if (ref)
		{
			if (ref->LabelType == label_Local)
			{
				// Set the enumerator for this label

				ref->LabelEnum = Count;

				Count++;
			}
		}

		real_ip++;	
		ip++;
	}

	Count--;
	
	if (Count > MaxEnumLabel)
		MaxEnumLabel = Count;
}

//****************************************
//Output disassembly from source file line
//****************************************

int DisassembleFromSource(int codeIP, char *out)
{
	char *AsmChars = (char *) ArrayGet(&AsmCharArray, codeIP);
	char c;
	int n;
	
	if (!AsmChars)
	{
		DecodeAsmEmit(out,"no source");
		return 0;
	}
	
	for (n=0;n<1024;n++)
	{
		c = *AsmChars++;
		
		if (c == 0)
			break;

		if (c == 13)
			break;

		if (c == 10)
			break;

		if (c == ';')
			break;

		if (c == '/')
			if (*AsmChars == '/')
				break;

		DecodeAsmEmit(out,"%c",c);
	}
	
	return n;
}

//****************************************
//Disassemble from data in source file
//****************************************

int DisassembleDataFromSource(int dataIP, char *out)
{
	char *AsmChars = (char *) ArrayGet(&AsmCharDataArray, dataIP);

	char c;
	int n;
	
	if (!AsmChars)
		return 0;
	
	for (n=0;n<1024;n++)
	{
		c = *AsmChars++;
		
		if (c == 0)
			break;

		if (c == 13)
			break;

		if (c == 10)
			break;

		if (c == ';')
			break;

/*		if (c == '/')
			if (*AsmChars == '/')
				break;
*/
		DecodeAsmEmit(out,"%c",c);
	}
	
	return n;
}

//****************************************
//	Set FilePtr from Code IP address
//****************************************

int SetFilePtrFromIP(int ip)
{
	char *AsmChars = (char *) ArrayGet(&AsmCharArray, ip);

	if (AsmChars)
		FilePtr = AsmChars;
		
	return (int) AsmChars;
}

//****************************************
//
//****************************************

char *cs_skip(char *ws)
{
	char *last_ws;
	
	PushTokenPtr(ws, 0);

	SkipWhiteSpace();

	last_ws = FilePtr;
	PopTokenPtr();
	
	return last_ws;
}

//****************************************
//		Convert '&' to '#'
//****************************************

void ConvAmp2Hash(char *str)
{
	char c;
	
	while(1)
	{
		c = *str;
		
		if (!c)
			break;
	
		if ((c == '#') && (*(str+1) == '0'))
		{
			*str++ = 'z';
			*str++ = 'r';
			continue;
		}
			
		if (c == '&')
			*str = '#';

		str++;
	}
}


//****************************************
//		Code Sanity Checker
//****************************************

int CodeSanityChecker(int ip, char *gen_code)
{
	char src_code[1024];
	char *src,*gen ;
	
	src_code[0] = 0;
	DisassembleFromSource(ip, src_code);

	ConvAmp2Hash(src_code);
	ConvAmp2Hash(gen_code);

	src = src_code;
	gen = gen_code;

	while(1)
	{
		src = cs_skip(src);
		gen = cs_skip(gen);

		if (*src != *gen)
		{
			RebuildEmit("\n\\\\ Sanity report\n");
			RebuildEmit("\\\\ src = '%s'\n", src_code);
			RebuildEmit("\\\\ gen = '%s'\n", gen_code);
			return 0;
		}
			
		if (*src == 0)
			break;

		src++;
		gen++;

	}

	return 1;	
}





















