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
//				       PIP-e II Assembler Routines
// 						   Written by A.R.Hartley
//			This code is based on the GameTasker v2.0 Module System
//*********************************************************************************************

#include "compile.h"

void WriteDataTypeArray(int type, int size)
{
	int info = (type << 24) | (size & 0x00ffffff);
	
	if 	((g_Section == SECT_res) || (g_Section == SECT_code))
		return;

	if 	(g_Section == SECT_data)
	{
		ArraySet(&g_DataTypeArray, g_DataIP, info);
		return;
	}

	if 	(g_Section == SECT_bss)
	{
		ArraySet(&g_DataTypeArray, g_DataIP + g_MaxDataIP, info);
		return;
	}

	return;
}

//***************************************
//
//***************************************

void WriteTypeAccessArray(int data_addr, int typebits)
{
	int v;
				
	v = ArrayGet(&g_DataMixArray, data_addr) | typebits;
	ArraySet(&g_DataMixArray, data_addr, v);
	return;
}

//***************************************
//				Obvious
//***************************************

void AlignDataSection()
{
	while(g_DataIP & 3)
	{
		ArraySet(&g_PaddingArray, g_DataIP, 1);

//!		*DataPtr++ = 0;
		ArraySet(&g_DataMemArray, g_DataIP, 0);

		g_DataIP++;

	}
}

//***************************************
//
//***************************************

void PadDataSection(int pads)
{
	int n;

	for (n=0;n<pads;n++)
	{
		ArraySet(&g_PaddingArray, g_DataIP++, 1);

//!		*DataPtr++ = 0;
		ArraySet(&g_DataMemArray, g_DataIP, 0);

		g_DataIP++;

	}
}

//***************************************
// Get the value of a byte from data mem
//***************************************

int GetDataMem(int addr)
{
	if (addr >= MAX_DATA_MEM)
		return 0;

//!	return DataMem[addr];
	return ArrayGet(&g_DataMemArray, addr);
		
}

//***************************************
// Get the value of a byte from data mem
//***************************************

int GetDataMemLong(int index)
{
	int max_addr = ((index + 1) * sizeof(int)) - 1;
	int *ptr;
	
	if (max_addr >= MAX_DATA_MEM)
		return 0;

//!	ptr =  ((int *) DataMem) + index;

	ptr = (int *) ArrayPtrBound(&g_DataMemArray, (index*4), (index*4)+4);
		
	return *ptr;
}

//****************************************
//	  Write a null alignment byte
//****************************************

void WriteAlignByte()
{
	if 	(g_Section == SECT_data)
	{
		if (g_LIST)
			printf("0x%-4x Byte %x : data\n",g_DataIP,0);

		ArraySet(&g_PaddingArray, g_DataIP, 1);

//!		*DataPtr++ = 0;
		ArraySet(&g_DataMemArray, g_DataIP, 0);

		g_DataIP += 1;
	}

	if 	(g_Section == SECT_code)
	{	

		Error(Error_Skip, "!!!!!! whoops -- code align 0x%x\n",g_CodeIP);

//!		if (g_LIST)
//!			printf("0x%-4lx Byte %lx : code\n",g_CodeIP,0);

//!		g_CodeIP += 1;
//!		*CodePtr++ = 0;
	}

	if 	(g_Section == SECT_bss)
		Error(Error_Skip, "Can't write predefined byte to IP = 0x%lx in '.bss'",g_BssIP);
	
}


//****************************************
// Write a byte to the current section
//****************************************

void WriteByte(int Value)
{
	if 	(g_Section == SECT_res)
	{
//!		*ResPtr++ = (char)Value;
		ArraySet(&g_ResMemArray, g_ResIP, (char) Value);
		g_ResIP++;
		return;
	}

	if 	(g_Section == SECT_data)
	{
		if (g_LIST)
			printf("0x%-4x Byte %x : data\n",g_DataIP,Value);

//!		*DataPtr++ = (char)Value;
		ArraySet(&g_DataMemArray, g_DataIP, (char) Value);

		g_DataIP+=1;
		return;
	}

	if 	(g_Section == SECT_code)
	{
//!		#if ALLOW_DATA_IN_CODE == 0
			Error(Error_Skip, "Can't write data to code section");
//!		#endif
	
//!		if (g_LIST)
//!			printf("0x%-4lx Byte %lx : code\n",g_CodeIP,Value);

//!		g_CodeIP += 1;
//!		*CodePtr++ = (char)Value;
		return;
	}


	if 	(g_Section == SECT_bss)
		Error(Error_Skip, "Can't write predefined byte to IP = 0x%lx in '.bss'",g_BssIP);
}

//****************************************
// Write a byte to the current section
//****************************************

void WriteByteQuiet(int Value)
{
/*	if 	(Section == SECT_ctor)
	{
		if (CtorIP >= MAX_CTOR_MEM)
		{
			Error(Error_Fatal, "Too mach data in ctor section");
			return;
		}

		*CtorPtr++ = (char)Value;
		CtorIP++;
		return;
	}
*/

/*	if 	(Section == SECT_dtor)
	{
		if (DtorIP >= MAX_CTOR_MEM)
		{
			Error(Error_Fatal, "Too mach data in dtor section");
			return;
		}

		*DtorPtr++ = (char)Value;
		DtorIP++;
		return;
	}
*/
	if 	(g_Section == SECT_res)
	{
//!		*ResPtr++ = (char)Value;

		ArraySet(&g_ResMemArray, g_ResIP, (char) Value);
		g_ResIP += 1;

		return;
	}

	if 	(g_Section == SECT_data)
	{
//!		*DataPtr++ = (char)Value;
		ArraySet(&g_DataMemArray, g_DataIP, (char) Value);
		g_DataIP += 1;
		return;
	}

	if 	(g_Section == SECT_code)
	{

//!		#if ALLOW_DATA_IN_CODE == 0
			Error(Error_Skip, "Can't write data to code section");
//!		#endif

//!		g_CodeIP += 1;
//!		*CodePtr++ = (char)Value;
	}

}

//****************************************
//		Write byte to resource section
//****************************************

void WriteResByte(int Value)
{
	ArraySet(&g_ResMemArray, g_ResIP, (char) Value);
	g_ResIP += 1;
	return;
}

//****************************************
// Write byte to the current section
//****************************************

void WriteByteSpace(int Value)
{
/*	if 	((Section == SECT_ctor) || (Section == SECT_dtor))
	{
		Error(Error_Skip, "Can't write space to ctor/dtor section");
		return;
	}
*/
	if 	(g_Section == SECT_data)
	{
//!		*DataPtr++ = (char)Value;
		ArraySet(&g_DataMemArray, g_DataIP, (char) Value);

		g_DataIP += 1;
		return;
	}

	if 	(g_Section == SECT_code)
	{
//!		#if ALLOW_DATA_IN_CODE == 0
			Error(Error_Skip, "Can't write data to code section");
//!		#endif

//!		g_CodeIP += 1;
//!		*CodePtr++ = (char)Value;
//!		return;
	}

	if 	(g_Section == SECT_bss)
	{
		g_BssIP++;
		return;
	}
}

//****************************************
//
//****************************************

void WriteWord(int Value)
{
/*	if 	((Section == SECT_ctor) || (Section == SECT_dtor))
	{
		Warning("writing .word to ctor/dtor section\n");

		WriteByteQuiet(Value & 0xff);
		WriteByteQuiet((Value >> 8) & 0xff);
		return;
	}
*/
	if 	(g_Section == SECT_res)
	{
		WriteByteQuiet(Value & 0xff);
		WriteByteQuiet((Value >> 8) & 0xff);
		return;
	}

	if 	(g_Section == SECT_data)
	{
		if (g_LIST)
			printf("0x%-4x Word %x : data\n",g_DataIP,Value);

//		g_DataIP += 2;
//		*((short *) DataPtr)++ = Value;

		WriteByteQuiet(Value & 0xff);
		WriteByteQuiet((Value >> 8) & 0xff);

		return;
	}

	if 	(g_Section == SECT_code)
	{
		#if ALLOW_DATA_IN_CODE == 0
			Error(Error_Skip, "Can't write data to code section");
		#endif

		if (g_LIST)
			printf("0x%-4x Word %x : code\n",g_CodeIP,Value);

//		g_CodeIP += 2;
//		*((short *) CodePtr)++ = Value;

		WriteByteQuiet(Value & 0xff);
		WriteByteQuiet((Value >> 8) & 0xff);
		return;
	}


	if 	(g_Section == SECT_bss)
		Error(Error_Skip, "Can't write predefined word to IP = 0x%lx in '.bss'",g_BssIP);

}

//****************************************
//
//****************************************

void WriteLongRef(int Value, SYMBOL *ref)
{
	if 	(g_Section != SECT_data)
		Error(Error_Skip, "illegal longref outside data section");

	if (ref)
		ArraySet(&g_DataArray, g_DataIP, (int) ref);

	WriteLong(Value);
}

//****************************************
//
//****************************************

void AddAlignRef(int immVal)
{
	int imm_copy = immVal;
	
	if 	(g_Section != SECT_data)
		Error(Error_Skip, "illegal alignref outside data section");

//	ArraySet(&DataAlignArray, g_DataIP, imm);

	if (immVal == 2)
		immVal = 1;
	else if (immVal == 4)
		immVal = 3;
	else if (immVal == 8)
		immVal = 7;
	else
		return;						// bad align so return
					
	while(GetSectIP() & immVal)
	{
		WriteAlignByte();
	}

	ArraySet(&g_DataAlignArray, g_DataIP, imm_copy);

}

//****************************************
//
//****************************************

void WriteLong(int Value)
{
/*	if 	((Section == SECT_ctor) || (Section == SECT_dtor))
	{
		WriteByteQuiet(Value & 0xff);
		WriteByteQuiet((Value >> 8) & 0xff);
		WriteByteQuiet((Value >> 16) & 0xff);
		WriteByteQuiet((Value >> 24) & 0xff);
		return;
	}
*/
	if 	(g_Section == SECT_res)
	{
		WriteByteQuiet(Value & 0xff);
		WriteByteQuiet((Value >> 8) & 0xff);
		WriteByteQuiet((Value >> 16) & 0xff);
		WriteByteQuiet((Value >> 24) & 0xff);
		return;
	}

	if 	(g_Section == SECT_data)
	{
		if (g_LIST)
			printf("0x%-4x Long %x \n",g_DataIP,Value);

		WriteByteQuiet(Value & 0xff);
		WriteByteQuiet((Value >> 8) & 0xff);
		WriteByteQuiet((Value >> 16) & 0xff);
		WriteByteQuiet((Value >> 24) & 0xff);
		return;
	}

	if 	(g_Section == SECT_code)
	{
		#if ALLOW_DATA_IN_CODE == 0
			Error(Error_Skip, "Can't write data to code section");
		#endif

		if (g_LIST)
			printf("0x%-4x Word %x : code\n",g_CodeIP,Value);
		
		WriteByteQuiet(Value & 0xff);
		WriteByteQuiet((Value >> 8) & 0xff);
		WriteByteQuiet((Value >> 16) & 0xff);
		WriteByteQuiet((Value >> 24) & 0xff);
		return;
	}


	if 	(g_Section == SECT_bss)
		Error(Error_Skip, "Can't write predefined int to IP = 0x%lx in '.bss'",g_BssIP);

}

//****************************************
// Store a int word in little endian
//****************************************

void PutLong(char *addr,int Value)
{
	*addr++ = (char)(Value & 0xff);
	*addr++ = (char)((Value >> 8) & 0xff);
	*addr++ = (char)((Value >> 16) & 0xff);
	*addr   = (char)((Value >> 24) & 0xff);
}

//****************************************
//		Get Current sections IP
//****************************************

int GetSectIP()
{
	if 	(g_Section == SECT_data)
		return g_DataIP;

	if 	(g_Section == SECT_bss)
		return g_BssIP;

	if 	(g_Section == SECT_code)
		return g_CodeIP;

	return SECT_null;
}

//****************************************
//		Get Current sections IP
//****************************************

char SectName[16];

char * GetSectName(short theSection)
{
	if 	(theSection == SECT_data)
		strcpy(SectName,".data");

	if 	(theSection == SECT_bss)
		strcpy(SectName,".bss");

	if 	(theSection == SECT_code)
		strcpy(SectName,".code");

	return SectName;
}

//****************************************
//		Write out object file
//****************************************

void WriteModule()
{
	MA_HEAD	Head;
	SYMBOL *ep;
	
	int DataLen = g_DataIP;
	int CodeLen = g_CodeIP;
	int BssLen  = g_BssIP;
	int v;
	int mem_needed;
	int res;

	if (g_Default_StackSize < 32)
	{
		printf("*Warning* stack size very small !!\n");
	}
	
	if (g_Default_DataSize == 0)
	{
			printf("Info: data section auto expands\n");
	}
	else
	{
		// !! compute memory overflow problems here !!
		// Test for datasection overflow
		
		if (BssLen >= g_Default_DataSize)
			printf("*Warning* BSS section exceeds chosen data section size\n");

		// test for underflow
		
		mem_needed = g_Default_StackSize + g_Default_HeapSize + g_DataIP + g_BssIP + 16;

		v = g_Default_DataSize - mem_needed;

		if (v < 1)
		{
/*			printf("\n");
			printf("DataIP %d\n", g_DataIP);
			printf("CodeIP %d\n", g_CodeIP);
			printf("BssIP %d\n", g_BssIP);
			printf("Default_StackSize %d\n", g_Default_StackSize);
			printf("Default_HeapSize %d\n", g_Default_HeapSize);
			printf("Default_DataSize %d\n", g_Default_DataSize);
			printf("\n");
*/
			// Auto setting
			g_Default_DataSize = mem_needed + 1;

			while(g_Default_DataSize & 3)
				g_Default_DataSize++;

			printf("Runtime Warning: data section is too small for the stack, heap & BSS\n");
			printf("(changed to -datasize=%d)\n", g_Default_DataSize);

		}
	}
	
	Head.Magic		= 'XDAM'; 							//'MADX' Mobile Author Dynamic eXecutable
	Head.CodeLen	= CodeLen;
	Head.DataLen	= DataLen;
	Head.DataSize	= g_Default_DataSize;
	Head.StackSize	= g_Default_StackSize;
	Head.HeapSize	= g_Default_HeapSize;

	Head.AppCode	= g_Default_AppCode;
	Head.AppID		= g_Default_AppID;

	Head.IntLen		= g_VarCount;

	Head.EntryPoint	= 0;

	ep	= GetGlobalSym(g_Code_EntryPoint);

	if (ep)
		Head.EntryPoint	= ep->Value;
	else
		printf("Warning: Code Entrypoint '%s' not found (defaulting to 0)\n", g_Code_EntryPoint);

	// Write Headers

	res = fwrite(&Head, 1, sizeof(MA_HEAD), g_CodeFile);		// Save the header
	if(res != sizeof(MA_HEAD))
		Error(Error_Fatal, "Could not write code file");

	// Write code

	if (CodeLen > 0)
//!		fwrite(CodeMem , 1, CodeLen , g_CodeFile);		// Save the code
		ArrayWriteFP(&g_CodeMemArray, g_CodeFile, CodeLen);

	// Write data

	if (DataLen > 0)
//!		fwrite(DataMem , 1, DataLen , g_CodeFile);		// Save the data
		ArrayWriteFP(&g_DataMemArray, g_CodeFile, DataLen);

	// Write int pool

	if (g_VarCount > 0) {
		res = fwrite(g_VarPool, 1, g_VarCount * 4, g_CodeFile);		// Int pool
		if(res != g_VarCount * 4)
			Error(Error_Fatal, "Could not write code file");
	}

    if (g_INFO)
	{
		printf("DataMem = %d\n",DataLen	);
		printf("BssMem  = %d\n",BssLen	);
		printf("CodeMem = %d\n",CodeLen	);
		printf("IntMem  = %d\n",g_VarCount * 4 );
	}
}

