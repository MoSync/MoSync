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

#include "config_platform.h"
#ifdef GUIDO

#include <hal.h>
#include <ctype.h>

#include <base/FileStream.h>

#include "syscall.h"
#include "AppView.h"
#include "core.h"
#include "errors.h"

#include <helpers/CPP_IX_GUIDO.H>



//#define SWEDISH
#define ENGLISH

#define ERIK

#if defined(SWEDISH) && defined(ENGLISH)
#error Only one language allowed.
#endif

#ifdef BABILE2
#if 0
#include "babmobs.lic.h"
#include "babmobs.lic.password.h"
#else
#include "babdevlopper.lic.h"
#include "babdevlopper.lic.password.h"
#endif	//0
#endif	//BABILE2

//***************************************************************************
//Con/de-structor
//***************************************************************************

void Syscall::ClearGuidoVariables() {
	gAudioStream = NULL;
	gBuffersQueued = gNumAllocs = gCurrentAudioBuffer = 0;
	ZERO_ARRAY(gAllocArray);
	gNumAllocs = 0;
	gMemTab = NULL;
	gBabileObj = NULL;
	gVolume = 100;
	gTtsNextId = 1;
}

static void BB_log_error(const char* name, const uint error) {
	int severity = ((error) & BB_ERRORSEVERITYMASK) >> (BB_ERRORSEVERITYSHIFT+1);
	int mod = ((error) & BB_ERRORMODIDMASK) >> (BB_ERRORMODIDSHIFT+1);
	int submod = ((error) & BB_ERRORSUBMODIDMASK) >> (BB_ERRORSUBMODIDSHIFT+1);
	int id = (error) & BB_ERRORIDMASK;
	LOG("%s: 0x%08x(%i)(%x %x %x %x)\n", name, error, error, severity, mod, submod, id);
}

static void dumpDba(const BB_DbLs* db) {
	if(db == NULL) {
		LOG("Database: NULL\n");
		return;
	}
	LOG("Database:\n");
	while(true) {
		LOG(" Descriptor: %s\n", db->descriptor);
		BB_DbId* id = db->pDbId;
		if(!id)
			break;
		LOG(" Type: %i Flags: %i Size: %i\n", id->type, id->flags, id->size);
		if(id->type == X_FILE) {
			LOG("  Link: %s\n", id->link);
		}
		db++;
	}
}

#define DUMPI(name) LOG("%s: %i\n", #name, mp->name)
#define DUMPX(name) LOG("%s: 0x%x\n", #name, mp->name)
#define DUMPS(name) LOG("%s: '%s'\n", #name, mp->name ? mp->name : "null")

static void dumpBab(const BABILE_MemParam* mp) {
	DUMPI(sSize);
	DUMPI(sMBRConfig); 
	DUMPI(sSELConfig); 
	DUMPI(sNLPConfig); 
	DUMPI(nlpModule);
	DUMPI(synthModule);
	DUMPI(u32MarkCallbackInstance);
	DUMPX(markCallback);
	DUMPI(nlpInitError);
	DUMPI(mbrInitError);
	DUMPI(selInitError);
	DUMPI(initError);
	DUMPS(license);
	DUMPX(uid.userId);
	DUMPX(uid.passwd);

	dumpDba(mp->nlpeLS);
	dumpDba(mp->synthLS);
}

static BB_S32 myMarkCallback(BB_S32 a, BB_S32 b, BB_S32 c, BB_U32 d) {
	LOG("markCallback(%i, %i, %i, %i)\n", a, b, c, d);
	return 0;
}

void Syscall::InitGuidoL() {
	LOG("InitGuidoL\n");
	/*for(int i=0; ; i++) {
		GPSLOGILENAME.Format(_L("C:\\gpslog%i.txt"), i);
		FileStream testfile(GPSLOGILENAME, false);
		if(!testfile.isOpen())
			break;
		i++;
	}
	gGpsLogFile = new (ELeave) FileStream(GPSLOGILENAME, true);
	LogDateTime();*/

#ifdef BABILE2
	MyRFs myrfs;
	myrfs.Connect();

	// read ini file... but which one?
	// read voice selection file
	char chosenDrive;
	char* voiceSelection = GetFileL("C" BABEL_BASE "voice.txt");
	if(voiceSelection) {
		chosenDrive = 'C';
	} else {
		char* voiceSelection = GetFileL("E" BABEL_BASE "voice.txt");
		if(!voiceSelection) {
			_LIT(KErrText, "Could not read "BABEL_BASE"voice.txt");
			ShowAknErrorNoteThenExitL(KErrText);
		}
		chosenDrive = 'E';
	}
	LOG("voiceSelection: '%s'\n", voiceSelection);
	// now we know which directory.
	// scan the directory for ini files, read the first one we find.
	TBuf8<KMaxFileName> voicePath;
	voicePath.Format(_L8("%c%s%s\\"), chosenDrive, BABEL_BASE, voiceSelection);
	TBuf16<KMaxFileName> iniPathMask;
	Append(iniPathMask, voicePath);
	_LIT(KIniMask, "*.ini");
	iniPathMask.Append(KIniMask);
	CDir* dir;
	LOG("GetDir(%s*.ini)\n", voicePath.PtrZ());
	LHEL(FSS.GetDir(iniPathMask, KEntryAttMaskSupported, ESortNone, dir));
	if(dir->Count() < 1) {
		_LIT(KErrText, "No ini file found");
		ShowAknErrorNoteThenExitL(KErrText);
	}
	TBuf8<KMaxFileName> iniPath;
	iniPath.Copy(voicePath);
	Append(iniPath, (*dir)[0].iName);
	char* iniContents = GetFileL(CCP iniPath.PtrZ());
	// what a lot of mucking about with buffers. that's Symbian for ya.
	
	// now we must parse the file.
	// skip empty lines.
	// first comes PATH, then the files for the Language Dba, terminated by an END line.
	// then comes the files for the Voice Dba, terminated by an END line.
	// then maybe a TXT, but we don't care about that.
	// each file has the format: <id> <quoted string> <description>\n
	// we'll ignore the description.
#endif

	{
		BB_S16 i, nBlocks;
		BABILE_MemParam babParam;
		ZERO_OBJECT(babParam);
		babParam.sSize = sizeof(babParam);
		babParam.sNLPConfig=0;
		dumpBab(&babParam);
#ifdef BABILE2
		babParam.sMBRConfig = BABILE_MBRE_BUFFERING10;
		babParam.sSELConfig = BUFFER_UNITACOUSTIC_ALL|BUFFER_UNITSOFFSETS_ALL|BUFFER_BERSTREAM_ALL|20;
		babParam.nlpModule = BABILE_NLPMODULE_NLPE;
		babParam.synthModule = BABILE_SYNTHMODULE_BBSELECTOR;
		babParam.markCallback = myMarkCallback;
		
		char* iniPtr = iniContents;
		voicePath[voicePath.Length() - 1] = '/';	// fugly hack
		babParam.nlpeLS = initDbaL(CCP voicePath.PtrZ(), iniPtr, babParam.synthLS);
		DEBUG_ASSERT(babParam.nlpeLS);
		DEBUG_ASSERT(babParam.synthLS);
		
		babParam.license = babLicense;  /* load your license file */
		babParam.uid.passwd=uid.passwd;	/* your password */
		babParam.uid.userId=uid.userId; /* your ID */
		
#else	// old version
		babParam.sMBRConfig=0;
		babParam.mbreLS = initVoiceDbaL();
		DEBUG_ASSERT(babParam.mbreLS);

		babParam.nlpeLS = initLanguageDbaL();
		DEBUG_ASSERT(babParam.nlpeLS);
#ifdef SWEDISH
		babParam.nlpModule = BABILE_NLPMODULE_TTF;
#elif defined(ENGLISH)
		babParam.nlpModule = BABILE_NLPMODULE_NLPE;
#else
#error Unsupported language
#endif	//SWEDISH
#endif	//BABILE2

		// memory descriptor creation
		nBlocks = BABILE_numAlloc();
		LOG("B_numAlloc %i\n", nBlocks);
		gMemTab = AllocZero<BB_MemRec>(nBlocks);
		if(!gMemTab) {
			LOG("BB_MemRec error!\n");
			User::Leave(ERROR_BABILE);
		}
		//PMemZero(gMemTab, nBlocks*sizeof(BB_MemRec));

		dumpBab(&babParam);
		LOG("B_alloc\n");
		int blocksToAlloc = BABILE_alloc(&babParam, gMemTab);
		LOG("blocksToAlloc: %i\n", blocksToAlloc);
		if(nBlocks != blocksToAlloc) {
			LOG("BB_Alloc discrepancy: %i != %i\n", nBlocks, blocksToAlloc);
			User::Leave(ERROR_BABILE);
		}
		dumpBab(&babParam);

		for(i=0; i<nBlocks; i++) {
			BB_MemRec& mr(gMemTab[i]);
			LOG("%i: size %i, space %i, attrs %i, align %i\n",
				i, mr.size, mr.space, mr.attrs, mr.alignment);
			if(mr.size > 0 && mr.space != BB_IALG_NONE) {
				mr.base = VoidAllocZero(mr.size);
			} else {
				mr.base = NULL;
			}
		}
		if(gMemTab[2].base == 0) {
			LOG("gMemTab error!\n");
		}
		
		babParam.selInitError = 0;
		babParam.initError = 0;
		
		dumpBab(&babParam);

		LOGD("B_init\n");
		if(!(gBabileObj = BABILE_init(gMemTab, &babParam))) {
			LOG("B_init failed!\n");
			BB_log_error("nlp", babParam.nlpInitError);
			BB_log_error("mbr", babParam.mbrInitError);
			BB_log_error("sel", babParam.selInitError);
			BB_log_error("init", babParam.initError);
			DEBIG_PHAT_ERROR;
		}
		LOGD("0x%X\n", gBabileObj);
		
		BB_S32 temp = 0xf00d;
		/*//retrieve the default volume
		CHECK_NZ(BABILE_getSetting(gBabileObj, BABIL_PARM_VOLUME, &temp));
		LOG("vol: %i\n", temp);*/
		
		temp = 0xf00d;
		//retrieve the output frequency
#ifdef BABILE2
		CHECK_NZ(BABILE_getSetting(gBabileObj, BABIL_PARM_VOICEFREQ, &temp));
#else
		CHECK_NZ(BABILE_getSetting(gBabileObj, BABIL_PARM_VOICE, &temp));
#endif
		LOG("freq: %i\n", temp);
		
		/*temp = 0xf00d;
		//retrieve the default volume ratio
		CHECK_NZ(BABILE_getSetting(gBabileObj, BABIL_PARM_VOLUMERATIO, &temp));
		LOG("volrat: %i\n", temp);*/
		
		gSpeed = 0xf00d;
		//retrieve the default speed
		CHECK_NZ(BABILE_getSetting(gBabileObj, BABIL_PARM_SPEED, &gSpeed));
		LOG("speed: %i\n", gSpeed);
		
		gPitch = 0xf00d;
		//retrieve the default pitch
		CHECK_NZ(BABILE_getSetting(gBabileObj, BABIL_PARM_PITCH, &gPitch));
		LOG("pitch: %i\n", gPitch);
	}
	LOGD("CMAOS:NewL\n");
	gAudioStream = CMdaAudioOutputStream::NewL(*this, EMdaPriorityNormal,
		EMdaPriorityPreferenceTime);
	LOGD("CMAOS.Open\n");
	gAudioStream->Open(&gAudioSettings);	//causes MaoscOpenComplete() to be called later
	LOGD("init done\n");
}

void Syscall::DestructGuido() {
	//LogDateTime();

	if(gAudioStream) {
		gAudioStream->Stop();
		delete gAudioStream;
	}

	if(gBabileObj)
		BABILE_free(gBabileObj, gMemTab);

	for(int i=0; i<gNumAllocs; i++) {
		User::Free(gAllocArray[i]);
	}
}

//***************************************************************************
//Various
//***************************************************************************

/*void Syscall::LogDateTime() {
	TBuf<64> buf;
	TTime time;
	time.HomeTime();
	time.FormatL(buf, _L("*DATETIME %H:%M:%S %*D %N %Y\n"));
	TBuf8<64> buf8;
	buf8.Copy(buf);
	MYASSERT(gGpsLogFile->write(buf8));
}*/

//***************************************************************************
//TTS Init
//***************************************************************************

void* VoidAlloc(int size) {
	void* p = User::Alloc(size);
	LOG("VoidAlloc %i\n", size);
	if(!p) {
		LOG("VA %i failed!\n", size);
		DUMPINT(User::Heap().Size());
		int bb;
		int avail = User::Heap().Available(bb);
		DUMPINT(avail);
		DUMPINT(bb);
		return NULL;
	}
	return p;
}

void* Syscall::VoidAllocZero(int size) {
	if(gNumAllocs >= MAXALLOCS) {
		LOG("OOM!\n");
		return NULL;
	}
	void* p = VoidAlloc(size);
	if(!p)
		return NULL;
	ZEROMEM(p, size);
	return gAllocArray[gNumAllocs++] = p;
}

char* Syscall::GetFileL(const char* filename) {
	int dummy;
	return GetFileWithSizeL(filename, dummy);
}

char* Syscall::GetFileWithSizeL(const char* fn1, int& size) {
#if 1	//temp hack
	Smartie<char> filename(new char[strlen(fn1) + 1]);
	
	// change slashes
	strcpy(filename(), fn1);
	for(char* ptr = filename(); *ptr != 0; ptr++) {
		if(*ptr == '/')
			*ptr = '\\';
	}
#endif

	FileStream file(filename());
	if(!file.length(size)) {
		LOG("GetFileL open error\n");
		return NULL;
	}
	void* buf = VoidAlloc(size + 1);
	if(!buf)
		return NULL;
	if(!file.read(buf, size)) {
		LOG("GetFileL read error\n");
		User::Free(buf);
		return NULL;
	}
	((char*)buf)[size] = 0;
	gAllocArray[gNumAllocs++];
	return (char*)buf;
}

char* Syscall::GetFileFromEitherDriveL(const char* partPath) {
	char buffer[128];
	strcpy(buffer + 1, partPath);

	buffer[0] = 'C';
	char* result = GetFileL(buffer);
	if(result)
		return result;

	buffer[0] = 'E';
	return GetFileL(buffer);
}

#ifdef SWEDISH
BB_DbLs* Syscall::initVoiceDbaL() {
	//DEBUG_ALWAYS("initVoiceDbaL\n");
	BB_DbLs* mbrDba = AllocZero<BB_DbLs>(2);
	mbrDba[0].pDbId = AllocZero<BB_DbId>();
	char* ptr;
	if(!(ptr = GetFileFromEitherDriveL(
		BABEL_BASE "ingmar_16k\\ingmar_min_16k.vol_35_C_l.rom")))
		return NULL;
	mbrDba[0].pDbId->type = X_RAM;
	mbrDba[0].pDbId->link = ptr;

	::strcpy(mbrDba[0].descriptor, "MBR");
	return mbrDba;
}
BB_DbLs* Syscall::initLanguageDbaL() {
	//DEBUG_ALWAYS("initLanguageDbaL\n");
	char* ptr;
	BB_DbLs* nlpDba = AllocZero<BB_DbLs>(4);
	//PMemZero(nlpDba, sizeof(BB_DbLs)*4);

	//DEBUG_ALWAYS("1\n");
	if(!(ptr = GetFileFromEitherDriveL(BABEL_BASE "ingmar_16k\\swrulesdle.ivx")))
		return NULL;
	//CleanupStack::PushL( language_PHO );//BBNBMALLOCS++;
	//DEBUG_ALWAYS("2\n");
	nlpDba[0].pDbId = AllocZero<BB_DbId>();
	nlpDba[0].pDbId->type = X_RAM;
	nlpDba[0].pDbId->link = ptr;
	//DEBUG_ALWAYS("3\n");
	::strcpy(nlpDba[0].descriptor, "IVX");

	if(!(ptr = GetFileFromEitherDriveL(BABEL_BASE "ingmar_16k\\sw01dle.phm")))
		return NULL;
	//CleanupStack::PushL( language_PRO );//BBNBMALLOCS++;
	//DEBUG_ALWAYS("4\n");
	nlpDba[1].pDbId = AllocZero<BB_DbId>();
	nlpDba[1].pDbId->type = X_RAM;
	nlpDba[1].pDbId->link = ptr;
	//DEBUG_ALWAYS("5\n");
	::strcpy(nlpDba[1].descriptor, "PHM");

	//DEBUG_ALWAYS("6\n");
	nlpDba[2].pDbId=NULL;
	::strcpy(nlpDba[2].descriptor, "END");
	//DEBUG_ALWAYS("done\n");
	return nlpDba;
}
#elif defined(ENGLISH)

// these functions panic on error.
static int iniCountFiles(char* iniPtr);
// these functions modify iniPtr, and may insert null terminators.
// reads the descriptor (3 bytes) and one terminating whitespace.
static const char* iniReadDescriptor(char*& iniPtr);
// reads whitespace, the filename.
static const char* iniReadFilename(char*& iniPtr);
// reads whitespace, then returns any text found until end-of-line.
static const char* iniReadExtra(char*& iniPtr);
// makes sure we have an END, then skips to the end of the line.
static void iniReadEnd(char*& iniPtr);
// makes sure we have a PATH, then skips to the end of the line.
static void iniReadPath(char*& iniPtr);
// skips the rest of the current line.
static void iniSkipLine(char*& iniPtr);

BB_DbLs* Syscall::initDbaL(const char* voicePath, char*& iniPtr, BB_DbLs*& voiceDba) {
	LOGD("initLanguageDbaL\n");
	iniReadPath(iniPtr);
	char* ptr;
	const int count = iniCountFiles(iniPtr);
	BB_DbLs* nlpDba = AllocZero<BB_DbLs>(count + 1);
	const int vpLen = strlen(voicePath);
	voiceDba = NULL;

	// then we can parse and load each file.
	for(int i=0; i<count; i++) {
		//LOG("Current ini: '%s'\n", iniPtr);
		nlpDba[i].pDbId = AllocZero<BB_DbId>();
		::strcpy(nlpDba[i].descriptor, iniReadDescriptor(iniPtr));
		if(strcmp(nlpDba[i].descriptor, "END") == 0) {
			if(voiceDba == NULL)
				voiceDba = &nlpDba[i];
			iniSkipLine(iniPtr);
			i--;
			continue;
		}
		const char* fn = iniReadFilename(iniPtr);
		int size;
		
		const char* extra = iniReadExtra(iniPtr);
		LOG("extra: '%s'\n", extra);
		if(strcmp(extra, "[FILE]") == 0) {
			const int fnLen = strlen(fn);
			char* path = (char*)VoidAlloc(vpLen + fnLen + 1);
			memcpy(path, voicePath, vpLen);
			memcpy(path + vpLen, fn, fnLen + 1);
			
#if 1	//temp hack
	Smartie<char> filename(new char[strlen(path) + 1]);
	
	// change slashes
	strcpy(filename(), path);
	for(char* ptr = filename(); *ptr != 0; ptr++) {
		if(*ptr == '/')
			*ptr = '\\';
	}
#endif

			FileStream file(filename());
			if(!file.isOpen())
				return NULL;
			if(!file.length(size))
				return NULL;
			
			nlpDba[i].pDbId->type = X_FILE;
			nlpDba[i].pDbId->link = path;
		} else {
			TBuf8<KMaxFileName> buf;
			buf.Format(_L8("%s%s"), voicePath, fn);
			if(!(ptr = GetFileWithSizeL(CCP buf.PtrZ(), size)))
				return NULL;
			nlpDba[i].pDbId->type = X_RAM;
			nlpDba[i].pDbId->link = ptr;
		}
		//nlpDba[i].pDbId->size = size;
	}
	//iniReadEnd(iniPtr);

	nlpDba[count].pDbId=NULL;
	nlpDba[count].descriptor[0] = 0;

	return nlpDba;
}


static void iniSkipLine(char*& iniPtr) {
	iniPtr = strchr(iniPtr, '\n');
	DEBUG_ASSERT(iniPtr != NULL);
	iniPtr++;
}
static void iniSkipWhitespace(char*& iniPtr) {
	while(isspace(*iniPtr)) {
		iniPtr++;
	}
}

static void iniReadExactLine(char*& iniPtr, const char* e) {
	// read exact
	const int len = strlen(e);
	DEBUG_ASSERT(memcmp(iniPtr, e, len) == 0);
	iniPtr += len;
	DEBUG_ASSERT(isspace(*iniPtr));
	// skip to next line
	iniSkipLine(iniPtr);
}

static void iniReadPath(char*& iniPtr) {
	iniReadExactLine(iniPtr, "PATH");
}

static void iniReadEnd(char*& iniPtr) {
	iniReadExactLine(iniPtr, "END");
}

static int iniCountFiles(char* iniPtr) {
	int count = 0;
	int ends = 0;
	do {
		iniSkipWhitespace(iniPtr);
		// 3-byte descriptor, whitespace, quoted.
		const char* descriptor = iniPtr;
		for(int i=0; i<3; i++) {
			DEBUG_ASSERT(isalpha(*iniPtr));
			iniPtr++;
		}
		if(memcmp(descriptor, "END", 3) == 0) {
			ends++;
			if(ends == 3)	//magic number. see ini file.
				break;
			iniSkipLine(iniPtr);
			continue;
		}
		DEBUG_ASSERT(isspace(*iniPtr));
		iniSkipWhitespace(iniPtr);
		DEBUG_ASSERT(*iniPtr == '"');
		iniPtr++;
		char* endQ = strchr(iniPtr, '"');
		DEBUG_ASSERT(endQ > iniPtr);
		iniPtr = endQ;
		iniSkipLine(iniPtr);
		count++;
	} while(true);
	return count;
}

static const char* iniReadDescriptor(char*& iniPtr) {
	iniSkipWhitespace(iniPtr);
	const char* descriptor = iniPtr;
	for(int i=0; i<3; i++) {
		DEBUG_ASSERT(isalpha(*iniPtr));
		iniPtr++;
	}
	DEBUG_ASSERT(isspace(*iniPtr));
	*iniPtr = 0;
	iniPtr++;
	return descriptor;
}

static const char* iniReadFilename(char*& iniPtr) {
	iniSkipWhitespace(iniPtr);
	DEBUG_ASSERT(*iniPtr == '"');
	iniPtr++;
	char* filename = iniPtr;
	char* endQ = strchr(iniPtr, '"');
	DEBUG_ASSERT(endQ > iniPtr);
	*endQ = 0;
	iniPtr = endQ + 1;
	
#if 0
	// change slashes
	for(char* ptr = filename; ptr != endQ; ptr++) {
		if(*ptr == '/')
			*ptr = '\\';
	}
#endif
	return filename;
}

static const char* iniReadExtra(char*& iniPtr) {
	iniSkipWhitespace(iniPtr);
	char* extra = iniPtr;
	char* eol = strchr(iniPtr, '\r');
	DEBUG_ASSERT(eol);
	DEBUG_ASSERT(eol[1] == '\n');
	*eol = 0;
	iniPtr = eol + 2;
	return extra;
}

#else
#error Unsupported language
#endif	//SWEDISH

//***************************************************************************
//Text-To-Speech
//***************************************************************************

void Syscall::MaoscOpenComplete(TInt aError) {
	LOGD("AuOpC\n");
	if(aError != KErrNone) {
		LOG("AuOpC error %i\n", aError);
		return;
	}
	
	//seems to contain garbage
	/*LOG("caps %X\n", gAudioSettings.iCaps);
	LOG("maxvol %i\n", gAudioSettings.iMaxVolume);
	LOG("rate %i\n", gAudioSettings.iSampleRate);
	LOG("chs %i\n", gAudioSettings.iChannels);
	LOG("vol %i\n", gAudioSettings.iVolume);
	LOG("flags %X\n", gAudioSettings.iFlags);*/

#if 1//ndef BABILE2	//???
#ifdef BABILE2
#define SAMPLE_RATE TMdaAudioDataSettings::ESampleRate11025Hz
#else
#define SAMPLE_RATE TMdaAudioDataSettings::ESampleRate16000Hz
#endif
	LTRAP(gAudioStream->SetAudioPropertiesL(SAMPLE_RATE,
		TMdaAudioDataSettings::EChannelsMono));
#endif

	// note that MaxVolume() is different in the emulator and the real device!
	DUMPINT(gAudioStream->MaxVolume());
	gAudioStream->SetVolume(gAudioStream->MaxVolume());
	gAudioStream->SetPriority(EMdaPriorityNormal, EMdaPriorityPreferenceTimeAndQuality);
}

SYSCALL(int, maStartSpeaking(const char* text)) {
	//LOGD("maStartSpeaking\n");
	int size = ValidatedStrLen(text) + 1;
	LOGR("S %i %s\n", gTtsNextId, text);
	if(maIsSpeaking())
		maStopSpeaking();

	/*{
	PFile dumpFile;
	for(int i=0; !dumpFile.IsOpen(); i++) {
	PSprintf(gSoundDumpFileName, "E:\\dump%i.pcm", i);
	dumpFile.Open(gSoundDumpFileName, FILE_ACCESS_WRITE | FILE_ACCESS_EXCL | FILE_ACCESS_CREATE);
	}
	LOG("%s\n", gSoundDumpFileName);
	}*/

	gTextBuffer = new (ELeave) char[size];
	memcpy(gTextBuffer(), text, size);
	gTextPos = 0;
	gCurrentAudioBuffer = 0;
	//gBuffersWaiting = NBUFWAIT;

	gTickBase = User::TickCount();
	gTotalSamples = gBuffersQueued = 0;

	for(int i=0; i<NBUFFERS && gTextBuffer(); i++) {
		UpdateBuffer();
	}
	LOGD("gTP: %i => %i @ %d\n", gTextPos, gTotalSamples, TICK);
	return gTtsNextId++;
}

void Syscall::UpdateBuffer() {
	BB_U32 samples;
	//LOG("rT \"%s\"\n", gTextBuffer + gTextPos);
	gCurrentAudioBuffer++;
	if(gCurrentAudioBuffer >= NBUFFERS)
		gCurrentAudioBuffer = 0;
	//LOGD("CAB %i\n", gCurrentAudioBuffer);
	AUDIOBUFFER& ab = gAudioBuffer[gCurrentAudioBuffer];
	//ZERO_ARRAY(audioBuffer);
	int res = BABILE_readText(gBabileObj, gTextBuffer() + gTextPos, ab.buf,
		AUDIOBUFFER_NSAMPLES, &samples);
	gTotalSamples += samples;
	//LOGD("UB %i %i @ %d\n", res, samples, TICK);
	/*{
	LOG("UB %i %i @ %d\n", res, samples, TICK);
	PFile file(gSoundDumpFileName, FILE_ACCESS_WRITE);
	file.Seek(0, PStream::STREAM_SEEK_END);
	file.Write(ab.buf, samples*2);
	file.Close();
	}*/
	if(res < 0) {
		LOGD("ReadText error %i\n", res);
		/*#define BB_disassembleError(error) \
		int severity = ((error) & BB_ERRORSEVERITYMASK) >> BB_ERRORSEVERITYSHIFT;\
		int mod = ((error) & BB_ERRORMODIDMASK) >> BB_ERRORMODIDSHIFT;\
		int submod = ((error) & BB_ERRORSUBMODIDMASK) >> BB_ERRORSUBMODIDSHIFT;\
		int id = (error) & BB_ERRORIDMASK;
		BB_disassembleError((BB_U32)res);
		LOG("%i %i %i %i\n", severity, mod, submod, id);
		LOG("0x%08X\n", res);*/
		goto finish;
	}
	gTextPos += res;
	if(samples == 0) {  //done
		goto finish;
	}
	ab.des.Set((TUint8*)ab.buf, samples*2);
	//LOGD("WriteB %i\n", gCurrentAudioBuffer);
	LTRAP(gAudioStream->WriteL(ab.des));
	gBuffersQueued++;
	return;
finish:
	gTextBuffer = NULL;
}

void Syscall::MaoscBufferCopied(TInt aError, const TDesC8&) {
	gBuffersQueued--;
	//LOGD("AuBuC %i @ %d\n", gBuffersQueued, TICK);
	if(aError != KErrNone && aError != -39) {
		LOG("AuBuC error %i\n", aError);
		return;
	}
	if(gTextBuffer())
		UpdateBuffer();
	if(!maIsSpeaking()) {
		MAEvent event;
		event.type = EVENT_TYPE_TTS;
		event.ked.key = gTtsNextId - 1;
		gAppView.AddEvent(event);
		
		//gAudioStream->Stop();
	}
}

SYSCALL(void, maStopSpeaking()) {
	LOG("SS @ %d\n", TICK);
	gAudioStream->Stop();
	gTextBuffer = NULL;
	
	MAEvent event;
	event.type = EVENT_TYPE_TTS;
	event.ked.key = gTtsNextId - 1;
	gAppView.AddEvent(event);
}

void Syscall::MaoscPlayComplete(TInt aError) {
	LOG("AuPlC %i @ %d\n", gBuffersQueued, TICK);
	if(aError != KErrNone && aError != KErrCancel) {
		LOG("AuPlC error %i\n", aError);
		return;
	}
}

SYSCALL(int, maIsSpeaking()) {
	return gBuffersQueued != 0;
}

//SYSCALL(int, maSetSpeechSetting(int setting, int volume)) {
SYSCALL(int, maSetSpeechVolume(int volume)) {
	if(volume < 0)
		volume = 0;
	if(volume > 100)
		volume = 100;
	int temp = gVolume;
	gVolume = volume;
	LOG("SV %i @ %d\n", volume, TICK);

	//TEST_NZ(BABILE_setSetting(gBabileObj, BABIL_PARM_VOLUME, gVolume));	//not working

	int symVol = (gAudioStream->MaxVolume() * volume) / 100;
	//DUMPINT(symVol);
	gAudioStream->SetVolume(symVol);	//works
	return temp;
}

int Syscall::setBabilSetting(int setting, int value, BB_S32 &oldValue) {
	BB_S32 temp = oldValue;
	LOG("sBS %i %i %i\n", setting, value, temp);
	CHECK_NZ(BABILE_setSetting(gBabileObj, setting, value));
	CHECK_NZ(BABILE_getSetting(gBabileObj, setting, &oldValue));
	return temp;
}

SYSCALL(int, maSetSpeechSpeed(int speed)) {
	return setBabilSetting(BABIL_PARM_SPEED, speed, gSpeed);
}

SYSCALL(int, maSetSpeechPitch(int pitch)) {
	return setBabilSetting(BABIL_PARM_PITCH, pitch, gPitch);
}

#endif	//GUIDO
