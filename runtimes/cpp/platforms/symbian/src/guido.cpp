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

#include <base/FileStream.h>

#include "syscall.h"
#include "AppView.h"
#include "core.h"
#include "errors.h"

#include <helpers/CPP_IX_GUIDO.H>



#define SWEDISH
//#define ENGLISH

#if defined(SWEDISH) && defined(ENGLISH)
#error Only one language allowed.
#endif

#ifdef BABILE2
#include "babmobs.lic.h"
#include "babmobs.lic.password.h"
#endif

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

void Syscall::InitGuidoL() {
	/*for(int i=0; ; i++) {
		GPSLOGILENAME.Format(_L("C:\\gpslog%i.txt"), i);
		FileStream testfile(GPSLOGILENAME, false);
		if(!testfile.isOpen())
			break;
		i++;
	}
	gGpsLogFile = new (ELeave) FileStream(GPSLOGILENAME, true);
	LogDateTime();*/

	{
		BB_S16 i, nBlocks;
		BABILE_MemParam babParam;
		ZERO_OBJECT(babParam);
		babParam.sSize = sizeof(babParam);
		babParam.sNLPConfig=0;
#ifdef BABILE2
		babParam.sSELConfig = BUFFER_UNITACOUSTIC_ALL|BUFFER_UNITSOFFSETS_ALL|BUFFER_BERSTREAM_ALL|20;
		babParam.nlpModule = BABILE_NLPMODULE_NLPE;
		babParam.synthModule = BABILE_SYNTHMODULE_BBSELECTOR;
		babParam.synthLS = initVoiceDbaL();
		if(!babParam.synthLS)
			User::Leave(ERROR_BABILE);
		babParam.license = (const BB_TCHAR*) babLicense;  /* load your license file */
		babParam.uid.passwd=uid.passwd;	/* your password */
		babParam.uid.userId=uid.userId; /* your ID */
#else
		babParam.sMBRConfig=0;
		babParam.mbreLS = initVoiceDbaL();
		if(!babParam.mbreLS)
			User::Leave(ERROR_BABILE);
#ifdef SWEDISH
		babParam.nlpModule = BABILE_NLPMODULE_TTF;
#elif defined(ENGLISH)
		babParam.nlpModule = BABILE_NLPMODULE_NLPE;
#else
#error Unsupported language
#endif	//SWEDISH
#endif	//BABILE2

		babParam.nlpeLS = initLanguageDbaL();
		if(!babParam.nlpeLS)
			User::Leave(ERROR_BABILE);

		// memory descriptor creation
		nBlocks = BABILE_numAlloc();
		LOGD("B_numAlloc %i\n", nBlocks);
		gMemTab = AllocZero<BB_MemRec>(nBlocks);
		if(!gMemTab) {
			LOG("BB_MemRec error!\n");
			User::Leave(ERROR_BABILE);
		}
		//PMemZero(gMemTab, nBlocks*sizeof(BB_MemRec));

		LOGD("B_alloc\n");
		int blocksToAlloc = BABILE_alloc(&babParam, gMemTab);
		if(nBlocks != blocksToAlloc) {
			LOG("BB_Alloc discrepancy: %i != %i\n", nBlocks, blocksToAlloc);
			User::Leave(ERROR_BABILE);
		}

		for(i=0; i<nBlocks; i++) {
			if(gMemTab[i].size > 0) {
				gMemTab[i].base = VoidAllocZero(gMemTab[i].size);
			} else {
				gMemTab[i].base = NULL;
			}
		}
		if(gMemTab[2].base == 0) {
			LOG("gMemTab error!\n");
		}

		LOGD("B_init\n");
		if(!(gBabileObj = BABILE_init(gMemTab, &babParam))) {
			LOG("failed!\n");
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
	FileStream file(filename);
	int size;
	if(!file.length(size)) {
		LOG("GetFileL open error\n");
		return NULL;
	}
	void* buf = VoidAlloc(size);
	if(!buf)
		return NULL;
	if(!file.read(buf, size)) {
		LOG("GetFileL read error\n");
		User::Free(buf);
		return NULL;
	}
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

#ifdef BABILE2
static bool FileExistL(const char* path) {
	FileStream file(path);
	return file.isOpen();
}

static char* GetFilePathFromEitherDriveL(const char* partPath) {
	char buffer[128];
	strcpy(buffer + 1, partPath);

	buffer[0] = 'C';
	if(FileExistL(buffer))
		return strdup(buffer);

	buffer[0] = 'E';
	if(FileExistL(buffer))
		return strdup(buffer);
	
	return NULL;
}
#endif

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
BB_DbLs* Syscall::initVoiceDbaL() {
	//DEBUG_ALWAYS("initVoiceDbaL\n");
	BB_DbLs* mbrDba = AllocZero<BB_DbLs>(3);

	mbrDba[0].pDbId = AllocZero<BB_DbId>();
	char* ptr;
	if(!(ptr = GetFilePathFromEitherDriveL(BABEL_BASE "kenny_11k\\kenny_11k_85.hnmc")))
		return NULL;
	mbrDba[0].pDbId->type = BB_ROFILE_DBMEMTYPE;
	mbrDba[0].pDbId->link = ptr;
	::strcpy(mbrDba[0].descriptor, "SLV");

	mbrDba[1].pDbId = AllocZero<BB_DbId>();
	if(!(ptr = GetFilePathFromEitherDriveL(BABEL_BASE "kenny_11k\\kenny_11k_85.hnmc.nuul214vqfp")))
		return NULL;
	mbrDba[1].pDbId->type = BB_ROFILE_DBMEMTYPE;
	mbrDba[1].pDbId->link = ptr;
	::strcpy(mbrDba[1].descriptor, "SLL");

	return mbrDba;
}
BB_DbLs* Syscall::initLanguageDbaL() {
	char* ptr;
	BB_DbLs* nlpDba = AllocZero<BB_DbLs>(8);

	if(!(ptr = GetFileFromEitherDriveL(BABEL_BASE "kenny_11k\\enu.trr")))
		return NULL;
	nlpDba[0].pDbId = AllocZero<BB_DbId>();
	nlpDba[0].pDbId->type = X_RAM;
	nlpDba[0].pDbId->link = ptr;
	::strcpy(nlpDba[0].descriptor, "PHO");

	if(!(ptr = GetFileFromEitherDriveL(BABEL_BASE "kenny_11k\\enu_hd.bnx")))
		return NULL;
	nlpDba[1].pDbId = AllocZero<BB_DbId>();
	nlpDba[1].pDbId->type = X_RAM;
	nlpDba[1].pDbId->link = ptr;
	::strcpy(nlpDba[1].descriptor, "BNX");

	if(!(ptr = GetFileFromEitherDriveL(BABEL_BASE "kenny_11k\\enu.dca")))
		return NULL;
	nlpDba[2].pDbId = AllocZero<BB_DbId>();
	nlpDba[2].pDbId->type = X_RAM;
	nlpDba[2].pDbId->link = ptr;
	::strcpy(nlpDba[2].descriptor, "DIC");

	if(!(ptr = GetFileFromEitherDriveL(BABEL_BASE "kenny_11k\\enu.ldi")))
		return NULL;
	nlpDba[3].pDbId = AllocZero<BB_DbId>();
	nlpDba[3].pDbId->type = X_RAM;
	nlpDba[3].pDbId->link = ptr;
	::strcpy(nlpDba[3].descriptor, "LDI");

	if(!(ptr = GetFileFromEitherDriveL(BABEL_BASE "kenny_11k\\enu.oso")))
		return NULL;
	nlpDba[4].pDbId = AllocZero<BB_DbId>();
	nlpDba[4].pDbId->type = X_RAM;
	nlpDba[4].pDbId->link = ptr;
	::strcpy(nlpDba[4].descriptor, "PRO");

	if(!(ptr = GetFileFromEitherDriveL(BABEL_BASE "kenny_11k\\enu_iv.gra")))
		return NULL;
	nlpDba[5].pDbId = AllocZero<BB_DbId>();
	nlpDba[5].pDbId->type = X_RAM;
	nlpDba[5].pDbId->link = ptr;
	::strcpy(nlpDba[5].descriptor, "GRI");

	if(!(ptr = GetFileFromEitherDriveL(BABEL_BASE "kenny_11k\\enu_oov.gra")))
		return NULL;
	nlpDba[6].pDbId = AllocZero<BB_DbId>();
	nlpDba[6].pDbId->type = X_RAM;
	nlpDba[6].pDbId->link = ptr;
	::strcpy(nlpDba[6].descriptor, "GRO");

	nlpDba[7].pDbId=NULL;
	::strcpy(nlpDba[7].descriptor,"END");

	return nlpDba;
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
		event.key = gTtsNextId - 1;
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
	event.key = gTtsNextId - 1;
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
