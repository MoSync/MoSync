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
//				       Resource compiler Routines
// 						   Written by A.R.Hartley
//*********************************************************************************************

#include "compile.h"

#define infoprintf		if (g_INFO) printf

//****************************************
//
//****************************************

void ResourceMain()
{
	SetError_Mode(ErrorMode_Full);

	g_Section = SECT_data;
	g_pass_count = 0;

	g_ResType = 0;

	g_DEBUG = 0;
	g_LIST = 0;
	g_INFO = 0;
	
	AsmAllocMem();
	SetResPtrs();

	ResetResource();

	g_CurrentResource = 1;

	g_Pass = 1;
	g_pass_count++;

	ResourceComp();
	FinalizeResource();

//!	printf("Pass 1 - Size %d\n", (int) (ResPtr - ResMem));
	printf("Pass 1 - Size %d\n", g_ResIP);

	//*** Pass 2 ***

	g_Section = SECT_data;
	g_ResType = 0;

	g_DEBUG = 0;
	g_LIST = 0;
	g_INFO = 0;
	
	AsmAllocMem();
	SetResPtrs();

	ResetResource();

	g_CurrentResource = 1;

	g_Pass = 2;
	g_pass_count++;

	ResourceComp();

	FinalizeResource();

	WriteResources();

//	printf("Pass 2 - Size %d\n", (int) (ResPtr - ResMem));
	printf("Pass 2 - Size %d\n", g_ResIP);

	AsmDisposeMem();

	printf("Done...\n");
}

//****************************************
//			 Assemble code
//****************************************

short g_EndComp = 0;

void ResourceComp()
{
	int Line = 0;
	char *FPtr = g_FilePtr;				// Preserve file pointer

	g_EndComp = 0;

	ResetErrorCount();

	SetErrorReturn();

	while(1)
	{
		SkipWhiteSpace();

		Line++;
	
		if (g_EndComp)
			break;

		if (*g_FilePtr == 0)
			break;

		if (*g_FilePtr == '{' || *g_FilePtr == '}')
		{
			g_FilePtr++;
			continue;	
		}
	
		if (*g_FilePtr == 0)
			break;
	
/*		if (TestAsmLabel())
		{
			GetAsmName();
			NeedToken(":");
	
			MakeNewResource();
			continue;
		}
*/
		if (ResourceCommands())
		{
			SkipWhiteSpace();
			continue;
		}
	}

	g_FilePtr = FPtr;
	
	ExitErrorCount();
	SetErrorReset();
}


//****************************************
//		   Allocate Output Memory
//****************************************

void SetResPtrs()
{
	g_LocalScope = 0;		//1;
//!	ResPtr = ResMem;
	g_ResIP = 0;
}

//****************************************
//		   Allocate Output Memory
//****************************************

void SetResAsmPtrs()
{
	g_LocalScope = 0;

//!	DataPtr = DataMem;
//!	CodePtr = CodeMem;
	
	g_CodeIP = 0;
	g_DataIP = 0;
	g_BssIP = 0;
		
	g_CurrentFunction = 0;

	g_CurrentFile[0] = 0;
//	g_CurrentFileLine = 0;
}

//****************************************
//
//****************************************

void hex8(int v)
{
	char hextab[] = "0123456789abcdef";
	
	printf("%c%c ", hextab[v >> 4], hextab[v & 0xf]);
}

//****************************************
//
//****************************************

void WriteEncodedInt(unsigned int v)
{
	if (g_DEBUG) printf("EncInt: ");

	while(1)
	{
		if (v < 128)
			break;

		WriteByte(v & 0x7f);

		if (g_DEBUG) hex8(v & 0x7f);

		v >>= 7;
	}

	WriteByte(v | 0x80);
	if (g_DEBUG) hex8(v | 0x80);

	if (g_DEBUG) printf("\n");

	return;
}

//****************************************
//
//****************************************

void WriteEncodedSignedInt(int v)
{
	int sign = 0;
	
	if (v < 0)
	{
		sign = 1;
		v = -v;
	}

	// Shift up and use sign bit

	v <<= 1;

	// Move sign bit to bottom

	if (sign)
		v |= 1;

	WriteEncodedInt(v);
	return;
}

//****************************************
//
//****************************************

void ResetResource()
{
	// Initialize new resource

	//SetAsmPtrs();
	SetResAsmPtrs();

	g_CodeIP = 0;
	g_DataIP = 0;
	g_BssIP = 0;
	g_ResType = 0;
	g_ResDispose = 0;

	g_IndexCount = 0;			// Clear index table
	g_IndexWidth = 0;

	// Set name to null
	
	g_ResName[0] = 0;
}

//****************************************
//
//****************************************

short spr_ind, spr_x, spr_y, spr_w, spr_h, spr_cx, spr_cy;

short ResourceCommands()
{	
	char *filemem;
	int filelen;
	int n,v;

	SkipWhiteSpace();

//---------------------------------------------------------

	if (QToken(".library"))
	{
		SkipLine();
		return 1;
	}

//------------------------------------
//
//------------------------------------

	if (QToken(".lfile"))
	{
		SkipWhiteSpace();

		GetLFileName();
		
		GetRelPath(g_Name);
		return 1;
	}


	if (QToken(".localscope"))
	{
		SkipWhiteSpace();
		NeedToken("+");

//		g_LocalScope++;
		return 1;
	}


	if (QToken(".res"))
	{
		// If there is previous data to be written out
		// do it here !!

		if (g_ResType != 0)
		{
			FinalizeResource();
		}

		SkipWhiteSpace();

		g_ResName[0] = 0;
		g_Name[0] = 0;

		if (isalpha(*g_FilePtr))
		{
			GetName();						// Get the new type Name
			strcpy(g_ResName, g_Name);
		}
		

		// Clear index table
		
		g_IndexCount = 0;
		g_IndexWidth = 0;
		
		MakeNewResource(g_Name);				// Make this symbolic
		return 1;
	}

//------------------------------------
//
//------------------------------------

	if (QToken(".index"))
	{
		int IndexOffset = GetDataIndex();
		
		SkipWhiteSpace();

		g_Name[0] = 0;

		if (isalpha(*g_FilePtr))
			GetName();						// Get the index ref name

		// add index to table
		
		g_IndexTable[g_IndexCount] = IndexOffset;

//		MakeIndexedResource(IndexName, g_IndexCount);
		
		infoprintf("%d: index = %d ('%s')\n",g_IndexCount, IndexOffset, g_Name);

		if (g_Pass == 2)
			if (g_Name[0])
				fprintf(g_HeaderFile,"#define idx_%s %d\n", g_Name, g_IndexCount);

		g_IndexCount++;
		return 1;
	}

//------------------------------------
//
//------------------------------------

	if (QToken(".wideindex"))
	{
		g_IndexWidth = 1;
		SkipWhiteSpace();	
		return 1;
	}

//------------------------------------
//
//------------------------------------

	if (QToken(".extension"))
	{
		SkipWhiteSpace();
		
		v = GetExpression();		// x

		g_ResType = v;

		infoprintf("%d: Prototype = %d\n",g_CurrentResource, g_ResType);
		return 1;
	}

//------------------------------------
//
//------------------------------------

	if (QToken(".dispose"))
	{
		SkipWhiteSpace();
		
		g_ResDispose = 1;
		return 1;
	}

//------------------------------------
//
//------------------------------------

	if (QToken(".placeholder"))
	{
		g_ResType = ResType_PlaceHolder;

		infoprintf("%d: Placeholder\n",g_CurrentResource);
		return 1;
	}


//------------------------------------
//
//------------------------------------

	if (QToken(".skip"))
	{
		g_ResType = ResType_Skip;

		infoprintf("%d: Skip\n",g_CurrentResource);
		return 1;
	}


//------------------------------------
//
//------------------------------------

	if (QToken(".bin"))
	{
		g_ResType = ResType_Binary;

		infoprintf("%d: Binary\n",g_CurrentResource);
		return 1;
	}
	
//------------------------------------
//
//------------------------------------

	if (QToken(".ubin"))
	{
		g_ResType = ResType_UBinary;

		infoprintf("%d: UBinary\n",g_CurrentResource);
		return 1;
	}

//------------------------------------
//
//------------------------------------

	if (QToken(".media"))
	{	
		int slen;

		SkipWhiteSpace();

		g_ResType = ResType_Binary;			//ResType_Media;
		GetStringName(128);

		slen = strlen(g_Name) + 1;
		
		for (n=0;n<slen;n++)
			WriteByte(g_Name[n]);

		SkipWhiteSpace();
		NeedToken(",");
		SkipWhiteSpace();

		GetStringName(128);

		if(g_Do_Export_Dependencies && g_Pass == 2) {
			fprintf(g_DependFile, "\t%s \\\n", AddRelPrefix(g_Name));
		}

		filemem = Open_FileAlloc(AddRelPrefix(g_Name));
		
		if (!filemem)
		{
			Error(Error_Fatal, "Error reading data file '%s'", g_Name);
			return 1;
		}
		
		filelen = FileAlloc_Len();

		for (n=0;n<filelen;n++)
		{
			WriteByte(filemem[n]);
		}

		Free_File(filemem);

		infoprintf("%d: Media Binary\n",g_CurrentResource);
		return 1;
	}

//------------------------------------
//
//------------------------------------

	if (QToken(".umedia"))
	{	
		int slen;

		SkipWhiteSpace();

		g_ResType = ResType_UBinary; //ResType_UMedia;
		GetStringName(128);

		slen = strlen(g_Name) + 1;
		
		for (n=0;n<slen;n++)
			WriteByte(g_Name[n]);

		SkipWhiteSpace();
		NeedToken(",");
		SkipWhiteSpace();

		GetStringName(128);

		if(g_Do_Export_Dependencies && g_Pass == 2) {
			fprintf(g_DependFile, "\t%s \\\n", AddRelPrefix(g_Name));
		}

		filemem = Open_FileAlloc(AddRelPrefix(g_Name));
		
		if (!filemem)
		{
			Error(Error_Fatal, "Error reading data file '%s'", g_Name);
			return 1;
		}
		
		filelen = FileAlloc_Len();

		for (n=0;n<filelen;n++)
		{
			WriteByte(filemem[n]);
		}

		Free_File(filemem);

		infoprintf("%d: Media Binary\n",g_CurrentResource);
		return 1;
	}

//------------------------------------
//
//------------------------------------
/*
	if (QToken(".file"))
	{
		SkipWhiteSpace();

		g_ResType = ResType_File;

		GetStringName(128);

		filemem = Open_FileAlloc(AddRelPrefix(g_Name));
		
		if (!filemem)
		{
			Error("Error reading data file '%s'", g_Name);
			return 1;
		}
		
		filelen = FileAlloc_Len();

		for (n=0;n<filelen;n++)
		{
			WriteByte(filemem[n]);
		}

		Free_File(filemem);

		printf("%d: File '%s' size %d\n", g_CurrentResource, g_Name, filelen);
		return 1;
	}
*/

//------------------------------------
//
//------------------------------------

	if (QToken(".label"))			// filename
	{
		int slen;

		SkipWhiteSpace();

		g_ResType = ResType_Label;
		GetStringName(128);

		slen = strlen(g_Name) + 1;
		
		for (n=0;n<slen;n++)
			WriteByte(g_Name[n]);

		return 1;
	}


//------------------------------------
//
//------------------------------------

// ResType_TileSet = 7
// ushort xsize, ysize
// bytes png_image

	if (QToken(".tileset"))			// filename
	{
		int xsize,ysize;

		SkipWhiteSpace();

		g_ResType = ResType_TileSet;

		GetStringName(128);		

		SkipWhiteSpace();
		NeedToken(",");
		SkipWhiteSpace();

		xsize = GetExpression();		// xsize

		SkipWhiteSpace();
		NeedToken(",");
		SkipWhiteSpace();

		ysize = GetExpression();		// ysize

		WriteWord(xsize);
		WriteWord(ysize);

		if(g_Do_Export_Dependencies && g_Pass == 2) {
			fprintf(g_DependFile, "\t%s \\\n", AddRelPrefix(g_Name));
		}

		filemem = Open_FileAlloc(AddRelPrefix(g_Name));
		
		if (!filemem)
		{
			Error(Error_Fatal, "Error reading tileset file '%s'", g_Name);
			return 1;
		}

		filelen = FileAlloc_Len();

		for (n=0;n<filelen;n++)
			WriteByte(filemem[n]);

		Free_File(filemem);

		infoprintf("%d: Tileset '%s' cxy %d,%d size %d\n", g_CurrentResource, g_Name, xsize, ysize, filelen);
		return 1;
	}

//------------------------------------
//
//------------------------------------

// ResType_TileMap = 8
// ushort mapWidth, mapHeight
// ushort map[mapWidth*mapHeight]

	if (QToken(".tilemap"))			// filename
	{
		int xsize,ysize;

		SkipWhiteSpace();

		g_ResType = ResType_TileMap;

		GetStringName(128);		

		SkipWhiteSpace();
		NeedToken(",");
		SkipWhiteSpace();

		xsize = GetExpression();		// xsize

		SkipWhiteSpace();
		NeedToken(",");
		SkipWhiteSpace();

		ysize = GetExpression();		// ysize

		if(g_Do_Export_Dependencies && g_Pass == 2) {
			fprintf(g_DependFile, "\t%s \\\n", AddRelPrefix(g_Name));
		}

		filemem = Open_FileAlloc(AddRelPrefix(g_Name));
		
		if (!filemem)
		{
			Error(Error_Fatal, "Error reading tilemap file '%s'", g_Name);
			return 1;
		}

		filelen = FileAlloc_Len();

		if (filelen != (xsize * ysize * 2))
		{
			Error(Error_Fatal, "%d: Tilemap '%s' xy %d,%d size %d\n", g_CurrentResource, g_Name, xsize, ysize, filelen);
			return 1;
		}

		WriteWord(xsize);
		WriteWord(ysize);

		for (n=0;n<filelen;n++)
			WriteByte(filemem[n]);

		Free_File(filemem);

		infoprintf("%d: Tilemap '%s' cxy %d,%d size %d\n", g_CurrentResource, g_Name, xsize, ysize, filelen);
		return 1;


	}

//------------------------------------
//
//------------------------------------

	if (QToken(".image"))			// filename
	{
		SkipWhiteSpace();

		g_ResType = ResType_Image;

		GetStringName(128);		

/*
		spr_cx = spr_cy = 0;

		SkipWhiteSpace();

		if (QToken(","))
		{
			spr_cx = GetExpression();		// cx

			SkipWhiteSpace();
			NeedToken(",");
			SkipWhiteSpace();

			spr_cy = GetExpression();		// cy
		}

		WriteWord(spr_cx);
		WriteWord(spr_cy);
*/
		if(g_Do_Export_Dependencies && g_Pass == 2) {
			fprintf(g_DependFile, "\t%s \\\n", AddRelPrefix(g_Name));
		}

		filemem = Open_FileAlloc(AddRelPrefix(g_Name));
		
		if (!filemem)
		{
			Error(Error_Fatal, "Error reading image file '%s'", g_Name);
			return 1;
		}
		
		filelen = FileAlloc_Len();

		// write the length
		//WriteEncodedInt(filelen);

		for (n=0;n<filelen;n++)
			WriteByte(filemem[n]);

		Free_File(filemem);

		infoprintf("%d: Image '%s' cxy %d,%d size %d\n", g_CurrentResource, g_Name, spr_cx, spr_cy, filelen);
		return 1;
	}

//------------------------------------
//
//------------------------------------

	if (QToken(".sprite"))			// index, x,y,w,h,cx,cy
	{
		g_ResType = ResType_Sprite;

		SkipWhiteSpace();

		spr_ind = v = GetExpression();		// ind
		WriteWord(v);

		SkipWhiteSpace();
		NeedToken(",");
		SkipWhiteSpace();

		spr_x = v = GetExpression();		// x
		WriteWord(v);

		SkipWhiteSpace();
		NeedToken(",");
		SkipWhiteSpace();

		spr_y = v = GetExpression();		// y
		WriteWord(v);

		SkipWhiteSpace();
		NeedToken(",");
		SkipWhiteSpace();

		spr_w = v = GetExpression();		// w
		WriteWord(v);

		SkipWhiteSpace();
		NeedToken(",");
		SkipWhiteSpace();

		spr_h = v = GetExpression();		// h
		WriteWord(v);

		spr_cx = spr_cy = 0;

		SkipWhiteSpace();

		if (QToken(","))
		{
			spr_cx = (short)GetExpression();		// cx

			SkipWhiteSpace();
			NeedToken(",");
			SkipWhiteSpace();

			spr_cy = (short)GetExpression();		// cy
		}

		WriteWord(spr_cx);
		WriteWord(spr_cy);


		infoprintf("%d: Sprite ind %d, xy %d,%d, wh %d,%d cxy %d,%d\n",
			g_CurrentResource, spr_ind, spr_x, spr_y, spr_w, spr_h, spr_cx, spr_cy);

		return 1;
	}

//------------------------------------
//
//------------------------------------

	if (QToken(".end"))
	{
		SkipLine();
//		g_EndComp = 1;
		return 1;
	}

//------------------------------------
//
//------------------------------------

	if (QToken(".eof"))
	{
		SkipLine();
		g_EndComp = 1;
		return 1;
	}

//------------------------------------
//
//------------------------------------

	if (QToken(".parseheader"))
	{
		GetStringName(128);
		ReadAndParseHeaders(g_Name);
		return 1;
	}

//------------------------------------
// Other directives
//------------------------------------

	if (QToken(".set"))
	{
		if (QToken("$"))
		{
			char VarName[256];
			
			// Accept a string

			GetName();							// Get the new type Name
			strcpy(VarName, g_Name);
			NeedToken("=");
			GetStringName(128);

			printf("string set %s = '%s'\n", VarName, g_Name);
			
			RedefENumString(VarName, g_Name);
			return 1;
		}

		GetName();							// Get the new type Name	
		NeedToken("=");
		RedefENum(g_Name, GetExpression());
		return 1;
	}

//------------------------------------
//
//------------------------------------

	if (QToken(".debug"))
	{
		g_DEBUG = (short)GetExpression();
		return 1;
	}

//------------------------------------
//
//------------------------------------

	if (QToken(".info"))
	{
		g_INFO = (short)GetExpression();
		return 1;
	}

//------------------------------------
//
//------------------------------------

	if (QToken(".list"))
	{
		g_LIST = (short)GetExpression();
		return 1;
	}

//------------------------------------
//
//------------------------------------

	if (QToken(".show"))
	{
		int exp = GetExpression();
		
		printf("%d",exp);

		if (!Token(";"))
			printf("\n");
	
		return 1;
	}

//------------------------------------
//
//------------------------------------

	if (QToken(".print") || QToken(".?"))
	{
		int exp;
		char c;
	
		if (Token("\""))
		{
			while(1)	
			{
				c = *g_FilePtr;
				
				if (c == 0 || c == '"')
					break;
				
				printf("%c",c);

				g_FilePtr++;
			}

			NeedToken("\"");

			if (!Token(";"))
				printf("\n");

			return 1;
		}

		exp = GetExpression();

		printf("%d",exp);

		if (!Token(";"))
			printf("\n");
	
		return 1;
	}

//------------------------------------
//
//------------------------------------

	if (QToken(".if"))
	{
		if (!GetExpression())
		{
			if (NextToken("{"))
			{
				SkipPair('{','}');
			}
		}
		return 1;
	}

//------------------------------------
//
//------------------------------------

	if (QToken(".ifdef"))
	{
		GetName();							// Get the new type Name
		SkipWhiteSpace();
		
		if (!SymbolExists(g_Name, section_Script, -1))
		{
			if (NextToken("{"))
			{
				SkipPair('{','}');
			}
		}
		return 1;
	}


//------------------------------------
//
//------------------------------------

	if (QToken(".ifndef"))
	{
		GetName();							// Get the new type Name	
		SkipWhiteSpace();
		
		if (SymbolExists(g_Name, section_Script, -1))
		{
			if (NextToken("{"))
			{
				SkipPair('{','}');
			}
		}
		return 1;
	}

//------------------------------------
//
//------------------------------------

	if (QToken(".while"))
	{
	
		if (GetExpression())
		{
			g_FilePtr = SearchScope(g_FilePtr);
		}
		return 1;
	}

//------------------------------------
//
//------------------------------------

	if (QToken(".enum"))
	{
		AsmEnums();
		return 1;
	}

//------------------------------------
//
//------------------------------------

	if (QToken(".include"))			// filename
	{
		SkipWhiteSpace();

		GetStringName(128);		

		if(g_Do_Export_Dependencies && g_Pass == 2) {
			fprintf(g_DependFile, "\t%s \\\n", AddRelPrefix(g_Name));
		}

		filemem = Open_FileAlloc(AddRelPrefix(g_Name));
		
		if (!filemem)
		{
			Error(Error_Fatal, "Error reading include file '%s'", g_Name);
			return 1;
		}
		
		filelen = FileAlloc_Len();

		for (n=0;n<filelen;n++)
			WriteByte(filemem[n]);

		Free_File(filemem);

		infoprintf("bin include '%s' size %d\n", g_Name, filelen);
		return 1;
	}

//------------------------------------
//
//------------------------------------

	if (QToken(".varint"))
	{
		do
		{
			g_imm = GetExpression();					// Get the number
			WriteEncodedInt(g_imm);
		}
		while (QToken(","));

		return 1;
	}

//------------------------------------
//
//------------------------------------

	if (QToken(".varsint"))
	{
		do
		{
			g_imm = GetExpression();					// Get the number
			WriteEncodedSignedInt(g_imm);
		}
		while (QToken(","));

		return 1;

	}

//------------------------------------
//
//------------------------------------

	if (QToken(".byte"))
	{
		do
		{
			g_imm = GetExpression();					// Get the number
			WriteByte(g_imm);
		}
		while (QToken(","));

		return 1;

	}

//------------------------------------
//
//------------------------------------

	if (QToken(".half"))
	{
	
		do
		{
			g_imm = GetExpression();					// Get the number

			WriteWord(g_imm);
		}
		while (QToken(","));

		return 1;
	}

//------------------------------------
//
//------------------------------------

	if (QToken(".word"))
	{
		do
		{
			g_imm = GetExpression();				// Get the number

			WriteLong(g_imm);
		}
		while (QToken(","));
		return 1;
	}

//------------------------------------
//
//------------------------------------

	if (QToken(".fill"))
	{
		g_imm = GetExpression();					// Get the number
	
		if (!g_imm)
			return 1;
	
		NeedToken(",");
	
		v = GetExpression();
		
		do
		{
			WriteByte(v);
		}
		while(--g_imm);

		return 1;
	}


//------------------------------------
//
//------------------------------------

	if (QToken(".pstring"))
	{
//		char *dataStart;
		int dataStart;
		int len;

		// Write length byte

		WriteByte(0);

		// Get current data location
		
//!		dataStart = DataPtr;
//!		dataStart = (char *) ArrayPtr(&g_DataMemArray, DataIP);
		dataStart = g_DataIP;
		
		// read string to mem

		GetStrings();
		
		// find the string length
		
//!		len = DataPtr - dataStart;
		len = g_DataIP - dataStart;

		if (len > 255)
		{
			Error(Error_Skip, "pstring is longer that 255 chars\n");
			return 1;
		}
		
		// Patch the string length
		
//!		*(dataStart-1) = len;
		ArraySet(&g_DataMemArray, dataStart-1, len);

		
		return 1;
	}
	
//------------------------------------
//
//------------------------------------

	if (QToken(".cstring"))
	{
		GetStrings();
		WriteByte(0);
		
		return 1;
	}
	
//------------------------------------
//
//------------------------------------

	if (QToken(".string"))
	{
		GetStrings();		
		return 1;
	}

//------------------------------------
//
//------------------------------------

// Error


	GetAsmName();
	Error(Error_Fatal, "Illegal resource directive '%s'",g_Name);

	return 0;
}

//****************************************
//		  Create a new resource
// Name holds the res symbol name
//****************************************

void MakeNewResource(char *name)
{
	SYMBOL	*Sym;
	SYMBOL	sym;

	if (name[0] == 0)
		return;

	Sym = FindSymbolsOld(name,section_Resource,section_Resource);
	
	// Symbol found
	
	if (Sym)
	{
		if (g_Pass == 1)
			Error(Error_Skip, "Symbol '%s' is already defined",name);

		Sym->Value	= g_CurrentResource;
		return;
	}
	
	// Symbol not found
	
	if (g_Pass == 2)
	{
		Error(Error_Skip, "Strange error symbol '%s' defined in second pass",name);
		return;
	}

	sym.Section		= section_Resource;
	sym.Type		= 0;
	sym.Value		= g_CurrentResource;
	sym.LocalScope	= g_LocalScope;

	StoreSymbol(&sym,name);
}

//****************************************
//		  Create a new resource
// Name holds the res symbol name
//****************************************

void MakeIndexedResource(char *name, int index)
{
	SYMBOL	*Sym;
	SYMBOL	sym;

	if (name[0] == 0)
		return;

	Sym = FindSymbolsOld(name,section_Resource,section_Resource);
	
	if (Sym)
	{
		Error(Error_Skip, "Symbol '%s' is already defined",name);
		return;
	}

	sym.Section		= section_Resource;
	sym.Type		= 0;
	sym.Value		= index;

	StoreSymbol(&sym,name);
}

//****************************************
//
//****************************************

int GetDataIndex()
{
//	return	DataPtr - DataMem;
	return 	g_DataIP;
}

//****************************************
//
//****************************************

void FinalizeResource()
{
	int n;
	
//!	int DataLen = DataPtr - DataMem;
	int DataLen = g_DataIP;
	
//!	int ResStart = (int) ResPtr;
	int ResStart = g_ResIP;

	int ResLen, IndexSize;
	
	// Save the resource header

	g_Section = SECT_res;

//----------------------------------------
// 			   Write Type
//----------------------------------------

	if (g_ResDispose)
		WriteByte(g_ResType | 0x80);
	else
		WriteByte(g_ResType);

//----------------------------------------
//		 Write Indices if needed
//----------------------------------------

	if (g_IndexCount)
	{
		if (g_IndexWidth)
		{			
			// ** 4 byte index tables **

			// Write data size + index size
			
			IndexSize = 2 + (g_IndexCount*4);
			WriteEncodedInt(DataLen + IndexSize);

			// Write table size + indices

//!			*ResPtr++ = g_IndexCount & 0xff;
//!			*ResPtr++ = (g_IndexCount >> 8) & 0xff;

			WriteResByte(g_IndexCount & 0xff);
			WriteResByte((g_IndexCount >> 8) & 0xff);

			for (n=0;n<g_IndexCount;n++)
			{
//!				*ResPtr++ = g_IndexTable[n] & 0xff;
//!				*ResPtr++ = (g_IndexTable[n] >> 8) & 0xff;
//!				*ResPtr++ = (g_IndexTable[n] >> 16) & 0xff;
//!				*ResPtr++ = (g_IndexTable[n] >> 24) & 0xff;

				WriteResByte(g_IndexTable[n] & 0xff);
				WriteResByte((g_IndexTable[n] >> 8) & 0xff);
				WriteResByte((g_IndexTable[n] >> 16) & 0xff);
				WriteResByte((g_IndexTable[n] >> 24) & 0xff);
			}
		}
		else
		{			
			// ** 2 byte index tables **

			// Write data size + index size
			
			IndexSize = 2 + (g_IndexCount*2);
			WriteEncodedInt(DataLen + IndexSize);

//			printf("index table: entries %d total size %d\n", g_IndexCount, DataLen + IndexSize);

			// Write table size + indices

//!			*ResPtr++ = g_IndexCount & 0xff;
//!			*ResPtr++ = (g_IndexCount >> 8) & 0xff;

			WriteResByte(g_IndexCount & 0xff);
			WriteResByte((g_IndexCount >> 8) & 0xff);

			for (n=0;n<g_IndexCount;n++)
			{

//				printf("ind %d - offset %d\n", n, g_IndexTable[n]);

//!				*ResPtr++ = g_IndexTable[n] & 0xff;
//!				*ResPtr++ = (g_IndexTable[n] >> 8) & 0xff;

				WriteResByte(g_IndexTable[n] & 0xff);
				WriteResByte((g_IndexTable[n] >> 8) & 0xff);


			}
		}



	}

//----------------------------------------
//  Write resource size without indices
//----------------------------------------

	if (g_IndexCount == 0)
	{
		// Write noraml data size

		WriteEncodedInt(DataLen);
	}

//----------------------------------------
// 	  Copy Data to resource section
//----------------------------------------

	for (n=0;n<DataLen;n++)
	{
//!1	*ResPtr++ = DataMem[n];
//!2	*ResPtr++ = ArrayGet(&g_DataMemArray, n);

		WriteResByte(ArrayGet(&g_DataMemArray, n));
	}

//!	ResLen = (int) ResPtr - ResStart;
	ResLen = g_ResIP - ResStart;
	
	if (g_Pass == 2)
	{
//!		printf("Res %d Total %d", g_CurrentResource, ResPtr - ResStart);
		printf("Res %d Total %d", g_CurrentResource, g_ResIP - ResStart);

		if (g_ResDispose)
			printf(" (Auto-dispose)");

		printf("\n");
	}


	// Write to header file

	if (g_Pass == 2)
	{
		if (g_ResName[0])
			fprintf(g_HeaderFile,"#define %s %d\n", g_ResName, g_CurrentResource);
		else
			fprintf(g_HeaderFile,"//not defined %d\n", g_CurrentResource);
	}

	// Initialize new resource

	ResetResource();
	
	// Move to next resource

	g_CurrentResource++;

	g_Section = SECT_data;
}

//****************************************
//			Save resource data
//****************************************

void WriteResources()
{
//!	int ResLen = ResPtr - ResMem;
	int ResLen = g_ResIP;
	int res;

	int Entries = g_CurrentResource - 1;
	int DataLen;
	char nullbyte = 0;

	// Write the header
	
	//SetAsmPtrs();
	SetResAsmPtrs();

	g_DataIP = 0;
	g_Section = SECT_data;
	
	// Write magic
	
	WriteLong('SRAM');

	// Write total object entries
	
	WriteEncodedInt(Entries);

	// Write total data length
	
	WriteEncodedInt(ResLen);

	// Write the header info

//!	DataLen = DataPtr - DataMem;
	DataLen = g_DataIP;

	if (DataLen > 0)
//!		fwrite(DataMem , 1, DataLen , g_CodeFile);	// Save the data
		ArrayWriteFP(&g_DataMemArray, g_CodeFile, DataLen);

	// Write the resouce data
	
	if (ResLen > 0)
//!		fwrite(ResMem , 1, ResLen , g_CodeFile);		// Save the data
		ArrayWriteFP(&g_ResMemArray, g_CodeFile, ResLen);


	res = fwrite(&nullbyte , 1, 1 , g_CodeFile);			// Save a null
	if(res != 1) {
		Error(Error_Fatal, "Could not write resources");
		return;
	}
}

