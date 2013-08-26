/* Copyright 2013 David Axmark

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

	http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
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
