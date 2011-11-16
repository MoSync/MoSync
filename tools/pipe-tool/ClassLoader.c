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

static int verbose = 0;

#define vprintf	if (verbose) printf

//****************************************
//		  		Load Binary
//****************************************

int FileLen = 0;

uchar *LoadToMemory(char *filename)
{
	FILE *InputFile;
	uchar *ThisMem;
	long Length;
	int res;

	if ((InputFile = fopen(filename, "rb")) == NULL)
	{
			Error(Error_System, "Problem opening file '%s'\n",filename);
			return 0;
	}

	fseek(InputFile, 0L ,SEEK_END);	// Point to end
	Length = ftell(InputFile);
	fseek(InputFile, 0L ,SEEK_SET);	// Point to beginning

	FileLen = Length;

	ThisMem = (uchar *) calloc(Length + 1000, 1);

	if (ThisMem == 0)
	{
		fclose(InputFile);

		Error(Error_System, "Problem allocating file '%s'\n",filename);
		return 0;
	}

	res = fread(ThisMem, 1, Length, InputFile);		// Load the data
	fclose(InputFile);
	if(res != Length)
	{
			Error(Error_System, "Problem loading file '%s'\n",filename);
			return 0;
	}

	//printf("Loaded file '%s' %ld Bytes\n\n",filename,Length);
	return ThisMem;
}

//****************************************
//
//****************************************

void SaveToFile(char *filename, char *start, int len)
{
	FILE *theFile;
	int res;

	theFile = fopen(filename, "wb");

	if (!theFile)
		return;

	res = fwrite(start, 1, len, theFile);
	if(res != len)
		return;	//FIXME: silent fail

	fclose(theFile);
	return;
}

//****************************************
//			  Fuction main()
//****************************************

void PatchClass(char *infile, char *outfile)
{
	printf("Patching %s\n", infile);
	ClassFilePtr = LoadToMemory(infile);
	ClassPtr = ClassFilePtr;

	ReadClass(&mainClass);

	//DispConstantPool(&mainClass);

	SaveToFile(outfile, (char *) ClassPtr, FileLen);
	free(ClassPtr);
}

//****************************************
//			  Fuction main()
//****************************************

void OptimizeClass()
{
	ClassFilePtr = LoadToMemory("Core.class");
	ClassPtr = ClassFilePtr;

	ReadClass(&mainClass);

	//DispConstantPool(&mainClass);

//	SaveToFile(outfile, (char *) ClassPtr, FileLen);
	free(ClassPtr);
}

//****************************************
//
//****************************************

uchar * GetReadPtr()
{
	return ClassFilePtr;
}

uchar ReadByte()
{
	return *ClassFilePtr++;
}

//****************************************
//
//****************************************

ushort ReadWord()
{
	ushort v = ReadByte() << 8;
	return v | ReadByte();
}

//****************************************
//
//****************************************

uint ReadLong()
{
	uint v = ReadByte() << 24;
	v |= ReadByte() << 16;
	v |= ReadByte() << 8;
	v |= ReadByte();

	return v;
}

//****************************************
//
//****************************************

float ReadFloat()
{
	union {
		float f;
		long l;
	} v;

	v.l = ReadLong();
	return v.f;
}

//****************************************
//
//****************************************

long long ReadLongLong()
{
	long long v = 0;

	v |= (long long)ReadByte() << 56;
	v |= (long long)ReadByte() << 48;
	v |= (long long)ReadByte() << 40;
	v |= (long long)ReadByte() << 32;

	v |= (long long)ReadByte() << 24;
	v |= (long long)ReadByte() << 16;
	v |= (long long)ReadByte() << 8;
	v |= (long long)ReadByte();
	return v;
}

//****************************************
//
//****************************************

double ReadDouble()
{
	union {
		double d;
		long long ll;
	} v;


	v.ll = ReadLongLong();
	return v.d;
}

//****************************************
//
//****************************************

void ReadData(char *ReadPtr,long Size)
{
	memcpy(ReadPtr,ClassFilePtr,Size);
	ClassFilePtr += Size;
}

//****************************************
//
//****************************************

void SkipData(long Size)
{
	ClassFilePtr += Size;
}

//****************************************
//			  Read Class
//****************************************

int ReadClass(ClassEntry *theClass)
{
	//long Magic;

	theClass->ClassFilePtr = ClassFilePtr;

	/*Magic =*/ ReadLong();

	theClass->Major_version = ReadWord();
	theClass->Minor_version = ReadWord();

	IndexConstantPool(theClass);

	theClass->AccessFlags = ReadWord();
	theClass->ThisClass = ReadWord();
	theClass->SuperClass = ReadWord();

	vprintf("ThisClass %d = '%s'\n", theClass->ThisClass,GetPoolString(theClass, theClass->ThisClass));
	vprintf("SuperClass %d = '%s'\n", theClass->SuperClass,GetPoolString(theClass, theClass->SuperClass));

	theClass->InterfaceCount = ReadWord();

	if (theClass->InterfaceCount)
	{
		Error(Error_System, "Interface unsupported\n");
		exit(1);
	}

	ReadFields(theClass);
	ReadMedthods(theClass);

	return 1;
}
//****************************************
//			  Read Fields
//****************************************

#define GPS(vv) GetPoolString(theClass,vv)
#define GPC(vv)	GetPoolConst(theClass,vv)

void ReadFields(ClassEntry *theClass)
{
	short	AccessFlags,NameIndex,TypeIndex,AttrCount;
	short	n,m;

	theClass->FieldsCount = ReadWord();
	theClass->FieldsFPos = ClassFilePtr;

	vprintf("\n\nFields Entries %d\n", theClass->FieldsCount);

	for (n=0;n<theClass->FieldsCount;n++)
	{
		vprintf("%d:\n",n);

		AccessFlags = ReadWord();
		vprintf("AccessFlags %d\n", AccessFlags);

		NameIndex = ReadWord();
		vprintf("NameIndex %d = '%s'\n", NameIndex,GPS(NameIndex));

		TypeIndex = ReadWord();
		vprintf("TypeIndex %d = '%s'\n", TypeIndex,GPS(TypeIndex));

		AttrCount = ReadWord();
		vprintf("AttrCount %d\n", AttrCount);

		//

		//printf("GLOBAL %s ",GPS(TypeIndex));
		//printf("%s\n",GPS(NameIndex));

/*		{
			PoolInfo theInfo;

			GetPoolInfo(&mainClass, NameIndex, &theInfo);
			printf("xGLOBAL '%s'\n",theInfo.String);

		}
*/
		for (m=0;m<AttrCount;m++)
		{
			//short	AttrNameIndex,ConstValueIndex;
			//long	BytesCount;

			/*AttrNameIndex =*/ ReadWord();
//			printf("AttrNameIndex %d = '%s'\n", AttrNameIndex, GPS(AttrNameIndex));

			/*BytesCount =*/ ReadLong();
//			printf("BytesCount %ld\n", BytesCount);

			/*ConstValueIndex =*/ ReadWord();
//			printf("ConstValueIndex %d = %ld\n", ConstValueIndex,GPC(ConstValueIndex));
		}

	}
}

//****************************************
//			  Read Medthods
//****************************************

void ReadMedthods(ClassEntry *theClass)
{
	short	AccessFlags,NameIndex,TypeIndex,AttrCount;
	short	n,m;

	theClass->MethodsCount = ReadWord();
	theClass->MethodsFPos = ClassFilePtr;

	vprintf("\n\nMethods Entries %d\n", theClass->MethodsCount);

	for (n=0;n<theClass->MethodsCount;n++)
	{
		vprintf("%d:\n",n);

		AccessFlags = ReadWord();
		vprintf("AccessFlags %d\n", AccessFlags);

		NameIndex = ReadWord();
		vprintf("NameIndex %d = '%s'\n", NameIndex, GPS(NameIndex));

#if 1	// Dont print function names
		vprintf("\nFUNC %s", GPS(theClass->ThisClass) );
		vprintf(".%s\n", GPS(NameIndex) );
#endif

		TypeIndex = ReadWord();
		vprintf("TypeIndex %d = '%s'\n", TypeIndex, GPS(TypeIndex));

		AttrCount = ReadWord();
		vprintf("AttrCount %d\n", AttrCount);

		for (m=0;m<AttrCount;m++)
		{
//			short	AttrNameIndex,ConstValueIndex;
			short	AttrNameIndex;
			long	BytesCount;

			AttrNameIndex = ReadWord();
			vprintf("AttrNameIndex %d = '%s'\n", AttrNameIndex,GPS(AttrNameIndex));

			BytesCount = ReadLong();
			vprintf("BytesCount %ld\n", BytesCount);

//			ConstValueIndex = ReadWord();
//			printf("ConstValueIndex %d = '%s'\n", ConstValueIndex,GPS(ConstValueIndex));

			if (strcmp(GPS(AttrNameIndex),"Code") == 0)
			{
				vprintf("Found code\n");
				Disassemble();
				//Recompile();

				SkipData(BytesCount);
			}
			else
			{
				SkipData(BytesCount);
			}

		}

	}
}

//****************************************
//		 Index the constant pool
//****************************************

long *CpIndex;

void IndexConstantPool(ClassEntry *theClass)
{
	long lastindex;
	int psize, n;

	theClass->PoolCount = ReadWord();

	psize = sizeof(long) * theClass->PoolCount;

	CpIndex = (long *) malloc(psize);

	if (!CpIndex)
	{
		printf("Could'nt allocate CpIndex\n");
		exit(1);
	}

	memset(CpIndex, 0, psize);

	theClass->PoolPtr	= CpIndex;

	for (n=1;n<theClass->PoolCount;n++)
	{
		lastindex = CpIndex[n-1] = (long) GetReadPtr();

		switch(ReadByte())
		{
			case CONSTANT_Utf8 :
				SkipData(ReadWord());
			break;

/*			case CONSTANT_Unicode :
				printf("Unicode(%d)",Type);
			break;
*/
			case CONSTANT_Integer :
			case CONSTANT_Float :
				ReadLong();
				break;

			case CONSTANT_Long :
			case CONSTANT_Double :

				CpIndex[n] = lastindex; /* fix */

				ReadLongLong();
				n++;
				break;

			case CONSTANT_Class :
			case CONSTANT_String :

				ReadWord();
				break;

			case CONSTANT_Fieldref :
			case CONSTANT_Methodref :
			case CONSTANT_InterfaceMethodref :
			case CONSTANT_NameAndType :

				ReadWord();
				ReadWord();
				break;

			default:
				printf("(Index Pool) Illegal Type in constant pool %ld", lastindex);
				exit(1);
		}
	}
}

//****************************************
//		Get Constant Pool Entry
//****************************************

void GetPoolInfo(ClassEntry *theClass, short Entry, PoolInfo *theInfo)
{
	uchar Type;
	uchar *oldClassFilePtr = ClassFilePtr;

	Entry--;

	if (Entry < 0 || Entry > theClass->PoolCount)
		Error(Error_System, "Attempt to access illegal pool entry");

	theInfo->Value = 0;

	ClassFilePtr = (uchar *) theClass->PoolPtr[Entry];

	if (!ClassFilePtr)
		return;

	theInfo->String[0] = 0;		// Clear the string field

	Type = ReadByte();

	theInfo->Type = Type;

	switch(Type)
	{
		case CONSTANT_Utf8 :
		{
			short Len = ReadWord();

			ReadData(theInfo->String,Len);
			theInfo->String[Len] = 0;
			break;
		}

		case CONSTANT_Integer :
			theInfo->Value = ReadLong();
			break;

		case CONSTANT_Float :
			theInfo->fValue = ReadFloat();
			break;

		case CONSTANT_Long :
			theInfo->llValue = ReadLongLong();
			break;

		case CONSTANT_Double :
			theInfo->dValue = ReadDouble();
			break;

		case CONSTANT_Class :
		{
			theInfo->Index = ReadWord();		// Value is the index to class

			AccumulateString(theClass, theInfo->String, theInfo->Index);
			break;
		}

		case CONSTANT_String :
		{
			theInfo->Index = ReadWord();		// Value is the index to string

			AccumulateString(theClass, theInfo->String, theInfo->Index);
			break;
		}

		case CONSTANT_Fieldref :
		case CONSTANT_Methodref :
		case CONSTANT_InterfaceMethodref :
		case CONSTANT_NameAndType :

			theInfo->Index = ReadWord();
			theInfo->Value = ReadWord();

			AccumulateString(theClass, theInfo->String, theInfo->Index);
			AccumulateString(theClass, theInfo->String, theInfo->Value);
			break;

		default:
			Error(Error_System, "Illegal Constant Pool Type");

	}

	ClassFilePtr = oldClassFilePtr;
	return;
}

//****************************************
// Make Strings Accumulate on recursion
//****************************************

void AccumulateString(ClassEntry *theClass, char *String, short Index)
{
	PoolInfo PoolString;

	if (strlen(String) > 490)
		Error(Error_System, "String is too long");

	GetPoolInfo(theClass, Index, &PoolString);

	if (PoolString.String[0] == 0)
		return;

	if (String[0])
		strcat(String,":");

	strcat(String,PoolString.String);
	return;
}

//****************************************
// 		Extract Pool String
//****************************************

char * PoolSubString(char *str, char *dst, int index)
{
	int count = 0;
	char c;
	char *outstr = dst;

	// Reset dst

	*dst = 0;

	// find start pos

	if (index > 0)
	{
		while(1)
		{
			c = *str++;

			if (c == 0)
				return outstr;			// return no index

			if (c == ':')
			{
				count++;
				if (index == count)
					break;
			}
		}
	}

	// copy to end of string

	while(1)
	{
			c = *str++;

			if (c == 0)
				break;

			if (c == ':')
				break;

			*dst++ = c;
	}

	*dst = 0;

	return outstr;
}

//****************************************
// 		Get a string from a CPE
//****************************************

PoolInfo thePoolString;

char * GetPoolString(ClassEntry *theClass, short Index)
{
	GetPoolInfo(theClass, Index, &thePoolString);
	return thePoolString.String;
}

//****************************************
// 		Get a constant from a CPE
//****************************************

long GetPoolConst(ClassEntry *theClass, short Index)
{
	GetPoolInfo(theClass, Index, &thePoolString);
	return thePoolString.Value;
}

//****************************************
//		 Read the constant pool
//****************************************

void DispPoolEntry(ClassEntry * theClass,short Entry)
{
	PoolInfo theInfo;

	GetPoolInfo(theClass, Entry, &theInfo);

	switch(theInfo.Type)
	{
		case CONSTANT_Utf8 :
			printf("Utf8 '%s'",theInfo.String);
			return;

		case CONSTANT_Integer :
			printf("Integer %d",theInfo.Value);
			return;

		case CONSTANT_Float :
			printf("Float %f",theInfo.fValue);
			return;

		case CONSTANT_Long :
			printf("Long %lld",theInfo.llValue);
			return;

		case CONSTANT_Double :
			printf("Double %f",(float) theInfo.dValue);
			return;

		case CONSTANT_Class :
			printf("Class %d = '%s'",theInfo.Index,theInfo.String);
			return;

		case CONSTANT_String :
			printf("String %d = '%s'",theInfo.Index,theInfo.String);
			return;


		case CONSTANT_Fieldref :
		case CONSTANT_Methodref :
		case CONSTANT_InterfaceMethodref :
		case CONSTANT_NameAndType :

			if (theInfo.Type == CONSTANT_Fieldref)
				printf("Fieldref Class %d NameType %d",theInfo.Index,theInfo.Value);

			if (theInfo.Type == CONSTANT_Methodref)
				printf("Methodref Class %d NameType %d",theInfo.Index,theInfo.Value);

			if (theInfo.Type == CONSTANT_InterfaceMethodref)
				printf("InterfaceMethodref Class %d NameType %d",theInfo.Index,theInfo.Value);

			if (theInfo.Type == CONSTANT_NameAndType)
				printf("NameAndType Class %d Type %d",theInfo.Index,theInfo.Value);

			printf("\t'%s'",theInfo.String);

			return;

			default:
				Error(Error_System, "Illegal Constant Pool Type ");
		}
}

//****************************************
//		 Read the constant pool
//****************************************

void DispConstantPool(ClassEntry *theClass)
{
	int	n;

	for (n=1;n<theClass->PoolCount;n++)
	{
		printf("%2d: ",n);
		DispPoolEntry(theClass,n);
		printf("\n");
	}
}


