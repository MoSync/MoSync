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

#include "../../../Base/FileStream.h"

#include "syscall.h"
#include "core.h"

#include "symbian_helpers.h"
#include "resourcearray.h"
#include "DirScrAccEng.h"
#include "AppView.h"
#include "symbian_errors.h"
#include "Stream.h"
#include "heapsize.h"

#ifdef GUIDO
#include <helpers/CPP_IX_GUIDO.H>
#endif

#ifdef SUPPORT_MOSYNC_SERVER
#include <helpers/CPP_IX_CELLID.h>
#endif

#ifdef CALL
#include <helpers/CPP_IX_CALL.h>
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
#ifdef SUPPORT_MOSYNC_SERVER
	gLocationSync = NULL;
#endif
#ifdef SUPPORT_AUDIOBUFFER
	gAudioStream = NULL;
	gAudioBuffer.ready = false;
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

#ifdef SUPPORT_MOSYNC_SERVER
	gLocationSync = new (ELeave) CClassSynchronizer<Syscall>(this, &Syscall::LocationHandlerL);
#endif

	if(!gScreenEngine.IsDrawing())
			gScreenEngine.StartDrawingL();

	gStartTime = maGetMilliSecondCount();

	DebugMarkStart();
}

void Syscall::StopEverything() {
	LOG("StopEverything()\n");

#ifdef SUPPORT_MOSYNC_SERVER
	maLocationStop();
#endif

#ifdef MA_PROF_SUPPORT_VIDEO_STREAMING
	if(gVideoPlayer != NULL) {
		gVideoPlayer->Close();
	}
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

	RestoreDrawTarget();

	StopNetworking();

#ifdef __SERIES60_3X__
	//SMS senders
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

#ifdef SUPPORT_MOSYNC_SERVER
	LOG("gServer.Close();\n");
	gServer.Close();
	
	//calls Cancel, which should really call the server's cancel mechanism...
	//except it doesn't have one. tricky...
	LOG("SAFE_DELETE(gLocationSync);\n");
	SAFE_DELETE(gLocationSync);
#endif

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

	SAFE_DELETE(gMmfCPSP);

	SAFE_DELETE(gMmfFSP);

	DestructBluetooth();
	DestructNetworking();
	gSocketServ.Close();

	gKeyLock.Close();

	SAFE_DELETE(gVibraControl);
	
	gStoreMap.Close();

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

SYSCALL(void, maDrawText(int left, int top, const char* str)) {
	if(*str == 0) {
		return;
	}
	TCleaner<HBufC16> buf(CreateHBufC16FromCStringLC(str));
	gScreenEngine.DrawTextL(*buf, TPoint(left, top));
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
	TBitmapUtil clr(img->Color());
	clr.Begin(TPoint(0,0));	//begin point is irrelevant, since we do SetPos.

	//set to color if no alpha, to avoid potential crash on NULL.
	TBitmapUtil alpha(hasAlpha ? img->Alpha() : img->Color());
	if(hasAlpha)
		alpha.Begin(TPoint(0,0));

	//iterate through the source, combining and copying pixels
	int dY = 0;
	for(int sY=lSrcRect.iTl.iY; sY<lSrcRect.iBr.iY; sY++) {
		int dX = 0;
		clr.SetPos(TPoint(lSrcRect.iTl.iX, sY));
		if(hasAlpha)
			alpha.SetPos(TPoint(lSrcRect.iTl.iX, sY));
		for(int sX=lSrcRect.iTl.iX; sX<lSrcRect.iBr.iX; sX++) {
			//will work properly only on 32-bit 3rd edition devices.
			TUint32 pixel = clr.GetPixel();
			clr.IncXPos();
			if(hasAlpha) {
				pixel |= alpha.GetPixel() << 24;
				alpha.IncXPos();
			} else {
				pixel |= 0xFF000000;
			}
			dstInt[dY*scanlength + dX] = pixel;
			dX++;
		}
		dY++;
	}

	clr.End();
	if(hasAlpha)
		alpha.End();	
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
	clrUtil.Begin(TPoint(0, 0));
	for(int y=0; y<height; y++) {
		for(int x=0; x<width; x++) {
			clrUtil.SetPixel(srcInt[y*width + x] & 0xFFFFFF);	//uuh
			clrUtil.IncXPos();	//wraps around for us
		}
		//clrUtil.IncYPos();	//not needed because of wrap
	}
	clrUtil.End();

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
		alphaUtil.Begin(TPoint(0, 0));
		for(int y=0; y<height; y++) {
			for(int x=0; x<width; x++) {
				alphaUtil.SetPixel(srcInt[y*width + x] >> 24);	//uuh
				alphaUtil.IncXPos();
			}
			//alphaUtil.IncYPos();
		}
		alphaUtil.End();
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

SYSCALL(MAHandle, maOpenStore(const char* name, int flags)) {
	MyRFs myrfs;
	myrfs.Connect();
#ifdef LOGGING_ENABLED
	int res =
#endif
	FSS.MkDir(KMAStorePath16);
	LOG("MkDir %i\n", res);

	TPtrC8 nameDesC(CBP name);
	TCleaner<HBufC8> path(HBufC8::NewLC(KMAStorePath8().Length() + nameDesC.Length() + 1));
	path->Des().Append(KMAStorePath8);
	path->Des().Append(nameDesC);
	path->Des().Append(0);	//hack

	FileStream readFile(CCP path->Ptr());
	if(!readFile.isOpen()) {
		if(flags & MAS_CREATE_IF_NECESSARY) {
			WriteFileStream writeFile(CCP path->Ptr());
			if(!writeFile.isOpen()) {
				return STERR_GENERIC;
			}
		} else {
			return STERR_NONEXISTENT;
		}		
	}
 	gStoreMap.InsertL(gNextStoreId, *path);
	return gNextStoreId++;
}

SYSCALL(int, maWriteStore(MAHandle store, MAHandle data)) {
	const char* path = gStoreMap.FindL(store);
	MyRFs myrfs;
	myrfs.Connect();

	WriteFileStream writeFile(path);
	Stream* bp = resources.get_RT_BINARY(data);

	int len;
	MYASSERT(bp->length(len), ERR_DATA_ACCESS_FAILED);
	TVolumeInfo vi;
	LHEL(FSS.Volume(vi, EDriveC));
	if(vi.iFree <= len + 10*1024) {	//magic number
		return STERR_FULL;
	}
	if(!writeFile.writeFully(*bp)) {
		return STERR_GENERIC;
	}
	return 1;
}

SYSCALL(int, maReadStore(MAHandle store, MAHandle placeholder))
{
	const char* path = gStoreMap.FindL(store);
	MYASSERT(path != NULL, ERR_STORE_READ_FAILED);
	MyRFs myrfs;
	myrfs.Connect();

	FileStream readFile(path);
	int len;
	MYASSERT(readFile.length(len), ERR_STORE_READ_FAILED);
	Smartie<MemStream> p(new MemStream(len));
	if(len != 0) {
		if(!p->isOpen())
			return RES_OUT_OF_MEMORY;
		MYASSERT(readFile.readFully(*p), ERR_STORE_READ_FAILED);
	}
	return resources.add_RT_BINARY(placeholder, p.extract());
}

SYSCALL(void, maCloseStore(MAHandle store, int del))
{
	const char* path = gStoreMap.FindL(store);
	MYASSERT(path != NULL, ERR_STORE_HANDLE_INVALID);
	MyRFs myrfs;
	myrfs.Connect();
	if(del) {
		TCleaner<HBufC> desc(CreateHBufC16FromCStringLC(path));
		LHEL(FSS.Delete(*desc));
	}
	gStoreMap.EraseL(store);
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
	DUMPINT(info.iStatus); //CTelephony::TBatteryStatus
	return info.iChargeLevel;
	/*int value, res;
	res = HAL::Get(HALData::EPowerBatteryStatus, value);
	DUMPINT(value);
	if(res != KErrNone) {
		LOG("GetBatteryCharge error %i\n", res);
		return res;
	}	//TPowerBatteryStatus
	return value;*/

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

SYSCALL(int, maIOCtl(int function, int a, int b, int /*c*/)) {
	//move to individual functions?
	if(!gBtAvailable) switch(function) {
	case maIOCtl_maBtStartDeviceDiscovery:
	case maIOCtl_maBtGetNewDevice:
	case maIOCtl_maBtStartServiceDiscovery:
	case maIOCtl_maBtGetNewService:
	case maIOCtl_maBtGetNextServiceSize:
		return IOCTL_UNAVAILABLE;
	}
	switch(function) {

	case maIOCtl_maCheckInterfaceVersion:
		return Base::maCheckInterfaceVersion(a);

#ifdef TELEPHONY
	case maIOCtl_maGetBatteryCharge:
		return maGetBatteryCharge();
#endif

#if 0
	case maIOCtl_maAccept:
		maAccept(a);
		return 0;
#endif

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

	case maIOCtl_sinh: {
		double& d = *GVMRA(double);
		d = ::sinh(d);
		return 0;
	}
	case maIOCtl_cosh: {
		double& d = *GVMRA(double);
		d = ::cosh(d);
		return 0;
	}
	case maIOCtl_atanh: {
		double& d = *GVMRA(double);
		d = (1+d) / (1-d);
		d = 0.5 * log(d);
		return 0;
	}
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

#ifdef MA_PROF_SUPPORT_VIDEO_STREAMING
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
#endif	//MA_PROF_SUPPORT_VIDEO_STREAMING

#ifdef SUPPORT_MOSYNC_SERVER
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

	default:
		return IOCTL_UNAVAILABLE;
	}
}

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


#ifdef SUPPORT_MOSYNC_SERVER
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
	gLocationSync->Cancel();
	LOG("maLocationStop done.\n");
	return 0;
}


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


void Syscall::LocationHandlerL(TInt status) {
	LOG("LocationHandlerL(%i)\n", status);
	//not so good; will fail silently.
	if(IS_SYMBIAN_ERROR(status))
		return;

	//send event
	MALocation* loc = new MALocation;
	const TPosition& p(gServer.Position());
	LOG("lat: %g\n", p.Latitude());
	loc->state = p.Datum() == KPositionDatumWgs84 ?
		MA_LOC_QUALIFIED : MA_LOC_INVALID;
	loc->lat = swapd(p.Latitude());
	loc->lon = swapd(p.Longitude());
	loc->horzAcc = swapd(p.HorizontalAccuracy());
	loc->vertAcc = swapd(p.VerticalAccuracy());

	gServer.LocationGet(*gLocationSync->Status());
	gLocationSync->SetActive();

	MAEvent e;
	e.type = EVENT_TYPE_LOCATION;
	//will be handled by GetEvent.
	//definite memory leak if event is never Gotten.
	//must go through all remaining events on shutdown and delete relevant memory.
	e.data = loc;
	gAppView.AddEvent(e);
}
#endif	//SUPPORT_MOSYNC_SERVER


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

#ifdef AUDIO_DEBUGGING_MODE
	LOGA("%i impls\n", ciia.Count());
	for(int i=0; i<ciia.Count(); i++) {
		CMMFControllerImplementationInformation* cii = ciia[i];
		LOGA("%i: 0x%08X\n", i, cii->Uid());
	}
#endif	//AUDIO_DEBUGGING_MODE

	//MYASSERT(ciia.Count() != 0, SYMERR_NO_MATCHING_DECODER);
	if(ciia.Count() == 0) {
		return -1;
	}
	DEBUG_ASSERT(ciia[0] != NULL);

	//just pick the first one
#ifdef MMF
	TMMFPrioritySettings dummySettings;	//ignored since we don't have MultimediaDD capbility.
	LHEL(gController.Open(*ciia[0], dummySettings));
	RMMFAudioControllerCustomCommands accc(gController);
#else	//Mda
	gPlayer = CMdaAudioRecorderUtility::NewL(*this);
	const TUid controllerUid = ciia[0]->Uid();
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

SYSCALL(void, maSoundStop()) {
	LOGA("SS\n");
	if(gPlayer)
		gPlayer->Close();
#ifdef MMF
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
	MAEVENT e;
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
		MAEVENT e;
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


#ifdef MA_PROF_SUPPORT_VIDEO_STREAMING
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
	return (int)(d.Int64() / 1000);
}

int Syscall::maStreamPos(MAHandle stream) {
	MYASSERT(gStreamState != SS_IDLE && stream == gStreamHandle, ERR_STREAM_HANDLE);
	TTimeIntervalMicroSeconds p;
	HANDLE_LEAVE(p = gVideoPlayer->, PositionL, ());
	return (int)(p.Int64() / 1000);
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
	return (int)(p.Int64() / 1000);
}


void Syscall::AddStreamEvent(int event, int result) {
	MAEVENT e;
	e.type = EVENT_TYPE_STREAM;
	STREAM_EVENT_DATA* sed = new (ELeave) STREAM_EVENT_DATA;
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
#endif	//MA_PROF_SUPPORT_VIDEO_STREAMING
