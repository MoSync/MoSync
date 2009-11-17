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
//				  		PIP-e II Assembler Librarian Routines
// 						   		Written by A.R.Hartley
//*********************************************************************************************
//#define FREEIMAGE_LIB

#include "compile.h"

//***************************************
//	   ZLib helpers
//***************************************

#ifdef USE_ZLIB

unsigned long ZLibCompress(unsigned char *target, unsigned long target_size,
	unsigned char *source, unsigned long source_size)
{
	uLongf dest_len = (uLongf)target_size;
	int zerr = compress(target, &dest_len, source, (uLong)source_size);
	switch(zerr)
	{
	case Z_MEM_ERROR:	// not enough memory
	case Z_BUF_ERROR:	// not enough room in the output buffer
		return 0;
	case Z_OK:
		return dest_len;
	}
	return 0;
}

unsigned long ZLibUncompress(unsigned char *target, unsigned long target_size,
	unsigned char *source, unsigned long source_size)
{
	uLongf dest_len = (uLongf)target_size;
	int zerr = uncompress(target, &dest_len, source, (uLong)source_size);
	switch(zerr)
	{
	case Z_MEM_ERROR:	// not enough memory
	case Z_BUF_ERROR:	// not enough room in the output buffer
		return 0;
	case Z_OK:
		return dest_len;
	}
	return 0;
}

#endif

//***************************************
//	   Basic Memory Management for PC
//***************************************

#if 1

unsigned char *gNewPtr(int size)
{
	unsigned char *p = (unsigned char *) NewPtr(size+8);
	return p;
}

unsigned char *gReallocPtr(char *thisPtr, int size)
{
	unsigned char *p = (unsigned char *) ReallocPtr(thisPtr, size);
	return p;
}

unsigned char *gNewPtrClear(int size)
{
	unsigned char *p = (unsigned char *) NewPtrClear(size+8);
	return p;
}

void gDisposePtr(unsigned char *thisPtr)
{
	if (thisPtr)
		DisposePtr((char *) thisPtr);
}

#endif

//****************************************
//
//****************************************

typedef struct
{
	char	magic[4];			// 0x89 'M' 'A' 'O'
	int		id[2];				// The ID
	int		numobj;				// Number of objects	
} MA_LIB;

// Each object is an int followed byte the compressed data

typedef struct
{
	int csize;
	int dsize;
} MA_OBJ;

//****************************************
//
//****************************************

#define REALLOC_CHUNK	16384			//1024

unsigned char *SourceTop = 0;
int SourceIdx = 0;
int SourceLen = 0;
int FileCount = 0;

int InitSourceInput(int default_bufsize)
{
	SourceTop = gNewPtrClear(default_bufsize);
	SourceLen = default_bufsize;
	SourceIdx = 0;
	
	if (!SourceTop)
		return 0;

	FileCount = 0;
	return 1;
}

//****************************************
//
//****************************************

void DisposeSourceInput()
{
	if (SourceTop)
		gDisposePtr(SourceTop);
		
	SourceLen = 0;
	SourceIdx = 0;
	return;
}

//****************************************
//
//****************************************

int ExpandSource(int length)
{
	int bytes_left;
	int diff;
	int new_size;

	bytes_left = SourceLen - SourceIdx;

	if (length < bytes_left)
		return 1;
	
	// Expand source buffer
	
	diff	 = length - bytes_left;
	new_size = diff + SourceLen + REALLOC_CHUNK;
			
	SourceTop = (unsigned char *) gReallocPtr((char *) SourceTop, new_size);		
	SourceLen = new_size;

	if (!SourceTop)			// Has failed ?
		return 0;			// Signal it
	
	return 1;
}

//****************************************
//
//****************************************

int AddSourceText(char *Template, ...)
{
	char 	tbuf[4096];

	va_list args;
	int		len;

	va_start(args,Template);
	vsprintf(tbuf,Template,args);
	va_end(args);

	len = strlen(tbuf);

	if (len == 0)
		return 0;

	// Check file will fit in to memory buffer

	if (!ExpandSource(len))
		return 0;

	memcpy(&SourceTop[SourceIdx], tbuf, len);

	SourceIdx += len;
	return 1;
}

//****************************************
//
//****************************************

int AddSourceBin(char *data, int len)
{
	if (len == 0)
		return 0;

	// Check file will fit in to memory buffer

	if (!ExpandSource(len))
		return 0;

	memcpy(&SourceTop[SourceIdx], data, len);
	SourceIdx += len;
	return 1;
}

//****************************************
//
//****************************************

//#define GET_PTR_INT(ptr) (ptr[0] | (ptr[1] << 8) | (ptr[2] << 16) | (ptr[3] << 24))

int AddSourceFile(char *FileName, int local_scope)
{
	unsigned char *memtop;
	unsigned char *memptr;
	MA_LIB head;
	MA_OBJ thisObj;
	int len,n;

	size_t file_length;
	FILE *SrcFile;
	unsigned char *inptr;
	int v;
	size_t res;
	
	SrcFile = fopen(FileName,"rb");

	if (!SrcFile)
		return 0;

	fseek(SrcFile,0,SEEK_END);
	file_length = ftell(SrcFile);
	fseek(SrcFile,0,SEEK_SET);

	if (file_length == 0)
		return 0;

	if (local_scope)
		AddSourceText(".localscope +\r\n");

	AddSourceText("\r\n.lfile '%s'\r\n", FileName);

	// Check file will fit in to memory buffer

	if (!ExpandSource(file_length))
		return 0;

	// Read file data

	res = fread(&SourceTop[SourceIdx], 1, file_length, SrcFile);
	fclose(SrcFile);
	if(res != file_length)
		Error(Error_Fatal, "Could not read source file '%s'", FileName);

	inptr = &SourceTop[SourceIdx];

	v = *inptr;
	
	if (v != 0x89)
	{
		// Wind source pointer forward
	
		SourceIdx += file_length;
		return 1;
	}

	//----------------------------------
	// 		deal with object files
	//----------------------------------
	
	
	if (v == 0x89)
	{
		if (file_length < sizeof(MA_LIB))
			return 0;

		// make some space for the compressed file

		memtop = memptr = gNewPtrClear(file_length + 16);

		if (!memtop)
			return 0;			// could'nt allocate

		// move the compressed data to the temp memory

		memcpy (memptr, inptr, file_length);
		
		// Get the header

		memcpy(&head, memptr, sizeof(MA_LIB));	

		if (head.magic[1] != 'M')
			return 0;				// unknown format

		if (head.magic[2] != 'A')
			return 0;				// unknown format

		if (head.magic[3] != 'O')
			return 0;				// unknown format

		// Step onto the first object
		
		memptr += sizeof(MA_LIB);

		// Looks ok
				
		for (n=0;n<head.numobj;n++)
		{		
			// get object header size

			memcpy(&thisObj, memptr, sizeof(MA_OBJ));
			memptr += sizeof(MA_OBJ);

			// insert another local id

			if (n != 0)
			if (local_scope)
				AddSourceText(".localscope +\r\n");

			// Check file will fit in to memory buffer

			if (!ExpandSource(thisObj.dsize))
				return 0;

			// Decrypt data

			if (head.id[0] && head.id[1])
			{
				// Do decrypt
			}

			// Decompress data

#ifdef USE_ZLIB					
			len = ZLibUncompress( &SourceTop[SourceIdx],
									thisObj.dsize,
									memptr,
									thisObj.csize);

#else
			len = FreeImage_ZLibUncompress( &SourceTop[SourceIdx],
											thisObj.dsize,
											memptr,
											thisObj.csize);

#endif

			if (len != thisObj.dsize)
				return 0;
			
			SourceIdx += len;
			memptr += thisObj.csize;
		}

		gDisposePtr(memtop);

		return 1;	
	}	
	
	// Big fat error, unknown format
	
	return 0;
}

//****************************************
//
//****************************************
/*
void Add_Ctor_Source(char *src_ptr, char *stype, char *label, char *sname)
{	
	int Found = 0;
	FilePtr = src_ptr;


	AddSourceText("\r\n");
	AddSourceText("\t.sourcefile 'internal ctor/dtor'\r\n");
	AddSourceText("\t.sourcedir '/'\r\n");
	AddSourceText("\t.data\r\n");
	AddSourceText("\t.align 4\r\n");
	AddSourceText("\t.global %s\r\n", label);
	AddSourceText("%s:\r\n", label);

	do
	{
		FilePtr = TokenSearch(stype, FilePtr);

		if (!FilePtr)
			break;

		SkipToken(stype);
		SkipWhiteSpace();
	
		GetName();

//		AddSourceText("\t.word %s\r\n", Name);

		AddSourceText("\t.word %s%d\r\n", sname, Found);
		Found++;
	}
	while(1);

	AddSourceText("\t.word 0\r\n", Name);
}
*/

//****************************************
//
//****************************************

//char *LastEOF;
int LastEOF;

void TerminateSourceFile(int add_eof)
{
	AddSourceText("\r\n");

//!! Add cdtors here !!

//	LastEOF = (char *) &SourceTop[SourceIdx];
	LastEOF = SourceIdx;

	if (add_eof)
	{
		AddSourceText(".eof\r\n");
	}
}

//****************************************
//
//****************************************

/*void SourceFileCheckEOF(char *eofptr)
{
	if (eofptr != LastEOF)
		printf("Error: File terminates prematurely\n");
}
*/

void SourceFileCheckEOF(int eofidx)
{
	if (ArgUseMasterDump)
		return;

	if (eofidx != LastEOF)
		printf("Error: File terminates prematurely\n");
}

//****************************************
//
//****************************************

int WriteSourceFile(char *name)
{
	FILE * SrcFile;
	int res;
	
	SrcFile = fopen(name,"wb");

	if (!SrcFile)
		return 0;

	res = fwrite(SourceTop, 1, SourceIdx, SrcFile);		// Save the header
	fclose(SrcFile);
	if(res != SourceIdx)
		return 0;
	return 1;
}

//****************************************
// 			Init Librarian
//****************************************

char *LibFiles[1024];
int LibFileCount = 0;

int InitLibrarian()
{
	int v;
	
	v = InitSourceInput(0x20000);

	if (!v)
		return 0;

	LibFileCount = 0;
	return 1;
}

//****************************************
// 
//****************************************

char *GetLibaryFilePtr()
{
	return (char *) SourceTop;
}

//****************************************
// 
//****************************************

//typedef unsigned char uchar;

void DisposeLibrarian()
{
	char *thisFile;
	int n;

	DisposeSourceInput();

	if (!LibFileCount)
		return;

	for (n=0;n<LibFileCount;n++)
	{
		thisFile = LibFiles[n];

		if (thisFile)
			gDisposePtr((uchar *) thisFile);
		
		LibFiles[n] = 0;
	}

	LibFileCount = 0;
}

//****************************************
// 
//****************************************

int AddLibrarian(char *file, int disp)
{
	char *newstr;
	char *libstr;
//	char *endstr;
	int v;

//	int addLocalScope = 0;

	int len = strlen(file);

	if (!len)
		return 0;
	
	// test for lib files
	
	libstr = SearchLibPath(file);

	if (libstr)
	{
		len = strlen(libstr);
		if (disp) printf("Found lib '%s'\n", libstr);
		file = libstr;
	}

/*	if (len > 2)
	{
		endstr = &file[len-2];
		
		if (strcmp(endstr(endstr, ".s") == 0)
			addLocalScope = 1;
	}

	v = AddSourceFile(file, addLocalScope);
*/
	// Add source files
	
	v = AddSourceFile(file, 1);

	if (!v)
		return 0;
			
	newstr = (char *) gNewPtrClear(len+1);
	
	if (!newstr)
		return 0;
		
	strcpy(newstr, file);
	
	LibFiles[LibFileCount] = newstr;
	LibFileCount++;
	
	if (disp)
		printf("Added '%s'\n", newstr);
	
	return 1;
}

//****************************************
//
//****************************************

int WriteLibrarian(char *outfile)
{
	FILE * SrcFile;
	MA_LIB head;		// Deal with compressed object file
	MA_OBJ thisObj;
	
	unsigned char *cptr,*cptrtop;
	//int dlen,clen;
	int res;

	cptrtop = cptr = gNewPtrClear(SourceIdx * 2);
	
	if (!cptrtop)
		return 0;
	
	SrcFile = fopen(outfile,"wb");

	if (!SrcFile)
		return 0;

	thisObj.dsize = SourceIdx;

#ifdef USE_ZLIB
	thisObj.csize = ZLibCompress(cptr, SourceIdx * 2, SourceTop, SourceIdx);
#else
	thisObj.csize = FreeImage_ZLibCompress(cptr, 0x40000, SourceTop, SourceIdx);
#endif

	if (thisObj.csize)
	{
		// Set up header
		
		head.magic[0] = 0x89;
		head.magic[1] = 'M';
		head.magic[2] = 'A';
		head.magic[3] = 'O';

		head.id[0] = 0;
		head.id[1] = 0;
		
		head.numobj = 1;
		
		// Save lib header
		
		res = fwrite(&head, 1, sizeof(head), SrcFile);
		if(res != sizeof(head))
			return 0;

		// Save object header
		res = fwrite(&thisObj, 1, sizeof(thisObj), SrcFile);
		if(res != sizeof(thisObj))
			return 0;

		// Save the data
		res = fwrite(cptr, 1, thisObj.csize, SrcFile);
		if(res != sizeof(thisObj.csize))
			return 0;
	}
	
	gDisposePtr(cptrtop);
	fclose(SrcFile);
	
	printf("Created '%s'\n", outfile);
	return 1;
}

//*********************************************************************************************
//
//*********************************************************************************************

#define MAX_LIB_PATHS	32

char *libpaths[MAX_LIB_PATHS];
int pathcount = 0;

//****************************************
//
//****************************************

void InitLibPath()
{
	pathcount = 0;
}

//****************************************
//
//****************************************

int AddLibPath(char *path)
{
	int len;
	char * str;
	
	if (pathcount >= MAX_LIB_PATHS)
		return 0;

	len = strlen(path);
	
	if (!len)
		return 0;

	str = NewPtr(len+4);

	strcpy(str, path);

	len = strlen(str);

	if (str[len-1] != '\\')
		strcat(str,"/");

	libpaths[pathcount++] = str;
	return 1;
}

//****************************************
//
//****************************************

char LibPath[1024];

char * SearchLibPath(char * file)
{	
	FILE *fp;
	int n;
	//int len = strlen(file);
		
//	if (strcmp(&file[len-4], ".lib") != 0)
//		return 0;

	// Test local path first

	fp = fopen(file, "rb");

	// If file was in local path, then just return

	if (fp)
	{
		fclose(fp);
		return 0;
	}

	// Test search paths
	
	if (!pathcount)
		return 0;

	for (n=0;n<pathcount;n++)
	{
		strcpy(LibPath, libpaths[n]);
		strcat(LibPath, file);
		
		fp = fopen(LibPath, "rb");
		
		if (fp)
		{
			fclose(fp);
			return LibPath;
		}
	}

	return 0;
}





