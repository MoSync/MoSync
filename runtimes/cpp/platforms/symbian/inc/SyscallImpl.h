/* Copyright (C) 2009 Mobile Sorcery AB

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License, version 2, as published by
the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

You should have received a copy of the GNU General Public License
along with MoSync; see the file COPYING.  If not, write to the Free
Software Foundation, 59 Temple Place - Suite 330, Boston, MA
02111-1307, USA.
*/

//This file is included in Base/Syscall.h

#define MAX_SOUND_CHANNELS 4

_LIT(KSoundThreadName, "MoSync Sound Thread");

class Syscall : public MSdpAgentNotifier, public MVibraControlObserver,
public MCameraObserver
#ifdef __SERIES60_3X__
, public MRSendSMSObserver
#else	//Series 60, 2nd Ed.
, public MSmsObserver
#endif	//__SERIES60_3X__
#ifdef MMF
, public MMMFControllerEventMonitorObserver
#else
, public MMdaObjectStateChangeObserver
#endif
#if defined(GUIDO) || defined(SUPPORT_AUDIOBUFFER)
, public MMdaAudioOutputStreamCallback
#endif
, public MVideoLoadingObserver, public MVideoPlayerUtilityObserver
{
private:
#define DECLARE_INIT_VAR(type, name, val) type name;
#define INIT_INIT_VAR(type, name, val) name(val)
#define DECLARE_ARG_VAR(type, name) type g##name;
#define DECLARE_ARG(type, name) type a##name
#define INIT_ARG_VAR(type, name) g##name(a##name)
#define PASS_ARG(type, name) a##name

#ifdef __SERIES60_3X__
#define S60V2_INITIALIZED_VARIABLES(m, com)
#else	//Series 60, 2nd Ed.
#define S60V2_INITIALIZED_VARIABLES(m, com)\
	com m(CSmsHandler*, gSmsHandler, NULL)
#endif	//__SERIES60_3X__

	enum StreamState {
		SS_IDLE, SS_OPENING, SS_PREPARING, SS_LOADING, SS_PLAYING, SS_PAUSED, SS_ERROR
	};
	
	enum CameraState {
		CS_IDLE, CS_RESERVING, CS_POWERING, CS_POWERED
	};

#define INITIALIZED_VARIABLES(m, com)\
	m(int, gCurrentUnconvertedColor, 0)\
	com m(int, gNextStoreId, 1)\
	com m(int, gVibraStatus, 0)\
	com m(int, gSoundVolume, 100)\
	com m(bool, gPlaying, false)\
	com m(TAlphaBitmap*, gDrawTarget, NULL)\
	com m(MAHandle, gDrawTargetHandle, HANDLE_SCREEN)\
	com m(CVibraControl*, gVibraControl, NULL)\
	com m(CMdaAudioRecorderUtility*, gPlayer, NULL)\
	com m(CMMFControllerPluginSelectionParameters*, gMmfCPSP, NULL)\
	com m(CMMFFormatSelectionParameters*, gMmfFSP, NULL)\
	com m(MemStream*, gSoundBuffer, NULL)\
	com m(CActiveEnder*, gSmsSenderCleanupQue, NULL)\
	com m(CVideoPlayerUtility*, gVideoPlayer, NULL)\
	com m(int, gStreamHandle, 0)\
	com m(StreamState, gStreamState, SS_IDLE)\
	com m(bool, gStreamSetPosPausable, false)\
	com m(int, gStreamNewPos, -1)\
	com m(CCamera*, gCamera, NULL)\
	com m(CameraState, gCameraState, CS_IDLE)\
	com m(TRequestStatus*, gCameraStatus, NULL)\
	com m(int, gFileListNextHandle, 1)\

	INITIALIZED_VARIABLES(DECLARE_INIT_VAR, NUL)
	S60V2_INITIALIZED_VARIABLES(DECLARE_INIT_VAR, NUL)

#define CONSTRUCTOR_ARGUMENTS(m, com)\
	m(CDirScrAccEng&, ScreenEngine)\
	com m(CAppView&, AppView)\

#if defined(__MARM_ARMI__) || defined(__MARM_THUMB__)	//buggy compiler
public:
#endif

	CONSTRUCTOR_ARGUMENTS(DECLARE_ARG_VAR, NUL)

public:
	Syscall(CONSTRUCTOR_ARGUMENTS(DECLARE_ARG, COMMA));
	void ConstructL(VMCore*);

	void StopEverything();

private:
	TAlphaBitmap* loadImage(MemStream& s);
	TAlphaBitmap* loadSprite(TAlphaBitmap* image, ushort left, ushort top,
		ushort width, ushort height, ushort cx, ushort cy);

private:
	TInt gMicroSecondsPerTick;
	VMCore* gCore;

#ifdef TELEPHONY
	CTelephony* gTelephony;

	int maGetBatteryCharge();
#endif	//TELEPHONY

#ifdef CALL
	friend class CNotifyExample;

	CTelephony::TCallId gCallId;
	CClassSynchronizer<Syscall>* gCallSync;
	CClassSynchronizer<Syscall>* gHangupSync;
	bool gCallIdIsValid;
	CNotifyExample* gNotify;

	void CallHandlerL(TInt status);
	void HangupHandlerL(TInt status);
#endif	//CALL

	//Location API
#if defined(SUPPORT_MOSYNC_SERVER) && !defined(__S60_50__)
	RMoSyncServerSession gServer;
#endif

#ifdef __S60_50__
	RPositioner gPositioner;
	RPositionServer gPositionServer;
	TPositionInfo gPositionInfo;
#endif

#if defined(SUPPORT_MOSYNC_SERVER) || defined(__S60_50__)
	CClassSynchronizer<Syscall>* gLocationSync;

	int maLocationStart();
	int maLocationStop();
	void LocationHandlerL(TInt status);
	void AddLocationEvent(const TPosition&);
#endif

	bool gStreamWantsToPause;

	int maStreamVideoStart(const char* url);
	MAExtent maStreamVideoSize(MAHandle stream);
	int maStreamVideoSetFrame(MAHandle stream, const MARect* rect);
	int maStreamLength(MAHandle stream);
	int maStreamPos(MAHandle stream);
	int maStreamSetPos(MAHandle stream, int pos);
	int maStreamClose(MAHandle stream);
	int maStreamPause(MAHandle stream);
	int maStreamResume(MAHandle stream);
	void AddStreamEvent(int event, int result);

	//MVideoLoadingObserver
	void MvloLoadingStarted();
	void MvloLoadingComplete();

	//MVideoPlayerUtilityObserver
	void MvpuoOpenComplete(TInt aError);
	void MvpuoPrepareComplete(TInt aError);
	void MvpuoFrameReady(CFbsBitmap &aFrame, TInt aError);
	void MvpuoPlayComplete(TInt aError);
	void MvpuoEvent(const TMMFEvent &aEvent);

	void createCamera();
	int maCameraFormatNumber();
	int maCameraFormat(int index, MA_CAMERA_FORMAT* fmt);
	int maCameraStart();
	int maCameraStop();
	int maCameraSnapshot(int formatIndex, MAHandle placeholder);
	
	TCameraInfo gCameraInfo;
	CCamera::TFormat gCameraFormat;
	MAHandle gCameraPlaceholder;
	
	//MCameraObserver
	virtual void ReserveComplete(TInt aError);
	virtual void PowerOnComplete(TInt aError);
	virtual void ViewFinderFrameReady(CFbsBitmap& aFrame);
	virtual void ImageReady(CFbsBitmap* aBitmap, HBufC8* aData, TInt aError);
	virtual void FrameBufferReady(MFrameBuffer* aFrameBuffer, TInt aError);

#ifdef __SERIES60_3X__
	RAknKeylock2
#else	//Series 60, 2nd Ed.
	RAknKeyLock
#endif	//__SERIES60_3X__
		gKeyLock;

	//MVibraControlObserver
	virtual void VibraModeStatus(CVibraControl::TVibraModeState aStatus);
	virtual void VibraRequestStatus(CVibraControl::TVibraRequestStatus aStatus);

#ifndef __SERIES60_3X__	//Series 60, 2nd Ed.
	//MSmsObserver
	void MovedToOutbox();
	void ServerSessionEstablished();
	void ServerSessionError();
	void ServerDown(MMsvSessionObserver::TMsvSessionEvent aEvent);
	void NoServiceCenter();
	void MessageReceived();
	void NoMoreUnread();
#endif	//__SERIES60_3X__

	class Channel {
	public:
		Channel() : src(NULL), buf(NULL), playing(false) {}

		RMutex mutex;
		Stream* src;	//pwnage!	//will be deleted if fully decoded.
		int loopCount;
		CMMFDataBuffer* buf;	//used always

		int position;	//in samples
		int volume;	//0-100
		bool playing;
	};
#define CHANNEL_IS_INITIALIZED(ch) (ch.src != NULL)

#ifdef SYMBIAN_SOUND
	RThread gSoundThread;

	class SoundSystem : public MMdaAudioOutputStreamCallback {
	public:
		static TInt Main(TAny*);
		void Notify();	//notifies the sound thread of a change that requires action
		void SetExit();
		void ConstructL();

		//MMdaAudioOutputStreamCallback
		virtual void MaoscOpenComplete(TInt aError);
		virtual void MaoscBufferCopied(TInt aError, const TDesC8 &aBuffer);
		virtual void MaoscPlayComplete(TInt aError);

		MutexArray<MAX_SOUND_CHANNELS, Channel> mChannels;

		bool mThreadIsOpen;

		SoundSystem() : mThreadIsOpen(false), mStream(NULL), mCodec(NULL), mExit(false) {}
		virtual ~SoundSystem();
	private:
#define SOUND_FREQUENCY 44100
#define SOUND_NSAMPLES 2048
#define SOUND_NCHANNELS 2
#define SOUND_BYTES_PER_SAMPLE (SOUND_NCHANNELS * 2)
#define SOUND_BYTES_PER_BUFFER (SOUND_NSAMPLES * SOUND_BYTES_PER_SAMPLE)
		TInt16 mWriteBuffer[SOUND_NSAMPLES * SOUND_NCHANNELS];
		TInt32 mMixBuffer[SOUND_NSAMPLES * SOUND_NCHANNELS];
		TPtrC8 mWritePtr;
		
		CMdaAudioOutputStream* mStream;
		TMdaAudioDataSettings mStreamSettings;
		
		CMMFCodec* mCodec;

		bool mStart;
		bool mExit;
		RMutex mMutex;
		RCondVar mCondVar;

		void Main2();
		bool Process();	//wrapper around ProcessL
		bool ProcessL();	//returns true iff WriteL returned.
	} gSoundSystem;
#endif	//SYMBIAN_SOUND

#ifdef MMF
	RMMFController gController;
	CMMFControllerEventMonitor* gControllerEventMonitor;
	
	//MMMFControllerEventMonitorObserver
	void HandleEvent(const TMMFEvent& aEvent);
#else	//Mda
	//MMdaObjectStateChangeObserver
	void MoscoStateChangeEvent(CBase *aObject, TInt aPreviousState, TInt aCurrentState,
		TInt aErrorCode);
#endif

	TMyPtr8 gSoundPtr;

#ifdef SUPPORT_AUDIOBUFFER
	int maAudioBufferInit(const MAAudioBufferInfo*);
	int maAudioBufferReady();
	int maAudioBufferClose();
	
	//MMdaAudioOutputStreamCallback
	void MaoscOpenComplete(TInt aError);
	void MaoscBufferCopied(TInt aError, const TDesC8 &aBuffer);
	void MaoscPlayComplete(TInt aError);
	
	struct {
		TMdaAudioDataSettings::TAudioCaps sampleRateId;
		TMdaAudioDataSettings::TAudioCaps chanId;
		TPtrC8 des;
		bool ready;	//ready for WriteL?
	} gAudioBuffer;

	CMdaAudioOutputStream* gAudioStream;
	TMdaAudioDataSettings gAudioSettings;
#endif

#ifndef __SERIES60_3X__	//Series 60, 2nd Ed.
	int maGetCellInfo(MACellInfo*);
#endif

	int maGetSystemProperty(const char* key, char* buf, int size);
	
	int maTextBox(const wchar* title, wchar* text, int maxSize, int constraints);

	StringMap gStoreMap;

#ifndef __SERIES60_3X__	//2nd ed ARMI compiler hack
public:
#endif
	RSocketServ gSocketServ;	//Used by both networking and bluetooth.

private:

	void RestoreDrawTarget();

	int gStartTime;
	
	HashMap<FileList> gFileLists;
	
	int getVolumeInfo(MAHandle file, TVolumeInfo& vi);

#ifdef	__SERIES60_3X__
	TDblQue<CRSendAsSender> gSmsSenders;

	//MRSendSMSObserver
	void MessageSentL(CRSendAsSender* aSender, TInt aError);
#endif

private:
#define NETWORKING_H  
#include "netImpl.h"

private:
#define BTIMPL_H  
#include "btImpl.h"

#ifdef GUIDO
private:
#define GUIDO_H
#include "guido.h"
#endif	//GUIDO

#ifdef WLAN
private:
#define WLANIMPL_H
#include "wlanImpl.h"
#endif	//WLAN

private:
#define PIMIMPL_H
#ifdef	__SERIES60_3X__
#include "pimImpl.h"
HashMap<PimItem> mPimItems;
#else
//#include "pimV2.h"
#endif	//__SERIES60_3X__
