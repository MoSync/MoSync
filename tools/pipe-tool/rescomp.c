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

#define infoprintf		if (INFO) printf

//****************************************
//
//****************************************

void ResourceMain()
{
	SetError_Mode(ErrorMode_Full);

	Section = SECT_data;
	pass_count = 0;

	ResType = 0;

	DEBUG = 0;
	LIST = 0;
	INFO = 0;
	
	AsmAllocMem();
	SetResPtrs();

	ResetResource();

	CurrentResource = 1;

	Pass = 1;
	pass_count++;

	ResourceComp();
	FinalizeResource();

//!	printf("Pass 1 - Size %d\n", (int) (ResPtr - ResMem));
	printf("Pass 1 - Size %d\n", ResIP);

	//*** Pass 2 ***

	Section = SECT_data;
	ResType = 0;

	DEBUG = 0;
	LIST = 0;
	INFO = 0;
	
	AsmAllocMem();
	SetResPtrs();

	ResetResource();

	CurrentResource = 1;

	Pass = 2;
	pass_count++;

	ResourceComp();

	FinalizeResource();

	WriteResources();

//	printf("Pass 2 - Size %d\n", (int) (ResPtr - ResMem));
	printf("Pass 2 - Size %d\n", ResIP);

	AsmDisposeMem();

	printf("Done...\n");
}

//****************************************
//			 Assemble code
//****************************************

short EndComp = 0;

void ResourceComp()
{
	int Line = 0;
	char *FPtr = FilePtr;				// Preserve file pointer

	EndComp = 0;

	ResetErrorCount();

	SetErrorReturn();

	while(1)
	{
		Line++;
	
		if (EndComp)
			break;

		if (*FilePtr == 0)
			break;

		if (*FilePtr == '{' || *FilePtr == '}')
		{
			FilePtr++;
			continue;	
		}
	
		if (*FilePtr == 0)
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
			continue;
	}

	FilePtr = FPtr;
	
	ExitErrorCount();
	SetErrorReset();
}


//****************************************
//		   Allocate Output Memory
//****************************************

void SetResPtrs()
{
	LocalScope = 0;		//1;
//!	ResPtr = ResMem;
	ResIP = 0;
}

//****************************************
//		   Allocate Output Memory
//****************************************

void SetResAsmPtrs()
{
	LocalScope = 0;

//!	DataPtr = DataMem;
//!	CodePtr = CodeMem;
	
	CodeIP = 0;
	DataIP = 0;
	BssIP = 0;
		
	CurrentFunction = 0;

	CurrentFile[0] = 0;
//	CurrentFileLine = 0;
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
	if (DEBUG) printf("EncInt: ");

	while(1)
	{
		if (v < 128)
			break;

		WriteByte(v & 0x7f);

		if (DEBUG) hex8(v & 0x7f);

		v >>= 7;
	}

	WriteByte(v | 0x80);
	if (DEBUG) hex8(v | 0x80);

	if (DEBUG) printf("\n");

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

	CodeIP = 0;
	DataIP = 0;
	BssIP = 0;
	ResType = 0;
	ResDispose = 0;

	IndexCount = 0;			// Clear index table
	IndexWidth = 0;

	// Set name to null
	
	ResName[0] = 0;
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
		
		GetRelPath(Name);
		return 1;
	}


	if (QToken(".localscope"))
	{
		SkipWhiteSpace();
		NeedToken("+");

//		LocalScope++;
		return 1;
	}


	if (QToken(".res"))
	{
		// If there is previous data to be written out
		// do it here !!

		if (ResType != 0)
		{
			FinalizeResource();
		}

		SkipWhiteSpace();

		ResName[0] = 0;
		Name[0] = 0;

		if (isalpha(*FilePtr))
		{
			GetName();						// Get the new type Name
			strcpy(ResName, Name);
		}
		

		// Clear index table
		
		IndexCount = 0;
		IndexWidth = 0;
		
		MakeNewResource(Name);				// Make this symbolic
		return 1;
	}

//------------------------------------
//
//------------------------------------

	if (QToken(".index"))
	{
		int IndexOffset = GetDataIndex();
		
		SkipWhiteSpace();

		Name[0] = 0;

		if (isalpha(*FilePtr))
			GetName();						// Get the index ref name

		// add index to table
		
		IndexTable[IndexCount] = IndexOffset;

//		MakeIndexedResource(IndexName, IndexCount);
		
		infoprintf("%d: index = %d ('%s')\n",IndexCount, IndexOffset, Name);

		if (Pass == 2)
			if (Name[0])
				fprintf(HeaderFile,"#define idx_%s %d\n", Name, IndexCount);

		IndexCount++;
		return 1;
	}

//------------------------------------
//
//------------------------------------

	if (QToken(".wideindex"))
	{
		IndexWidth = 1;
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

		ResType = v;

		infoprintf("%d: Prototype = %d\n",CurrentResource, ResType);
		return 1;
	}

//------------------------------------
//
//------------------------------------

	if (QToken(".dispose"))
	{
		SkipWhiteSpace();
		
		ResDispose = 1;
		return 1;
	}

//------------------------------------
//
//------------------------------------

	if (QToken(".placeholder"))
	{
		ResType = ResType_PlaceHolder;

		infoprintf("%d: Placeholder\n",CurrentResource);
		return 1;
	}


//------------------------------------
//
//------------------------------------

	if (QToken(".skip"))
	{
		ResType = ResType_Skip;

		infoprintf("%d: Skip\n",CurrentResource);
		return 1;
	}


//------------------------------------
//
//------------------------------------

	if (QToken(".bin"))
	{
		ResType = ResType_Binary;

		infoprintf("%d: Binary\n",CurrentResource);
		return 1;
	}
	
//------------------------------------
//
//------------------------------------

	if (QToken(".ubin"))
	{
		ResType = ResType_UBinary;

		infoprintf("%d: UBinary\n",CurrentResource);
		return 1;
	}

//------------------------------------
//
//------------------------------------

	if (QToken(".media"))
	{	
		int slen;

		SkipWhiteSpace();

		ResType = ResType_Binary;			//ResType_Media;
		GetStringName(128);

		slen = strlen(Name) + 1;
		
		for (n=0;n<slen;n++)
			WriteByte(Name[n]);

		SkipWhiteSpace();
		NeedToken(",");
		SkipWhiteSpace();

		GetStringName(128);

		if(Do_Export_Dependencies && Pass == 2) {
			fprintf(DependFile, "\t%s \\\n", AddRelPrefix(Name));
		}

		filemem = Open_FileAlloc(AddRelPrefix(Name));
		
		if (!filemem)
		{
			Error(Error_Fatal, "Error reading data file '%s'", Name);
			return 1;
		}
		
		filelen = FileAlloc_Len();

		for (n=0;n<filelen;n++)
		{
			WriteByte(filemem[n]);
		}

		Free_File(filemem);

		infoprintf("%d: Media Binary\n",CurrentResource);
		return 1;
	}

//------------------------------------
//
//------------------------------------

	if (QToken(".umedia"))
	{	
		int slen;

		SkipWhiteSpace();

		ResType = ResType_UBinary; //ResType_UMedia;
		GetStringName(128);

		slen = strlen(Name) + 1;
		
		for (n=0;n<slen;n++)
			WriteByte(Name[n]);

		SkipWhiteSpace();
		NeedToken(",");
		SkipWhiteSpace();

		GetStringName(128);

		if(Do_Export_Dependencies && Pass == 2) {
			fprintf(DependFile, "\t%s \\\n", AddRelPrefix(Name));
		}

		filemem = Open_FileAlloc(AddRelPrefix(Name));
		
		if (!filemem)
		{
			Error(Error_Fatal, "Error reading data file '%s'", Name);
			return 1;
		}
		
		filelen = FileAlloc_Len();

		for (n=0;n<filelen;n++)
		{
			WriteByte(filemem[n]);
		}

		Free_File(filemem);

		infoprintf("%d: Media Binary\n",CurrentResource);
		return 1;
	}

//------------------------------------
//
//------------------------------------
/*
	if (QToken(".file"))
	{
		SkipWhiteSpace();

		ResType = ResType_File;

		GetStringName(128);

		filemem = Open_FileAlloc(AddRelPrefix(Name));
		
		if (!filemem)
		{
			Error("Error reading data file '%s'", Name);
			return 1;
		}
		
		filelen = FileAlloc_Len();

		for (n=0;n<filelen;n++)
		{
			WriteByte(filemem[n]);
		}

		Free_File(filemem);

		printf("%d: File '%s' size %d\n", CurrentResource, Name, filelen);		
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

		ResType = ResType_Label;
		GetStringName(128);

		slen = strlen(Name) + 1;
		
		for (n=0;n<slen;n++)
			WriteByte(Name[n]);

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

		ResType = ResType_TileSet;

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

		if(Do_Export_Dependencies && Pass == 2) {
			fprintf(DependFile, "\t%s \\\n", AddRelPrefix(Name));
		}

		filemem = Open_FileAlloc(AddRelPrefix(Name));
		
		if (!filemem)
		{
			Error(Error_Fatal, "Error reading tileset file '%s'", Name);
			return 1;
		}

		filelen = FileAlloc_Len();

		for (n=0;n<filelen;n++)
			WriteByte(filemem[n]);

		Free_File(filemem);

		infoprintf("%d: Tileset '%s' cxy %d,%d size %d\n", CurrentResource, Name, xsize, ysize, filelen);
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

		ResType = ResType_TileMap;

		GetStringName(128);		

		SkipWhiteSpace();
		NeedToken(",");
		SkipWhiteSpace();

		xsize = GetExpression();		// xsize

		SkipWhiteSpace();
		NeedToken(",");
		SkipWhiteSpace();

		ysize = GetExpression();		// ysize

		if(Do_Export_Dependencies && Pass == 2) {
			fprintf(DependFile, "\t%s \\\n", AddRelPrefix(Name));
		}

		filemem = Open_FileAlloc(AddRelPrefix(Name));
		
		if (!filemem)
		{
			Error(Error_Fatal, "Error reading tilemap file '%s'", Name);
			return 1;
		}

		filelen = FileAlloc_Len();

		if (filelen != (xsize * ysize * 2))
		{
			Error(Error_Fatal, "%d: Tilemap '%s' xy %d,%d size %d\n", CurrentResource, Name, xsize, ysize, filelen);
			return 1;
		}

		WriteWord(xsize);
		WriteWord(ysize);

		for (n=0;n<filelen;n++)
			WriteByte(filemem[n]);

		Free_File(filemem);

		infoprintf("%d: Tilemap '%s' cxy %d,%d size %d\n", CurrentResource, Name, xsize, ysize, filelen);
		return 1;


	}

//------------------------------------
//
//------------------------------------

	if (QToken(".image"))			// filename
	{
		SkipWhiteSpace();

		ResType = ResType_Image;

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
		if(Do_Export_Dependencies && Pass == 2) {
			fprintf(DependFile, "\t%s \\\n", AddRelPrefix(Name));
		}

		filemem = Open_FileAlloc(AddRelPrefix(Name));
		
		if (!filemem)
		{
			Error(Error_Fatal, "Error reading image file '%s'", Name);
			return 1;
		}
		
		filelen = FileAlloc_Len();

		// write the length
		//WriteEncodedInt(filelen);

		for (n=0;n<filelen;n++)
			WriteByte(filemem[n]);

		Free_File(filemem);

		infoprintf("%d: Image '%s' cxy %d,%d size %d\n", CurrentResource, Name, spr_cx, spr_cy, filelen);
		return 1;
	}

//------------------------------------
//
//------------------------------------

	if (QToken(".sprite"))			// index, x,y,w,h,cx,cy
	{
		ResType = ResType_Sprite;

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
			CurrentResource, spr_ind, spr_x, spr_y, spr_w, spr_h, spr_cx, spr_cy);

		return 1;
	}

//------------------------------------
//
//------------------------------------

	if (QToken(".end"))
	{
		SkipLine();
//		EndComp = 1;
		return 1;
	}

//------------------------------------
//
//------------------------------------

	if (QToken(".eof"))
	{
		SkipLine();
		EndComp = 1;
		return 1;
	}


//------------------------------------
// Other directives
//------------------------------------

	if (QToken(".set"))
	{
		GetName();							// Get the new type Name	
		NeedToken("=");
		RedefENum(Name, GetExpression());	
		return 1;
	}

//------------------------------------
//
//------------------------------------

	if (QToken(".debug"))
	{
		DEBUG = (short)GetExpression();	
		return 1;
	}

//------------------------------------
//
//------------------------------------

	if (QToken(".info"))
	{
		INFO = (short)GetExpression();	
		return 1;
	}

//------------------------------------
//
//------------------------------------

	if (QToken(".list"))
	{
		LIST = (short)GetExpression();	
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
				c = *FilePtr;
				
				if (c == 0 || c == '"')
					break;
				
				printf("%c",c);

				FilePtr++;
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

	if (QToken(".while"))
	{
	
		if (GetExpression())
		{
			FilePtr = SearchScope(FilePtr);
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

		if(Do_Export_Dependencies && Pass == 2) {
			fprintf(DependFile, "\t%s \\\n", AddRelPrefix(Name));
		}

		filemem = Open_FileAlloc(AddRelPrefix(Name));
		
		if (!filemem)
		{
			Error(Error_Fatal, "Error reading include file '%s'", Name);
			return 1;
		}
		
		filelen = FileAlloc_Len();

		for (n=0;n<filelen;n++)
			WriteByte(filemem[n]);

		Free_File(filemem);

		infoprintf("bin include '%s' size %d\n", Name, filelen);
		return 1;
	}

//------------------------------------
//
//------------------------------------

	if (QToken(".varint"))
	{
		do
		{
			imm = GetExpression();					// Get the number
			WriteEncodedInt(imm);
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
			imm = GetExpression();					// Get the number			
			WriteEncodedSignedInt(imm);
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
			imm = GetExpression();					// Get the number
			WriteByte(imm);
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
			imm = GetExpression();					// Get the number

			WriteWord(imm);
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
			imm = GetExpression();				// Get the number

			WriteLong(imm);
		}
		while (QToken(","));
		return 1;
	}

//------------------------------------
//
//------------------------------------

	if (QToken(".fill"))
	{
		imm = GetExpression();					// Get the number
	
		if (!imm)
			return 1;
	
		NeedToken(",");
	
		v = GetExpression();
		
		do
		{
			WriteByte(v);
		}
		while(--imm);

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
//!		dataStart = (char *) ArrayPtr(&DataMemArray, DataIP);
		dataStart = DataIP;
		
		// read string to mem

		GetStrings();
		
		// find the string length
		
//!		len = DataPtr - dataStart;
		len = DataIP - dataStart;

		if (len > 255)
		{
			Error(Error_Skip, "pstring is longer that 255 chars\n");
			return 1;
		}
		
		// Patch the string length
		
//!		*(dataStart-1) = len;
		ArraySet(&DataMemArray, dataStart-1, len);

		
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
	Error(Error_Fatal, "Illegal resource directive '%s'",Name);

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
		if (Pass == 1)
			Error(Error_Skip, "Symbol '%s' is already defined",name);

		Sym->Value	= CurrentResource;
		return;
	}
	
	// Symbol not found
	
	if (Pass == 2)
	{
		Error(Error_Skip, "Strange error symbol '%s' defined in second pass",name);
		return;
	}

	sym.Section		= section_Resource;
	sym.Type		= 0;
	sym.Value		= CurrentResource;
	sym.LocalScope	= LocalScope;

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
	return 	DataIP;
}

//****************************************
//
//****************************************

void FinalizeResource()
{
	int n;
	
//!	int DataLen = DataPtr - DataMem;
	int DataLen = DataIP;
	
//!	int ResStart = (int) ResPtr;
	int ResStart = ResIP;

	int ResLen, IndexSize;
	
	// Save the resource header

	Section = SECT_res;

//----------------------------------------
// 			   Write Type
//----------------------------------------

	if (ResDispose)
		WriteByte(ResType | 0x80);
	else
		WriteByte(ResType);

//----------------------------------------
//		 Write Indices if needed
//----------------------------------------

	if (IndexCount)
	{
		if (IndexWidth)
		{			
			// ** 4 byte index tables **

			// Write data size + index size
			
			IndexSize = 2 + (IndexCount*4);
			WriteEncodedInt(DataLen + IndexSize);

			// Write table size + indices

//!			*ResPtr++ = IndexCount & 0xff;
//!			*ResPtr++ = (IndexCount >> 8) & 0xff;

			WriteResByte(IndexCount & 0xff);
			WriteResByte((IndexCount >> 8) & 0xff);

			for (n=0;n<IndexCount;n++)
			{
//!				*ResPtr++ = IndexTable[n] & 0xff;
//!				*ResPtr++ = (IndexTable[n] >> 8) & 0xff;
//!				*ResPtr++ = (IndexTable[n] >> 16) & 0xff;
//!				*ResPtr++ = (IndexTable[n] >> 24) & 0xff;

				WriteResByte(IndexTable[n] & 0xff);
				WriteResByte((IndexTable[n] >> 8) & 0xff);
				WriteResByte((IndexTable[n] >> 16) & 0xff);
				WriteResByte((IndexTable[n] >> 24) & 0xff);
			}
		}
		else
		{			
			// ** 2 byte index tables **

			// Write data size + index size
			
			IndexSize = 2 + (IndexCount*2);
			WriteEncodedInt(DataLen + IndexSize);

//			printf("index table: entries %d total size %d\n", IndexCount, DataLen + IndexSize);

			// Write table size + indices

//!			*ResPtr++ = IndexCount & 0xff;
//!			*ResPtr++ = (IndexCount >> 8) & 0xff;

			WriteResByte(IndexCount & 0xff);
			WriteResByte((IndexCount >> 8) & 0xff);

			for (n=0;n<IndexCount;n++)
			{

//				printf("ind %d - offset %d\n", n, IndexTable[n]);

//!				*ResPtr++ = IndexTable[n] & 0xff;
//!				*ResPtr++ = (IndexTable[n] >> 8) & 0xff;			

				WriteResByte(IndexTable[n] & 0xff);
				WriteResByte((IndexTable[n] >> 8) & 0xff);


			}
		}



	}

//----------------------------------------
//  Write resource size without indices
//----------------------------------------

	if (IndexCount == 0)
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
//!2	*ResPtr++ = ArrayGet(&DataMemArray, n);

		WriteResByte(ArrayGet(&DataMemArray, n));
	}

//!	ResLen = (int) ResPtr - ResStart;
	ResLen = ResIP - ResStart;
	
	if (Pass == 2)
	{
//!		printf("Res %d Total %d", CurrentResource, ResPtr - ResStart);
		printf("Res %d Total %d", CurrentResource, ResIP - ResStart);

		if (ResDispose)
			printf(" (Auto-dispose)");

		printf("\n");
	}


	// Write to header file

	if (Pass == 2)
	{
		if (ResName[0])
			fprintf(HeaderFile,"#define %s %d\n", ResName, CurrentResource);
		else
			fprintf(HeaderFile,"//not defined %d\n", CurrentResource);
	}

	// Initialize new resource

	ResetResource();
	
	// Move to next resource

	CurrentResource++;

	Section = SECT_data;
}

//****************************************
//			Save resource data
//****************************************

void WriteResources()
{
//!	int ResLen = ResPtr - ResMem;
	int ResLen = ResIP;
	int res;

	int Entries = CurrentResource - 1;
	int DataLen;
	char nullbyte = 0;

	// Write the header
	
	//SetAsmPtrs();
	SetResAsmPtrs();

	DataIP = 0;
	Section = SECT_data;
	
	// Write magic
	
	WriteLong('SRAM');

	// Write total object entries
	
	WriteEncodedInt(Entries);

	// Write total data length
	
	WriteEncodedInt(ResLen);

	// Write the header info

//!	DataLen = DataPtr - DataMem;
	DataLen = DataIP;

	if (DataLen > 0)
//!		fwrite(DataMem , 1, DataLen , CodeFile);	// Save the data
		ArrayWriteFP(&DataMemArray, CodeFile, DataLen);

	// Write the resouce data
	
	if (ResLen > 0)
//!		fwrite(ResMem , 1, ResLen , CodeFile);		// Save the data
		ArrayWriteFP(&ResMemArray, CodeFile, ResLen);


	res = fwrite(&nullbyte , 1, 1 , CodeFile);			// Save a null
	if(res != 1) {
		Error(Error_Fatal, "Could not write resources");
		return;
	}
}

