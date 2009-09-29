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
#ifdef SYMBIAN_SOUND

//#include <mmf/plugin/mmfcontrollerimplementationuids.hrh>

#include "syscall.h"

using namespace Base;

#define STEST(code, self, final) if(code) {	self->SetExit();\
	LOG("In " __FILE__ " on line %d: %d\n", __LINE__, code); final; }
#define STRAP(statement, self, final) { TRAPD(_err, statement); STEST(_err, self, final) }
#define SVTRAP(statement) STRAP(statement, this, return)
#define SBTRAP(statement) STRAP(statement, this, return false)

void Syscall::SoundSystem::ConstructL() {
	mChannels.ConstructL();
	LHEL(mMutex.CreateLocal());
	LHEL(mCondVar.CreateLocal());
}

Syscall::SoundSystem::~SoundSystem() {
	mCondVar.Close();
	mMutex.Close();
}

void Syscall::SoundSystem::Notify() {
	mMutex.Wait();
	mStart = true;
	mMutex.Signal();
	mCondVar.Signal();
}

void Syscall::SoundSystem::SetExit() {
	mExit = true;
}

//***************************************************************************
//Sound thread functions
//***************************************************************************

TInt Syscall::SoundSystem::Main(TAny* arg) {
	//Create a cleanup stack
	CTrapCleanup* cleanup = CTrapCleanup::New();
	if(cleanup == NULL) {
		Panic(ECreateTrapCleanup);
	}

	// create an active scheduler
	CActiveScheduler *pA=new CActiveScheduler;
	__ASSERT_ALWAYS(pA!=NULL,Panic(EMainSchedulerError));
	CActiveScheduler::Install(pA);

	SoundSystem* self = (SoundSystem*)arg;
	STRAP(self->Main2(), self, ;);

	// Game over, tidy up...
	delete pA;
	delete cleanup; 

	self->mThreadIsOpen = false;

	return(KErrNone);
}

void Syscall::SoundSystem::Main2() {
	LOGA("Main2\n");
	//Open CMdaAudioOutputStream
	/*SVTRAP*/(mStream = CMdaAudioOutputStream::NewL(*this));
	CleanupStack::PushL(mStream);
	LOGA("Opening...\n");
	mStream->Open(&mStreamSettings);
	
	{	//This is heavy wizardry
		/*SVTRAP*///(mCodec = CMMFCodec::NewL(KMMFFourCCCodeMP3, KMMFFourCCCodePCM16));
		//CleanupStack::PushL(mCodec);
		/*RArray<TInt> configParams;

		//This setup converts MP3 to 16 bit mono PCM
		configParams.Append(1); // stereo to mono switch (TRUE(1) or default FALSE(0))
		configParams.Append(0); // decode left or right channel (1 left, 2 right, 0 default all)
		configParams.Append(1); // decimation factor (2, 4 or default 1)
		configParams.Append(1); // error concealment level (0 none, default 1)
		configParams.Append(16); // resolution of PCM samples (default 16 bits)
		configParams.Append(SOUND_FREQUENCY*2); //Output sampling frequency

		(mCodec->ConfigureL(TUid::Uid(KMmfUidControllerAudio), (TDesC8&)configParams));*/
	}

	mStart = false;

	while(!mExit) {
		LOGA("Starting CActiveScheduler\n");
		CActiveScheduler::Start();
		LOGA("waiting...\n");
		mMutex.Wait();
		while(!mStart) {
			int res = mCondVar.Wait(mMutex);
			STEST(res, this, User::Leave(42));
		}
		mStart = false;
		Process();
		mMutex.Signal();
	}
	LOGA("Exiting...\n");

//	CleanupStack::PopAndDestroy(mCodec);
	CleanupStack::PopAndDestroy(mStream);
}

void Syscall::SoundSystem::MaoscOpenComplete(TInt aError) {
	LOGA("MaoscOC\n");
	if(aError) {
		LOG("MaoscOC %i\n", aError);
		CActiveScheduler::Stop();
		return;
	}
	//DUMPHEX(mStreamSettings.iCaps);
	//DUMPINT(mStreamSettings.iMaxVolume);
	DUMPINT(mStream->MaxVolume());
	//DUMPINT(mStreamSettings.iSampleRate);
	//DUMPINT(mStreamSettings.iChannels);
	//DUMPINT(mStreamSettings.iVolume);
	//DUMPINT(mStreamSettings.iFlags);
	SVTRAP(mStream->SetAudioPropertiesL(TMdaAudioDataSettings::ESampleRate44100Hz,
		TMdaAudioDataSettings::EChannelsStereo));
	SVTRAP(mStream->SetDataTypeL(KMMFFourCCCodePCM16));
	mStream->SetVolume(mStream->MaxVolume() / 4);

	if(!Process())
		CActiveScheduler::Stop();
}
void Syscall::SoundSystem::MaoscBufferCopied(TInt aError, const TDesC8 &) {
	LOGA("MaoscBC\n");
	if(aError) {
		LOG("MaoscBC %i\n", aError);
		CActiveScheduler::Stop();
		return;
	}
	if(!Process()) {
		mStream->Stop();
	}
}
void Syscall::SoundSystem::MaoscPlayComplete(TInt aError) {
	LOGA("MaoscPC\n");
	if(aError) {
		LOG("MaoscPC %i\n", aError);	//-3 is ok (caused by Stop()).
	}
	CActiveScheduler::Stop();
}

bool Syscall::SoundSystem::Process() {
	SBTRAP(return ProcessL());
	return false;
}

bool Syscall::SoundSystem::ProcessL() {
	LOGA("Process\n");
	//make sure that we're always waiting for MaoscBufferCopied
	//at least while we have something to do

	TPtrC8 ptrc8(NULL, 0);

	for(int i=0; i<MAX_SOUND_CHANNELS; i++) {
		Channel& ch = mChannels.Wait(i);
		LOGA("Channel %i%s playing\n", i, ch.playing ? "" : " not");
		if(!ch.playing) {
			ch.mutex.Signal();
			continue;
		}
		
		MYASSERT(ch.buf != NULL);
		
		if(ch.src &&
			ch.buf->Position() + SOUND_BYTES_PER_BUFFER > ch.buf->BufferSize())
		{
			//we read from source and convert
			int srcSize;
			TEST(ch.src->length(srcSize));
			int srcPos;
			TEST(ch.src->tell(srcPos));
			TPtrC8 srcPtrC((byte*)ch.src->ptr(), srcSize - srcPos);
			TEST(ch.src->ptrc());
			TPtr8 srcPtr((TUint8*)srcPtrC.Ptr(), srcPtrC.Length());

			CMMFPtrBuffer* mmfSrc = CMMFPtrBuffer::NewL(srcPtr);
			CleanupStack::PushL(mmfSrc);

			TCodecProcessResult result = mCodec->ProcessL(*mmfSrc, *ch.buf);
			LOGA("ProcessResult: %i, src %i, dst %i\n", result.iStatus,
				result.iSrcBytesProcessed, result.iDstBytesAdded);
			
			if(srcPos == 0 && mmfSrc->Position() >= mmfSrc->BufferSize()) {
				SAFE_DELETE(ch.src);
			} else {
				BIG_PHAT_ERROR;	//incomplete
			}
			ch.position = mmfSrc->BufferSize() / SOUND_BYTES_PER_SAMPLE;	//imperfect

			CleanupStack::PopAndDestroy(mmfSrc);
		}
		//buf is now filled with sound

		MYASSERT(ptrc8.Length() == 0);	//max one channel for now
		
		const TDesC8& data = ch.buf->Data();	//imperfect
		memcpy(mWriteBuffer, data.Ptr(), data.Length());
		ptrc8.Set((TUint8*)mWriteBuffer, data.Length());

		//mix if there is more than one sound stream
		//store TPtrC8 to result in mWritePtr

		/*ch.position += readSamples;
		MYASSERT(ch.position <= srcSamples)
		if(ch.position == srcSamples) {
			ch.position = 0;
			ch.loopCount--;
			MYASSERT(ch.loopCount >= 0);
			if(ch.loopCount == 0)
				ch.playing = false;
		}*/
		ch.playing = false;	//imperfect

		ch.mutex.Signal();
		//LOGA("Writing %i samples...\n", readSamples);
	}
	if(ptrc8.Length() == 0)
		return false;
	mWritePtr.Set(ptrc8);
	LOGA("Writing %i bytes...\n", mWritePtr.Length());
	SBTRAP(mStream->WriteL(mWritePtr));
	return true;
}

#endif	//SYMBIAN_SOUND
