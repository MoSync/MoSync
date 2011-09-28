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
//*********************************************************************************************

#include "compile.h"

void WriteDataTypeArray(int type, int size)
{
	int info = (type << 24) | (size & 0x00ffffff);

	if 	((Section == SECT_res) || (Section == SECT_code))
		return;

	if 	(Section == SECT_data)
	{
		ArraySet(&DataTypeArray, DataIP, info);
		return;
	}

	if 	(Section == SECT_bss)
	{
		ArraySet(&DataTypeArray, DataIP + MaxDataIP, info);
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

	v = ArrayGet(&DataMixArray, data_addr) | typebits;
	ArraySet(&DataMixArray, data_addr, v);
	return;
}

//***************************************
//				Obvious
//***************************************

void AlignDataSection()
{
	while(DataIP & 3)
	{
		ArraySet(&PaddingArray, DataIP, 1);
		ArraySet(&DataMemArray, DataIP, 0);
		DataIP++;
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
		ArraySet(&PaddingArray, DataIP++, 1);
		ArraySet(&DataMemArray, DataIP, 0);
		DataIP++;
	}
}

//***************************************
// Get the value of a byte from data mem
//***************************************

int GetDataMem(int addr)
{
	if (addr >= MAX_DATA_MEM)
		return 0;

	return ArrayGet(&DataMemArray, addr);
}

//***************************************
// Get the value of a byte from code mem
//***************************************

int GetCodeMem(int addr)
{
	if (addr >= MAX_CODE_MEM)
		return 0;

	return ArrayGet(&CodeMemArray, addr);
}

//***************************************
// Get the value of a int from data mem
//***************************************

int GetDataMemLong(int index)
{
	int max_addr = ((index + 1) * sizeof(int)) - 1;
	int *ptr;

	if (max_addr >= MAX_DATA_MEM)
		return 0;

	ptr = (int *) ArrayPtrBound(&DataMemArray, (index*4), (index*4)+4);

	return *ptr;
}

//****************************************
//	  Write a null alignment byte
//****************************************

void WriteAlignByte()
{
	if 	(Section == SECT_data)
	{
		if (LIST)
			printf("0x%-4x Byte %x : data\n",DataIP,0);

		ArraySet(&PaddingArray, DataIP, 1);
		ArraySet(&DataMemArray, DataIP, 0);
		DataIP += 1;
	}

	if 	(Section == SECT_code)
	{
		Error(Error_Skip, "!!!!!! whoops -- code align 0x%x\n",CodeIP);
	}

	if 	(Section == SECT_bss)
		Error(Error_Skip, "Can't write predefined byte to IP = 0x%lx in '.bss'",BssIP);
}

//****************************************
// Write a byte to the current section
//****************************************

void WriteByte(int Value)
{
	if 	(Section == SECT_res)
	{
		ArraySet(&ResMemArray, ResIP, (char) Value);
		ResIP++;
		return;
	}

	if 	(Section == SECT_data)
	{
		if (LIST)
			printf("0x%-4x Byte %x : data\n",DataIP,Value);

		ArraySet(&DataMemArray, DataIP, (char) Value);
		DataIP+=1;
		return;
	}

	if 	(Section == SECT_code)
	{
		Error(Error_Skip, "Can't write data to code section");
	}


	if 	(Section == SECT_bss)
		Error(Error_Skip, "Can't write predefined byte to IP = 0x%lx in '.bss'",BssIP);
}

//****************************************
// Write a byte to the current section
//****************************************

void WriteByteQuiet(int Value)
{
	if 	(Section == SECT_res)
	{
		ArraySet(&ResMemArray, ResIP, (char) Value);
		ResIP += 1;
		return;
	}

	if 	(Section == SECT_data)
	{
		ArraySet(&DataMemArray, DataIP, (char) Value);
		DataIP += 1;
		return;
	}

	if 	(Section == SECT_code)
	{
		Error(Error_Skip, "Can't write data to code section");
	}

}

//****************************************
//		Write byte to resource section
//****************************************

void WriteResByte(int Value)
{
	ArraySet(&ResMemArray, ResIP, (char) Value);
	ResIP += 1;

	return;
}

//****************************************
// Write byte to the current section
//****************************************

void WriteByteSpace(int Value)
{
	if 	(Section == SECT_data)
	{
		ArraySet(&DataMemArray, DataIP, (char) Value);
		DataIP += 1;
		return;
	}

	if 	(Section == SECT_code)
	{
		Error(Error_Skip, "Can't write data to code section");
	}

	if 	(Section == SECT_bss)
	{
		BssIP++;
		return;
	}
}

//****************************************
//
//****************************************

void WriteWord(int Value)
{
	if 	(Section == SECT_res)
	{
		WriteByteQuiet(Value & 0xff);
		WriteByteQuiet((Value >> 8) & 0xff);
		return;
	}

	if 	(Section == SECT_data)
	{
		if (LIST)
			printf("0x%-4x Word %x : data\n",DataIP,Value);

		WriteByteQuiet(Value & 0xff);
		WriteByteQuiet((Value >> 8) & 0xff);
		return;
	}

	if 	(Section == SECT_code)
	{
		#if ALLOW_DATA_IN_CODE == 0
			Error(Error_Skip, "Can't write data to code section");
		#endif

		if (LIST)
			printf("0x%-4x Word %x : code\n",CodeIP,Value);

		WriteByteQuiet(Value & 0xff);
		WriteByteQuiet((Value >> 8) & 0xff);
		return;
	}

	if 	(Section == SECT_bss)
		Error(Error_Skip, "Can't write predefined word to IP = 0x%lx in '.bss'",BssIP);
}

//****************************************
//
//****************************************

void WriteLongRef(int Value, SYMBOL *ref)
{
	if 	(Section != SECT_data)
		Error(Error_Skip, "illegal longref outside data section");

	if (ref)
		ArraySet(&DataArray, DataIP, (int) ref);

	WriteLong(Value);
}

//****************************************
//
//****************************************

void AddAlignRef(int immVal)
{
	int imm_copy = immVal;

	if 	(Section != SECT_data)
		Error(Error_Skip, "illegal alignref outside data section");

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

	ArraySet(&DataAlignArray, DataIP, imm_copy);
}

//****************************************
//
//****************************************

void WriteLong(int Value)
{
	if 	(Section == SECT_res)
	{
		WriteByteQuiet(Value & 0xff);
		WriteByteQuiet((Value >> 8) & 0xff);
		WriteByteQuiet((Value >> 16) & 0xff);
		WriteByteQuiet((Value >> 24) & 0xff);
		return;
	}

	if 	(Section == SECT_data)
	{
		if (LIST)
			printf("0x%-4x Long %x \n",DataIP,Value);

		WriteByteQuiet(Value & 0xff);
		WriteByteQuiet((Value >> 8) & 0xff);
		WriteByteQuiet((Value >> 16) & 0xff);
		WriteByteQuiet((Value >> 24) & 0xff);
		return;
	}

	if 	(Section == SECT_code)
	{
		#if ALLOW_DATA_IN_CODE == 0
			Error(Error_Skip, "Can't write data to code section");
		#endif

		if (LIST)
			printf("0x%-4x Word %x : code\n",CodeIP,Value);

		WriteByteQuiet(Value & 0xff);
		WriteByteQuiet((Value >> 8) & 0xff);
		WriteByteQuiet((Value >> 16) & 0xff);
		WriteByteQuiet((Value >> 24) & 0xff);
		return;
	}

	if 	(Section == SECT_bss)
		Error(Error_Skip, "Can't write predefined int to IP = 0x%lx in '.bss'",BssIP);
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
	if 	(Section == SECT_data)
		return DataIP;

	if 	(Section == SECT_bss)
		return BssIP;

	if 	(Section == SECT_code)
		return CodeIP;

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

	int DataLen = DataIP;
	int CodeLen = CodeIP;
	int BssLen  = BssIP;
	int v;
	int mem_needed;
	int res;

	if (Default_StackSize < 32)
	{
		printf("*Warning* stack size very small !!\n");
	}

	if (Default_DataSize == 0)
	{
			printf("Info: data section auto expands\n");
	}
	else
	{
		// !! compute memory overflow problems here !!
		// Test for datasection overflow

		if (BssLen >= Default_DataSize)
			printf("*Warning* BSS section exceeds chosen data section size\n");

		// test for underflow

		mem_needed = Default_StackSize + Default_HeapSize + DataIP + BssIP + 16;

		v = Default_DataSize - mem_needed;

		if (v < 1)
		{
#if 0 		// Test Code (Don't delete)
			printf("\n");
			printf("DataIP %d\n", DataIP);
			printf("CodeIP %d\n", CodeIP);
			printf("BssIP %d\n", BssIP);
			printf("Default_StackSize %d\n", Default_StackSize);
			printf("Default_HeapSize %d\n", Default_HeapSize);
			printf("Default_DataSize %d\n", Default_DataSize);
			printf("\n");
#endif
			// Auto setting
			Default_DataSize = mem_needed + 1;

			while(Default_DataSize & 3)
				Default_DataSize++;

			printf("Runtime Warning: data section is too small for the stack, heap & BSS\n");
			//printf("(changed to -datasize=%d)\n", Default_DataSize);
			Error(Error_Fatal, "Data section is too small!");
		}
	}

	Head.Magic		= 'XDAM'; 							//'MADX' Mobile Author Dynamic eXecutable
	Head.CodeLen	= CodeLen;
	Head.DataLen	= DataLen;
	Head.DataSize	= Default_DataSize;
	Head.StackSize	= Default_StackSize;
	Head.HeapSize	= Default_HeapSize;

	Head.AppCode	= Default_AppCode;
	Head.AppID		= Default_AppID;

	Head.IntLen		= VarCount;

	Head.EntryPoint	= 0;

	ep	= GetGlobalSym(Code_EntryPoint);

	if (ep)
		Head.EntryPoint	= ep->Value;
	else
		printf("Warning: Code Entrypoint '%s' not found (defaulting to 0)\n", Code_EntryPoint);

	// Write Headers

	res = fwrite(&Head, 1, sizeof(MA_HEAD), CodeFile);		// Save the header
	if(res != sizeof(MA_HEAD))
		Error(Error_Fatal, "Could not write code file");

	// Write code

	if (CodeLen > 0)
		ArrayWriteFP(&CodeMemArray, CodeFile, CodeLen);

	// Write data

	if (DataLen > 0)
		ArrayWriteFP(&DataMemArray, CodeFile, DataLen);

	// Write int pool

	if (VarCount > 0) {
		res = fwrite(VarPool, 1, VarCount * 4, CodeFile);		// Int pool
		if(res != VarCount * 4)
			Error(Error_Fatal, "Could not write code file");
	}

    if (INFO)
	{
		printf("DataMem = %d\n",DataLen	);
		printf("BssMem  = %d\n",BssLen	);
		printf("CodeMem = %d\n",CodeLen	);
		printf("IntMem  = %d\n",VarCount * 4 );
	}
}

