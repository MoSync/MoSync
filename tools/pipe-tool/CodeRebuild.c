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
//				  			  	Code Rebuilding System
// 						   		Written by A.R.Hartley
//*********************************************************************************************

#include "compile.h"

//#define DEBUG_REBUILD

#ifdef INCLUDE_CODE_REBUILD

#define CRPRINT	if (0) printf

//****************************************
//				Defines
//****************************************


//****************************************
//	
//****************************************

void RebuildInit()
{
}

//****************************************
//		  Peephole Optimizer
//****************************************

void Peeper(uchar *ip, uchar *ip_end)
{
	OpcodeInfo opi[4];
	uchar *start_ip = ip;
	int n;
	int op_count = 0;
	
	for (n=0;n<4;n++)
	{
		if (ip > ip_end)
			break;

		ip = DecodeOpcode(&opi[n], ip);
		op_count++;
	}

	if (op_count != 4)
		return;


	// Double optimizer

	while(1)
	{
/*
	ld r0,r14	
	ld r1,r15	
	ld r14,r0	
	ld r15,r1	
*/
		// All are ldr
		
		if (opi[0].op != _LDR)
			break;
		if (opi[1].op != _LDR)
			break;
		if (opi[2].op != _LDR)
			break;
		if (opi[3].op != _LDR)
			break;

		// check r0's
		if (opi[0].rd != opi[2].rs)
			break;

		// check r1's
		if (opi[1].rd != opi[3].rs)
			break;

		// check r14's
		if (opi[0].rs != opi[2].rd)
			break;

		// check r15's
		if (opi[1].rs != opi[3].rd)
			break;

		*start_ip++ = 0;
		*start_ip++ = 0;
		*start_ip++ = 0;

		*start_ip++ = 0;
		*start_ip++ = 0;
		*start_ip++ = 0;

		*start_ip++ = 0;
		*start_ip++ = 0;
		*start_ip++ = 0;

		*start_ip++ = 0;
		*start_ip++ = 0;
		*start_ip++ = 0;
		return;
	}
	
	
	// Register to register switches
	
	while(1)
	{
/*
		ld r0,r1	
		ld r1,r0	
*/
		// All are ldr

		if (opi[0].op != _LDR)
			break;
		if (opi[1].op != _LDR)
			break;

		// check r0's
		if (opi[0].rd != opi[1].rs)
			break;

		// check r1's
		if (opi[1].rd != opi[0].rs)
			break;

		*start_ip++ = 0;
		*start_ip++ = 0;
		*start_ip++ = 0;

		*start_ip++ = 0;
		*start_ip++ = 0;
		*start_ip++ = 0;
		return;
	}
	
	
	//memset(start_ip, 0, start_ip - ip);
}


//***************************************
// 
//***************************************

void RebuildEmit(char *Template, ...)
{
		char 	Str[1024];
		va_list args;
		int n,len;
		
		va_start(args,Template);
		vsprintf(Str,Template,args);
		va_end(args);

		//printf("%s",Str);

		len = strlen(Str);
		
		for (n=0;n<len;n++)
			ArrayAppend(&RebuildArray, Str[n]);

}

//****************************************
//
//****************************************

char RBType[32];

char * RebuildRetType(int type)
{
	switch(type)
	{
		case RET_void:
			strcpy(RBType, "void");
		break;

		case RET_float:
		case RET_int:
			strcpy(RBType, "int");
		break;

		case RET_double:
			strcpy(RBType, "double");
		break;

		default:
			Error(Error_System, "Invalid return type in function");
	}

	return RBType;
}

//****************************************
//		Disassemble Function
//****************************************

void RebuildFunc(SYMBOL *sym)
{
	OpcodeInfo thisOp;
	SYMBOL *ref;

	uchar *ip, *ip_end, *ip_last;
	
	int real_ip;
	char str[256];

	if (!sym)
		return;

	RebuildEmit("\n//****************************************\n");
	RebuildEmit("// Function: %s\n", sym->Name);
	RebuildEmit("//****************************************\n\n");

#if 0 //OLD
	if (strcmp(sym->Name, Code_EntryPoint) == 0)
	{
		RebuildEmit(".global %s\n", sym->Name);
		RebuildEmit(".func %s\n", sym->Name);
	}
	else
	{
		RebuildEmit(".func %s_%d\n", sym->Name, sym->LocalScope);
	}
#else //New

	if (strcmp(sym->Name, Code_EntryPoint) == 0)
	{
		RebuildEmit(".global %s\n", sym->Name);
		RebuildEmit(".func %s, %d, %s\n", sym->Name, sym->Params, RebuildRetType(sym->RetType) );
	}
	else
	{
		RebuildEmit(".func %s_%d, %d, %s\n", sym->Name, sym->LocalScope, sym->Params, RebuildRetType(sym->RetType));
	}

#endif

	ip_end = (uchar *) ArrayPtr(&CodeMemArray, sym->EndIP);
	ip = (uchar *) ArrayPtr(&CodeMemArray, sym->Value);
	real_ip	= sym->Value;

	while(1)
	{
		ip_last = ip;
		
		if (ip > ip_end)
			break;

		// Print labels
		
		ref = (SYMBOL *) ArrayGet(&CodeLabelArray, real_ip);

		if (ref)
		{
			if (ref->LabelType == label_Local)
				RebuildEmit("%s_%d:\n", ref->Name, ref->LocalScope);
		}

		RebuildEmitStabs(real_ip);
	
//		Peeper(ip, ip_end);

		if (ArgSkipElim == 0)
			if (ArrayGet(&CodeTouchArray, real_ip) == 0)
				RebuildEmit("// ");

		CaseRef = 0;

		ip = DecodeOpcode(&thisOp, ip);
		DecodeAsmString(&thisOp, str, 1);
		RebuildEmit("\t%s", str);

//		DecodeAsmString(&thisOp, str, 0);		// no scoping here
//		CodeSanityChecker(thisOp.rip, str);

		if (ArgDebugRebuild)
		{
			int len = 4 + strlen(str);		
			str[0] = 0;
			
			while(len < 40)
			{
				RebuildEmit(" ", str);
				len++;
			}
			
			RebuildEmit("; 0x%x - ", real_ip);
			
			DisassembleFromSource(real_ip, str);
			RebuildEmit("%s", str);
		}

		RebuildEmit("\n");

		// Check for case statement, which need case data after them
		
		if (CaseRef)
		{
			RebuildEmit(".data\n");
			Rebuild_Data(CaseRef);
			RebuildEmit(".code\n");
		}

		real_ip += (ip - ip_last);	
	}
}

//****************************************
//
//****************************************

//	ArraySet(&SLD_Line_Array, CodeIP, line);
//	ArraySet(&SLD_File_Array, CodeIP, This_SLD_File);

int lastfileno = -1;

void RebuildEmitStabs(int ip)
{
	int line  = ArrayGet(&SLD_Line_Array, ip);
	int file = ArrayGet(&SLD_File_Array, ip);
	char *FileStr;
	
	if (line == 0)
		return;

	if (file != lastfileno)
	{
		lastfileno = file;

		FileStr = GetFileNumString(file);

		if (FileStr)
			RebuildEmit(".sourcefile '%s'\n", FileStr);
	}

	RebuildEmit(".line %d\n", line);
}

//****************************************
//
//****************************************

void Rebuild_Code()
{
	SYMBOL *sym;
	int n;
	int c = 0;

	for (n=0;n<CodeIP+1;n++)
	{
		sym = (SYMBOL *) ArrayGet(&CodeLabelArray, n);

		if (sym)
		{
			if ((sym->Flags & SymFlag_Ref) || ArgSkipElim)
			{
				if (sym->LabelType >= label_Function)
				{
					//TestAnalyse(sym);
					RebuildFunc(sym);
					c++;
				}
			}
		}
	}

	CRPRINT("Processed %d functions\n", c);
}

//****************************************
//	  Check if a data field can be
//		     moved to bss
//****************************************

int Rebuild_CanMoveToBss(int ip, int len)
{
	SYMBOL *thisSym;
	int n;

	if (ArgSkipElim)
		return 0;

	for (n=ip;n<ip+len;n++)
	{
		// if a symbol exists in this data, don't optimize
		
		thisSym = (SYMBOL *) ArrayGet(&DataArray, n);

		if (thisSym)
			return 0;

		// If the data contains non-zero's, don't optimize

		if (GetDataMem(n))
			return 0;
	}

	return 1;
}

//****************************************
//
//****************************************

void Rebuild_Data(SYMBOL *sym)
{
	SYMBOL *thisSym;

	//uint ta = 0;
	
	int ip;
	int len;
	int n;
	int c;
	int left;
	int align;
	int opt_bss;

	// decode the field
	
	ip = sym->Value;

	if (sym->Type == SECT_bss)
		ip += MaxDataIP;

	len = sym->EndIP;
	left = len;

	// Has this been done already

	if (ArrayGet(&LabelDone, ip))
		return;

	// Mark as done

	ArraySet(&LabelDone, ip, 1);
	
	if (!len)
	{
		len = FindLabelExtent(ip);


		if (!len)
		{
			RebuildEmit("// empty %s_%d,%d\n", sym->Name, sym->LocalScope, len);
			return;
		}

		// Save the result for later

		sym->EndIP = len;

		RebuildEmit("// found extent %s_%d,%d\n", sym->Name, sym->LocalScope, len);
	}

	align = ArrayGet(&DataAlignArray, ip);
	
	if (sym->Type == SECT_bss)
	{
		RebuildEmit("\t.comm %s_%d,%d\n", sym->Name, sym->LocalScope, len);
		return;
	}
	
	if (sym->Type != SECT_data)
		Error(Error_System, "(Rebuild_Data) Illegal section in data output");

	// Check if this data field can be moved to bss

#if 0
//	opt_bss = 0;
	opt_bss  = Rebuild_CanMoveToBss(ip, len);

	if (opt_bss)
	{
		// Make sure bss output is aligned
		int bss_len = len;
		
		while(bss_len & 3)
			bss_len++;
		
		RebuildEmit("\t.comm %s_%d,%d	//moved to bss\n", sym->Name, sym->LocalScope, bss_len);
		return;
	}
#endif

	if (align)
		RebuildEmit("\t.align %d\n", align);

	// write a data section field

	RebuildEmit("%s_%d:\n", sym->Name, sym->LocalScope );

	// write array

	c = 0;

	for (n=ip;n<ip+len;n++)
	{
		thisSym = (SYMBOL *) ArrayGet(&DataArray, n);

		if (!thisSym)
		{
			RebuildEmit("\t.byte 0x%s\n", Hex8(GetDataMem(n)) );
			left--;
			continue;
		}

		// check if we hit a .word reference

		if (left >= 4)
		{
			if (thisSym)
			{
				SYMBOL *labref = NULL;
				int addr;

				addr = thisSym->Value;
	
				if (thisSym->Type == SECT_code)
				{
					labref = (SYMBOL *) ArrayGet(&CodeLabelArray, addr);

					if (labref == 0)
						Error(Error_System, "Could not repoint label !!");
//						labref = thisSym;
				}

				if (thisSym->Type == SECT_data)
				{
					labref = (SYMBOL *) ArrayGet(&LabelArray, addr);

					if (labref == 0)
						Error(Error_System, "Could not repoint label !!");
				}

				if (thisSym->Type == SECT_bss)
				{
					labref = (SYMBOL *) ArrayGet(&LabelArray, addr + MaxDataIP);

					if (labref == 0)
						Error(Error_System, "Could not repoint label !!");
				}
	
				if(labref == NULL)
					Error(Error_System, "Broken label");

				RebuildEmit("\t.word %s_%d\n", labref->Name, labref->LocalScope);
	
				left-=4;
				n+=3;

			}
		}
	}

	RebuildEmit("\n\n");
	return;
}

//****************************************
//
//****************************************

void Rebuild_Memory()
{
	SYMBOL *sym;
	int n;

	for (n=0;n<MaxDataIP + BssIP;n++)
	{
		sym = (SYMBOL *) ArrayGet(&LabelArray, n);

		if (sym)
		{
			if ((sym->Flags & SymFlag_Ref) || ArgSkipElim)
			{
				//printf("%d: Found '%s' size %d\n", n, sym->Name, sym->EndIP);

				Rebuild_Data(sym);
			}
		}
	}
}

//****************************************
//
//****************************************

void Rebuild_Main()
{
	ArrayInit(&RebuildArray, sizeof(char), 0);
	ArrayInit(&LabelDone, sizeof(char), 0);

	lastfileno = -1;
	Rebuild_Mode = 1;

	RebuildEmit("//****************************************\n");
	RebuildEmit("// Generated code\n");
	RebuildEmit("//****************************************\n\n");

	RebuildEmit(".lfile 'rebuild.s'\n");

	RebuildEmit(".code\n");
	Rebuild_Code();

	RebuildEmit(".data\n");
	Rebuild_Memory();

	ArrayWrite(&RebuildArray, "rebuild.s");
} 

//****************************************

#endif // INCLUDE_CODE_REBUILD
