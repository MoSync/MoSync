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

#ifndef GUIDO_H
#error "Bad include!"
#endif	//GUIDO_H

//***************************************************************************
//Con/de-structors
//***************************************************************************

void ClearGuidoVariables();
void InitGuidoL();
void DestructGuido();

//***************************************************************************
//Syscalls
//***************************************************************************

int maStartSpeaking(const char* text);
void maStopSpeaking();
int maIsSpeaking();
int maSetSpeechVolume(int volume);
int maSetSpeechSpeed(int speed);
int maSetSpeechPitch(int pitch);

//***************************************************************************
//Text-To-Speech
//***************************************************************************
CMdaAudioOutputStream* gAudioStream;
TMdaAudioDataSettings gAudioSettings;
#define AUDIOBUFFER_NSAMPLES 2500
#define AUDIOBUFFER_SIZE AUDIOBUFFER_NSAMPLES * 2
#define NBUFFERS 32
struct AUDIOBUFFER {
	TInt16 buf[AUDIOBUFFER_NSAMPLES];
	TPtrC8 des;
} gAudioBuffer[NBUFFERS];
int gCurrentAudioBuffer;
Smartie<char> gTextBuffer;
int gTextPos;
TUint gTickBase;
int gTotalSamples;
int gBuffersQueued;
int gVolume;
BB_S32 gSpeed, gPitch;
int gTtsNextId;

#define TICK (User::TickCount() - gTickBase)

virtual void MaoscOpenComplete(TInt aError);
virtual void MaoscBufferCopied(TInt aError, const TDesC8 &aBuffer);
virtual void MaoscPlayComplete(TInt aError);
void UpdateBuffer();

int setBabilSetting(int setting, int value, BB_S32 &oldValue);

//BABILE helpers
#define MAXALLOCS 64
void* gAllocArray[MAXALLOCS];
int gNumAllocs;
void* VoidAllocZero(int size);
template<class T> T* AllocZero(int num = 1) {
	return (T*)VoidAllocZero(sizeof(T) * num);
}
char* GetFileL(const char* filename);
char* GetFileWithSizeL(const char* filename, int& size);
char* GetFileFromEitherDriveL(const char* partPath);

//BABILE stuff
BB_MemRec* gMemTab;
BABILE_Obj* gBabileObj;

#ifdef BABILE2
BB_DbLs* initDbaL(const char* voicePath, char*& iniPtr, BB_DbLs*& voiceDba);
#else
BB_DbLs* initVoiceDbaL();
BB_DbLs* initLanguageDbaL();
#endif	//BABILE2

#ifdef __WINS__

#define BABEL_BASE ":\\BABEL\\"

#elif defined(__MARM__)

#ifdef __SERIES60_3X__
#define BABEL_BASE ":\\Data\\BABEL\\"
#else	//2nd edition
#define BABEL_BASE ":\\BABEL\\"
#endif	//__SERIES60_3X__

#else	//__defined(__MARM__)
#error Unsupported platform!
#endif	//__WINS__
