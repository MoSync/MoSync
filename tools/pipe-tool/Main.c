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
//				  PIP-e II Assembler TopLevel Routines
// 						   Written by A.R.Hartley
//			This code is based on the GameTasker v2.0 Module System
//*********************************************************************************************

#ifdef WIN32
#include <windows.h>
#endif

#define __DEFGLOBALS__
#include "compile.h"

#define MASTER_FILE_DUMP

//****************************************
//				main()
//****************************************

#define dbprintf	if (0) printf

//****************************************
//
//****************************************

void ExitApp(int code)
{
	theExitCode = code;

	EJmp = 1;
	longjmp(ErrorJump, code);
}
//****************************************
//
//****************************************
/*
void ShutdownApp()
{
	DisposeMasterAlloc();
	exit(theExitCode);
}
*/
//****************************************
//
//****************************************

#include <stdio.h>
#include <time.h>


int main(int argc,char *argv[])
{
	char *output;
	char *input;
	char *ArgStr = 0;
	char *config_file;
	int v;

/*	time_t rawtime;
	struct tm * ptm;

	time ( &rawtime );
	ptm = gmtime ( &rawtime );
*/
	// Initialize the master allocation table

	InitTokenSystem();
	InitMasterAlloc();
	
//--------------------------------
//Create the code and data arrays
// 	  used for assem mem
//--------------------------------

	ArrayInit(&CodeMemArray,	1, 0);
	ArrayInit(&DataMemArray,	1, 0);
	ArrayInit(&ResMemArray,		1, 0);

	ArrayInit(&CodeMemArrayCopy,1, 0);

//--------------------------------
//Create the code and data arrays
// 	  used for elimination
//--------------------------------

	ArrayInit(&CallArray,		4, 0);
	ArrayInit(&LabelArray,		4, 0);
	ArrayInit(&CodeLabelArray,	4, 0);
	ArrayInit(&DataArray,		4, 0);
	ArrayInit(&DataAccessArray,	4, 0);
	ArrayInit(&DataTypeArray,	4, 0);		// Data types
	ArrayInit(&DataMixArray,	4, 0);		// Data type mix array

	ArrayInit(&PaddingArray,	1, 0);
	ArrayInit(&DataAlignArray,	1, 0);

	ArrayInit(&CtorArray,		4, 0);
	ArrayInit(&DtorArray,		4, 0);
	ArrayInit(&CtorArrayImm,	4, 0);
	ArrayInit(&DtorArrayImm,	4, 0);

	ArrayInit(&AsmCharArray,	4, 0);
	ArrayInit(&AsmCharDataArray,4, 0);

	ArrayInit(&AsmCharIPArray,	4, 0);
	ArrayInit(&CodeTouchArray,	1, 0);

	ArrayInit(&SLD_Line_Array,	4, 0);
	ArrayInit(&SLD_File_Array,	4, 0);

//--------------------------------
//Implement the shutdown exception
//--------------------------------

	// Set the exception flag to 0

	EJmp = 0;

	// Application exit point

	setjmp(ErrorJump);
	
	// Catch the shutdown when it occurs
	
	if (EJmp)
	{
		DisposeMasterAlloc();			// Shutdown completely
		//printf("EJmp exit %i\n", theExitCode);
		exit(theExitCode);
	}

//--------------------------------
//			  Init
//--------------------------------

	//printf("Path = %s\n", argv[0]);

	InitSysCalls();
	InitLibPath();

	output = NULL;
	input = NULL;
	argno = 1;

	ArgRes = 0;				// Is Assembler
	ArgBuild = 0;
	ArgJavaNative = 0;
	ArgBrewGen = 0;
	ArgCppGen = 0;
	ArgSLD = 0;
	ArgDebugRebuild = 0;
	ArgUseStabs = 0;

	DisasFunc[0] = 0;

	INFO = 0;

	CurrentModel = MODEL_null;

	Default_DataSize  = 0x20000;	// 128
	Default_StackSize = 0x08000;	// 32	
	Default_HeapSize  = 0x10000;	// 64
	Default_AppCode = 'TEST';
	Do_Dump_Symbols = 0;
	relPath[0] = 0;

	mosync_dir = getenv("MOSYNCDIR");

	// Set the default profile path to empty

	ProfilePath[0] = 0;

	if (argc < 2)
	{
		usage();
		ExitApp(1);
	}

//--------------------------------
//		 Parse switches
//--------------------------------

	while(1)
	{
		ArgStr = argv[argno];
	
		if (!ArgStr)
			break;

		FilePtr = ArgStr;
		
		if (!Token("-"))
			break;

		argno++;

/*		if (Token("test"))
		{
			printf("Test!!\n");
			//OptimizeClass();
			ExitApp(0);
		}
*/
		if (Token("-help"))
		{
			usage ();
			ExitApp(0);
		}

		if (Token("xerr"))
		{
			xerr = 1;
			continue;
		}

		if (Token("use-master-dump"))
		{
			ArgUseMasterDump = 1;
			continue;
		}

		if (Token("debug-elim"))
		{
			ArgDebugRebuild = 1;
			continue;
		}

		if (Token("skip-elim"))
		{
			ArgSkipElim = 1;
			continue;
		}

		if (Token("sld="))
		{
			ArgSLD = 1;
			GetCmdString();
			strcpy(SldName, Name);
			continue;
		}

		if (Token("stabs="))
		{
			ArgUseStabs = 1;
			GetCmdString();
			strcpy(StabsName, Name);
			continue;
		}

		if (Token("meta="))
		{
			ArgWriteMeta = 1;
			GetCmdString();
			strcpy(MetaFileName, Name);
			continue;
		}

		if (Token("no-verify"))
		{
			ArgVerifierOff = 1;
			continue;
		}

		if (Token("master-dump"))
		{
			ArgMasterDump = 1;
			continue;
		}

		if (Token("custom-config"))
		{
			ArgCustomConfig = 1;
			continue;
		}

		if (Token("elim"))
		{
			Do_Elimination = 1;

			// Switch of reg constant optimization

			ArgConstOpt = 0;
			continue;
		}

		if (Token("dump-syms"))
		{
			Do_Dump_Symbols = 1;
			continue;
		}

		if (Token("dump-unref"))
		{
			Do_Dump_Unref_Symbols = 1;
			continue;
		}
	
		if (Token("error-paths"))
		{
			ArgFilePaths = 1;
			continue;
		}
	
		if (Token("entry="))
		{
			GetName();
			strcpy(Code_EntryPoint, Name);
			dbprintf("Code entry point set to '%s'\n",Code_EntryPoint);
			continue;
		}

		if (Token("disas="))
		{
			GetName();
			strcpy(DisasFunc, Name);
			continue;
		}

		if (Token("depend="))
		{
			GetCmdString();
			strcpy(DependName, Name);
			Do_Export_Dependencies = 1;
			continue;
		}

		if (Token("gcj="))
		{
			GetCmdString();
			strcpy(GcjFlags, Name);
			continue;
		}

		if (Token("datasize="))
		{
			Default_DataSize = GetNum();
			dbprintf("Data size set to %d\n",Default_DataSize);
			continue;
		}

		if (Token("stacksize="))
		{
			Default_StackSize = GetNum();
			dbprintf("Stack size set to %d\n",Default_StackSize);
			continue;
		}

		if (Token("heapsize="))
		{
			Default_HeapSize = GetNum();
			dbprintf("Stack size set to %d\n",Default_HeapSize);
			continue;
		}

		if (Token("appcode="))
		{
			Default_AppCode = GetNum();
			dbprintf("AppCode set to '%s'(%d)\n",AppCodeString(Default_AppCode), Default_AppCode);
			continue;
		}

		if (Token("appid="))
		{
			Default_AppID = GetNum();
			dbprintf("AppID set to '%s'(%d)\n",AppCodeString(Default_AppID), Default_AppID);
			continue;
		}

		if (Token("quiet"))
		{
			ArgQuiet = 1;
			continue;
		}

		if (Token("-credits"))
		{
			printf("\nMoSync Team Credits\n");

			printf("\tPatrick Broman       - SW Eng\n");
			printf("\tFredrik Eldh         - SW Eng\n");
			printf("\tNiklas Nummelin      - SW Eng\n");
			printf("\tBen Chua             - SW Eng/Web\n");
			printf("\tAaron Chan           - Web Services\n");
			printf("\tRonnie Helin         - Test SW Eng\n");
			printf("\tHenrik Von Schoultz  - BizDev\n");
			printf("\tAlex Jonsson         - Tech Evangelist\n");
			printf("\tAntony Hartley       - CTO, SW Eng\n");
			printf("\tTomas Uppgard        - CEO\n");
			ExitApp(0);
		}

		if (Token("v"))
		{
			INFO = 1;
			continue;
		}

		if (Token("B"))
		{
			ArgBuild = 1;
			continue;
		}

		if (Token("L"))
		{
			ArgLink = 1;
			continue;
		}

		if (Token("R"))
		{
			ArgRes = 1;
			continue;
		}

		if (Token("Z"))
		{
			ArgDumpFile = 1;
			continue;
		}

		if (Token("java"))
		{
			ArgJavaNative = 1;
			ArgConstOpt = 0;
			Do_Elimination = 1;

			dbprintf("Native java build on\n");
			continue;
		}

/*		if (Token("brew"))
		{
			ArgBrewGen = 1;
			ArgConstOpt = 0;
			Do_Elimination = 1;

			dbprintf("BREW build on\n");
			continue;
		}
*/

		if (Token("cpp"))
		{
			ArgCppGen = 1;
			ArgConstOpt = 0;
			Do_Elimination = 1;

			dbprintf("Native cpp build on\n");
			continue;
		}

		if (Token("s"))
		{
			GetCmdString();

			if (strlen(Name))
			{
				AddLibPath(Name);
				dbprintf("Added search path '%s'\n",Name);
			}
			else
				printf("invalid search path '%s'\n",Name);
						
			continue;
		}


		if (Token("p"))
		{
			GetCmdString();

			if (strlen(Name))
			{
				strcpy(ProfilePath, Name);
				dbprintf("profile '%s'\n",Name);
			}
			else
				printf("invalid profile '%s'\n",Name);
						
			continue;
		}


		if (Token("h"))
		{
			usage();
			ExitApp(0);
		}

		if (Token("H"))
		{
			usage();
			ExitApp(0);
		}

		printf("ignoring unrecognized switch '%s'\n", ArgStr);
	}

	if ((ArgBuild + ArgRes + ArgLink) > 1)
	{
		printf("Illegal options combination\n");
		ExitApp(1);
	}

//--------------------------------
//		Get ENV settings
//--------------------------------

	if (!mosync_dir)
	{
		printf("Warning: missing environment setting MOSYNCDIR, defaulting to local directory\n");
	}

//--------------------------------
//		Get output file name
//--------------------------------

	if (!argv[argno])
	{
		printf("missing output file.\n");
		ExitApp(1);
	}

	output = argv[argno++];

//--------------------------------
// 		Get input file(s)
//--------------------------------

	if (!argv[argno])
	{
		printf("missing input file(s).\n");
		ExitApp(1);
	}

	v = InitLibrarian();

	if (!v)
	{
		printf("Source reader failed to initialise\n");
		ExitApp(1);
	}


	if (!ArgLink && !ArgDumpFile && !ArgCustomConfig)
	{
		// Add users defines
		
		AddLibrarian("defines.lst", 0);
	}

	if (!ArgLink && !ArgRes && !ArgDumpFile && !ArgCustomConfig)
	{
		// Add users defines
		
		//AddLibrarian("defines.lst", 0);

		if (ArgUseMasterDump == 0)			// if the input is a master dump don't load config_asm
		{
			// Add the asm config path

			config_file = GetMosyncPath("bin/","asm_config.lst");

			v = AddLibrarian(config_file, 0);

			if (!v)
				v = AddLibrarian("asm_config.lst", 0);

			if (!v)
			{
				printf("failed to load '%s'\n", config_file);
				ExitApp(1);
			}
		}
	}

//--------------------------------
//Loop over files to load and log
//--------------------------------

	{
		int this_arg = argno;
		
		AddSourceText("\r\n.library");	

		while(argv[this_arg])
		{
			input = argv[this_arg++];
			
			AddSourceText(" '%s'", input);	
		}
		AddSourceText("\r\n");	
	}

//--------------------------------
//			load files
//--------------------------------

	while(argv[argno])
	{
		input = argv[argno++];

//		SetRelPath(input);
//		AddSourceText("\r\n.relpath '%s'\r\n", relPath);
		
		v = AddLibrarian(input, !ArgBuild && !ArgQuiet);

		if (!v)
		{
			printf("failed to load '%s'\n", input);
			ExitApp(1);
		}
		
	}

	if (ArgDumpFile)
	{
		TerminateSourceFile(1);
	
		WriteSourceFile("_dump.s");
		DisposeLibrarian();
		ExitApp(0);
	}

//--------------------------------
//		Librarian Mode ?
//--------------------------------

	if (ArgLink)
	{
		// Create a obj/lib

		TerminateSourceFile(0);			// Don't place an eof at the end of libraries

		WriteLibrarian(output);
		DisposeLibrarian();
		ExitApp(0);
	}
	
//--------------------------------
//		Resourcer Mode ?
//--------------------------------

	input = GetLibaryFilePtr();

	if (ArgRes)
	{
		TerminateSourceFile(1);
		input = GetLibaryFilePtr();

		if (ArgMasterDump)
			WriteSourceFile("_masterdump.s");

		MainRes(GetLibaryFilePtr(), output);
		DisposeLibrarian();
		ExitApp(0);
	}

//--------------------------------
//		Asm/Linker Mode
//--------------------------------

	if (ArgBuild)
	{
		TerminateSourceFile(1);
		input = GetLibaryFilePtr();

		if (ArgMasterDump)
			WriteSourceFile("_masterdump.s");

		MainAsm(GetLibaryFilePtr(), output);
		DisposeLibrarian();
		ExitApp(0);
	}

	printf("Error: No option selected, doing nothing\n");
	ExitApp(1);
	return 0;
}

//****************************************
//
//****************************************

void usage ()
{
	printf("pipe-tool Copyright 2007-2010 Mobile Sorcery AB\n");
	printf("Version 1.5 " __DATE__ " " __TIME__ "\n\n");
	printf("\n\
Usage: pipe-tool <mode> [options] outfile infile1 [infile2 ...]\n\
\n\
Mode Switches:\n\
  -L                   create library\n\
  -R                   compile resources\n\
  -B                   build application\n\
\n\
General options:\n\
  -h | --help          show this usage info\n\
  -error-paths         show file paths in errors\n\
	-xerr                extra information in case of errors\n\
	-master-dump         also dump the input into a single text file\n\
	-s<dir>              search <dir> for input libraries\n\
\n\
Build application (-B) options:\n\
  -entry=sym           set code entry point (default '%s')\n\
  -datasize=size       set data size (default %d)\n\
  -stacksize=size      set stack size (default %d)\n\
  -heapsize=size       set heap size (default %d)\n\
  -appcode=value       set application code (default '%s'(0x%x))\n\
  -p=vendor/model      link with device profile\n\
  -dump-syms           dump symbol tables\n\
  -dump-unref          dump unreferenced symbols\n\
  -sld=file            output source/line translation\n\
  -stabs=file          output debug information\n\
  -elim                eliminate unreferenced code/data\n\
  -no-verify           prevent code verification\n\
	-java                build a Java class file\n\
  -gcj=flags           for -java option: set flags for GCJ\n\
\n\
Resource compiler (-R) options:\n\
  -depend=file         output dependencies in makefile syntax\n\
\n\
Librarian (-L) options:\n\
	-quiet               don't display the component files\n\
\n\
Library mode (-L) is used to combine .s files into a .lib library file.\n\
In resource mode (-R), resource files are compiled to the output file.\n\
In normal mode (-B), one or more input files are built and linked to create a\n\
single output file.\n\n",
Code_EntryPoint,
Default_DataSize,
Default_StackSize,
Default_HeapSize,
AppCodeString(Default_AppCode),
Default_AppCode
);

}

//****************************************
//
//****************************************
/*
void AppExit(short code)
{
	exit(code);
}
*/
//****************************************
//
//****************************************

char appstr[5];

char * AppCodeString(int appcode)
{
	int v;
	memset(appstr, 0, sizeof(appstr));

	// Appcode 1
		
	v = (appcode >> 24) & 0xff;

	if (!isprint(v))
		v = '.';

	appstr[0] = v;


	// Appcode 2
		
	v = (appcode >> 16) & 0xff;

	if (!isprint(v))
		v = '.';

	appstr[1] = v;

	// Appcode 3
		
	v = (appcode >> 8) & 0xff;

	if (!isprint(v))
		v = '.';

	appstr[2] = v;

	// Appcode 4
		
	v = appcode & 0xff;

	if (!isprint(v))
		v = '.';

	appstr[3] = v;

	return appstr;
}


/****************************************
			 Compare a Chunk Name
****************************************/

char * GetRelPath(char *path)
{	
	int lbrac = -1;
	int len;
	int n;
	char c;
		
	relPath[0] = 0;
	
	len = strlen(path);
	
	if (len == 0)
		return relPath;
	
	for (n=0;n<len;n++)
	{
		c = path[n];
		
		if ((c == '\\') || (c == '/'))
		{
			lbrac = n;

			path[n] = '/';		// for window's
		}
	}	

	if (lbrac != -1)
	{
		strcpy(relPath, path);

		relPath[lbrac++] = '/';
		relPath[lbrac] = 0;
	}
	//dbprintf("Relative path set: %s\n", relPath);
	return relPath;
}


/****************************************
 Add the relative prefix to a file name
****************************************/

char relPrefix[1024];

char * AddRelPrefix(char *name)
{
	relPrefix[0] = 0;

	if (relPath[0])
		strcat(relPrefix, relPath);

	strcat(relPrefix, name);
	
//	printf("\nPrefix '%s'\n", relPrefix);
	return relPrefix;
}

/****************************************
			 Compare a Chunk Name
****************************************/

char tempName[1024];

char * GetMosyncPath(char *fpath, char *fname)
{	
	int len;
	
	tempName[0] = 0;

	if (mosync_dir)
	{	
		if (mosync_dir[0] == '"')
		{
			// Remove quotes on env setting

			strcat(tempName, mosync_dir+1);
			len = strlen(tempName);

			if (tempName[len-1] == '"')
				tempName[len-1] = 0;	
		}
		else
		{
			strcat(tempName, mosync_dir);
		}

		len = strlen(tempName);

		if (tempName[len-1] != '\\')
			strcat(tempName, "/");

		strcat(tempName, fpath);
	}

	strcat(tempName, fname);
	return tempName;
}

//****************************************
//			Main entry point
//****************************************

void LoadProfileData()
{
	char *filemem;
	int len;

	tempName[0] = 0;

	if (ProfilePath[0] == 0)
		return;

	// add the MOSYNC dir env

	if (mosync_dir)
	{	
		if (mosync_dir[0] == '"')
		{
			// Remove quotes on env setting

			strcat(tempName, mosync_dir+1);
			len = strlen(tempName);

			if (tempName[len-1] == '"')
				tempName[len-1] = 0;	
		}
		else
		{
			strcat(tempName, mosync_dir);
		}

		len = strlen(tempName);

		if (tempName[len-1] != '\\')
			strcat(tempName, "\\");
	}

	// finish off the file name
	
	strcat(tempName, "profiles\\vendors\\");
	strcat(tempName, ProfilePath);
	strcat(tempName, "\\profile.h");
	
	//printf("profile = '%s'\n", tempName);
	
	filemem = Open_FileAlloc(tempName);
	
	if (!filemem)
		return;

	ProfileReader(filemem, tempName);

	Free_File(filemem);
}

//****************************************
//			Main entry point
//****************************************

extern char * FilePtr;

void MainAsm(char *input, char *output)
{ 
	printf("pipe-tool Copyright 2007-2009 Mobile Sorcery AB\n");

	Stabs_Init();

	// Set up input
	
	FileTop = FilePtr = input;

	// Setup output file

	CodeFile = fopen(output,"wb");
	
	if (CodeFile == NULL)
	{
          if (AsmMsg) printf("Error : Problem creating '%s' RTL file\n", output);
          ExitApp(1);
	}

//#ifdef INCLUDE_JAVAGEN
//	if (ArgJavaNative)
//		JavaGen_Init();
//#endif

	if (InitSymbolTable())
	{
		InitExpMain();
		LoadProfileData();

		AsmMain();

#ifdef CODE_ANALYSIS

		if (DisasFunc[0])
			DisassembleFunc(DisasFunc, 0);
#endif


#if 0
	{
		char *codemem = ArrayPtr(&CodeMemArray, 0);
		int *constpool = VarPool;
		char distest[256];
		
		DisasInit(codemem, constpool);
		DisasInst(0, distest);

	}


#endif

	}

//#ifdef INCLUDE_JAVAGEN
//	if (ArgJavaNative)
//		JavaGen_Dispose();
//#endif

	if (CodeFile)
		fclose(CodeFile);

	CloseSymbolTable();
	Stabs_Dispose();
}


//****************************************
//			Main entry point
//****************************************

void MainRes(char *input, char *output)
{		
  	if (AsmMsg) printf("Resource Compiler Copyright 2007-2010 MoSync AB\n\n");

	FileTop = FilePtr = input;

	if ((CodeFile = fopen(output,"wb")) == NULL)
	{
		if (AsmMsg)
			printf("Error : Problem creating '%s' file\n", output);

		ExitApp(1);
	}

	if ((HeaderFile = fopen("MAHeaders.h","w") ) == NULL)
	{
		if (AsmMsg)
			printf("Error : Problem creating 'MAHeaders.h' file\n");

		fclose(CodeFile);
		ExitApp(1);
	}

	if(Do_Export_Dependencies)
	{
		if ((DependFile = fopen(DependName,"w") ) == NULL)
		{
			if (AsmMsg)
				printf("Error : Problem creating '%s' file\n", DependName);

			fclose(CodeFile);
			fclose(HeaderFile);
			ExitApp(1);
		}
		fprintf(DependFile, "%s: \\\n", output);
	}


	if (InitSymbolTable())
	{
		InitExpMain();
		LoadProfileData();

		ResourceMain();
	}

	if (HeaderFile)
		fclose(HeaderFile);

	if (CodeFile)
		fclose(CodeFile);

	if (DependFile) {
		fprintf(DependFile, "\n\n");
		fclose(DependFile);
	}

	CloseSymbolTable();
}

//****************************************
//		Performance counter init
//****************************************
#if 0
LARGE_INTEGER perf_begin;
LARGE_INTEGER perf_end;

LARGE_INTEGER perf_local_begin;
LARGE_INTEGER perf_local_end;

long long local_total_time = 0;
long long local_total_count = 0;

void PerfBegin()
{
	QueryPerformanceCounter(&perf_begin);
}

void PerfEnd()
{
	long long total_time = 0;
	double time_spent = 0.0;
	
	QueryPerformanceCounter(&perf_end);
	total_time = perf_end.QuadPart - perf_begin.QuadPart;
	
	if (local_total_time)
		time_spent = ((double) local_total_time / (double) total_time) * 100.0;
	
	printf("Time spent %f percent\n", time_spent);
	printf("Total calls %lld\n", local_total_count);

}


void PerfLocalBegin()
{
	QueryPerformanceCounter(&perf_local_begin);
}

void PerfLocalEnd()
{
	QueryPerformanceCounter(&perf_local_end);
	
	local_total_time += perf_local_end.QuadPart - perf_local_begin.QuadPart;
	local_total_count++;
}



//	printf("QuadPart %lld\n", pf.QuadPart);

#endif	//0

