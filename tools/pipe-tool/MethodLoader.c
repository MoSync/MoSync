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

//********************************************************************************
//									Class Reader
//********************************************************************************

#include "compile.h"

//static int verbose = 0;

#define vprintf	if (verbose) printf
#define dprintf	if (0) printf

uchar *CodePtr;
uchar *CodeStart;
uchar *CodeEnd;

int IsWide;

//****************************************

char *ArrayTypes[16] = 
{
	"error",
	"error",
	"error",
	"error",

	"bool",
	"char",
	"float",
	"double",

	"byte",
	"short",
	"int",
	"long",
	
	"error",
	"error",
	"error",
	"error"
};

//****************************************
// 			Opcode strings
//****************************************

#define INSTRDEF(str, sym, flag, len, perm) str,

char *opcode_name[256] = 
{
#include "Bytecode.h"
};

#undef INSTRDEF


//****************************************
//			opcode lengths
//****************************************

#define INSTRDEF(str, sym, flag, len, perm) len,

short opcode_length[256] =
{
#include "Bytecode.h"
};

#undef INSTRDEF

//****************************************
//		    Get opcode length
//****************************************

void SetCodeStart(unsigned char *cs, int len)
{
	CodeStart = cs;
	CodeEnd = cs + len;
}

//****************************************
//		    Get opcode length
//****************************************

int GetOpcodeLen(unsigned char *pc)
{
	uint n;
	uchar opc = *pc;

	if (opc > IMPDEP2)
		return -1;

	switch (opc)
	{
		case TABLESWITCH:
			n = (4 - ((uint) (pc - CodeStart) & 0x03)) + 4;
			pc += n;
			n += (memL(pc + 4) - memL(pc) + 1) * 4;
			return n;
			
		case LOOKUPSWITCH:
			n = (4 - ((uint) (pc - CodeStart) & 0x03)) + 4;
			pc += n;
			n += memL(pc) * 8 + 4;
			return n;

		case WIDE:
			if (pc[1] == IINC)	// All wide instructions are 4 bytes except IINC
				return 6;
			return 4;
	}

	return opcode_length[opc];
}

//****************************************
//
//****************************************

int ValidWide(int opc)
{
	switch(opc)
	{
		case ILOAD:
		case FLOAD:
		case ALOAD:
		case LLOAD:
		case DLOAD:
		case ISTORE:
		case FSTORE:
		case ASTORE:
		case LSTORE:
		case DSTORE:
			return 1;
	}

	return 0;
}

//****************************************
//
//****************************************

void Dis_LookUpSwitch()
{
	int entries,off;
	int default_off;
	int key,offset;
	int n,pad;

	dprintf("lookupswitch ");
	
	// Skip padding

	off = CODE_OFFSET();	
	pad = 4 - (off % 4);
	CodePtr += pad;
	
	default_off = memLI(CodePtr);
	dprintf("default_off %d ", default_off);

	entries = memLI(CodePtr);
	dprintf("entries %d\n", entries);
			
	for (n=0;n<entries;n++)
	{
		key = memLI(CodePtr);
		offset = memLI(CodePtr);

		dprintf("\t%d: key %d off %d\n", n, key, offset + off);
	}
}

//****************************************
//
//****************************************

void Dis_TableSwitch()
{
	int off;
	int default_off;
	int low,high,offset;
	int n,pad;

	dprintf("tableswitch ");
	
	// Skip padding
	
	off = CODE_OFFSET();
	pad = 4 - (off % 4);
	CodePtr += pad;
	
	default_off = memLI(CodePtr);
	dprintf("default_off %d ", default_off+ off - 1);
	
	low = memLI(CodePtr);
	dprintf("low %d ", low);

	high = memLI(CodePtr);
	dprintf("high %d\n", high);

	for (n = 0; n < high - low + 1; n++)	
	{
		offset = memLI(CodePtr);

		dprintf("\t%d: offset %d\n", low + n, offset + off - 1);
	}

	return;
}

//****************************************
//
//****************************************

char ms_str[256];
int ms_trigger;

void DecodeBytecode(int opc, int off)
{
	char *opPtr = opcode_name[opc];
	char c;
	int v;

	ms_trigger = 0;
	
	while(1)
	{
		c = *opPtr++;

		if (!c)
			return;
		
		if (c == '%')
		{
			c = *opPtr++;

			switch(c)
			{
				case 0:
					return;
					
				case 'b':
					v = memBI(CodePtr);
					dprintf("%d",(char) v);
					break;		

				case 'w':
					v = memWI(CodePtr);
					dprintf("%d",(short) v);
					break;		

				case 'n':
					v = memBI(CodePtr);
					dprintf("%d", v);
				break;		

				case 'o':
					v = memWI(CodePtr);
					dprintf("%d", off + (short) v);
				break;		

				case 'p':
				{
					PoolInfo theInfo;
					char str[100];
					char *substr;
					
					v = memWI(CodePtr);
					GetPoolInfo(&mainClass, v, &theInfo);

					substr = PoolSubString(theInfo.String, str, 0);
					dprintf("%s", substr );
					
					if (strcmp(substr, "ms") == 0)
						ms_trigger++;
					
					substr = PoolSubString(theInfo.String, str, 1);
					dprintf(".%s", substr );

					if (ms_trigger)
					{
						strcpy(ms_str, substr);
					}

//					printf("'%s'",theInfo.String);

				}
				break;		

				case 't':
					v = memBI(CodePtr) & 0x0f;
					dprintf("%s", ArrayTypes[v]);
				break;		
				
				default:
					dprintf("!!! %%%c", c);
			}
			
			continue;
		}

		dprintf("%c", c);
	}
}

//****************************************
//			Print opcode
// returns length / ClassFilePtr uneffected
//****************************************

void PrintOpcode()
{
	unsigned char *thisInst = CodePtr;
	unsigned char opc;
	int opsize;

	int off = CODE_OFFSET();

	opsize = GetOpcodeLen(CodePtr);

	if (opsize == -1)
	{
		Error(Error_System, "illegal bytecode size\n");
		exit(1);
	}

	IsWide = 0;

	dprintf("%d: ",off);

	if (*CodePtr == WIDE)
	{
		IsWide = 1;
		CodePtr++;
	}

	off = CODE_OFFSET();	
	opc = memBI(CodePtr);

	if (IsWide)
	{
		//ToDo: must check if valid wide opcode here
		//if (!ValidWide(op))
	}
	
	if (opc == LOOKUPSWITCH)
	{
		Dis_LookUpSwitch();
		return;
	}

	if (opc == TABLESWITCH)
	{
		Dis_TableSwitch();
		return;
	}

	DecodeBytecode(opc, off);

	// Avoid possible errors by recalculating next intstruction address

	CodePtr = thisInst + opsize;
	return;
}

//****************************************
//		 Read the constant pool
//****************************************

int LabelTable[1024];

char dstr[4096];

void Disassemble()
{
	uchar *LastCodePtr;
	uchar *CodeTop;

	int LastCodeOff;
	int MaxStack;
	int MaxLocal;
	int CodeLen;
	int n;

	int label_num = 0;
	int goto_num = 0;

	CodePtr = ClassFilePtr;

	MaxStack = memWI(CodePtr);
	MaxLocal = memWI(CodePtr);
	CodeLen = memLI(CodePtr);

	// Mark the code start position
		
	SetCodeStart(CodePtr, CodeLen);

	CodeTop = CodePtr;

	// Local labels
	
	dprintf("Code Block (MaxStack=%d : MaxLocal=%d : CodeLen=%d)\n\n",MaxStack,MaxLocal,CodeLen);
	
	for (n=0;n<CodeLen;n++)
	{
		if (CodePtr >= CodeEnd)
			break;
		
		LastCodePtr = CodePtr;
		LastCodeOff = CODE_OFFSET();
		
		dprintf("%d :", CodePtr - ClassPtr);
		PrintOpcode();

		if (ms_trigger)
		{
			if (strncmp(ms_str, "label_", 6) == 0)
			{
				sscanf(ms_str, "label_%d", &label_num);
				dprintf("LABEL %d at %d\n", label_num, LastCodePtr - ClassPtr);

				LabelTable[label_num] = LastCodeOff; // file index

				// Patch lable code

				LastCodePtr[0] = NOP;
				LastCodePtr[1] = NOP;
				LastCodePtr[2] = NOP;
				
			}
		}
	}

	// Patch goto's

	CodePtr = CodeTop;

	dprintf("Code Block (MaxStack=%d : MaxLocal=%d : CodeLen=%d)\n\n",MaxStack,MaxLocal,CodeLen);
	
	for (n=0;n<CodeLen;n++)
	{
		if (CodePtr >= CodeEnd)
			break;
		
		LastCodePtr = CodePtr;
		LastCodeOff = CODE_OFFSET();	
		
		dprintf("%d :", CodePtr - ClassPtr);
		PrintOpcode();

		if (ms_trigger)
		{
			if (strncmp(ms_str, "goto_", 5) == 0)
			{
				int off;

				sscanf(ms_str, "goto_%d", &goto_num);
				dprintf("GOTO %d at %d\n", goto_num, LastCodePtr - ClassPtr);

				off = 3 + (LabelTable[goto_num] - LastCodeOff);

				// Patch lable code

				LastCodePtr[0] = GOTO;
				LastCodePtr[1] = (off >> 8) & 0xff;
				LastCodePtr[2] = off & 0xff;
			}
		}
	}

	// Result

#if 0

	CodePtr = CodeTop;

	dprintf("Result\n\n");
	
	for (n=0;n<CodeLen;n++)
	{
		if (CodePtr >= CodeEnd)
			break;
		
		LastCodePtr = CodePtr;
		LastCodeOff = CODE_OFFSET();	
		
		dprintf("%d :", CodePtr - ClassPtr);
		PrintOpcode();
	}

	dprintf("\n");

#endif
}

//****************************************
//		 Read the constant pool
//****************************************
#if 0
void DisassembleNew()
{
	uchar *LastCodePtr;
	uchar *CodeTop;

	int LastCodeOff;
	int MaxStack;
	int MaxLocal;
	int CodeLen;
	int n, b;
	int len;

	int label_num = 0;
	int goto_num = 0;

	CodePtr = ClassFilePtr;

	MaxStack = memWI(CodePtr);
	MaxLocal = memWI(CodePtr);
	CodeLen = memLI(CodePtr);

	// Mark the code start position
		
	SetCodeStart(CodePtr, CodeLen);

	CodeTop = CodePtr;

	// Local labels
	
	dprintf("Code Block (MaxStack=%d : MaxLocal=%d : CodeLen=%d)\n\n",MaxStack,MaxLocal,CodeLen);
	
	for (n=0;n<CodeLen;n++)
	{
		if (CodePtr >= CodeEnd)
			break;
		
		LastCodePtr = CodePtr;
		LastCodeOff = CODE_OFFSET();
		
		dprintf("%d :", CodePtr - ClassPtr);
		PrintOpcode();
		
		len = GetOpcodeLen(LastCodePtr);

		printf("\t\t");

		for (b=0;b<len;b++)
		{
			printf("%s ", Hex8(LastCodePtr[b]));
		}
		

		dprintf("\n");

	}

}
#endif
//****************************************
//		 
//****************************************
/*
void ReadHandlers()
{
	int n;
	int HandlerCount = ReadWord();

	vprintf("\nHandlerCount=%ld\n\n", HandlerCount);

	for (n=0;n<HandlerCount;n++)
	{
		int start_pc		= ReadWord();
		int end_pc			= ReadWord();
		int handler_pc		= ReadWord();
		int catch_type_index= ReadWord();
		
		vprintf("start_pc=%ld\n", start_pc);
		vprintf("end_pc=%ld\n", end_pc);
		vprintf("handler_pc=%ld\n", handler_pc);
		vprintf("catch_type_index=%ld\n\n", catch_type_index);
	}

}
*/
//****************************************
//
//****************************************
/*
void ReadAttributes()
{
	int n;
	int AttrCount = ReadWord();

	vprintf("AttrCount=%ld\n\n", AttrCount);

	for (n=0;n<AttrCount;n++)
	{
		int start_pc		= ReadWord();
		int end_pc			= ReadWord();
		int handler_pc		= ReadWord();
		int catch_type_index= ReadWord();
		
		printf("start_pc=%ld\n", start_pc);
		printf("end_pc=%ld\n", end_pc);
		printf("handler_pc=%ld\n", handler_pc);
		printf("catch_type_index=%ld\n\n", catch_type_index);
	}

}
*/
