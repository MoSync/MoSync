// MAINFUNC.CPP
//
// Copyright (c) 1997-2003 Symbian Ltd.  All rights reserved.
//

#include <iostream>
#include <toolsver.h>
#include "bmconv.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>

using namespace std;

const int KMaxArguments = 512;

const char* ErrorMessage(int aErrorNumber)
{
	switch(aErrorNumber)
	{
	case NoError:
		return "Success.";
	case NoMemory:
		return "Out of memory.";
	case Arg:
		return "Bad argument.";
	case Files:
		return "File does not exist";
	case SourceFile:
		return "Bad source file(s).";
	case DestFile:
		return "Bad destination file(s).";
	case CommandFile:
		return "Bad command file.";
	case OutOfRange:
		return "Number of sources/targets mismatch.";
	case TooManyArgs:
		return "Too many arguments.";
	case UnknownCompression:
		return "Unknown source compression type.";
	case CompressionError:
		return "Compression error.";
	case DecompressionError:
		return "Decompression error.";
	case Bpp:
		return "Invalid bitmap mode specified.";
	case PaletteFile:
		return "Bad palette file.";
	case PaletteSupportNotImplemented:
		return "Palettes not supported";
	default:
		return "Unknown error!";
	};
}

void Header()
{
	cout << "\n";
	cout << "\n";
	cout << "BMCONV version "<< version << ".\n";
}

void Report(int aError)
{
	Header();
	cout << ErrorMessage(aError) << "\n";
}

void CompilationReport(int aQuiet,int aError,TStoreType aType,char* aDestfile,char** aBitmapFiles,int aNumFiles)
{	
	if(!aQuiet || aError)
	{
		Header();
		cout << "Compiling...\n";
		if(aType!=ENoStore)
			cout << "Multiple bitmap store type: ";
		if(aType==EFileStore)
			cout << "File store" << "\n";
		else if(aType==ERomStore)
			cout << "ROM image store" << "\n";
		else if(aType==ECompressedRomStore)
			cout << "Compressed ROM image store" << "\n";
		if(aDestfile!=NULL)
			cout << "Epoc file: " << aDestfile << "\n\n";
		for(int count=0;count<aNumFiles;count++)
		{
			cout << "Bitmap file " << count+1 << "	: ";
			cout << aBitmapFiles[count] << "\n";
		}
		cout << ErrorMessage(aError) << "\n";
	}
}

void DecompilationReport(int aError,char* aDestfile,char** aBitmapFiles,int aNumFiles)
{	
	Header();
	cout << "Decompiling...\n";
	if(aDestfile!=NULL)
		cout << "Epoc file: " << aDestfile << "\n\n";
	for(int count=0;count<aNumFiles;count++)
	{
		cout << "Bitmap file " << count+1 << "	: ";
		cout << aBitmapFiles[count] << "\n";
	}
	cout << ErrorMessage(aError) << "\n";
}

void Usage()
{
	cout << "\n";
	cout << "BMCONV version "<< version << ".\n";
	cout << "Symbian OS multiple bitmap file/rom store conversion program.\n";
	cout << "Copyright (c) 1998-2001 Symbian Ltd.  All rights reserved.";
	cout << "\n";
	cout << "\n";
	cout << "Usage:\n";
	cout << "BMCONV [/r|/s|/n] [/hfilename] [/q] [/pfilename] epocfile [OPT]bmp_1 ... [OPT]bmp_n\n";
	cout << "BMCONV [/r|/s|/n] [/q] [/pfilename] epocfile /mepocfile2\n";
	cout << "BMCONV /u epocfile bmp_1 [... bmp_n]\n";
	cout << "BMCONV /v epocfile\n";
	cout << "BMCONV commandfile\n";
	cout << "\n";
	cout << " /r specifies a ROM image destination file,\n";
	cout << " /s specifies a compressed ROM image file,\n";
	cout << " /n disables bitmap File Store compression,\n";
	cout << " the default is a compressed File Store file.\n\n";
	cout << " /q specifies quiet mode - only errors are reported.\n\n";
	cout << " /hfilename specifies the filename for the automatic\n";
	cout << " generation of a header file for inclusion into code.\n\n";
	cout << " /pfilename gives the filename of a palette file containing 256 hex\n";
	cout << " numbers (0x00BBGGRR) specifying the palette for 8bpp colour bitmaps.\n";
	cout << " (Omission results in the use of a default palette.)\n\n";
	cout << " OPT may be one of /1, /2, /4, /8, /c4, /c8, /c12, /c16, /c24 /c32\n";
	cout << " specifying bits per pixel and grey-scale/colour, or /mepocfile2\n";
	cout << " to specify an existing multiple bitmap file. default is /2.\n\n";
	cout << " epocfile specifies the epoc multi-bitmap file name.\n";
	cout << " bmp_n specifies the nth bitmap file name.\n\n";
	cout << " /u decompiles epocfile to bmp_1,...,bmp_n\n";
	cout << " /v displays a summary of the bitmaps in epocfile\n";
	cout << " otherwise bmp_1,...,bmp_n are compiled to epocfile\n\n";
	cout << " commandfile specifies a file containing the commandline\n";
	cout << " with commands separated by spaces or newlines.\n";
}

int IsWhiteSpace(char aCharacter)
{
	return(aCharacter==' ' || aCharacter=='\n' || aCharacter=='\r' || aCharacter==0x1a);
}

int ProcessCommandFile(char* aCommandFileName,char** aArgPtrs,int& aNumArgs)
{
	struct stat fileinfo;
	if (stat(aCommandFileName,&fileinfo)==-1)
		return CommandFile;

	int filesize=fileinfo.st_size;
	if (filesize==0)
		return NoError;
	fstream commandfile(aCommandFileName,ios::in|ios::binary);
	if(!commandfile.is_open())
		return CommandFile;

	char* commandData=new char[filesize+1];
	if(commandData==NULL)
		return NoMemory;

	memset(commandData,0,filesize+1);
	commandfile.read(commandData,filesize);
	commandData[filesize]='\0';

	char* commandptr = (char*)commandData;
	char* commandptrLimit = (char*)(commandData + filesize);
	while (commandptr < commandptrLimit)
	{
		if(*commandptr=='/' && *(commandptr+1)=='/')
			while(*commandptr!='\n' && *commandptr!='\r' && commandptr < commandptrLimit)
				*commandptr++=' ';
		else if (*commandptr==0x1a)
			*commandptr++=' ';
		commandptr++;
	}

	commandptr = (char*)commandData;
	while (commandptr < commandptrLimit)
	{
		while(IsWhiteSpace(*commandptr) && commandptr < commandptrLimit)
			*commandptr++='\0';
		if (commandptr == commandptrLimit)
			break;
		aArgPtrs[aNumArgs]=commandptr;
		while(!IsWhiteSpace(*commandptr) && commandptr < commandptrLimit)
			commandptr++;
		if (commandptr == commandptrLimit)
			break;
		aNumArgs++;
	}

	commandfile.close();
	return NoError;
}

int Decompile(int aArgc,int aNumArgs,char** aArgPtrs)
{
	int ret=OutOfRange;
	char* destfilename=aArgPtrs[1];

	if(aArgc>=4 || aArgc==2)
	{
		for(int count=2;count<aNumArgs;count++)
		{
			EpocLoader pl;
			ret=pl.LoadEpocBitmap(destfilename,count-2);
			if(!ret) ret=pl.SaveBitmap(aArgPtrs[count]);
			if(ret) break;
		}
		DecompilationReport(ret,destfilename,&aArgPtrs[2],aNumArgs-2);
	}
	else
		DecompilationReport(ret,NULL,NULL,0);

	return ret;
}

int Compile(int aNumArgs,char** aArgPtrs)
{
	TStoreType storeType = EFileStore;
	bool compression = 1;
	int quiet = 0;
	char* headerfilename = NULL;
	char* palettefilename = NULL;
	char* destfilename = NULL;
	int ret = OutOfRange;

	for(int argnum=0;argnum<aNumArgs;argnum++)
	{
		if(aArgPtrs[argnum] && aArgPtrs[argnum][0] == '/')
		{
			if(aArgPtrs[argnum][1]=='r' || aArgPtrs[argnum][1]=='R')
			{
				storeType=ERomStore;
				aArgPtrs[argnum] = NULL;
			}
			else if(aArgPtrs[argnum][1]=='s' || aArgPtrs[argnum][1]=='S')
			{
				storeType=ECompressedRomStore;
				aArgPtrs[argnum] = NULL;
			}
			else if(aArgPtrs[argnum][1]=='n' || aArgPtrs[argnum][1]=='N')
			{
				compression=0;
				aArgPtrs[argnum] = NULL;
			}
			else if(aArgPtrs[argnum][1]=='h' || aArgPtrs[argnum][1]=='H')
			{
				headerfilename = &aArgPtrs[argnum][2];
				aArgPtrs[argnum] = NULL;
			}
			else if(aArgPtrs[argnum][1]=='q' || aArgPtrs[argnum][1]=='Q')
			{
				quiet=1;
				aArgPtrs[argnum] = NULL;
			}
			else if(aArgPtrs[argnum][1]=='p' || aArgPtrs[argnum][1]=='P')
			{
				palettefilename = &aArgPtrs[argnum][2];
				aArgPtrs[argnum] = NULL;
			}
		}
		else
			break;		// the RNHQP arguments must precede the output filename
	}

	int firstsource=0;
	while(firstsource<KMaxArguments && aArgPtrs[firstsource]==NULL)
		firstsource++;
	if(firstsource==KMaxArguments) firstsource=0;
	destfilename=aArgPtrs[firstsource];
	firstsource++;
	int numsources=firstsource;
	while(numsources<KMaxArguments && aArgPtrs[numsources]!=NULL)
		numsources++;
	if(numsources==KMaxArguments) numsources=0;
	numsources-=firstsource;

	if (numsources > 0)
	{
		BitmapCompiler mp(&aArgPtrs[firstsource],numsources);
		ret = mp.Compile(storeType,compression,destfilename,headerfilename,palettefilename);
	}

	CompilationReport(quiet,ret,storeType,destfilename,&aArgPtrs[firstsource],aNumArgs-firstsource);

	return ret;
}

void GetInfo(char* aSourceFile)
{
	Header();

	EpocLoader pl;
	int numSources=-1;
	int romFormat=0;
	int ret = pl.EpocBitmapCount(aSourceFile, numSources, romFormat);
	if (ret)
	{
		cout << "Problem reading number of bitmaps \n";
		cout << ErrorMessage(ret) << "\n";
		return;
	}

	cout << aSourceFile << " is a " << (romFormat? "ROM image":"File store") 
		<< " containing " << numSources << ((numSources==1)? " bitmap\n":" bitmaps\n");

	for (int count = 0;count<numSources;count++)
	{
		ret = pl.LoadEpocBitmap(aSourceFile,count);
		if (ret == OutOfRange)
			break;
		cout << "\n";
		if (ret)
		{
			cout << "Problem loading bitmap number " << count << "\n";
			cout << ErrorMessage(ret) << "\n";
			break;
		}
		else
		{
			SEpocBitmapHeader h = pl.Header();
			cout << "Bitmap " << count + 1 << " information:\n";
			cout << "Pixel size " << h.iWidthInPixels << " x " << h.iHeightInPixels << "\n";
			cout << "Twips size " << h.iWidthInTwips << " x " << h.iHeightInTwips << "\n";
			cout << h.iBitsPerPixel << " Bpp ";
			if (h.iColor)
				cout << "Colour";
			else
				cout << "Monochrome";
			cout << "\n";
			if (h.iPaletteEntries > 0)
				cout << "Palette entries " << h.iPaletteEntries;

			int byteSize = BitmapUtils::ByteWidth(h.iWidthInPixels,h.iBitsPerPixel) * h.iHeightInPixels;
			int compressionRatio = 0;
			if (byteSize > 0)
				compressionRatio = (h.iBitmapSize - sizeof(SEpocBitmapHeader)) * 100 / byteSize;

			switch (h.iCompression)
			{
			case ENoBitmapCompression:
				cout << "No compression\n";
				break;
			case EByteRLECompression:
				cout << "Bytewise RLE compression " << compressionRatio << "%\n";
				break;
			case ETwelveBitRLECompression:
				cout << "12 bit RLE compression " << compressionRatio << "%\n";
				break;
			case ESixteenBitRLECompression:
				cout << "16 bit RLE compression " << compressionRatio << "%\n";
				break;
			case ETwentyFourBitRLECompression:
				cout << "24 bit RLE compression " << compressionRatio << "%\n";
				break;
			}
		}
	}

	cout << "\n";
}


#ifdef _MSC_VER
#define __VERSION__ _MSC_VER
#endif

static int check_compiler(void)
{
	struct TBitmapFileHeader hdr;
	const size_t base = (size_t)&hdr.bfType;
	const size_t offs = (size_t)&hdr.bfSize;
	const size_t res1 = (size_t)&hdr.bfReserved1;

	if((offs-base != 2) || (res1-base != 6)) {
		printf("compiler struct align error (0x%lx,0x%lx,0x%lx) (%s)\n",
			base, offs, res1, __VERSION__);
		return 1;
	}

	if(sizeof(struct SEpocBitmapHeader) != 40 ||
		sizeof(struct TBitmapFileHeader) != 14 ||
		sizeof(struct TBitmapInfoHeader) != 40) {
			printf("compiler struct packing error (%u,%u,%u) (%s)\n",
				(int)sizeof(struct SEpocBitmapHeader),
				(int)sizeof(struct TBitmapFileHeader),
				(int)sizeof(struct TBitmapInfoHeader),
				__VERSION__);
			return 1;
	}

	return 0;
}

int main(int argc,char* argv[],char* [])
{
	if(check_compiler())
		exit(1);

	if (argc <= 1)
	{
		Usage();
		return 0;
	}

	if (argc > KMaxArguments)
	{
		Report(TooManyArgs);
		return 0;
	}

	char* argptrs[KMaxArguments]; // Maximum number of argument handled is KMaxArguments
	for (int ptrcount = 0; ptrcount < KMaxArguments; ptrcount++)
		argptrs[ptrcount] = NULL;

	int numargs = 0;
	if(argc>2) // Explicit arguments are present
	{
		for(int count=0;count<argc-1;count++)
			argptrs[count]=argv[count+1];
		numargs = argc-1;
	}
	else // The single argument must be a command file name
	{
		int ret = ProcessCommandFile(argv[1],argptrs,numargs);
		if (ret)
		{
			Report(ret);
			return 0;
		}
	}

	if (argptrs[0]!=NULL && argptrs[0][0]=='/' && (argptrs[0][1]=='u' || argptrs[0][1]=='U'))
		return Decompile(argc,numargs,argptrs);

	if (argptrs[0]!=NULL && argptrs[0][0]=='/' && (argptrs[0][1]=='v' || argptrs[0][1]=='V'))
	{
		GetInfo(argptrs[1]);
		return 0;
	}

	return Compile(numargs,argptrs);
}

