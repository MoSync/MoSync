/*
* Copyright (c) 2000-2010 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of the License "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description: 
*
*/


#include <assert.h>
#include <stdio.h>
#include <string.h>


#include "unistd.h"

#if defined( __MSVCDOTNET__) || defined(__TOOLS2__)
#include <iostream>
#include <fstream>
using std::cout;
using std::endl;
using std::cerr;
#ifdef WIN32
#define snprintf _snprintf
#endif
#else //!__MSVCDOTNET__
#ifndef __LINUX__
#include <io.h>
#endif //!__LINUX__
#endif //__MSVCDOTNET__

#include "resource.h"
#include "datatype.h"
#include "mem.h"     
#include "rcbinstr.h"
#include "numval.h"  
#include "errorhan.h"
#include "fileacc.h" 
#include "version.h" 
#include "ctable.h"  
#include "main.h"
#include "tokens.h"
#include "localise.h"
#include "qualifar.h"
#include "messages.h"

extern NameIdMap* pResourceNameIds;
extern long CurrentId;

static void WriteHeaderFile(FILE* aRSG, IndexTable& aIndex)
	{
	IndexTableIterator next(aIndex);
	IndexTableItem * p;
	while( ( p = next() ) != NULL)
		{
		ResourceHeader& r=p->Resource();
		if (r.iLabel.Length()>0 && !r.iLocal)
			{
			r.iLabel.Upper();
			if (r.iFormatAsHex)
				fprintf(aRSG, "#define %-41s 0x%lx\n", r.iLabel.GetAssertedNonEmptyBuffer(), r.iResourceId);
			else
				fprintf(aRSG, "#define %-41s %ld\n", r.iLabel.GetAssertedNonEmptyBuffer(), r.iResourceId);
			}
		}
	}

static void WriteBitArrayOfResourcesContainingCompressedUnicode(RCBinaryStream& aRSC, const IndexTable& aIndex)
	{
	IndexTableIterator next(aIndex);
	unsigned char bitBuffer = 0;
	int numberOfBitsInBuffer = 0;
	for (;;)
		{
		IndexTableItem* const p = next();
		if (p == NULL)
			{
			if (numberOfBitsInBuffer > 0)
				{
				aRSC.Write(&bitBuffer, 1);
				}
			break;
			}
		if (p->Resource().ContainsCompressedUnicode())
			{
			bitBuffer |= (1 << numberOfBitsInBuffer);
			}
		++numberOfBitsInBuffer;
		if (numberOfBitsInBuffer == 8)
			{
			aRSC.Write(&bitBuffer, 1);
			bitBuffer = 0;
			numberOfBitsInBuffer = 0;
			}
		}
	}

static void WriteBinaryResourceData(RCBinaryStream& aRSC, IndexTable& aIndex, int& aSizeOfLargestResourceWhenUncompressed, const char* aDumpDirectory)
	{
	IndexTableIterator next(aIndex);
	IndexTableItem * p;
	int resourceIndex=1;
	while( ( p = next() ) != NULL)
		{
		char* dumpFile=NULL;
		if (aDumpDirectory!=NULL)
			{
			dumpFile=new char[strlen(aDumpDirectory)+20];
			strcpy(dumpFile, aDumpDirectory);
			char resourceIndexAsString[20];
			sprintf(resourceIndexAsString, "%d", resourceIndex);
			strcat(dumpFile, resourceIndexAsString);
			}
		p->SetOffset(aRSC.GetPosition()); // record start of this resource in the index
		p->Resource().StreamOut(aRSC, aSizeOfLargestResourceWhenUncompressed, dumpFile); // write out binary form of resource
		delete [] dumpFile;
		++resourceIndex;
		}
	}

static void WriteResourceFile(RCBinaryStream& aRSC, IndexTable& aIndex, bool aThirdUidIsOffset, const char* aDumpDirectory)
	{
	char flags=0;
	if (aThirdUidIsOffset)
		{
		flags|=0x01;
		}
	aRSC << flags; // these flags are to be used only by a dictionary-compressing program rather than to be used directly by Bafl when reading non-dictionary-compressed resource files (as output by Rcomp)
	const int positionToOverWriteFrom=aRSC.GetPosition();
	NumericValue twoByteSizeOfLargestResourceWhenUncompressed(L_WORD);
	aRSC << twoByteSizeOfLargestResourceWhenUncompressed;
	WriteBitArrayOfResourcesContainingCompressedUnicode(aRSC, aIndex); // simply makes space for the bit-array without writing anything sensible in it (as we don't yet know which resources will contain compressed Unicode)
	int sizeOfLargestResourceWhenUncompressed=0;
	WriteBinaryResourceData(aRSC, aIndex, sizeOfLargestResourceWhenUncompressed, aDumpDirectory);
	aIndex.SetIndexOffset(aRSC.GetPosition());
	aRSC << aIndex;
	aRSC.SetPosition(positionToOverWriteFrom);
	twoByteSizeOfLargestResourceWhenUncompressed=sizeOfLargestResourceWhenUncompressed;
	aRSC << twoByteSizeOfLargestResourceWhenUncompressed;
	WriteBitArrayOfResourcesContainingCompressedUnicode(aRSC, aIndex); // overwrites the bit array with correct data

	if(verbose) 
		{
		MOFF; cout << aIndex; cout << endl; MON;
		}
	}

static void CheckLabels() // checks whether the labels that are used in the input have been declared
	{
	QualifiedStringArrayIterator nextLabel(pG->UsedIdentifiers);
	QualifiedString * pLabel;
	while ((pLabel = nextLabel() ) != NULL)
		{
		bool found = false; // gets set to true if the label in question is declared
		StringArrayIterator nextDeclared(pG->AllIdentifiers);
		String * pDeclared;
		while ( ( (pDeclared = nextDeclared() ) != NULL) && ( ! found ))
			{
			StringLess stringCompare;
			if( !stringCompare(*pDeclared,(*pLabel).GetEntry()) && !stringCompare((*pLabel).GetEntry(),*pDeclared) )
				{ // this comparison returns true if the label is the same as the declared label
				found = true;
				}
			}
		if( ! found ) // if label hasn't been declared emit warning
			{
			Message * message = pG->Messages.GetEntry(LT_045);
			String fileName = (*pLabel).GetFileName();
			int lineNumber = (*pLabel).GetLineNumber();
			if(message->GetActivated())
				{
				String comment = message->GetMessageOutput();
				comment += (*pLabel).GetEntry();
				ErrorHandler::OutputWholeLine(fileName, lineNumber, comment);
				}
			}
		}
	}


/*  Tokenize expects a string in the following format:
 *	  \d{3}(,\d{3})*
 *  i.e. comma-separated three digit numbers. 
 *  The string should contain no whitespace.
 */
static void Tokenize(String aString)
	{
	int length = aString.Length();

	for(int end=3; end<=length; end+=4)
		{
		String messageNumber = aString.ExtractSubString(end-3,end-1);
		if(messageNumber.IsDecNatural())
			{
			Message * message = pG->Messages.GetTextEntry(messageNumber);
			if(message != NULL)
				{
				message->SetActivationStatus(false);
				}
			}
		}
	}
				

static void OutputHelp()
	{
	cerr << "Resource compiler version " << version << " (Build " << build << ") (C) 1997-2010 Nokia Corporation." << endl;
	cerr << "Usage: rcomp [-vpul] [-force] [-oRSCFile] [-{uid2,uid3}] [-hHeaderFile] [-sSourceFile] [-iBaseInputFileName]" << endl;
	cerr << "\tv" << "\tverbose" << endl;
	cerr << "\tp" << "\tParser debugging" << endl;
	cerr << "\tl" << "\tCheck localisation comments" << endl;
	cerr << "\tforce" << "\tEmit localisation warnings even if no localisation tags are present" << endl;
	cerr << "\tadd-defaults" << "\tAmend input rss/rpp file to add missing default localisation options" << endl;
	cerr << endl;
	cerr << "\tu" << "\tGenerate Unicode resource binary" << endl;
	cerr << endl;
	cerr << "If no source file is specified, the source will be read from standard input." << endl;
	cerr << "(-i is used to specify the file given to the preprocessor this " << endl;
	cerr << "    name is used in generating debug output.)" << endl;
	}



GlobalData *pG;
GlobalLocalisationData *pGL;
String InputBaseName;

int main(int argc, char * argv[])
	{
	cout << "\n"; 
	int vParam=0;
	bool lParam = false; // used as flag to specify whether checking of localisation comment tags should be performed
	bool lForce = false; // used as flag to force localisation output even if there are no localisation comments
	bool lAddDefaults = false; // used as flag to add missing default localisation data to input rss/rpp file, this is not the default behaviour
	logmemorysetting = 0;
	unsigned short	pParam = 0;
	String			DataOutputFileName;
	String			HeaderOutputFileName;
	String			MessageSuppressionList;
	String			BasePath;
	String			SourceFileName;
	FILE *			pSourceFile;
	char*			uidsParameter=NULL;
	char*			dumpDirectory=NULL;
	fpos_t			filePositionIndicator;
	int				i;
	
	char *fullcommand = argv[0];
	std::string s(fullcommand);

	if(argc<=1)
		{
		OutputHelp();
		exit(-1);
		}
	else
		{
		// Look through arguments for ones beginning with '-?'.
		for(i = 1; i < argc; i++)
			{
			if(* argv[i] == '-')
				{
				char * settings = argv[i] + 1;
			
				if(strchr(settings, '?') )
					{
					OutputHelp();
					exit(-1);
					}
				}
			}

		for(i = 1; i < argc; i++)
			{
			if(* argv[i] == '-')
				{
				char * settings = argv[i] + 1;
			
				if(* settings == 'o' || * settings == 'O')
					{
					DataOutputFileName = (settings + 1);
					continue;
					}

				if(* settings == 'm' || * settings == 'M')
					{
					MessageSuppressionList = (settings + 1);
					continue;
					}
					
				if(* settings == 'h' || * settings == 'H')
					{
					HeaderOutputFileName = (settings + 1);
					continue;
					}

				if(* settings == 'i' || * settings == 'I')
					{
					InputBaseName = (settings + 1);
					String DriveAndDirectory = FileAccess::GetDriveAndDirectory(InputBaseName);
					BasePath = FileAccess::FullPath(DriveAndDirectory);
					continue;
					}

				if(* settings == 's' || * settings == 'S')
					{
					SourceFileName = (settings + 1);
					continue;
					}

				if(* settings == '{')
					{
					uidsParameter = settings + 1;
					char* temp = strchr(uidsParameter, ',');
					if ((temp == NULL) || (temp == uidsParameter) || (strchr(temp + 1, ',') != NULL)) // check that there is *one* comma in this parameter (no more and no less), and that it is not the first thing immediately after the '{'
						{
						OutputHelp();
						exit(-1);
						}
					*temp = ' ';
					temp = strchr(uidsParameter, '}');
					if ((temp == NULL) || (temp[1] != '\0'))
						{
						OutputHelp();
						exit(-1);
						}
					*temp = ' ';
					continue;
					}

				if(* settings == ':')
					{
					dumpDirectory=settings+1;
					continue;
					}

				if(strchr(settings, 'u') || strchr(settings, 'U') )
					{
					SourceCharacterSet = String::CP1252;
					TargetCharacterSet = String::Unicode;
					}

				if(strchr(settings, 'v') || strchr(settings, 'V') )
					vParam = 1;
				if(strchr(settings, 'p') || strchr(settings, 'P') )
					pParam = 1;
				if(strchr(settings, 'l') || strchr(settings, 'L') )
					lParam = true;
				if(strchr(settings, 'f') || strchr(settings, 'F') )
					lForce = true;
				if(strchr(settings, 'a') || strchr(settings, 'A') )
					lAddDefaults = true;
				}
			}
		}
	if(SourceFileName.Length() == 0)
		{
		pSourceFile = NULL ; //stdin;
		}
	else  
	{
		if((pSourceFile = fopen(SourceFileName.GetAssertedNonEmptyBuffer(), "r") ) == NULL)
		{
			cerr << "Failed to open " << SourceFileName << endl;
			exit(-2);
		}
		//Searchig for BOM signature which if found will be ignored
		unsigned char buffer[3];
		int res = fread( buffer, sizeof( char ), 3, pSourceFile);
		if(res < 0) {
			perror("fread error");
			exit(-2);
		}
		if((buffer[0]!=239) && (buffer[1]!=187) && (buffer[2]!=191))
		{
			// BOM not found. Set the file-position indicator to 0
			filePositionIndicator = fpos_t();
			if(fsetpos(pSourceFile, &filePositionIndicator) !=0)
			{
				perror("fsetpos error");
				exit(-2);
			} 
		}		
	}
		
	verbose = vParam;
	pG = new GlobalData;
	if (pG==NULL)
		exit(-4);
		
	if(NULL == pSourceFile){ // Cache the standard input		
		pG->StdInBufLength = 0; 
		unsigned long allocSize = 0x100000 ; // 1M bytes
		pG->StdInfBufPos = 0 ;
		pG->StdInBuffer = (char*)malloc(allocSize);
		if(NULL == pG->StdInBuffer){
			delete pG ;
			exit(-4);
		}
		int result = 1;
		char* buffer = pG->StdInBuffer;
		FILE *file = stdin ;
		const int KIOBytes = 0x20000 ;
		while(1) {		 
			result = fread(buffer, 1, KIOBytes, file);  
			if(result == 0){
				*buffer = 0;
				break ;
			}
			buffer += result ;
			pG->StdInBufLength += result ;
			if((pG->StdInBufLength + KIOBytes) > allocSize) {
				allocSize += KIOBytes ;
				pG->StdInBuffer = (char*)realloc(pG->StdInBuffer,allocSize);
				if(NULL == pG->StdInBuffer){
					delete pG ;
					exit(-4);
				}
				buffer = pG->StdInBuffer + pG->StdInBufLength;
			}
			
		}
		const unsigned char BOM[] = {0xef , 0xbb, 0xbf, 0x0 };
		if(0 == memcmp(pG->StdInBuffer,BOM,3))
			pG->StdInfBufPos = 3 ; 
	}
	else {
		pG->StdInBuffer = 0 ;
		pG->StdInBufLength = 0; 
	}
	
	Tokenize(MessageSuppressionList);

	pGL = new GlobalLocalisationData;
	if(pG==NULL)
		exit(-4);

	pG->WarningMultiExplained = false;
	pG->FileLineHandler.SetPath(BasePath);
	
	#ifdef __TOOLS2__
	pG->FileLineHandler.SetBase(SourceFileName,0);
	#endif
	
	int ret=ParseSourceFile(pSourceFile, pParam);
	if(pSourceFile != NULL)
		fclose(pSourceFile);
	if(pG->StdInBuffer != NULL)
		free(pG->StdInBuffer);
	
	pGL->StoreFinalComment(); // final comment not stored during running of lex and yacc
	if(lParam && (pGL->LocalisationCommentsExist() || lForce))
		{
		pGL->AnalyseLocalisationData();
		pGL->PrintLocalisationWarnings();
		if(lAddDefaults)
			{
			// only add deafult localisation values to rpp/rss file if the option has been set on the command line
			if(verbose)
				{
				cout << "* Reparsing source file and adding any missing default localisation comments" << endl;
				}
			pGL->OutputLocalisedFile(SourceFileName);
			}
		}
	if (ret != 0) 
		{
		cerr << "RCOMP failed with code " << ret << endl;
		exit(ret);
		}
	// A successful parse, now generate the output files

	CheckLabels(); // check that all labels are declared and emit suitable warnings if not

	if(DataOutputFileName.Length() != 0)
		{


#ifdef __LINUX__

		std::string totalpath(s.substr( 0, s.rfind("/")+1 ));
		const char* uidTool = "uidcrc";

#else

		std::string totalpath(s.substr( 0, s.rfind("\\")+1 ));
		const char* uidTool = "uidcrc.exe";
    
		// in case unix style of seperator is used in windows.
		if(!totalpath.length()){
			totalpath = s.substr( 0, s.rfind("/")+1 );
		}
		
#endif
		
		std::string uidpath(uidTool);
		
		// try to call the uidcrc tool with the full path to where RCOMP resides in
		// if does not exists, search uidcrc in PATH 
		if(totalpath.length()){
		
			std::string trypath(totalpath);
			trypath += uidpath;
			
			std::fstream _file;
			_file.open(trypath.c_str(), std::ios::in);
			if(!_file){
				// try to search from PATH
				totalpath = uidpath;
			}
			else{
				_file.close();
				// invoke from the path RCOMP resides in
				totalpath += uidpath;
			}
		}
		else{
			// search from PATH
			totalpath = uidpath;
		}
		
		// Find and replace all occurences of \ with /
		std::string searchString( "\\" ); 
		std::string replaceString( "/" );
 		std::string::size_type pos = 0;
	    	while ( (pos = totalpath.find("\\", pos)) != std::string::npos ) {
        		totalpath.replace( pos, searchString.size(), replaceString );
		        pos++;
		    }
			
		const char *uidcrcTool = totalpath.c_str();

		bool thirdUidIsOffset=true;
		
		char uidcrcUIDs[3][100];
		strcpy (uidcrcUIDs[0], "0x101f4a6b");

		if (uidsParameter)
			{
			// Command line argument takes precedence 
			
			strcpy (uidcrcUIDs[1], strtok (uidsParameter, " "));
			strcpy (uidcrcUIDs[2], strtok (NULL, " "));
 
			char* const temp = strchr(uidcrcUIDs[2], '*');		
			if (temp == NULL)
				{
				thirdUidIsOffset=false;
				}
			}
		else
			{
			// otherwise use values supplied in source

			extern unsigned long Uid2;
			extern unsigned long Uid3;
			sprintf(uidcrcUIDs[1], "0x%08lx", Uid2);
			if (Uid3 != 0)
				{
				sprintf(uidcrcUIDs[2], "0x%08lx", Uid3);
				thirdUidIsOffset=false;
				}
			}


		if (thirdUidIsOffset)
			{
			const unsigned int idOfAnyResource = CurrentId; // *must* be unsigned so that when we right-shift it, the top bit doesn't get propagated if its set (i.e. "negative")
			sprintf(uidcrcUIDs[2], "0x%08x", idOfAnyResource >> 12); // use the 20 bits derived from the resource file's NAME as the 3rd UID
			}

		if (verbose)
			{
			MOFF; cout << uidcrcTool << " " << uidcrcUIDs[0] << " " << uidcrcUIDs[1] << " " << uidcrcUIDs[2] << " " << DataOutputFileName.GetAssertedNonEmptyBuffer(); cout << endl; MON;
			}

		char uidcrc_params[512];
		const int ret = snprintf(uidcrc_params,
					 sizeof(uidcrc_params),
					 "%s %s %s %s %s",
					 uidcrcTool,
					 uidcrcUIDs[0],
					 uidcrcUIDs[1],
					 uidcrcUIDs[2],
					 DataOutputFileName.GetBuffer());
		if(ret <= 0) {
			cerr << "Failed to write UIDs to " << DataOutputFileName << endl;
			exit(ret);
		}
		const int error = system(uidcrc_params);

		if(error != 0)
			{
			cerr << "Failed to write UIDs to " << DataOutputFileName << endl;
			exit(error);
			}
		RCBinaryStream RSCStream;
		RSCStream.OpenForAppend(DataOutputFileName);
		if(! RSCStream.IsOpen())
			{
			cerr << "Failed to open " << DataOutputFileName << endl;
			exit(-2);
			}
		WriteResourceFile(RSCStream, pG->Index, thirdUidIsOffset, dumpDirectory);
		}
	
	if(HeaderOutputFileName.Length() != 0)
		{
		FILE* RSG;
		RSG = fopen(HeaderOutputFileName.GetAssertedNonEmptyBuffer(), "w");
		if(RSG==NULL)
			{
			cerr << "Failed to open " << HeaderOutputFileName << endl;
			exit(-2);
			}
		WriteHeaderFile(RSG, pG->Index);
		fclose(RSG);
		}

	delete pG;
	delete pGL;

	return 0;
	}




