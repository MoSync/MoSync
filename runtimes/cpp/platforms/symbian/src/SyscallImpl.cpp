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

#include <hal.h>
#include <e32math.h>
#include <math.h>

//#include <f32file.h>	//should clash with Base::Stream::Seek enums, but doesn't!!
#include <mmf/server/mmfDATASOURCESINK.HRH>
#include <mmf/server/mmfdes.h>
#include <apgcli.h>

#include "config_platform.h"
#define AUDIO_DEBUGGING_MODE

#include "../../../Base/FileStream.h"

#include "syscall.h"
#include "core.h"

#include "symbian_helpers.h"
#include "resourcearray.h"
#include "DirScrAccEng.h"
#include "AppView.h"
#include "AppUi.h"
#include "symbian_errors.h"
#include "Stream.h"
#include "heapsize.h"

#ifdef GUIDO
#include <helpers/CPP_IX_GUIDO.H>
static double _atanh(double d) {
	d = (1+d) / (1-d);
	return 0.5 * log(d);
}
#endif

#ifdef CALL
#include <helpers/CPP_IX_CALL.h>
#endif

#ifndef __SERIES60_3X__
#include <etel.h>
#include <etelbgsm.h>
#endif

using namespace MoSyncError;

static int maSendToBackground();
static int maBringToForeground();


#ifdef CALL
//***************************************************************************
//CNotifyExample
//***************************************************************************

class Base::CNotifyExample : public CActive
{
public:
	// Constructor/Destructor
	CNotifyExample(CTelephony* aTelephony, CAppView& aAppView, Syscall& aSyscall);

	~CNotifyExample();

	// Request voice line status change notification
	void RequestNotification();

	CTelephony::TCallStatus GetStatus() const {
		return iLineStatus.iStatus;
	}

private:
	/*
	These are the pure virtual methods from CActive that
	MUST be implemented by all active objects
	*/
	void RunL();
	void DoCancel();

private:
	CTelephony* iTelephony;
	CAppView& iAppView;
	Syscall& iSyscall;

	/*
	When the voice line status changes (and hence the asynchronous
	operation completes) the new voice line status is written into
	iLineStatus. Until this point, iLineStatus is not valid.

	If you change this class to get notification of other events,
	then change this class. CTelephony::TNotificationEvent lists
	the classes associated with each event.
	*/
	CTelephony::TCallStatusV1 iLineStatus;
	CTelephony::TCallStatusV1Pckg iLineStatusPckg;
};

/*
Default constructor. Pass it a reference to a CTelephony, created
with CTelephony::NewL() or NewLC()
*/
Base::CNotifyExample::CNotifyExample( CTelephony* aTelephony, CAppView& aAppView,
	Syscall& aSyscall)
: CActive( EPriorityStandard ),
iTelephony( aTelephony ),
iAppView( aAppView ),
iSyscall( aSyscall ),
iLineStatusPckg( iLineStatus )
{
	//default constructor
	iLineStatus.iStatus = CTelephony::EStatusUnknown;
	CActiveScheduler::Add(this);
	LOG("Notify constructed\n");
}

CNotifyExample::~CNotifyExample() {
	LOG("~Notify()\n");
	Cancel();
}

/*
This method requests notification by calling
CTelephony::NotifyChange() to initialise an asynchronous operation.
Then it immediately calls CActive::SetActive to start the
operation. The operation completes when the notification event
occurs.

In this case, we tell CTelephony to wait for the
CTelephony::EVoiceLineStatusChange notification event, hence the
event occurs when the voice line status changes.

For other events, change the notification event in the call to
CTelephony::NotifyChange(). The CTelephony::TNotificationEvent
description all the notification events.
It also list the information class to
pass to CTelephony::NotifyChange(). The TCallStatus
class is associated with CTelephony::EVoiceLineStatusChange.
*/
void CNotifyExample::RequestNotification()
{
	LOG("Notify::RequestNotification()\n");
	/*
	Panic if this object is already performing an asynchronous
	operation
	*/
	_LIT( KNotifyExamplePanic, "CNotifyExample" );
	__ASSERT_ALWAYS( !IsActive(), User::Panic( KNotifyExamplePanic, 1 ));

	iTelephony->NotifyChange( iStatus,
		CTelephony::EVoiceLineStatusChange,
		iLineStatusPckg );
	SetActive();
}

/*
The active object's RunL method is called when the asynchronous
event completes. In this case, RunL is called when the voice line
status changes. When this occurs, the new voice line status is
written to the iLineStatus. iLineStatus was passed to
CTelephony::NotifyChange() when the asynchronous operation was
started.

This is where you put your code to respond to your chosen
notification event.

In all active objects, iStatus indicates whether the asynchronous
operation succeeded, failed, or is still in progress. KErrNone
indicates success, and hence the value in iStatus is valid.
*/
void CNotifyExample::RunL()
{
	LOG("Notify::RunL(), status %i\n", iStatus.Int());
	bool callEnded = true;
	if( iStatus==KErrNone )
	{
		/*
		Insert code to respond to the change of voice line status
		here. A typical use to answer a call if the voice line
		status is 'ringing' (EStatusRinging)

		Remember that you must implement a RunError() method if
		your code in RunL can leave.
		*/
		//if( iLineStatus.iStatus == CTelephony::EStatusRinging )
		//; // Answer call by calling 'AnswerIncomingCall()'
		LOGT("Line status %i", iLineStatus.iStatus);

		MAEvent event;
		event.type = EVENT_TYPE_CALL;
		event.state = iLineStatus.iStatus;
		DEBUG_ASSERT(iAppView.AddEvent(event));

		if(iLineStatus.iStatus == CTelephony::EStatusIdle) {
			iSyscall.gCallIdIsValid = false;
		} else {
			callEnded = false;
		}

		RequestNotification();
	}
	if(callEnded) {
		LOGD("CoeWinPriorityNormal\n");
		CCoeEnv::Static()->RootWin().SetOrdinalPosition(0, ECoeWinPriorityNormal);
	}
}

/*
Like all active objects, you must supply a DoCancel() method to
cancel the asynchronous operation. This must cancel a CTelephony
notification operation with CTelephony::CancelAsync. Give the method
the appropriate 'cancellation event' from those in 'TCancellationRequest'

In this case, we are cancelling the "voice line status Change" event,
and so we need to supply the EVoiceLineStatusChange cancellation code.
If you change this class to respond to a different notification
event, remember to change the call to CTelephony::CancelAsync.
*/
void CNotifyExample::DoCancel()
{
	LOG("Notify::DoCancel()\n");
	iTelephony->CancelAsync( CTelephony::EVoiceLineStatusChangeCancel );
}
#endif	//CALL

//***************************************************************************
//Initialization
//***************************************************************************

Syscall::Syscall(CONSTRUCTOR_ARGUMENTS(DECLARE_ARG, COMMA)) :
INITIALIZED_VARIABLES(INIT_INIT_VAR, COMMA)
S60V2_INITIALIZED_VARIABLES(INIT_INIT_VAR, COMMA),
CONSTRUCTOR_ARGUMENTS(INIT_ARG_VAR, COMMA),
#ifdef TELEPHONY
gTelephony(NULL),
#endif
gBtDeviceArray(8, 0)
{
	init();
	ClearNetworkingVariables();
	ClearBluetoothVariables();
#ifdef GUIDO
	ClearGuidoVariables();
#endif
#ifdef WLAN
	ClearWlanVariables();
#endif
#ifdef CALL
	gCallIdIsValid = false;
	gCallSync = NULL;
	gHangupSync = NULL;
	gNotify = NULL;
#endif
#if defined(SUPPORT_MOSYNC_SERVER) || defined(__S60_50__)
	gLocationSync = NULL;
#endif
#ifdef SUPPORT_AUDIOBUFFER
	gAudioStream = NULL;
	gAudioBuffer.ready = false;
#endif
#ifdef MMF
	gControllerEventMonitor = NULL;
#endif
}

void Syscall::ConstructL(VMCore* aCore) {
	DebugMarkStart();
	gCore = aCore;

#ifdef SUPPORT_MOSYNC_SERVER
	LHEL(gServer.Connect());
#endif	//SUPPORT_MOSYNC_SERVER

	LHEL(HAL::Get(HALData::ESystemTickPeriod, gMicroSecondsPerTick));
	DUMPINT(gMicroSecondsPerTick);
#ifdef WINDOWS_TIME
	LARGE_INTEGER freq;
	QueryPerformanceFrequency(&freq);
	_LIT8(KTemp, "HighPerformanceFreqency");
	DumpInt64(KTemp, MAKE_TINT64(freq.HighPart, freq.LowPart));
#endif  //WINDOWS_TIME

	gVibraControl = VibraFactory::NewL(this);
	maVibrate(1);	//hack

	LHEL(gKeyLock.Connect());
	//gKeyLock.EnableSoftNotifications(EFalse);

	LHEL(gSocketServ.Connect());
	ConstructNetworkingL();
	ConstructBluetoothL();

	//Sound
	/*gSoundSystem.ConstructL();
	LHEL(gSoundThread.Create(KSoundThreadName, SoundSystem::Main, 0x5000,
		//KMinHeapSize, 1024*1024, &gSoundSystem));
		NULL, &gSoundSystem));
	gSoundSystem.mThreadIsOpen = true;
	gSoundThread.Resume();*/

#ifdef GUIDO
	InitGuidoL();
#endif	//GUIDO
#ifdef WLAN
	ConstructWlanL();
#endif

#ifdef TELEPHONY
	gTelephony = CTelephony::NewL();
#endif
#ifdef CALL
	gCallIdIsValid = false;
	gCallSync = new (ELeave) CClassSynchronizer<Syscall>(this, &Syscall::CallHandlerL);
	gHangupSync = new (ELeave) CClassSynchronizer<Syscall>(this, &Syscall::HangupHandlerL);
	gNotify = new (ELeave) CNotifyExample(gTelephony, gAppView, *this);
	gNotify->RequestNotification();
#endif

#ifdef __SERIES60_3X__
	gSmsSenders.SetOffset(_FOFF(CRSendAsSender, mLink));
	gSmsSenderCleanupQue = new (ELeave) CActiveEnder(_FOFF(CRSendAsSender, mLink));
#else	//Series 60, 2nd Ed.
	gSmsHandler = CSmsHandler::NewL(this);
#endif	//__SERIES60_3X__

#if defined(SUPPORT_MOSYNC_SERVER) || defined(__S60_50__)
	gLocationSync = new (ELeave) CClassSynchronizer<Syscall>(this, &Syscall::LocationHandlerL);
#endif

#ifdef MMF
	gControllerEventMonitor = CMMFControllerEventMonitor::NewL(*this, gController);
#endif
	if(!gScreenEngine.IsDrawing())
			gScreenEngine.StartDrawingL();


	gStartTime = maGetMilliSecondCount();

	DebugMarkStart();
}

void Base::StopEverything() {
	CAppUi* ui = (CAppUi*)(CEikonEnv::Static()->AppUi());
	if(ui) {
		Syscall* syscall = ui->GetSyscall();
		if(syscall) {
			syscall->StopEverything();
		}
	}	
}

void Syscall::StopEverything() {
	LOG("StopEverything()\n");

#if defined(SUPPORT_MOSYNC_SERVER) || defined(__S60_50__)
	maLocationStop();
#endif

	LOG("gCamera\n");
	if(gCamera) {
		gCamera->StopViewFinder();
		gCamera->PowerOff();
		gCamera->Release();
	}

	if(gVideoPlayer != NULL) {
		gVideoPlayer->Close();
	}

#ifdef MMF
	if(gControllerEventMonitor)
		gControllerEventMonitor->Cancel();
#endif

#ifdef SYMBIAN_SOUND
	if(gSoundSystem.mThreadIsOpen) {
		gSoundSystem.SetExit();
		gSoundSystem.Notify();
		{
			TRequestStatus status;
			gSoundThread.Rendezvous(status);
			User::WaitForRequest(status);	//may block	//should block?
			LOGA("Sound thread exit code: %i\n", status.Int());
		}
		gSoundThread.Close();
	}
#endif	//SYMBIAN_SOUND

	LOG("RestoreDrawTarget\n");
	RestoreDrawTarget();

	LOG("StopNetworking\n");
	StopNetworking();

#ifdef __SERIES60_3X__
	//SMS senders
	LOG("gSmsSenders\n");
	TDblQueIter<CRSendAsSender> itr(gSmsSenders);
	itr.SetToFirst();
	CRSendAsSender* op;
	while((op = itr++) != NULL) {
		op->Cancel();
	}
#endif

	LOG("StopEverything done\n");
}

void Syscall::RestoreDrawTarget() {
	if(gDrawTargetHandle != HANDLE_SCREEN) {
		SYSCALL_THIS->resources.extract_RT_FLUX(gDrawTargetHandle);
		ROOM(SYSCALL_THIS->resources.add_RT_IMAGE(gDrawTargetHandle, gDrawTarget));
		gDrawTargetHandle = HANDLE_SCREEN;
	}
}


void Syscall::platformDestruct() {
	LOG("platformDestruct()\n");
	
	StopEverything();

#ifdef __SERIES60_3X__
	pimClose();
#endif

#ifdef SUPPORT_MOSYNC_SERVER
	LOG("gServer.Close();\n");
	gServer.Close();
#endif
	
#if defined(SUPPORT_MOSYNC_SERVER) || defined(__S60_50__)
	//calls Cancel, which should really call the server's cancel mechanism...
	//except it doesn't have one. tricky...
	LOG("SAFE_DELETE(gLocationSync);\n");
	SAFE_DELETE(gLocationSync);
#endif

	SAFE_DELETE(gCamera);

	LOG("SAFE_DELETE(gVideoPlayer);\n");
	SAFE_DELETE(gVideoPlayer);

#ifdef __SERIES60_3X__
	SAFE_DELETE(gSmsSenderCleanupQue);
#else	//Series 60, 2nd Ed.
	SAFE_DELETE(gSmsHandler);
#endif	//__SERIES60_3X__

#ifdef CALL
	SAFE_DELETE(gCallSync);
	SAFE_DELETE(gHangupSync);
	SAFE_DELETE(gNotify);
#endif

#ifdef TELEPHONY
	SAFE_DELETE(gTelephony);
#endif

#ifdef WLAN
	DestructWlan();
#endif

#ifdef GUIDO
	DestructGuido();
#endif	//GUIDO

	/*for(int i=0; i<MAX_SOUND_CHANNELS; i++)
	{
		maSoundDispose();
	}*/
	maSoundStop();

#ifdef MMF
	SAFE_DELETE(gControllerEventMonitor);
#endif

	SAFE_DELETE(gMmfCPSP);

	SAFE_DELETE(gMmfFSP);

	DestructNetworking();
	DestructBluetooth();
	gSocketServ.Close();

	gKeyLock.Close();

	SAFE_DELETE(gVibraControl);
	
	gFileLists.close();

	DebugMarkEnd();
	LOG("platformDestruct() done\n");
}

//***************************************************************************
//Resource loading
//***************************************************************************

TAlphaBitmap* Syscall::loadImage(MemStream& s) {
	int size;
	TEST(s.length(size));
	TPtrC8 ptr(CBP s.ptrc(), size);
	TAlphaBitmap* image;
	int res = 0;
	LTRAP(res = LoadEncodedImageWAlphaL(ptr, &image));
	if(res == RES_OUT_OF_MEMORY) {
		BIG_PHAT_ERROR(ERR_OOM);
	} else if(res != RES_OK) {
		BIG_PHAT_ERROR(SYMERR_IMAGE_LOAD_FAILED);
	}
	return image;
}

TAlphaBitmap* Syscall::loadSprite(TAlphaBitmap* src, ushort left, ushort top,
	ushort width, ushort height, ushort /*cx*/, ushort /*cy*/) {
	TCleaner<CFbsBitGc> gc(CFbsBitGc::NewL());

	//Create/copy alpha
	TCleaner<CFbsBitmap> spriteAlpha(NULL);
	if(src->Alpha()) {
		spriteAlpha = new (ELeave) CFbsBitmap;
		CleanupStack::PushL(spriteAlpha());
		spriteAlpha->Create(TSize(width, height), src->Alpha()->DisplayMode());
		TCleaner<CFbsBitmapDevice> device(CFbsBitmapDevice::NewL(spriteAlpha()));

		gc->Activate(device());
		gc->BitBlt(TPoint(0,0), src->Alpha(), TRect(TPoint(left, top), TSize(width, height)));
		gc->Reset();
	}
	//Create/copy colormap
	TCleaner<CFbsBitmap> spriteColor(new (ELeave) CFbsBitmap);
	CleanupStack::PushL(spriteColor());

	spriteColor->Create(TSize(width, height), src->Color()->DisplayMode());
	TCleaner<CFbsBitmapDevice> device(CFbsBitmapDevice::NewL(spriteColor()));
	CleanupStack::PushL(device());

	gc->Activate(device());
	gc->BitBlt(TPoint(0,0), src->Color(), TRect(TPoint(left, top), TSize(width, height)));
	gc->Reset();

	//Save and cleanup
	TAlphaBitmap* sprite = new (ELeave) TAlphaBitmap(spriteColor(), spriteAlpha());
	CleanupStack::Pop(spriteColor.extract());
	if(src->Alpha())
		CleanupStack::Pop(spriteAlpha.extract());
	return sprite;
}

//******************************************************************************
//Proper syscalls
//******************************************************************************
#if 0//!defined(__SERIES60_3X__)	//S60v2

extern "C" {
#include "softfloat.h"

/*
int float_detect_tininess = float_tininess_after_rounding;
fp_rnd float_rounding_mode = float_round_nearest_even;
fp_except float_exception_flags;
fp_except float_exception_mask;
*/
}

void float_raise( fp_except type) {
	LOGD("float_raise(0x%x)\n", type);
	//BIG_PHAT_ERROR(ERR_INTERNAL);
}

float64 __negdf2(float64 a)
{
	/* libgcc1.c says -a */
	return a ^ FLOAT64_MANGLE(0x8000000000000000ULL);
}

#define F64(d) MAKE(float64, d)

SYSCALL(double, __adddf3(double a, double b)) {
	float64 result = float64_add(F64(a), F64(b));
	return MAKE(double, result);
}

SYSCALL(double, __subdf3(double a, double b)) {
	float64 result = float64_sub(F64(a), F64(b));
	return MAKE(double, result);
}

SYSCALL(double, __muldf3(double a, double b)) {
	float64 result = float64_mul(F64(a), F64(b));
	return MAKE(double, result);
}

SYSCALL(double, __divdf3(double a, double b)) {
	float64 result = float64_div(F64(a), F64(b));
	return MAKE(double, result);
}

SYSCALL(double, __negdf2(double a)) {
	float64 result = ::__negdf2(F64(a));
	return MAKE(double, result);
}
SYSCALL(int, __fixdfsi(double a)) {
	return float64_to_int32_round_to_zero(F64(a));
}
SYSCALL(double, __floatsidf(int a)) {
	float64 result = int32_to_float64(a);
	return MAKE(double, result);
}

SYSCALL(double, f2d(float f)) {
	float64 result = float32_to_float64(MAKE(float32, f));
	return MAKE(double, result);
}
SYSCALL(int, dcmp(double a, double b)) {
	if(float64_lt(F64(b), F64(a)))
		return 1;
	else if(float64_eq(F64(a), F64(b)))
		return 0;
	else	//a < b		//or NaN!
		return -1;
}
#endif	//S60v2

SYSCALL(double, sin(double x)) {
	double d;
	//LOGD("sin(%f %016LX)\n", x, MAKE(TInt64, x));

	//bugfix for 6630
	if(x == 0)
		return 0;
	//LOGD("sin(%f %016LX)\n", x, MAKE(TInt64, x));
	int result = Math::Sin(d, x);
	if(result == -6) {
		LOGD("sin error, returning 1\n");
		/*LOGD("Returning NaN\n");
		const TInt64 nan = MAKE_TINT64(0x7ff80000, 0x00000000);
		double d = MAKE(double, nan);
		LOGD("Returning NaN 2\n");*/
		return 1;
	}
	LHEL(result);
	LOGC("sin(%f %016LX) = %f %016LX\n", x, MAKE(TInt64, x), d, MAKE(TInt64, d));
	return d;
}
SYSCALL(double, cos(double x)) {
	double d;
	LHEL(Math::Cos(d, x));
	return d;
}
SYSCALL(double, tan(double x)) {
	double d;
	LHEL(Math::Tan(d, x));
	return d;
}
SYSCALL(double, sqrt(double x)) {
	double d;
	LHEL(Math::Sqrt(d, x));
	return d;
}

SYSCALL(int, maSetColor(int argb)) {
	int oldColor = gCurrentUnconvertedColor;
	gCurrentUnconvertedColor = argb;
	gScreenEngine.SetColor(argb);
	return oldColor;
}

SYSCALL(void, maPlot(int posX, int posY)) {
	gScreenEngine.PlotClip(posX, posY);
}
SYSCALL(void, maLine(int startX, int startY, int endX, int endY)) {
	//LOGD("Line %ix%i %ix%i\n", startX, startY, endX, endY);
	gScreenEngine.LineDrawClip(startX, startY, endX-startX, endY-startY);
}
SYSCALL(void, maFillRect(int left, int top, int width, int height)) {
	gScreenEngine.FillRectClip(left, top, width, height);
}
SYSCALL(void, maFillTriangleStrip(const MAPoint2d* points, int count)) {
	SYSCALL_THIS->ValidateMemRange(points, sizeof(MAPoint2d) * count);
	MYASSERT(((int)points & 0x3) == 0, ERR_MEMORY_ALIGNMENT);
	gScreenEngine.FillTriangleStripClip(points, count);
}
SYSCALL(void, maFillTriangleFan(const MAPoint2d* points, int count)) {
	SYSCALL_THIS->ValidateMemRange(points, sizeof(MAPoint2d) * count);
	MYASSERT(((int)points & 0x3) == 0, ERR_MEMORY_ALIGNMENT);
	//gScreenEngine.FillTriangleStripClip(points, count);
	BIG_PHAT_ERROR(ERR_FUNCTION_UNIMPLEMENTED);
}

SYSCALL(MAExtent, maGetTextSize(const char* str)) {
	if(*str == 0) {
		return 0;
	}
	TCleaner<HBufC16> buf(CreateHBufC16FromCStringLC(str));
	MAExtent ex = gScreenEngine.GetTextSizeL(*buf);
	return ex;
}
SYSCALL(MAExtent, maGetTextSizeW(const wchar* str)) {
	if(*str == 0) {
		return 0;
	}
	TPtrC16 ptr(str);
	MAExtent ex = gScreenEngine.GetTextSizeL(ptr);
	return ex;
}

SYSCALL(void, maDrawText(int left, int top, const char* str)) {
	if(*str == 0) {
		return;
	}
	TCleaner<HBufC16> buf(CreateHBufC16FromCStringLC(str));
	gScreenEngine.DrawTextL(*buf, TPoint(left, top));
}
SYSCALL(void, maDrawTextW(int left, int top, const wchar* str)) {
	if(*str == 0) {
		return;
	}
	TPtrC16 ptr(str);
	gScreenEngine.DrawTextL(ptr, TPoint(left, top));
}

SYSCALL(void, maUpdateScreen()) {
	LOGG("maUpdateScreen\n");
	VM_Yield();

#if 0//ndef MOSYNC_COMMERCIAL
#define WHITE 0xffffff
#define BLACK 0
#define PERIOD_MS (5*1000)
	if(maGetMilliSecondCount() < gStartTime + PERIOD_MS) {
		//save old settings
		if(gDrawTargetHandle != HANDLE_SCREEN) {
			gScreenEngine.SetDrawSurfaceL(HANDLE_SCREEN);
		}

		int oldColor = gCurrentUnconvertedColor;

		//handle clipping
		const TRect& oldClipRect((gDrawTargetHandle == HANDLE_SCREEN) ?
			gScreenEngine.GetScreenClip() : gDrawTarget->ClipRect());

		TRect fullScreenClipRect(TSize(gScreenEngine.Width(), gScreenEngine.Height()));
		gScreenEngine.SetClip(fullScreenClipRect);

		//draw text
		gScreenEngine.SetColor(BLACK);
		maDrawText(1, 1, "MoSync non-commercial");
		gScreenEngine.SetColor(WHITE);
		maDrawText(0, 0, "MoSync non-commercial");

		//restore
		gScreenEngine.SetColor(oldColor);
		gScreenEngine.SetClip(oldClipRect);

		if(gDrawTargetHandle != HANDLE_SCREEN) {
			gScreenEngine.SetDrawSurfaceL(gDrawTarget);
		}
	}
#endif	//MOSYNC_COMMERCIAL

	gScreenEngine.UpdateScreen();
}

SYSCALL(void, maResetBacklight()) {
	User::ResetInactivityTime();
}

SYSCALL(MAExtent, maGetScrSize()) {
	return EXTENT(gScreenEngine.Width(), gScreenEngine.Height());
}

SYSCALL(void, maDrawImage(MAHandle image, int left, int top)) {
	TAlphaBitmap* img = resources.get_RT_IMAGE(image);
	gScreenEngine.DrawImage(img, left, top);
}

SYSCALL(void, maDrawImageRegion(MAHandle image, const MARect* src,
		const MAPoint2d* dstTopLeft, int transform))
{
	TAlphaBitmap* img = resources.get_RT_IMAGE(image);
	TRect srcRect(TPoint(src->left, src->top), TSize(src->width, src->height));
	//HACK, but it should work in all current and future versions of Symbian
	const TPoint& dstPoint = *(const TPoint*)dstTopLeft;

	gScreenEngine.DrawImage(img, srcRect, dstPoint, transform);
}

SYSCALL(MAExtent, maGetImageSize(MAHandle image)) {
	TAlphaBitmap* img = resources.get_RT_IMAGE(image);
	const TSize& size = img->Color()->SizeInPixels();
	return EXTENT(size.iWidth, size.iHeight);
}

SYSCALL(MAHandle, maSetDrawTarget(MAHandle handle)) {
	MAHandle temp = gDrawTargetHandle;
	RestoreDrawTarget();
	if(handle == HANDLE_SCREEN) {
		gScreenEngine.SetDrawSurfaceL(HANDLE_SCREEN);
	} else {
		TAlphaBitmap* img = SYSCALL_THIS->resources.extract_RT_IMAGE(handle);
		gScreenEngine.SetDrawSurfaceL(img);
		gDrawTarget = img; 
		ROOM(SYSCALL_THIS->resources.add_RT_FLUX(handle, NULL));
	}
	gDrawTargetHandle = handle;
	return temp;
}

SYSCALL(void, maSetClipRect(int left, int top, int width, int height)) {
	TRect rect(TPoint(left, top), TSize(width, height));
	if(gDrawTargetHandle != HANDLE_SCREEN) {
		gDrawTarget->SetClipRect(rect);
	}
	gScreenEngine.SetClip(rect);
}

SYSCALL(void, maGetClipRect(MARect* rect)) {
	SYSCALL_THIS->ValidateMemRange(rect, sizeof(MARect));
	const TRect& clipRect((gDrawTargetHandle == HANDLE_SCREEN) ?
		gScreenEngine.GetScreenClip() : gDrawTarget->ClipRect());
	rect->left = clipRect.iTl.iX;
	rect->top = clipRect.iTl.iY;
	rect->width = clipRect.Width();
	rect->height = clipRect.Height();
}

#if 0
static int scanlineLength(CFbsBitmap* fb) {
	return fb->ScanLineLength(fb->SizeInPixels().iWidth, fb->DisplayMode());
}
#endif

SYSCALL(void, maGetImageData(MAHandle image, void* dst,
	const MARect* srcRect, int scanlength))
{
	SYSCALL_THIS->ValidateMemRange(srcRect, sizeof(MARect));
	CHECK_INT_ALIGNMENT(srcRect);
	const MARect* aSrcRect = (const MARect*)ALIGN_INT(srcRect);

	SYSCALL_THIS->ValidateMemRange(dst,
		sizeof(int)*aSrcRect->height*aSrcRect->height);
	CHECK_INT_ALIGNMENT(dst);
	int* dstInt = ALIGN_INT(dst);

	TAlphaBitmap* img = resources.get_RT_IMAGE(image);
	const TSize& size = img->Color()->SizeInPixels();

	MYASSERT(scanlength >= srcRect->width, ERR_IMAGE_OOB);
	MYASSERT(srcRect->left >= 0 && srcRect->top >= 0 &&
		srcRect->left + srcRect->width <= size.iWidth &&
		srcRect->top + srcRect->height <= size.iHeight, ERR_IMAGE_OOB);

	TRect lSrcRect(TPoint(aSrcRect->left, aSrcRect->top),
		TSize(aSrcRect->width, aSrcRect->height));

	bool hasAlpha = img->Alpha() != NULL;
#if 0
	LOG("maGetImageData(image): %ix%i, alpha %i\n", size.iWidth, size.iHeight, hasAlpha);
	if(hasAlpha) {
		TSize as(img->Alpha()->SizeInPixels());
		LOG("alpha size: %ix%i\n", as.iWidth, as.iHeight);
		LOG("pitch. color: %i, alpha: %i\n", scanlineLength(img->Color()), scanlineLength(img->Alpha()));
	}
#endif
	TBitmapUtil clr(img->Color());
	clr.Begin(TPoint(0,0));	//begin point is irrelevant, since we do SetPos.

	//set to color if no alpha, to avoid potential crash on NULL.
	TBitmapUtil alpha(hasAlpha ? img->Alpha() : img->Color());
	if(hasAlpha) {
		alpha.Begin(TPoint(0,0), clr);
	}

	//iterate through the source, combining and copying pixels
	int dY = 0;
	for(int sY=lSrcRect.iTl.iY; sY<lSrcRect.iBr.iY; sY++) {
		int dX = 0;
		clr.SetPos(TPoint(lSrcRect.iTl.iX, sY));

		// bugged on 5th edition when image width isn't pitch-aligned.
#ifndef __S60_50__
		if(hasAlpha)
			alpha.SetPos(TPoint(lSrcRect.iTl.iX, sY));
#endif
		for(int sX=lSrcRect.iTl.iX; sX<lSrcRect.iBr.iX; sX++) {
			TUint32 pixel = clr.GetPixel();
#if !(defined(__SERIES60_3X__) || defined(MA_PROF_SUPPORT_FRAMEBUFFER_32BIT))
			// 2nd edition, format 565
			// convert to 888.
			TUint32 blue = pixel & 0x001F;
			blue |= (blue & 0x1C) << 3;
			TUint32 green = (pixel & 0x07E0) >> 5;
			green |= (green & 0x30) << 2;
			TUint32 red = (pixel & 0xF800) >> 11;
			red |= (red & 0x1C) << 3;
			pixel = blue | (green << 8) | (red << 16);
#endif
			//LOG("Color(%ix%i): 0x%x\n", sX, sY, pixel);
			clr.IncXPos();
			if(hasAlpha) {
				// clr.GetPixel() may have the alpha-bits set. Indeed, they are undefined.
				pixel &= ~0xFF000000;

				pixel |= alpha.GetPixel() << 24;
				//LOG("Alpha: 0x%x\n", alpha.GetPixel());
				alpha.IncXPos();
			} else {
				pixel |= 0xFF000000;
			}
			dstInt[dY*scanlength + dX] = pixel;
			dX++;
		}	//for x
		dY++;
	}	//for y

	if(hasAlpha)
		alpha.End();
	clr.End();
}

SYSCALL(void, maDrawRGB(const MAPoint2d* dstPoint, const void* src,
	const MARect* srcRect, int scanlength))
{
	SYSCALL_THIS->ValidateMemRange(dstPoint, sizeof(MAPoint2d));
	CHECK_INT_ALIGNMENT(dstPoint);
	const MAPoint2d* dst = (const MAPoint2d*)ALIGN_INT(dstPoint);

	SYSCALL_THIS->ValidateMemRange(srcRect, sizeof(MARect));
	CHECK_INT_ALIGNMENT(srcRect);
	const MARect* rect = (const MARect*)ALIGN_INT(srcRect);

	int left = rect->left;
	int top = rect->top;
	int right = left + rect->width;
	int bottom = top + rect->height;

	SYSCALL_THIS->ValidateMemRange(src, sizeof(int)*rect->height*scanlength);
	CHECK_INT_ALIGNMENT(src);
	int* srcInt = ALIGN_INT(src);

	//allocate color bitmap
	CFbsBitmap* clr = new (ELeave) CFbsBitmap;
	CleanupStack::PushL(clr);
	clr->Create(TSize(rect->width, rect->height), EColor16M);
		//CCoeEnv::Static()->SystemGc().Device()->DisplayMode());
	//TBitmapUtil::SetPixel does no conversion.
	//suboptimal on 2nd ed.

	//copy color data
	TBitmapUtil clrUtil(clr);
	clrUtil.Begin(TPoint(0, 0));
	for(int y=top; y<bottom; y++) {
		for(int x=left; x<right; x++) {
			clrUtil.SetPixel(srcInt[y*scanlength + x] >> 8);
			clrUtil.IncXPos();	//wraps around for us
		}
		//clrUtil.IncYPos();	//not needed because of wrap
	}
	clrUtil.End();

	//allocate alpha bitmap
	CFbsBitmap* aph = new (ELeave) CFbsBitmap();
	CleanupStack::PushL(aph);
	aph->Create(TSize(rect->width, rect->height), EGray256);

	//copy alpha data
	TBitmapUtil alphaUtil(aph);
	alphaUtil.Begin(TPoint(0, 0));
	for(int y=top; y<bottom; y++) {
		for(int x=left; x<right; x++) {
			alphaUtil.SetPixel(srcInt[y*scanlength + x] >> 24);
			alphaUtil.IncXPos();
		}
		//alphaUtil.IncYPos();
	}
	alphaUtil.End();

	//create TAlphaBitmap, draw, and delete.
	TAlphaBitmap img(clr, aph);
	CleanupStack::Pop(aph);
	CleanupStack::Pop(clr);
	gScreenEngine.DrawImage(&img, dst->x, dst->y);
	//autodeletion here
}

SYSCALL(int, maCreateImageRaw(MAHandle placeholder, const void* src,
	MAExtent size, int alpha))
{
	SYSCALL_THIS->ValidateMemRange(src, sizeof(int)*EXTENT_X(size)*EXTENT_Y(size));
	CHECK_INT_ALIGNMENT(src);
	int* srcInt = ALIGN_INT(src);
	
	int width = EXTENT_X(size);
	int height = EXTENT_Y(size);

	//allocate color bitmap
	TCleaner<CFbsBitmap> clr(new CFbsBitmap);
	if(!clr)
		return RES_OUT_OF_MEMORY;
	CleanupStack::PushL(clr());

	LHEL_OOM(clr->Create(TSize(width, height), EColor16M));
	
		//CCoeEnv::Static()->SystemGc().Device()->DisplayMode());
	//TBitmapUtil::SetPixel does no conversion.
	//suboptimal on 2nd ed.

	//copy color data
	TBitmapUtil clrUtil(clr());
	for(int y=0; y<height; y++) {
		clrUtil.Begin(TPoint(0, y));
		for(int x=0; x<width; x++) {
			clrUtil.SetPixel(srcInt[y*width + x] & 0xFFFFFF);
			clrUtil.IncXPos();
		}
		clrUtil.End();
	}

	TCleaner<CFbsBitmap> aph(NULL);
	if(alpha) {
		//allocate alpha bitmap
		aph = new CFbsBitmap();
		if(!aph)
			return RES_OUT_OF_MEMORY;
		CleanupStack::PushL(aph());
		LHEL_OOM(aph->Create(TSize(width, height), EGray256));

		//copy alpha data
		TBitmapUtil alphaUtil(aph());
		for(int y=0; y<height; y++) {
			alphaUtil.Begin(TPoint(0, y));
			for(int x=0; x<width; x++) {
				alphaUtil.SetPixel(srcInt[y*width + x] >> 24);
				alphaUtil.IncXPos();
			}
			alphaUtil.End();
		}
	}

	//cleanup and store
	TAlphaBitmap* img = new TAlphaBitmap(clr(), aph());
	if(!img)
		return RES_OUT_OF_MEMORY;
	aph.pop();
	clr.pop();
	return resources.add_RT_IMAGE(placeholder, img);;
}

SYSCALL(int, maCreateImageFromData(MAHandle placeholder, MAHandle data, int offset, int size))
{
	Stream *src = resources.get_RT_BINARY(data);
	int length;
	MYASSERT(src->length(length), ERR_DATA_ACCESS_FAILED);
	MYASSERT(length > 0, ERR_DATA_OOB);
	MYASSERT(offset >= 0, ERR_DATA_OOB);
	MYASSERT(size > 0, ERR_DATA_OOB);
	MYASSERT(length >= offset + size, ERR_DATA_OOB);
	byte* ptrc = (byte*)src->ptrc();
	TAlphaBitmap* img;
	int res;
	if(ptrc) {
		TPtrC8 desc(ptrc + offset, size);
		res = LoadEncodedImageWAlphaL(desc, &img);
	} else {
		MYASSERT(src->seek(Seek::Start, offset), ERR_DATA_OOB);
		MemStream ms(size);
		MYASSERT(src->readFully(ms), ERR_DATA_OOB);

		TPtrC8 desc((byte*)ms.ptrc(), size);
		res = LoadEncodedImageWAlphaL(desc, &img);
	}
	if(res != RES_OK)
		return res;
	return resources.add_RT_IMAGE(placeholder, img);
}

SYSCALL(int, maCreateDrawableImage(MAHandle placeholder, int width, int height)) {
	MYASSERT(width > 0 && height > 0, ERR_IMAGE_SIZE_INVALID);
	TCleaner<CFbsBitmap> clr(new CFbsBitmap);
	if(!clr)
		return RES_OUT_OF_MEMORY;
	CleanupStack::PushL(clr());
	LHEL_OOM(clr->Create(TSize(width, height),
		CCoeEnv::Static()->SystemGc().Device()->DisplayMode()));
	TAlphaBitmap* img = new TAlphaBitmap(clr(), NULL);
	if(!img)
		return RES_OUT_OF_MEMORY;
	clr.pop();
	return resources.add_RT_IMAGE(placeholder, img);;
}

SYSCALL(void, maLoadProgram(MAHandle data, int reload)) {
	VM_Yield();
	gAppView.SetReload(data, reload);
}

SYSCALL(int, maGetKeys()) {
	VM_Yield();
	return gAppView.GetKeys();
}

SYSCALL(int, maGetEvent(MAEvent* ep)) {
	VM_Yield();
	MYASSERT(((uint)ep & 3) == 0, ERR_MEMORY_ALIGNMENT);	//alignment
	ValidateMemRange(ep, sizeof(MAEvent));
	return gAppView.GetEvent(ep);
}

SYSCALL(int, maTime()) {
	TTime ut;
	ut.UniversalTime();
	return unixTime(ut);
}

SYSCALL(int, maLocalTime()) {
	TTime ht;
	ht.HomeTime();
	return unixTime(ht);
}

SYSCALL(int, maGetMilliSecondCount()) {
#ifdef WINDOWS_TIME
	LARGE_INTEGER time, freq;
	QueryPerformanceCounter(&time);
	QueryPerformanceFrequency(&freq);
	double dt = time.QuadPart, df = freq.QuadPart;
	DEBUG_ASSERT(freq.QuadPart > 1000);
	int res = I64LOW((TInt64)((dt * 1000) / df));
#else
	DEBUG_ASSERT(gMicroSecondsPerTick > 1000);
	int res = (int)((((double)User::TickCount()) * gMicroSecondsPerTick) / 1000);
#endif
	return res;
}

SYSCALL(int, maFreeObjectMemory()) {
	int biggestBlock;
	return User::Available(biggestBlock) + (HEAP_SIZE - User::Heap().Size());
}
SYSCALL(int, maTotalObjectMemory()) {
	int value;
	LHEL(HAL::Get(HALData::EMemoryRAM, value));
	return value;
}


SYSCALL(int, maVibrate(int duration)) {
	int err = KErrNone;
	if(duration > 0) {
		TRAP(err, gVibraControl->StartVibraL(duration));
	} else if(duration == 0) {
		TRAP(err, gVibraControl->StopVibraL());
	} else {
		BIG_PHAT_ERROR(SYMERR_VIBRATION_DURATION_NEGATIVE);
	}
	if(err != KErrNone) {
		LOG("maVibrate(%i) left with %i\n", duration, err);
		return 0;
	}
	return gVibraStatus == 0;
}

void Syscall::VibraModeStatus(CVibraControl::TVibraModeState LOG_ARG(aStatus)) {
	LOG("VMS %i\n", aStatus);
}
void Syscall::VibraRequestStatus(CVibraControl::TVibraRequestStatus aStatus) {
	LOGD("VRS %i\n", aStatus);
	gVibraStatus = aStatus;
}


SYSCALL(void, maWait(int timeout)) {
	if(gAppView.HasEvent())
		return;
	VM_Yield();
	gAppView.Wait(timeout);
}

#ifdef TELEPHONY
int Syscall::maGetBatteryCharge() {
	CLocalSynchronizer ls;
	TRequestStatus& status(*ls.Status());

	CTelephony::TBatteryInfoV1 info;
	CTelephony::TBatteryInfoV1Pckg infoPkg(info);
	gTelephony->GetBatteryInfo(status, infoPkg);
	ls.Wait();

	if(status != KErrNone) {
		LOG("GetBatteryCharge error %i\n", status.Int());
		return IOCTL_UNAVAILABLE;  //magic number, identify it!
	}
#ifdef DEBUGGING_MODE
	DUMPINT(info.iStatus); //CTelephony::TBatteryStatus
#endif
	return info.iChargeLevel;

#if 0
	int value, res;
	res = HAL::Get(HALData::EPowerBatteryStatus, value);
	DUMPINT(value);
	if(res != KErrNone) {
		LOG("GetBatteryCharge error %i\n", res);
		return res;
	}	//TPowerBatteryStatus
	return value;
#endif

	//other alternatives:
	//HALData								//not found in 3rd ed docs.
	//RSystemAgent					//only three levels. enough? available on 3rd?
	//RMobilePhone					//7.0 DevKit licensees only. no docs available.
	//EPSHWRMBatteryLevel		//coarse steps
}
#endif	//TELEPHONY

#if 0//def __SERIES60_3X__
	const TInt KBrowserUid = 0x1020724D;
#else
	const TInt KBrowserUid = 0x10008D39;
#endif

static int LaunchBrowser(const char* url) {
	RApaLsSession appArcSession;
	TEST_SYMBIAN(appArcSession.Connect());
	TThreadId id;
	TPtrC8 url8(CBP url);
	Smartie<HBufC16> url16(HBufC16::NewL(url8.Length()));
	url16->Des().Copy(url8);
	TEST_SYMBIAN(appArcSession.StartDocument(*url16, TUid::Uid(KBrowserUid), id));
	appArcSession.Close();
	return KErrNone;
}

SYSCALL(int, maInvokeExtension(int, int, int, int)) {
	BIG_PHAT_ERROR(ERR_FUNCTION_UNIMPLEMENTED);
}

SYSCALL(longlong, maIOCtl(int function, int a, int b, int c)) {
	//move to individual functions?
	if(gBtState != eAvailable) switch(function) {
	case maIOCtl_maBtStartDeviceDiscovery:
	case maIOCtl_maBtGetNewDevice:
	case maIOCtl_maBtStartServiceDiscovery:
	case maIOCtl_maBtGetNewService:
	case maIOCtl_maBtGetNextServiceSize:
		switch(gBtState) {
		case eTurnedOff: return CONNERR_UNAVAILABLE;
		case eForbidden: return CONNERR_FORBIDDEN;
		case eError: return IOCTL_UNAVAILABLE;
		default: DEBIG_PHAT_ERROR;
		}
	}
	switch(function) {

#ifdef TELEPHONY
	case maIOCtl_maGetBatteryCharge:
		return maGetBatteryCharge();
#endif

	case maIOCtl_maAccept:
		SYSCALL_THIS->maAccept(a);
		return 1;

	case maIOCtl_maBtStartDeviceDiscovery:
		BLUETOOTH(maBtStartDeviceDiscovery)(a != 0);
		return 0;
	case maIOCtl_maBtGetNewDevice:
		return SYSCALL_THIS->maBtGetNewDevice(GVMRA(MABtDevice));
		
	case maIOCtl_maBtCancelDiscovery:
		return BLUETOOTH(maBtCancelDiscovery)();
		
	case maIOCtl_maBtStartServiceDiscovery:
		BLUETOOTH(maBtStartServiceDiscovery)(GVMRA(MABtAddr), GVMR(b, MAUUID));
		return 0;
	case maIOCtl_maBtGetNewService:
		return SYSCALL_THIS->maBtGetNewService(GVMRA(MABtService));
	case maIOCtl_maBtGetNextServiceSize:
		return BLUETOOTH(maBtGetNextServiceSize)(GVMRA(MABtServiceSize));

#ifdef LOGGING_ENABLED
	case maIOCtl_maWriteLog: {
		void* ptr = GetValidatedMemRange(a, b);
		LogBin(ptr, b);
		return 0;
	}
#endif	//LOGGING_ENABLED

#ifdef GUIDO
	case maIOCtl_maStartSpeaking:
		{
			const char* src = (const char*)GetValidatedMemRange(a, 1);
			return maStartSpeaking(src);
		}
	case maIOCtl_maStopSpeaking:
		maStopSpeaking();
		return 0;
	case maIOCtl_maIsSpeaking:
		return maIsSpeaking();
	case maIOCtl_maSetSpeechVolume:
		return maSetSpeechVolume(a);
	case maIOCtl_maSetSpeechSpeed:
		return maSetSpeechSpeed(a);
	case maIOCtl_maSetSpeechPitch:
		return maSetSpeechPitch(a);

	maIOCtl_sinh_case(::sinh);
	maIOCtl_cosh_case(::cosh);
	maIOCtl_atanh_case(_atanh);
#endif	//GUIDO

	case maIOCtl_maLockKeypad:
		gKeyLock.EnableWithoutNote();
		return 1;
	case maIOCtl_maUnlockKeypad:
		gKeyLock.DisableWithoutNote();
		return 1;
	case maIOCtl_maKeypadIsLocked:
		return gKeyLock.IsKeyLockEnabled();

	case maIOCtl_maPlatformRequest: {
		const char* url = SYSCALL_THIS->GetValidatedStr(a);
		if(sstrcmp(url, "http://") == 0) {
			int result = LaunchBrowser(url);
			if(IS_SYMBIAN_ERROR(result)) {
				return CONNERR_GENERIC;
			}
			return 0;
		//} else if(sstrcmp(url, "tel:") == 0) {
		} else {
			return IOCTL_UNAVAILABLE;
		}
	}
	
#ifdef WLAN
	case maIOCtl_maWlanStartDiscovery:
		return maWlanStartDiscovery();
	case maIOCtl_maWlanGetNewAp:
		return maWlanGetNewAp(GVMRA(MAWlanAp));
#endif

	case maIOCtl_maSendTextSMS: {
		const char* sa = SYSCALL_THIS->GetValidatedStr(a);
		const char* sb = SYSCALL_THIS->GetValidatedStr(b);		
		LOG("maSendTextSMS \"%s\" \"%s\"\n", sa, sb);
#ifdef __SERIES60_3X__
		HBufC16* recipientNumber = CreateHBufC16FromCStringLC(sa);
		HBufC16* messageText = CreateHBufC16FromCStringLC(sb);
		CRSendAsSender* sender = CRSendAsSender::NewL(*this, *messageText, *recipientNumber);
		gSmsSenders.AddLast(*sender);
		CleanupStack::PopAndDestroy(messageText);
		CleanupStack::PopAndDestroy(recipientNumber);
		return 0;
#else	//Series 60, 2nd Ed.
		//will work properly on 2nd edition, but not on 3rd.
		TBool result = gSmsHandler->SendL(sa, sb);
		return result ? 0 : CONNERR_GENERIC;
#endif	//__SERIES60_3X__
	}

#if defined(SUPPORT_MOSYNC_SERVER) && defined(__SERIES60_3X__)
	case maIOCtl_maGetCellInfo: {
		MACellInfo* ci = (MACellInfo*)GetValidatedMemRange(a, sizeof(MACellInfo));

		CTelephony::TNetworkInfoV1 info;
		TPckg<CTelephony::TNetworkInfoV1> pckg(info);
		int res = gServer.GetNetworkInfo(pckg);

		ci->cellId = info.iCellId;
		ci->lac = info.iLocationAreaCode;
		TPtr8((byte*)ci->mcc, sizeof(ci->mcc)-1).Copy(info.iCountryCode);
		TPtr8((byte*)ci->mnc, sizeof(ci->mnc)-1).Copy(info.iNetworkId);
		ci->mcc[info.iCountryCode.Length()] = 0;
		ci->mnc[info.iNetworkId.Length()] = 0;

		return -res;	//negate the Symbian error code
	}
#elif !defined(__SERIES60_3X__)	//Series 60, 2nd Ed.
	case maIOCtl_maGetCellInfo:
		return maGetCellInfo(GVMRA(MACellInfo));
#endif	//SUPPORT_MOSYNC_SERVER

#ifdef CALL
	case maIOCtl_maCallDial: {
		const char* number = SYSCALL_THIS->GetValidatedStr(a);
		TPtrC8 np(CBP number);
		
		LOG("maCallDial \"%s\"\n", number);
		
		if(gCallSync->Status()->Int() == KRequestPending ||
			gHangupSync->Status()->Int() == KRequestPending ||
			gCallIdIsValid)
		{
			LOG("Call already in progress!\n");
			return -2;
		}

		//should make phone calls happen in the background,
		//also makes it impossible to put MoSync in the background.
		//therefore, turn it off as soon as the call is disconnected.
		LOGD("CoeWinPriorityAlwaysAtFront\n");
		CCoeEnv::Static()->RootWin().SetOrdinalPosition(0, ECoeWinPriorityAlwaysAtFront);

		CTelephony::TCallParamsV1 params;
		CTelephony::TCallParamsV1Pckg paramPckg(params);
		params.iIdRestrict = CTelephony::ESendMyId;
		CTelephony::TTelNumber tNumber;
		tNumber.Copy(np);
		gTelephony->DialNewCall(*gCallSync->Status(), paramPckg, tNumber, gCallId,
			CTelephony::EVoiceLine);
		gCallSync->SetActive();
		return 0;
	}
	
	case maIOCtl_maCallAnswer: {
		LOG("maCallAnswer\n");
		if(gCallSync->Status()->Int() == KRequestPending ||
			gHangupSync->Status()->Int() == KRequestPending ||
			gCallIdIsValid)
		{
			LOG("Call already in progress!\n");
			return -2;
		}
		if(gNotify->GetStatus() != CTelephony::EStatusRinging) {
			LOG("Line not ringing!\n");
			return -3;
		}
		gTelephony->AnswerIncomingCall(*gCallSync->Status(), gCallId);
		gCallSync->SetActive();
		return 0;
	}

	case maIOCtl_maCallHangup: {
		LOG("maCallHangup\n");
		if(gCallSync->Status()->Int() == KRequestPending) {
			LOG("Canceling DialNewCall...\n");
			gTelephony->CancelAsync(CTelephony::EDialNewCallCancel);
			return 0;
		} else if(gHangupSync->Status()->Int() == KRequestPending) {
			LOG("Already hanging up...\n");
			return 0;
		} else if(gCallIdIsValid) {
			LOG("Starting Hangup...\n");
			gCallIdIsValid = false;
			gTelephony->Hangup(*gHangupSync->Status(), gCallId);
			gHangupSync->SetActive();
			return 0;
		} else {
			LOG("Do nothing\n");
			return 0;
		}
	}
#endif	//CALL

	case maIOCtl_maStreamVideoStart: {
		const char* url = SYSCALL_THIS->GetValidatedStr(a);
		return maStreamVideoStart(url);
	}
	case maIOCtl_maStreamClose: {
		return maStreamClose(a);
	}
	case maIOCtl_maStreamPause: {
		return maStreamPause(a);
	}
	case maIOCtl_maStreamResume: {
		return maStreamResume(a);
	}
	case maIOCtl_maStreamVideoSize: {
		return maStreamVideoSize(a);
	}
	case maIOCtl_maStreamVideoSetFrame: {
		return maStreamVideoSetFrame(a, GVMR(b, MARect));
	}
	case maIOCtl_maStreamLength: {
		return maStreamLength(a);
	}
	case maIOCtl_maStreamPos: {
		return maStreamPos(a);
	}
	case maIOCtl_maStreamSetPos: {
		return maStreamSetPos(a, b);
	}

#if defined(SUPPORT_MOSYNC_SERVER) || defined(__S60_50__)
	case maIOCtl_maLocationStart:
		return maLocationStart();
	case maIOCtl_maLocationStop:
		return maLocationStop();
#endif

	case maIOCtl_maFrameBufferGetInfo:
		return gScreenEngine.FrameBufferGetInfo(GVMRA(MAFrameBufferInfo));
	case maIOCtl_maFrameBufferInit:
		return gScreenEngine.FrameBufferInit(
			SYSCALL_THIS->GetValidatedMemRange(a, gScreenEngine.GetFrameBufferLen()));
	case maIOCtl_maFrameBufferClose:
		return gScreenEngine.FrameBufferClose();
		
#ifdef SUPPORT_AUDIOBUFFER
	case maIOCtl_maAudioBufferInit:
		return maAudioBufferInit(GVMRA(MAAudioBufferInfo));
	case maIOCtl_maAudioBufferReady:
		return maAudioBufferReady();
	case maIOCtl_maAudioBufferClose:
		return maAudioBufferClose();
#endif

	case maIOCtl_maSendToBackground:
		return maSendToBackground();
	case maIOCtl_maBringToForeground:
		return maBringToForeground();
	
	case maIOCtl_maCameraFormatNumber:
		return maCameraFormatNumber();
	case maIOCtl_maCameraFormat:
		return maCameraFormat(a, GVMR(b, MA_CAMERA_FORMAT));
	case maIOCtl_maCameraStart:
		return maCameraStart();
	case maIOCtl_maCameraStop:
		return maCameraStop();
	case maIOCtl_maCameraSnapshot:
		return maCameraSnapshot(a, b);

	maIOCtl_syscall_case(maFileOpen);

	maIOCtl_syscall_case(maFileExists);
	maIOCtl_syscall_case(maFileClose);
	maIOCtl_syscall_case(maFileCreate);
	maIOCtl_syscall_case(maFileDelete);
	maIOCtl_syscall_case(maFileSize);
	maIOCtl_syscall_case(maFileAvailableSpace);
	maIOCtl_syscall_case(maFileTotalSpace);
	maIOCtl_syscall_case(maFileDate);
	maIOCtl_syscall_case(maFileRename);
	maIOCtl_syscall_case(maFileTruncate);

	case maIOCtl_maFileWrite:
		return SYSCALL_THIS->maFileWrite(a, SYSCALL_THIS->GetValidatedMemRange(b, c), c);
	case maIOCtl_maFileRead:
		return SYSCALL_THIS->maFileRead(a, SYSCALL_THIS->GetValidatedMemRange(b, c), c);

	maIOCtl_syscall_case(maFileWriteFromData);
	maIOCtl_syscall_case(maFileReadToData);

	maIOCtl_syscall_case(maFileTell);
	maIOCtl_syscall_case(maFileSeek);

	maIOCtl_syscall_case(maFileListStart);
	case maIOCtl_maFileListNext:
		return SYSCALL_THIS->maFileListNext(a, (char*)SYSCALL_THIS->GetValidatedMemRange(b, c), c);
	maIOCtl_syscall_case(maFileListClose);

#ifdef __SERIES60_3X__	//todo: s60v2 implementation
	case maIOCtl_maPimListOpen:
		return maPimListOpen(a);
	case maIOCtl_maPimListNext:
		return maPimListNext(a);
	case maIOCtl_maPimListClose:
		return maPimListClose(a);
	case maIOCtl_maPimItemCount:
		return maPimItemCount(a);
	case maIOCtl_maPimItemGetField:
		return maPimItemGetField(a, b);
	case maIOCtl_maPimItemFieldCount:
		return maPimItemFieldCount(a, b);
	case maIOCtl_maPimItemGetAttributes:
		return maPimItemGetAttributes(a, b, c);
	case maIOCtl_maPimFieldType:
		return maPimFieldType(a, b);
	case maIOCtl_maPimItemGetValue:
		return maPimItemGetValue(GVMRA(MA_PIM_ARGS), b);
	/*case maIOCtl_maPimItemSetValue:
		return maPimItemSetValue(GVMRA(MA_PIM_ARGS), b, c);
	case maIOCtl_maPimItemAddValue:
		return maPimItemAddValue(GVMRA(MA_PIM_ARGS), b);
	case maIOCtl_maPimItemRemoveValue:
		return maPimItemRemoveValue(a, b, c);*/
	case maIOCtl_maPimItemClose:
		return maPimItemClose(a);
	/*case maIOCtl_maPimItemCreate:
		return maPimItemCreate(a);
	case maIOCtl_maPimItemRemove:
		return maPimItemRemove(a, b);*/
#endif	//__SERIES60_3X__

#ifdef TELEPHONY
	case maIOCtl_maGetSystemProperty:
		return maGetSystemProperty(SYSCALL_THIS->GetValidatedStr(a),
			(char*)SYSCALL_THIS->GetValidatedMemRange(b, c), c);
#endif

	maIOCtl_syscall_case(maTextBox);

	default:
		return IOCTL_UNAVAILABLE;
	}
}

//------------------------------------------------------------------------------
// maGetSystemProperty
//------------------------------------------------------------------------------
static const TDesC& getIso639() {
	// find current UI language
	TLanguage tl = User::Language();

#define LANGUAGES(m)\
	m(ELangEnglish, en)\
	m(ELangFrench, fr)\
	m(ELangGerman, de)\
	m(ELangSpanish, es)\
	m(ELangItalian, it)\
	m(ELangSwedish, sv)\
	m(ELangDanish, da)\
	m(ELangNorwegian, no)\
	m(ELangFinnish, fi)\
	m(ELangAmerican, en)\
	m(ELangSwissFrench, fr)\
	m(ELangSwissGerman, de)\
	m(ELangPortuguese, pt)\
	m(ELangTurkish, tr)\
	m(ELangIcelandic, is)\
	m(ELangRussian, ru)\
	m(ELangHungarian, hu)\
	m(ELangDutch, nl)\
	m(ELangBelgianFlemish, nl)\
	m(ELangAustralian, en)\
	m(ELangBelgianFrench, fr)\
	m(ELangAustrian, de)\
	m(ELangNewZealand, en)\
	m(ELangInternationalFrench, fr)\
	m(ELangCzech, cs)\
	m(ELangSlovak, sk)\
	m(ELangPolish, pl)\
	m(ELangSlovenian, sl)\
	m(ELangTaiwanChinese, zh)\
	m(ELangHongKongChinese, zh)\
	m(ELangPrcChinese, zh)\
	m(ELangJapanese, ja)\
	m(ELangThai, th)\
	m(ELangAfrikaans, af)\
	m(ELangAlbanian, sq)\
	m(ELangAmharic, am)\
	m(ELangArabic, ar)\
	m(ELangArmenian, hy)\
	m(ELangTagalog, tl)\
	m(ELangBelarussian, be)\
	m(ELangBengali, bn)\
	m(ELangBulgarian, bg)\
	m(ELangBurmese, my)\
	m(ELangCatalan, ca)\
	m(ELangCroatian, hr)\
	m(ELangCanadianEnglish, en)\
	m(ELangInternationalEnglish, en)\
	m(ELangSouthAfricanEnglish, en)\
	m(ELangEstonian, et)\
	m(ELangFarsi, fa)\
	m(ELangCanadianFrench, fr)\
	m(ELangScotsGaelic, gd)\
	m(ELangGeorgian, ka)\
	m(ELangGreek, el)\
	m(ELangCyprusGreek, el)\
	m(ELangGujarati, gu)\
	m(ELangHebrew, he)\
	m(ELangHindi, hi)\
	m(ELangIndonesian, id)\
	m(ELangIrish, ga)\
	m(ELangSwissItalian, )\
	m(ELangKannada, kn)\
	m(ELangKazakh, kk)\
	m(ELangKhmer, km)\
	m(ELangKorean, ko)\
	m(ELangLao, lo)\
	m(ELangLatvian, lv)\
	m(ELangLithuanian, lt)\
	m(ELangMacedonian, mk)\
	m(ELangMalay, ms)\
	m(ELangMalayalam, ml)\
	m(ELangMarathi, mr)\
	m(ELangMoldavian, ro)\
	m(ELangMongolian, mn)\
	m(ELangNorwegianNynorsk, no)\
	m(ELangBrazilianPortuguese, pt)\
	m(ELangPunjabi, pa)\
	m(ELangRomanian, ro)\
	m(ELangSerbian, sr)\
	m(ELangSinhalese, si)\
	m(ELangSomali, so)\
	m(ELangInternationalSpanish, es)\
	m(ELangLatinAmericanSpanish, es)\
	m(ELangSwahili, sw)\
	m(ELangFinlandSwedish, sv)\
	m(ELangTamil, ta)\
	m(ELangTelugu, te)\
	m(ELangTibetan, bo)\
	m(ELangTigrinya, ti)\
	m(ELangCyprusTurkish, tr)\
	m(ELangTurkmen, tk)\
	m(ELangUkrainian, uk)\
	m(ELangUrdu, ur)\
	m(ELangVietnamese, vi)\
	m(ELangWelsh, cy)\
	m(ELangZulu, zu)\

	
#define CASE_LANGUAGE(id, iso) case id: { _LIT(KTemp, #iso); return KTemp(); }
	switch(tl) {
		//case ELangEnglish: { _LIT(KTemp, "en"); return KTemp(); }
		LANGUAGES(CASE_LANGUAGE)
		default:
			return KNullDesC;
	}
}

int Syscall::maGetSystemProperty(const char* key, char* buf, int size) {
	TPtrC8 keyC8(CBP key);
	TPtrC16 value;
	_LIT8(KISO639, "mosync.iso-639-1");
#ifdef TELEPHONY
	_LIT8(KIMEI, "mosync.imei");
	_LIT8(KIMSI, "mosync.imsi");
	if(keyC8.Compare(KIMEI) == 0) {
		// fetch imei from telephony server
		Smartie<CLocalSynchronizer> ls(new CLocalSynchronizer());
		CTelephony::TPhoneIdV1 pid;
		CTelephony::TPhoneIdV1Pckg pidP(pid);
		gTelephony->GetPhoneId(*ls->Status(), pidP);
		ls->Wait();
		int result = ls->Status()->Int();
		LOG("IMEI result: %i\n", result);
		if(result < 0)
			return -3;
		value.Set(pid.iSerialNumber);
	} else if(keyC8.Compare(KIMSI) == 0) {
		// fetch imsi
		Smartie<CLocalSynchronizer> ls(new CLocalSynchronizer());
		CTelephony::TSubscriberIdV1 sid;
		CTelephony::TSubscriberIdV1Pckg sidP(sid);
		gTelephony->GetSubscriberId(*ls->Status(), sidP);
		ls->Wait();
		int result = ls->Status()->Int();
		LOG("IMSI result: %i\n", result);
		if(result < 0)
			return -3;
		value.Set(sid.iSubscriberId);
	} else
#endif	//TELEPHONY
	if(keyC8.Compare(KISO639) == 0) {
		value.Set(getIso639());
	}
	int len = value.Length();
	if(len == 0)
		return -2;
	// copy value to target buffer
	TPtr8 bufP((byte*)buf, size);
	if(size < len + 1)
		return len + 1;
	bufP.Copy(value);
	buf[len] = 0;
	return len + 1;
}


//------------------------------------------------------------------------------
// FileList
//------------------------------------------------------------------------------

class RootFileList : public FileList {
private:
	int mPos;
public:
	TDriveList mList;
	
	RootFileList() : mPos(0) {}
	int next(char* nameBuf, int bufSize) {
		LOGD("RootFileList::next(%i)\n", mPos);
		while(mPos < KMaxDrives) {
			if(mList[mPos] != 0) {
				//we've got a drive.
				if(bufSize > 3) {
					nameBuf[0] = 'A' + mPos;
					nameBuf[1] = ':';
					nameBuf[2] = '/';
					nameBuf[3] = 0;
					mPos++;
				}
				LOGD("RootFileList::next() returning 3 (%i) at %i\n", bufSize, mPos);
				return 3;
			}
			mPos++;
		}
		LOGD("RootFileList::next() returning 0 at %i\n", mPos);
		return 0;
	}
};

class DirFileList : public FileList {
private:
	int mPos;
public:
	CDir* mDir;
	
	DirFileList() : mPos(0), mDir(NULL) {}
	~DirFileList() {
		if(mDir)
			delete mDir;
	}
	int next(char* nameBuf, int bufSize) {
		if(mPos >= mDir->Count())
			return 0;
		const TEntry& e((*mDir)[mPos]);
		int len = e.iName.Length();
		if(e.IsDir()) {
			len++;
		}
		if(len < bufSize) {
			TPtr8 ptr((byte*)nameBuf, bufSize-1);
			ptr.Copy(e.iName);
			if(e.IsDir()) {
				ptr.Append('/');
			}
			nameBuf[len] = 0;
			mPos++;
		}
		return len;
	}
};

static int translateFileListErrorCode(int sym) {
	LOG("Error %i\n", sym);
	switch(sym) {
	case KErrPermissionDenied:
		return MA_FERR_FORBIDDEN;
	case KErrPathNotFound:
		return MA_FERR_NOTFOUND;
	default:
		return MA_FERR_GENERIC;
	}
}

MAHandle Syscall::maFileListStart(const char* path, const char* filter) {
	LOGD("maFileListStart(%s, %s)\n", path, filter);
	TCleaner<FileList> fl(NULL);
	MyRFs myrfs;
	myrfs.Connect();
	//LOG("connected\n");
	if(path[0] == 0) {	//empty string
		LOGD("list filesystem roots\n");
		//list filesystem roots
		Smartie<RootFileList> rfl(new RootFileList);
		int res = FSS.DriveList(rfl->mList);
		if(res < 0)
			return translateFileListErrorCode(res);
		fl = rfl.extract();
	} else {
		//list files in a directory
		Smartie<DirFileList> dfl(new DirFileList);
		//LOG("string hacking...\n");
		TPtrC8 pathPtrC8(CBP path);
		TPtrC8 filterPtrC8(CBP filter);
		Smartie<HBufC16> desc(HBufC16::NewL(pathPtrC8.Length() + filterPtrC8.Length()));
		TPtr16 des(desc->Des());
		
		//Append(des, pathPtrC8);
		//gotta swap '/' to '\'
		for(int i=0; i<pathPtrC8.Length(); i++) {
			if(pathPtrC8[i] == '/')
				des.Append('\\');
			else
				des.Append(pathPtrC8[i]);
		}
		
		Append(des, filterPtrC8);
		Smartie<HBufC8> temp8(CreateHBufC8FromDesC16L(des));
		LOGD("GetDir '%S'\n", temp8());
		int res = FSS.GetDir(des, KEntryAttMaskSupported, ESortNone, dfl->mDir);
		LOGD("res: %i\n", res);
		if(res < 0)
			return translateFileListErrorCode(res);
		//LOG("extract\n");
		fl = dfl.extract();
	}
	//LOG("PushL\n");
	CleanupStack::PushL(fl());
	gFileLists.insert(gFileListNextHandle, fl());
	//LOG("pop\n");
	fl.pop();
	//LOG("return\n");
	return gFileListNextHandle++;
}

int Syscall::maFileListNext(MAHandle list, char* nameBuf, int bufSize) {
	LOGD("maFileListNext(%i)\n", list);
	FileList* flp = gFileLists.find(list);
	MYASSERT(flp, ERR_FILE_HANDLE_INVALID);
	return flp->next(nameBuf, bufSize);
}

int Syscall::maFileListClose(MAHandle list) {
	LOGD("maFileListClose(%i)\n", list);
	FileList* flp = gFileLists.find(list);
	MYASSERT(flp, ERR_FILE_HANDLE_INVALID);
	gFileLists.erase(list);
	return 0;
}

//------------------------------------------------------------------------------
// maFile*
//------------------------------------------------------------------------------

#define FILE_FAIL(val) do { LOG_VAL(val); return val; } while(0)

int Syscall::maFileDate(MAHandle file) {
	LOGD("maFileDate(%i)\n", file);
	FileHandle& fh(getFileHandle(file));
	TTime modTime;
	// TODO: improve error code translation
	SYMERR_CONVERT(fh.fs->mFile.Modified(modTime), MA_FERR_GENERIC);
	return unixTime(modTime);
}

int Syscall::maFileTruncate(MAHandle file, int offset) {
	LOGD("maFileTruncate(%i, %i)\n", file, offset);
	FileHandle& fh(getFileHandle(file));
	if(!fh.fs) FILE_FAIL(MA_FERR_GENERIC);
	if(!fh.fs->isOpen()) FILE_FAIL(MA_FERR_GENERIC);
	SYMERR_CONVERT(fh.fs->mFile.SetSize(offset), MA_FERR_GENERIC);
	return 0;
}

int Syscall::maFileRename(MAHandle file, const char* newName) {
	LOGD("maFileRename(%i, %s)\n", file, newName);
	FileHandle& fh(getFileHandle(file));
	if(!fh.fs) FILE_FAIL(MA_FERR_GENERIC);
	if(!fh.fs->isOpen()) FILE_FAIL(MA_FERR_GENERIC);
	Smartie<HBufC16> nn(CreateHBufC16FromCStringL(newName));
	// TODO: The RFile::Rename function is simple, but doesn't follow the MoSync spec in a few cases.
	// Use RFs::Rename() instead.
	// May have to switch directory separators, too.
	SYMERR_CONVERT(fh.fs->mFile.Rename(*nn), MA_FERR_GENERIC);
	return 0;
}

int Syscall::getVolumeInfo(MAHandle file, TVolumeInfo& vi) {
	FileHandle& fh(getFileHandle(file));
	int drive;
	if(fh.name[1] == ':') {
		drive = EDriveA + (fh.name[0] - 'A');
	} else {
		drive = KDefaultDrive;
	}
	MyRFs myrfs;
	myrfs.Connect();
	return FSS.Volume(vi, drive);
}

int Syscall::maFileAvailableSpace(MAHandle file) {
	LOGD("maFileAvailableSpace(%i)\n", file);
	TVolumeInfo vi;
	SYMERR_CONVERT(getVolumeInfo(file, vi), MA_FERR_GENERIC);
	return I64LOW(MIN(vi.iFree, TInt64(0x7fffffff)));
}

int Syscall::maFileTotalSpace(MAHandle file) {
	LOGD("maFileTotalSpace(%i)\n", file);
	TVolumeInfo vi;
	SYMERR_CONVERT(getVolumeInfo(file, vi), MA_FERR_GENERIC);
	return I64LOW(MIN(vi.iSize, TInt64(0x7fffffff)));
}

	
//------------------------------------------------------------------------------
// CellID
//------------------------------------------------------------------------------
#ifndef __SERIES60_3X__	//Series 60, 2nd Ed.
int Syscall::maGetCellInfo(MACellInfo* ci) {
	RBasicGsmPhone phone;
	RTelServer server;		

	LHEL(server.Connect());
	// load a phone profile
	_LIT(KGsmModuleName, "phonetsy.tsy");
	LHEL(server.LoadPhoneModule(KGsmModuleName));

	// initialize the phone object
	RTelServer::TPhoneInfo info;
	LHEL(server.GetPhoneInfo(0, info));
	LHEL(phone.Open(server, info.iName));

	MBasicGsmPhoneNetwork::TCurrentNetworkInfo ni;
	LHEL(phone.GetCurrentNetworkInfo(ni));

	ci->cellId = ni.iCellId;
	ci->lac = ni.iLocationAreaCode;
	MBasicGsmPhoneNetwork::TBscNetworkId& id(ni.iNetworkInfo.iId);

	_LIT8(KFmt, "%i");
	{
		DEBUG_ASSERT(id.iMCC <= 999);	//max size: 4 bytes
		TPtr8 ptr((byte*)ci->mcc, sizeof(ci->mcc));
		ptr.Format(KFmt, id.iMCC);
		ptr.ZeroTerminate();
	}
	{
		DEBUG_ASSERT(id.iMNC <= 9999999);	//max size: 8 bytes
		TPtr8 ptr((byte*)ci->mnc, sizeof(ci->mnc));
		ptr.Format(KFmt, id.iMNC);
		ptr.ZeroTerminate();
	}

	//unload
	phone.Close();
	server.UnloadPhoneModule(KGsmModuleName);
	server.Close();

	return 0;
}
#endif

//------------------------------------------------------------------------------
// Camera
//------------------------------------------------------------------------------
void Syscall::createCamera() {
	if(!gCamera) {
		gCamera = CCamera::NewL(*this, 0);
		gCamera->CameraInfo(gCameraInfo);
		DUMPINT(gCameraInfo.iNumImageSizesSupported);
		DUMPHEX(gCameraInfo.iImageFormatsSupported);
		if(gCameraInfo.iImageFormatsSupported & CCamera::EFormatJpeg) {
			gCameraFormat = CCamera::EFormatJpeg;
		} else if(gCameraInfo.iImageFormatsSupported & CCamera::EFormatExif) {
			gCameraFormat = CCamera::EFormatExif;
		} else {	//camera doesn't support any jpeg version? unlikely. let's quit.
			DEBIG_PHAT_ERROR;
		}
	}
}

int Syscall::maCameraFormatNumber() {
	createCamera();
	return gCameraInfo.iNumImageSizesSupported;
}

int Syscall::maCameraFormat(int index, MA_CAMERA_FORMAT* fmt) {
	createCamera();
	MYASSERT(index >= 0 || index < gCameraInfo.iNumImageSizesSupported,
		ERR_INVALID_CAMERA_FORMAT_INDEX);
	TSize s;
	gCamera->EnumerateCaptureSizes(s, index, gCameraFormat);
	fmt->width = s.iWidth;
	fmt->height = s.iHeight;
	return 0;
}

int Syscall::maCameraStart() {
	LOG("maCameraStart()\n");
	if(gCameraState != CS_IDLE)
		return 0;
	createCamera();
	//we'll need an active object of our own to synchronize.
	CLocalSynchronizer sync;
	gCameraStatus = sync.Status();
	*gCameraStatus = KRequestPending;
	LOG("Reserve()\n");
	gCamera->Reserve();
	gCameraState = CS_RESERVING;
	sync.Wait();
	LOG("Start status: %i\n", sync.Status()->Int());
	if(IS_SYMBIAN_ERROR(*sync.Status()))
		return sync.Status()->Int();
	gScreenEngine.StopDrawing();

#if 1
	TSize size(gScreenEngine.iWindow.Size());
#else
	TSize size;
	//gCamera->EnumerateCaptureSizes(size, 1, CCamera::EFormatFbsBitmapColor16M);
#endif
	LOG("ViewFinder size: %ix%i\n", size.iWidth, size.iHeight);

#if 0	//KErrNotSupported
	TRect rect(size);
	LTRAP(gCamera->StartViewFinderDirectL(gScreenEngine.iClient,
		gScreenEngine.iScreenDevice, gScreenEngine.iWindow, rect));
#else
	LTRAP(gCamera->StartViewFinderBitmapsL(size));
#endif
	return 1;
}

void Syscall::ViewFinderFrameReady(CFbsBitmap& aFrame) {
	//TSize size = aFrame.SizeInPixels();
	//LOG("ViewFinderFrameReady(%ix%i)\n", size.iWidth, size.iHeight);
	TPoint dst(0,0);
	gScreenEngine.DrawImageDirect(aFrame, dst);
}

void Syscall::ReserveComplete(TInt aError) {
	LOG("ReserveComplete(%i)\n", aError);
	DEBUG_ASSERT(gCameraState == CS_RESERVING);
	if(IS_SYMBIAN_ERROR(aError)) {
		gCameraState = CS_IDLE;
		User::RequestComplete(gCameraStatus, aError);
		return;
	}
	LOG("PowerOn()\n");
	gCamera->PowerOn();
	gCameraState = CS_POWERING;
}
void Syscall::PowerOnComplete(TInt aError) {
	LOG("PowerOnComplete(%i)\n", aError);
	DEBUG_ASSERT(gCameraState == CS_POWERING);
	if(IS_SYMBIAN_ERROR(aError)) {
		gCamera->Release();
		gCameraState = CS_IDLE;
	} else {
		gCameraState = CS_POWERED;
	}
	User::RequestComplete(gCameraStatus, aError);
}

int Syscall::maCameraStop() {
	LOG("maCameraStop\n");
	if(gCameraState != CS_POWERED)
		return 0;
	gCamera->StopViewFinder();
	gCamera->PowerOff();
	gCamera->Release();
	if(!gScreenEngine.IsDrawing()) {
		LTRAP(gScreenEngine.StartDrawingL());
	}
	gCameraState = CS_IDLE;
	return 1;
}

int Syscall::maCameraSnapshot(int formatIndex, MAHandle placeholder) {
	LOG("maCameraSnapshot(%i, 0x%x)\n", formatIndex, placeholder);
	MYASSERT(gCameraState == CS_POWERED, ERR_CAMERA_UNPOWERED);
	LTRAP(gCamera->PrepareImageCaptureL(gCameraFormat, formatIndex));
	gCameraPlaceholder = placeholder;
	CLocalSynchronizer sync;
	gCameraStatus = sync.Status();
	*gCameraStatus = KRequestPending;
	gCamera->CaptureImage();
	sync.Wait();
	if(IS_SYMBIAN_ERROR(*sync.Status()))
		return sync.Status()->Int();
	return 0;
}

class HBufC8MemStream : public MemStream {
private:
	HBufC8* mData;
public:
	HBufC8MemStream(HBufC8* aData) : MemStream((char*)aData->Ptr(), aData->Size()) {
		mData = aData;
	}
	virtual ~HBufC8MemStream() {
		delete mData;
		mBuffer = NULL;	//prevent delete of Symbian heap memory
	}
};

void Syscall::ImageReady(CFbsBitmap* aBitmap, HBufC8* aData, TInt aError) {
	LOG("ImageReady(0x%X, 0x%X, %i)\n", aBitmap, aData, aError);
	DEBUG_ASSERT(aBitmap == NULL);
	DEBUG_ASSERT(aData != NULL);
	if(!IS_SYMBIAN_ERROR(aError)) {
		LOG("Adding memstream: %i bytes\n", aData->Size());
		HBufC8MemStream* ms = new HBufC8MemStream(aData);
		DEBUG_ASSERT(ms != NULL);
		SYSCALL_THIS->resources.add_RT_BINARY(gCameraPlaceholder, ms);
	}
	User::RequestComplete(gCameraStatus, aError);
}

void Syscall::FrameBufferReady(MFrameBuffer* aFrameBuffer, TInt aError) {
}


//------------------------------------------------------------------------------
// Application focus
//------------------------------------------------------------------------------
static int maSendToBackground() {
	CEikonEnv* eikonEnv = CEikonEnv::Static();
	TApaTask task(eikonEnv->WsSession());
	task.SetWgId(eikonEnv->RootWin().Identifier());
	task.SendToBackground();
	return 0;
}

static int maBringToForeground() {
	CEikonEnv* eikonEnv = CEikonEnv::Static();
	TApaTask task(eikonEnv->WsSession());
	task.SetWgId(eikonEnv->RootWin().Identifier());
	task.BringToForeground();
	return 0;
}


//------------------------------------------------------------------------------
// Location
//------------------------------------------------------------------------------
#if defined(SUPPORT_MOSYNC_SERVER) && !defined(__S60_50__)
int Syscall::maLocationStart() {
	LOG("maLocationStart()\n");
	if(gLocationSync->IsActive())
		return 0;
	gServer.LocationGet(*gLocationSync->Status());
	gLocationSync->SetActive();
	return MA_LPS_AVAILABLE;
}

int Syscall::maLocationStop() {
	int res = gServer.LocationStop();	//silent fail
	LOG("LocationStop: %i\n", res);
	if(gLocationSync)
		gLocationSync->Cancel();
	LOG("maLocationStop done.\n");
	return res;
}

void Syscall::LocationHandlerL(TInt status) {
	LOG("LocationHandlerL(%i)\n", status);
	//not so good; will fail silently.
	if(IS_SYMBIAN_ERROR(status))
		return;

	//send event
	AddLocationEvent(gServer.Position());

	//wait for the next location
	gServer.LocationGet(*gLocationSync->Status());
	gLocationSync->SetActive();
}
#endif	//SUPPORT_MOSYNC_SERVER && !__S60_50__

#if defined(SUPPORT_MOSYNC_SERVER) || defined(__S60_50__)

//quick hack
union DV {
	int i[2];
	double dbl;
};

static double swapd(double d) {
	DV dv1;
	dv1.dbl = d;
	DV dv2;
	dv2.i[0] = dv1.i[1];
	dv2.i[1] = dv1.i[0];
	return dv2.dbl;
}

void Syscall::AddLocationEvent(const TPosition& p) {
	MALocation* loc = new MALocation;
	LOG("lat: %g\n", p.Latitude());
	loc->state = p.Datum() == KPositionDatumWgs84 ?
		MA_LOC_QUALIFIED : MA_LOC_INVALID;
	loc->lat = swapd(p.Latitude());
	loc->lon = swapd(p.Longitude());
	loc->horzAcc = swapd(p.HorizontalAccuracy());
	loc->vertAcc = swapd(p.VerticalAccuracy());
	loc->alt = p.Altitude();

	MAEvent e;
	e.type = EVENT_TYPE_LOCATION;
	//will be handled by GetEvent.
	//definite memory leak if event is never Gotten.
	//must go through all remaining events on shutdown and delete relevant memory.
	e.data = loc;
	gAppView.AddEvent(e);
}
#endif	//SUPPORT_MOSYNC_SERVER || __S60_50__

#ifdef __S60_50__
int Syscall::maLocationStart() {
	LOG("maLocationStart()\n");
	if(gLocationSync->IsActive())
		return 0;

	SYMTESTFAIL(gPositionServer.Connect());
	SYMTESTFAIL(gPositioner.Open(gPositionServer));

	// Needed to placate Symbian's sense of "user privacy".
	SYMTESTFAIL(gPositioner.SetRequestor(CRequestor::ERequestorService,
		CRequestor::EFormatApplication, KMoSync));

	gPositioner.NotifyPositionUpdate(gPositionInfo, *gLocationSync->Status());
	gLocationSync->SetActive();

	return MA_LPS_AVAILABLE;
}

int Syscall::maLocationStop() {
	LOG("maLocationStop()\n");
	if(gLocationSync) {
		if(gLocationSync->IsActive())
			TSNR(gPositioner.CancelRequest(EPositionerNotifyPositionUpdate));
		gPositioner.Close();
		gPositionServer.Close();
	}
	LOG("maLocationStop done.\n");
	return 0;
}

void Syscall::LocationHandlerL(TInt status) {
	//not so good; will fail silently.
	if(IS_SYMBIAN_ERROR(status)) {
		LOG("LocationHandlerL(%i)\n", status);
		return;
	}

	//send event
	TPosition p;
	gPositionInfo.GetPosition(p);
	AddLocationEvent(p);

	//wait for the next location
	gPositioner.NotifyPositionUpdate(gPositionInfo, *gLocationSync->Status());
	gLocationSync->SetActive();
}
#endif	//__S60_50__


#ifdef CALL
void Syscall::CallHandlerL(int status) {
	LOG("CallHandlerL %i\n", status);
	if(status == KErrNone) {
		gCallIdIsValid = true;
	}
}
void Syscall::HangupHandlerL(int status) {
	LOG("HangupHandlerL %i\n", status);
}
#endif	//CALL

//SMS handling
#ifdef __SERIES60_3X__
void Syscall::MessageSentL(CRSendAsSender* aSender, TInt aError) {
	LOG("MessageSentL %i\n", aError);
	gSmsSenderCleanupQue->move(aSender);
}
#else	//Series 60, 2nd Ed.
//MSmsObserver
void Syscall::MovedToOutbox() {
	LOG("MovedToOutbox\n");
}
void Syscall::ServerSessionEstablished() {
	LOG("ServerSessionEstablished\n");
}
void Syscall::ServerSessionError() {
	LOG("ServerSessionError\n");
}
void Syscall::ServerDown(MMsvSessionObserver::TMsvSessionEvent /*aEvent*/) {
	LOG("ServerDown\n");
}
void Syscall::NoServiceCenter() {
	LOG("NoServiceCenter\n");
}
void Syscall::MessageReceived() {
	LOG("MessageReceived\n");
}
void Syscall::NoMoreUnread() {
	LOG("NoMoreUnread\n");
}
#endif	//__SERIES60_3X__


#if defined(PUBLIC_DEBUG) || defined(LOGGING_ENABLED)
#define MAPANIC_RESULT result
#else
#define MAPANIC_RESULT
#endif
SYSCALL(void, maPanic(int MAPANIC_RESULT, const char* message)) {
	LOG("maPanic %i, \"%s\"\n", result, message);

	//show message and wait until user reponds
	HBufC16* buf = CreateHBufC16FromCStringLC(message);
#ifdef PUBLIC_DEBUG
	writePanicReport(REPORT_USER_PANIC, result, *buf);
#endif
	ShowAknErrorNoteThenExitL(*buf);
	CleanupStack::PopAndDestroy(buf);
}

SYSCALL(int, maSoundPlay(MAHandle sound_res, int offset, int size)) {
	LOGA("SP %i\n",sound_res);
	maSoundStop();

	Stream* src = resources.get_RT_BINARY(sound_res);

	//read the MIME type
	MYASSERT(src->seek(Seek::Start, offset), ERR_DATA_ACCESS_FAILED);
	TBuf8<64> mime(64);
	{
		int pos = 0;
		do {
			if(!src->readByte(mime[pos])) {
				BIG_PHAT_ERROR(ERR_MIME_READ_FAILED);
			}
			if(mime[pos] == 0)
				break;
			pos++;
		} while(true);
		mime.SetLength(pos);
	}
	LOGA("MIME type: \"%S\"\n", &mime);
	const int mimeLength = mime.Length() + 1;	//include the null terminator
	const int dataLength = size - mimeLength;

	//find Controller UID here
	if(!gMmfCPSP) {
		gMmfCPSP = CMMFControllerPluginSelectionParameters::NewL();
		gMmfFSP = CMMFFormatSelectionParameters::NewL();
	}
	gMmfFSP->SetMatchToMimeTypeL(mime);
	gMmfCPSP->SetRequiredPlayFormatSupportL(*gMmfFSP);

	RMMFControllerImplInfoArray ciia;
	gMmfCPSP->ListImplementationsL(ciia);
	CleanupResetAndDestroyPushL(ciia);

	LOGA("%i impls\n", ciia.Count());
	if(ciia.Count() == 0) {
		CleanupStack::Pop();	//ciia
		return -1;
	}

	//CMMFControllerImplementationInformation* chosenCii = ciia[0];
	//CMMFControllerImplementationInformation* chosenCii = ciia[ciia.Count() - 1];

	//pick the most specialized implementation
	CMMFControllerImplementationInformation* chosenCii = NULL;
	int minIds = (1 << 30);
	for(int i=0; i<ciia.Count(); i++) {
		CMMFControllerImplementationInformation* cii = ciia[i];
		int nids = cii->SupportedMediaIds().Count();
#ifdef AUDIO_DEBUGGING_MODE
		HBufC8* buf = CreateHBufC8FromDesC16LC(cii->DisplayName());
		LOGA("%i: 0x%08X(%S), %i\n", i, cii->Uid(), buf, nids);
		CleanupStack::PopAndDestroy(buf);
#endif
		if(nids > 0 && nids < minIds) {
			chosenCii = cii;
			minIds = nids;
		}
	}
	DEBUG_ASSERT(chosenCii != NULL);

#ifdef MMF
	TMMFPrioritySettings dummySettings;	//ignored since we don't have MultimediaDD capbility.
	LHEL(gController.Open(*chosenCii, dummySettings));
	RMMFAudioControllerCustomCommands accc(gController);
#else	//Mda
	gPlayer = CMdaAudioRecorderUtility::NewL(*this);
	const TUid controllerUid = chosenCii->Uid();
#endif	//MMF

	CleanupStack::PopAndDestroy();	//ciia, see Push above.

	if(src->ptrc() == NULL) {
#if 1	//INEFFICIENT
		gSoundBuffer = new MemStream(dataLength);
		MYASSERT(gSoundBuffer != NULL, ERR_OOM);
		MYASSERT(src->readFully(*gSoundBuffer), ERR_DATA_ACCESS_FAILED);
		LOGA("DataLength: %i\n", dataLength);
		gSoundPtr.Set(((byte*)gSoundBuffer->ptr()) + offset,
			dataLength, dataLength);
#else
		LimitedFileStream* lfs = (LimitedFileStream*)src;
		//might not work, possibly depending on format/phone combo.
		//update: doesn't work on 3rd ed fp1 emulator
		gPlayer = CMdaAudioPlayerUtility::NewL(*this);
		gPlayer->OpenFileL(lfs->mFile);
#endif	//1
	} else {
		LOGA("DataLength: %i\n", dataLength);
		gSoundPtr.Set(((byte*)src->ptr()) + offset + mimeLength,
			dataLength, dataLength);
	}
#ifdef MMF
#if 0	//StreamSource
	TMMFStreamParams params;
	params.source = src;
	TMMFStreamConfig config(params);
	LHEL(gController.AddDataSource(KUidMmfStreamSource, config));

#else	//DescriptorSource
	TMMFDescriptorParams params;
	//TMyPtr8 dummyPtr;
	params.iDes = &gSoundPtr;//&dummyPtr;//
	params.iDesThreadId = RThread().Id();	//guess
	TMMFDescriptorConfig config(params);
	LHEL(gController.AddDataSource(KUidMmfDescriptorSource, config));

	TBuf8<4> buf;
	buf.SetLength(4);
	TFourCC& fcc((TFourCC&)*(buf.Ptr()));
	int res = accc.GetSourceDataType(fcc);
	if(IS_SYMBIAN_ERROR(res)) {
		LOG("Error getting FourCC: %i\n", res);
	} else {
		LOGA("FourCC: \"%S\"\n", &buf);
	}
#endif	//0

	gControllerEventMonitor->Start();
	LHEL(gController.AddDataSink(KUidMmfAudioOutput, KNullDesC8));
	LHEL(gController.Prime());
	maSoundSetVolume(gSoundVolume);
	LHEL(gController.Play());
#else	//Mda
	maSoundSetVolume(gSoundVolume);
#if defined(__SERIES60_3X__)
	gPlayer->OpenDesL(gSoundPtr, controllerUid);
#else	//2nd edition
	//_LIT(KFileName, "C:\\mobilesorcery2.mp3");	//file test

	//should pick a file ending from
	//CMMFControllerImplementationInformation->PlayFormats()
	//CMMFFormatImplementationInformation

	_LIT(KFileName, "D:\\system\\temp\\temp.blaah");
	//nah, file ending is ignored.
	{
		WriteFileStream file("D:\\system\\temp\\temp.blaah");
		Smartie<Stream> soundSrc(src->createLimitedCopy(dataLength));
		MYASSERT(file.writeFully(*soundSrc), ERR_DATA_ACCESS_FAILED);
	}
	gPlayer->OpenFileL(KFileName, controllerUid);
#endif	//__SERIES60_3X__
#endif	//MMF

	gPlaying = true;

	VM_Yield();
	return 1;
}

#ifdef MMF
void Syscall::HandleEvent(const TMMFEvent& aEvent) {
	LOGA("MMFEvent: 0x%08X %i\n", aEvent.iEventType.iUid, aEvent.iErrorCode);
	if(aEvent.iEventType == KMMFEventCategoryPlaybackComplete) {
		gPlaying = false;
	}
}
#endif	//MMF

SYSCALL(void, maSoundStop()) {
	LOGA("SS\n");
	if(gPlayer)
		gPlayer->Close();
#ifdef MMF
	gControllerEventMonitor->Cancel();
	gController.Close();
#endif
	SAFE_DELETE(gPlayer);
	SAFE_DELETE(gSoundBuffer);
	gPlaying = false;
}

SYSCALL(int, maSoundIsPlaying()) {
	return gPlaying;
}

SYSCALL(int, maSoundGetVolume()) {
	return gSoundVolume;
}	

SYSCALL(void, maSoundSetVolume(int vol)) {
	LOGA("SV %i\n", vol);
	if(vol < 0) vol = 0;
	if(vol > 100) vol = 100;
	gSoundVolume = vol;

	int maxVolume;
#ifdef MMF
	RMMFAudioPlayDeviceCustomCommands cc(gController);
	LHEL(cc.GetMaxVolume(maxVolume));
#else
	if(!gPlayer)
		return;
	maxVolume = gPlayer->MaxVolume();
#endif
	LOGA("maxVolume: %i\n", maxVolume);
	if(maxVolume == 0) {
		return;
	}

	float fvol = vol;
	fvol /= 100;
	fvol *= maxVolume;
	LOGA("Native volume: %i\n", (int)fvol);

#ifdef MMF
	LHEL(cc.SetVolume((int)fvol));
#else	//Mda
	gPlayer->SetVolume((int)fvol);
#endif	//MMF
}

#ifndef MMF
void Syscall::MoscoStateChangeEvent(CBase* /*aObject*/, TInt aPreviousState,
	TInt aCurrentState, TInt aErrorCode)
{
	LOGA("MoscoSC %d %d %d\n", aPreviousState, aCurrentState, aErrorCode);
	//States are CMdaAudioClipUtility::TState
	if(aErrorCode != KErrNone) {
		LOG("MoscoSC %d %d %d\n", aPreviousState, aCurrentState, aErrorCode);
		BIG_PHAT_ERROR(SYMERR_SOUND_DECODING_ERROR);
	}
	if(aPreviousState == CMdaAudioClipUtility::ENotReady &&
		aCurrentState == CMdaAudioClipUtility::EOpen)
	{
		gPlayer->PlayL();
	}
	if(aPreviousState == CMdaAudioClipUtility::EPlaying &&
		aCurrentState != CMdaAudioClipUtility::EPlaying)
	{
		gPlaying = false;
		//TODO: add_sound_event
	}
}
#endif	//MMF


#ifdef SUPPORT_AUDIOBUFFER

#define SAMPLE_RATES(m)\
	m(8000)\
	m(11025)\
	m(12000)\
	m(16000)\
	m(22050)\
	m(24000)\
	m(32000)\
	m(44100)\
	m(48000)\
	m(96000)\
	m(64000)\


int Syscall::maAudioBufferInit(const MAAudioBufferInfo* info) {
	LOGA("maAudioBufferInit\n");
	maAudioBufferClose();

	gAudioBuffer.des.Set(
		CBP GetValidatedMemRange((int)info->buffer, info->bufferSize),
		info->bufferSize);
	
	switch(info->sampleRate) {
#define CASE_SAMPLE_RATE(rate)\
	case rate: gAudioBuffer.sampleRateId = TMdaAudioDataSettings::ESampleRate##rate##Hz; break;
		SAMPLE_RATES(CASE_SAMPLE_RATE);
	default:
		LOG("Unsupported sample rate: %i\n", info->sampleRate);
		return -1;
	}

	switch(info->numChannels) {
	case 1:
		gAudioBuffer.chanId = TMdaAudioDataSettings::EChannelsMono;
		break;
	case 2:
		gAudioBuffer.chanId = TMdaAudioDataSettings::EChannelsStereo;
		break;
	default:
		LOG("Unsupported channel number: %i\n", info->numChannels);
		return -1;
	}
	
	gAudioBuffer.ready = false;

	LOGA("CMAOS:NewL\n");
	gAudioStream = CMdaAudioOutputStream::NewL(*this, EMdaPriorityNormal,
		EMdaPriorityPreferenceTime);
	LOGA("CMAOS.Open\n");
	gAudioStream->Open(&gAudioSettings);	//causes MaoscOpenComplete() to be called later
	LOGA("init complete\n");
	return 1;
}

void Syscall::MaoscOpenComplete(TInt aError) {
	LOGA("MaoscOpenComplete %i\n", aError);
	if(aError >= 0) {
		LTRAP(gAudioStream->SetAudioPropertiesL(gAudioBuffer.sampleRateId,
			gAudioBuffer.chanId));
		DUMPINT(gAudioStream->MaxVolume());
		gAudioStream->SetVolume(gAudioStream->MaxVolume());

		gAudioBuffer.ready = true;
	}

	//send event
	MAEvent e;
	e.type = EVENT_TYPE_AUDIOBUFFER_FILL;
	e.state = aError;
	gAppView.AddEvent(e);
}

int Syscall::maAudioBufferReady() {
	LOGA("maAudioBufferReady\n");
	if(gAudioBuffer.ready) {
		LTRAP(gAudioStream->WriteL(gAudioBuffer.des));
		gAudioBuffer.ready = false;
		return 1;
	} else {
		return 0;
	}
}

void Syscall::MaoscBufferCopied(TInt aError, const TDesC8 &aBuffer) {
	LOGA("MaoscBufferCopied %i\n", aError);
	if(aError >= 0) {
		gAudioBuffer.ready = true;
		//send event
		MAEvent e;
		e.type = EVENT_TYPE_AUDIOBUFFER_FILL;
		e.state = aError;
		gAppView.AddEvent(e);
	}
}

int Syscall::maAudioBufferClose() {
	LOGA("maAudioBufferClose\n");
	if(gAudioStream) {
		gAudioBuffer.ready = false;
		gAudioStream->Stop();
		delete gAudioStream;
		gAudioStream = NULL;
	}
	return 1;
}

void Syscall::MaoscPlayComplete(TInt aError) {
	LOGA("MaoscPlayComplete %i\n", aError);
	gAudioBuffer.ready = false;
}
#endif


int Syscall::maStreamVideoStart(const char* url) {
	MYASSERT(gStreamState == SS_IDLE, ERR_STREAM_TOO_MANY);
	LOGT("maStartVideoStream %s", url);
	if(gVideoPlayer == NULL) {
		const TRect& clip = gScreenEngine.GetScreenClip();
		TRect size(TSize(gScreenEngine.Width(), gScreenEngine.Height()));
		gVideoPlayer = CVideoPlayerUtility::NewL(*this, EMdaPriorityNormal,
			EMdaPriorityPreferenceNone,
			gScreenEngine.iClient, gScreenEngine.iScreenDevice, gScreenEngine.iWindow,
			size, clip);
		gVideoPlayer->RegisterForVideoLoadingNotification(*this);
	}
	HBufC16* url16 = CreateHBufC16FromCStringLC(url);
	gVideoPlayer->OpenUrlL(*url16);
	gStreamState = SS_OPENING;
	gStreamWantsToPause = false;
	return ++gStreamHandle;
}

int Syscall::maStreamClose(MAHandle stream) {
	MYASSERT(gStreamState != SS_IDLE && stream == gStreamHandle, ERR_STREAM_HANDLE);
	gVideoPlayer->Close();
	gStreamState = SS_IDLE;
	return 0;
}

int Syscall::maStreamPause(MAHandle stream) {
	MYASSERT(gStreamState != SS_IDLE && stream == gStreamHandle, ERR_STREAM_HANDLE);
	if(gStreamState == SS_PLAYING) {
		gVideoPlayer->PauseL();
		gStreamState = SS_PAUSED;
	} else if(gStreamState != SS_PAUSED) {
		//don't play when you otherwise would
		gStreamWantsToPause = true;
	}
	return 0;
}

int Syscall::maStreamResume(MAHandle stream) {
	MYASSERT(gStreamState != SS_IDLE && stream == gStreamHandle, ERR_STREAM_HANDLE);
	//turn off any previous pause
	gStreamWantsToPause = false;
	if(gStreamState == SS_PAUSED) {
		gVideoPlayer->Play();
		gStreamState = SS_PLAYING;
	}
	return 0;
}


#define HANDLE_LEAVE(prefix, function, args) {\
	TRAPD(res##function, prefix function args);\
	if(res##function != KErrNone) {\
		LOG("%s left with %i\n", #function, res##function);\
		return -2;	/*FIXME*/\
	}\
}


MAExtent Syscall::maStreamVideoSize(MAHandle stream) {
	MYASSERT(gStreamState != SS_IDLE && stream == gStreamHandle, ERR_STREAM_HANDLE);
	TSize size;
	/*TRAPD(res, gVideoPlayer->VideoFrameSizeL(size));
	if(res != KErrNone) {
		LOG("VideoFrameSizeL left with %i\n", res);
		return -2;	//FIXME
	}*/
	HANDLE_LEAVE(gVideoPlayer->, VideoFrameSizeL, (size));
	return EXTENT(size.iWidth, size.iHeight);
}

int Syscall::maStreamVideoSetFrame(MAHandle stream, const MARect* rect) {
	MYASSERT(gStreamState != SS_IDLE && stream == gStreamHandle, ERR_STREAM_HANDLE);
	TRect frameRect(TPoint(rect->left, rect->top), TSize(rect->width, rect->height));
	DUMP_RECT(frameRect);

	HANDLE_LEAVE(gVideoPlayer->, SetDisplayWindowL, (gScreenEngine.iClient,
		gScreenEngine.iScreenDevice, gScreenEngine.iWindow, frameRect,
		frameRect));
	return 1;
}

int Syscall::maStreamLength(MAHandle stream) {
	MYASSERT(gStreamState != SS_IDLE && stream == gStreamHandle, ERR_STREAM_HANDLE);
	TTimeIntervalMicroSeconds d;
	HANDLE_LEAVE(d = gVideoPlayer->, DurationL, ());
	return I64LOW(d.Int64() / 1000);
}

int Syscall::maStreamPos(MAHandle stream) {
	MYASSERT(gStreamState != SS_IDLE && stream == gStreamHandle, ERR_STREAM_HANDLE);
	TTimeIntervalMicroSeconds p;
	HANDLE_LEAVE(p = gVideoPlayer->, PositionL, ());
	return I64LOW(p.Int64() / 1000);
}

int Syscall::maStreamSetPos(MAHandle stream, int pos) {
	MYASSERT(gStreamState != SS_IDLE && stream == gStreamHandle, ERR_STREAM_HANDLE);
	if(pos < 0)
		pos = 0;
	if(gStreamState == SS_PLAYING && !gStreamSetPosPausable) {
		//delay until LoadingStarted
		gStreamNewPos = pos;
		return pos;
	}
	//gotta pause, if playing
	if(gStreamState == SS_PLAYING && gStreamSetPosPausable) {
		gVideoPlayer->PauseL();
	}
	TTimeIntervalMicroSeconds p = TInt64(pos) * 1000;
	HANDLE_LEAVE(gVideoPlayer->, SetPositionL, (p));
	HANDLE_LEAVE(p = gVideoPlayer->, PositionL, ());
	//unpause if you were playing before.
	if(gStreamState == SS_PLAYING && gStreamSetPosPausable) {
		gVideoPlayer->Play();
		gStreamSetPosPausable = false;
	}
	return I64LOW(p.Int64() / 1000);
}


void Syscall::AddStreamEvent(int event, int result) {
	MAEvent e;
	e.type = EVENT_TYPE_STREAM;
	MAStreamEventData* sed = new (ELeave) MAStreamEventData;
	sed->event = event;
	sed->result = result;
	//will be handled by GetEvent.
	//definite memory leak if event is never gotten.
	//must go through all remaining events on shutdown and delete relevant memory.
	e.data = sed;
	gAppView.AddEvent(e);
}

//MVideoLoadingObserver
void Syscall::MvloLoadingStarted() {
	LOGT("MvloLoadingStarted");
	gStreamSetPosPausable = true;
	AddStreamEvent(STREAMEVENT_LOADING_STARTED, 0);
	if(gStreamNewPos != -1) {
		int res = maStreamSetPos(gStreamHandle, gStreamNewPos);
		gStreamNewPos = -1;
		if(res < 0) {
			AddStreamEvent(STREAMEVENT_UNKNOWN_EVENT, res);
		}
	}
}
void Syscall::MvloLoadingComplete() {
	LOGT("MvloLoadingComplete");
	AddStreamEvent(STREAMEVENT_LOADING_COMPLETE, 0);
}

//MVideoPlayerUtilityObserver
void Syscall::MvpuoOpenComplete(TInt aError) {
	LOGT("MvpuoOpenComplete %i", aError);
	AddStreamEvent(STREAMEVENT_OPEN_COMPLETE, aError);
	if(aError == KErrNone) {
		LOG("Prepare...\n");
		gVideoPlayer->Prepare();
		gStreamState = SS_PREPARING;
	} else {
		gStreamState = SS_ERROR;
	}
}

void Syscall::MvpuoPrepareComplete(TInt aError) {
	LOGT("MvpuoPrepareComplete %i", aError);
	AddStreamEvent(STREAMEVENT_PREPARE_COMPLETE, aError);
	if(aError == KErrNone) {
		TSize videoSize;
		gVideoPlayer->VideoFrameSizeL(videoSize);
		TRect frameRect(videoSize);
		DUMP_RECT(frameRect);

		gVideoPlayer->SetDisplayWindowL(gScreenEngine.iClient,
			gScreenEngine.iScreenDevice, gScreenEngine.iWindow, frameRect,
			frameRect);

		if(gStreamWantsToPause) {
			gStreamState = SS_PAUSED;
		} else {
			gVideoPlayer->Play();
			gStreamState = SS_PLAYING;
		}
	} else {
		gStreamState = SS_ERROR;
	}
}
void Syscall::MvpuoFrameReady(CFbsBitmap &aFrame, TInt aError) {
	LOGT("MvpuoFrameReady %i", aError);
}
void Syscall::MvpuoPlayComplete(TInt aError) {
	LOGT("MvpuoPlayComplete %i", aError);
	AddStreamEvent(STREAMEVENT_PLAY_COMPLETE, aError);
	if(aError == KErrNone) {
		gStreamState = SS_PAUSED;
	} else {
		gStreamState = SS_ERROR;
	}
}
void Syscall::MvpuoEvent(const TMMFEvent &aEvent) {
	LOGT("MvpuoEvent 0x%x %i", aEvent.iEventType, aEvent.iErrorCode);
	AddStreamEvent(STREAMEVENT_UNKNOWN_EVENT, aEvent.iErrorCode);
	if(aEvent.iErrorCode != KErrNone) {
		gStreamState = SS_ERROR;
	}
}

int Syscall::maTextBox(const wchar* title, const wchar* inText, wchar* outText,
	int maxSize, int constraints)
{
	TPtrC tTitle(title);
	TPtrC tInText(inText);
	TPtr tOutText(outText, maxSize - 1);
	int res = gAppView.TextBox(tTitle, tInText, tOutText, constraints);
	DEBUG_ASSERT(tOutText.Length() < maxSize);
	outText[tOutText.Length()] = 0;
	
	// send message
	MAEvent e;
	e.type = EVENT_TYPE_TEXTBOX;
	e.textbox.textboxResult = res;
	e.textbox.textboxLength = tOutText.Length();
	gAppView.AddEvent(e);
	
	return res;
}
