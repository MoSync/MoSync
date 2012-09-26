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

#include <coemain.h>
#include <BACLINE.H>
#include <eikgted.h>
#include <gulftflg.hrh>
#include <AknQueryDialog.h>
#ifdef __SERIES60_3X__
#include <MoSync_3rd.RSG>
#else
#include <MoSync.RSG>
#endif

#include "config_platform.h"

#include "AppView.h"
#include "AppUi.h"
#include "Document.h"
#include "DirScrAccEng.h"
#include "syscall.h"
#include "core.h"
#include "helpers.h"
#include "symbian_errors.h"
#include "symbian_helpers.h"
#include "mosync.pan"

#include <base/Stream.h>
#include <base/FileStream.h>

#ifdef GUIDO
#include <helpers/CPP_IX_GUIDO.H>
#endif

//#undef LOG
//#define LOG(arg...) Log("%d ", User::TickCount()); Log(arg)

using namespace MoSyncError;

//***************************************************************************
//Initialization
//***************************************************************************

CAppView* CAppView::NewL(CAppUi& aAppUi)
{
	CAppView* self = NewLC(aAppUi);
	CleanupStack::Pop(self);
	return self;
}
CAppView* CAppView::NewLC(CAppUi& aAppUi)
{
	CAppView* self = new (ELeave) CAppView(aAppUi);
	CleanupStack::PushL(self);
	self->ConstructL();
	return self;
}

CAppView::CAppView(CAppUi& aAppUi) : iEngine(0), iSyscall(0), iIdle(0), iStopForever(false),
iReloadHandle(0), iKeys(0), iWait(0), iWaitTimerSequence(0),
iWaitTimer(0), iCloseTimer(0), iDelayTimer(0), iIdleLoopDelay(0), iAppUi(aAppUi)
{
	LOGD("AV\n");
}

void CAppView::ConstructL()
{
	DebugMarkStart();
	LOGD("HELLOTE\n");

#if 0
	CCommandLineArguments* cla = CCommandLineArguments::NewLC();
	LOGD("CLA count: %i\n", cla->Count());
	for(int i=0; i<cla->Count(); i++) {
		HBufC8* buf = CreateHBufC8FromDesC16LC(cla->Arg(i));
		LOGD("%i \"%S\"\n", i, buf);
		CleanupStack::PopAndDestroy(buf);
	}
	CleanupStack::PopAndDestroy(cla);
#endif	//0

	CreateWindowL();  // Create a window for this application view
	// Set the window size
	SetRect(CCoeEnv::Static()->ScreenDevice()->SizeInPixels());
	iEngine = CDirScrAccEng::NewL(iEikonEnv->WsSession(),
		*(CCoeEnv::Static()->ScreenDevice()), Window());
	LOGD("AVC1\n");

	EnableDragEvents();

	iEventBuffer.SetLengthL(EVENT_BUFFER_SIZE);

	ActivateL();  // Activate the window, which makes it ready to be drawn
	LOGD("AVC3\n");

	iWaitTimer = new (ELeave) CWaitTimer(*this);
	iWaitTimer->ConstructL();

	iDelayTimer = new (ELeave) CDelayTimer(*this);
	iDelayTimer->ConstructL();

	iIdle = CIdle::NewL(MyPrioIdle);
	iIdle->Start(TCallBack(StartStuff, this));
}

TInt CAppView::StartStuff(TAny* arg) {
	CAppView* self = (CAppView*)arg;
#ifdef MOSYNC_COMMERCIAL

	//TODO: handle Exit calls from within CreateCoreL.
#ifdef SUPPORT_RELOAD
	LTRAP(self->CreateCoreL());
#else
	self->CreateCoreL();
#endif
	self->Resume();	//Start VM loop

#else	//non-commercial
	self->ShowSplashScreen();

	//start timer
	self->iWaitTimerSequence = 0;
#define PERIOD_MS (1000)
	self->Wait(PERIOD_MS);

#ifdef SUPPORT_RELOAD
	LTRAP(self->CreateCoreL());
#else
	self->CreateCoreL();
#endif

	//clear the back-buffer of the nc text
#define BLACK 0
	self->iEngine->SetColor(BLACK);
	self->iEngine->ClearScreen();

#endif	//MOSYNC_COMMERCIAL
	return 0;	//don't keep running
}

#ifndef MOSYNC_COMMERCIAL

//****************************************
//    Decode Message
//****************************************

//str must point to a buffer at least 64 bytes long.
void decode_msg(char* str)
{
	int n;
	static const int msg[] = {
		0x8814,0xccd5,0x155d8,0x231e,0xac31,0x13544,0x471c,0xadae,
		0xd04e,0xaeac,0x204c5,0x1383e,0xd24a,0x8e87,0x8f06,0x17e7c,
		0x13aa9,0x13b38,0x2087d,0xf7f4,0x6fdf,0
	};

	for (n=0;n<63;n++)
	{
		// Decode

		if (!msg[n])
			break;

		str[n] = ((msg[n] - (127*n)) * 8191) % 131071;
	}

	str[n] = 0;

	LOGD("Message: %s\n", str);
}


void CAppView::ShowSplashScreen() {
	iEngine->ClearScreen();
#define WHITE 0xffffff
	iEngine->SetColor(WHITE);
	//maDrawText(0, 0, "MoSync non-commercial");
	//_LIT(KNC, "MoSync non-commercial");
	char str[64];
	decode_msg(str);
	TCleaner<HBufC16> buf(CreateHBufC16FromCStringLC(str));
	iEngine->DrawTextL(*buf, TPoint(0, 0));
	iEngine->UpdateScreen();
	LOG("Splash screen shown.\n");
}
#endif	//MOSYNC_COMMERCIAL

void CAppView::CreateCoreL() {
	LOG("LoadTheProgram\n");
	//Initialize Core & Syscall
	iSyscall = new (ELeave) Base::Syscall(*iEngine, *this);
	iCore = CreateCore(*iSyscall);
	iSyscall->ConstructL(iCore);

	LoadProgramL();
}

void CAppView::LoadProgramL() {
#ifdef PHONE_RELEASE

#if defined(__SERIES60_3X__)
	_LIT8(KPathFmt, "%S\\resource\\apps\\%08X.comb");
#else
	_LIT8(KPathFmt, "%S\\system\\apps\\%08X.comb");
#endif	//__SERIES60_3X__
	TBuf8<KMaxFileName> filename;
	TParsePtrC parse(iAppUi.iDocument.iApp.AppFullName());
	TBuf8<8> drive;
	drive.Copy(parse.Drive());
	filename.Format(KPathFmt, &drive, iAppUi.iDocument.iApp.AppDllUid().iUid);
	FileStream file(CCP filename.PtrZ());
	MYASSERT(LoadVMApp(iCore, file, CCP filename.PtrZ()), ERR_PROGRAM_LOAD_FAILED);

#else	//PHONE_RELEASE

	//Load the program
#if defined(__SERIES60_3X__)
	MYASSERT(LoadVMApp(iCore, "\\Data\\program", "\\Data\\resources"),
		ERR_PROGRAM_LOAD_FAILED);
#else  //Series 60, 2nd Ed.
	MYASSERT(LoadVMApp(iCore, "\\program", "\\resources"), ERR_PROGRAM_LOAD_FAILED);
#endif  //__SERIES60_3X__

#endif	//PHONE_RELEASE
	LOG("Program loaded.\n");
}

CAppView::~CAppView()
{
	LOGD("~CAppView\n");
	if(iCloseTimer) {
		iCloseTimer->Cancel();
		delete iCloseTimer;
	}
	if(iWaitTimer) {
		iWaitTimer->Cancel();
		delete iWaitTimer;
	}
	if(iDelayTimer) {
		iDelayTimer->Cancel();
		delete iDelayTimer;
	}
	if(iIdle) {
		//iKeepRunning = false; //maybe unneeded
		delete iIdle;
	}
	if(iCore) {
		DeleteCore(iCore);
	}
	SAFE_DELETE(iSyscall);
	if(iEngine) {
		Stop();
		delete iEngine;
	}

	//clear the event queue, handle custom event leaks
	MAEvent event;
	while(GetEvent(&event)) {}

	LOGD("~CAppView ends\n");
	DebugMarkEnd();
}

//***************************************************************************
//Other stuff
//***************************************************************************

void CAppView::FocusChanged(TDrawNow aDrawNow) {
	if(iStopForever)
		return;
	if(IsFocused()) {
		LOG("FocusGained, DrawNow %i\n", aDrawNow);
		if(!iEngine->IsDrawing()) {
			LTRAP(iEngine->StartDrawingL());
		}
		if(aDrawNow == EDrawNow)
			iEngine->UpdateScreen();

		//send FOCUS_GAINED
		MAEvent event;
		event.type = EVENT_TYPE_FOCUS_GAINED;
		AddEvent(event);
	} else {
		LOG("FocusLost, DrawNow %i\n", aDrawNow);
		if(iEngine->IsDrawing())
			iEngine->StopDrawing();

		//send FOCUS_LOST
		MAEvent event;
		event.type = EVENT_TYPE_FOCUS_LOST;
		AddEvent(event);
	}
}

void CAppView::HandleResourceChange(TInt aType) {
	LOG("HandleResourceChange(0x%x)\n", aType);
	CCoeControl::HandleResourceChange(aType);
	if(aType != KEikDynamicLayoutVariantSwitch)
		return;

	// resize the window
	SetRect(CCoeEnv::Static()->ScreenDevice()->SizeInPixels());
	iEngine->UpdateScreenSize();

	MAEvent event;
	event.type = EVENT_TYPE_SCREEN_CHANGED;
	AddEvent(event);
	LOG("EVENT_TYPE_SCREEN_CHANGED sent\n");
}

void CAppView::Draw(const TRect& /* aRect */) const {
	LOGG("AVD\n");
	if(iStopForever)
		return;
	LOGG("AVD1\n");
	iEngine->UpdateScreen();
	LOGG("AVD2\n");
}

void CAppView::Stop() {
	LOGD("CAppView::Stop()\n");
	if(iIdle) if(iIdle->IsActive())
		iIdle->Cancel();
	if(iEngine->IsDrawing())
		iEngine->StopDrawing();
	iStopForever = true;

	//if(iSyscall && !iStopForever)	//useless on account of previous assignment
		//iSyscall->StopEverything();
	LOGD("CAppView::Stop() ends\n");
}

//***************************************************************************
//Run control
//***************************************************************************

TInt CAppView::RunIdle(TAny* a) {
	CAppView* self = (CAppView*)a;
	int res = 0;
	res = self->RunL();
	return res;
}
TInt CAppView::RunL() {
	if(iStopForever) {
		LOGD("RunIdle stopped forever.\n");
		return iKeepRunning = 0;
	}
	//LOGD("RIS\n");
#ifdef SUPPORT_RELOAD
	iCore->symbianError = KErrNone;
#endif
	Run2(iCore);
	//LOGD("RIE\n");
#ifdef SUPPORT_RELOAD
	if(iCore->symbianError != KErrNone) {
		LOG("Found core leave, code %i\n", iCore->symbianError);
		if(iCore->symbianError == KLeaveMoSyncExit) {
			iAppUi.Exit();
		} else if(iCore->symbianError == KLeaveMoSyncReload) {
			iStopForever = false;
			iAppUi.iReload = false;
			iSyscall->StopEverything();
			iSyscall->maSetColor(0);
			iEngine->ClearScreen();
			if(!iEngine->IsDrawing()) {
				LTRAP(iEngine->StartDrawingL());
			}
			LoadProgramL();
		} else {
			iKeepRunning = false;
			LHEL(iCore->symbianError);
		}
	}
#endif
	if(iReloadHandle > 0) {
		Base::Stream* stream =
			iSyscall->resources.extract_RT_BINARY(iReloadHandle);
		bool res = Core::LoadVMApp(iCore, *stream);
		delete stream;
		iReloadHandle = 0;
		MYASSERT(res, ERR_PROGRAM_LOAD_FAILED);
	}
	if(iIdleLoopDelay && iKeepRunning) {
		// timer
		iDelayTimer->After(iIdleLoopDelay * 1000);
		return false;
	}
	return iKeepRunning;
}

void CAppView::Resume() {
	LOGD("Resume\n");
	if(iWait)
		return;
	iWaitTimerSequence++;
	if(iWaitTimer)
		iWaitTimer->Cancel();
	if(!iKeepRunning) {
		iKeepRunning = true;
		if(iIdle) if(!iIdle->IsActive()) {
			LOGD("iIdle->Start\n");
			iIdle->Start(TCallBack(RunIdle, this));
		}
	}
}

void CAppView::Wait(int timeout) {
	LOGD("W %i\n", timeout);
	if(timeout > 0) {
		iWaitTimer->mSequence = iWaitTimerSequence;
		// careful: if (timeout * 1000) > INT_MAX, then Symbian Panic USER 87.
		static const int MAX_MS = (0x7FFFFFFF / 1000);	// about 35 minutes.
		LOGD("MAX_MS: %i\n", MAX_MS);
		if(timeout > MAX_MS) {
			// maWait() will return sooner than expected, with no events, but that's ok;
			// any well-behaved MoSync program will simply wait again.
			timeout = MAX_MS;
			LOGD("timeout reset.\n", MAX_MS);
		}
		iWaitTimer->After(timeout * 1000);
	}
	iWait = true;
	iKeepRunning = false;
}

void CAppView::SetReload(MAHandle data, bool reload) {
	iReloadHandle = data;
	iAppUi.iReload |= reload;
}

void CAppView::DoCloseEventL() {
#ifndef MOSYNC_COMMERCIAL
	//splash wait, unless we're really unlucky and hit the 1-in-4billion 32-bit wraparound.
	//TODO: make perfectly sure by using a bool.
	if(iWait && iWaitTimerSequence == 0) {
		MoSyncExit(0);
	}
#endif

	MAEvent event;
	event.type = EVENT_TYPE_CLOSE;
	AddEvent(event);

	if(!iCloseTimer) {
		iCloseTimer = new (ELeave) CCloseTimer();
		iCloseTimer->ConstructL();
		iCloseTimer->After(EVENT_CLOSE_TIMEOUT * 1000);
	}
}

//***************************************************************************
//CMyBaseTimer
//***************************************************************************

CAppView::CMyBaseTimer::CMyBaseTimer() : CTimer(MyPrioStandard)
{
	CActiveScheduler::Add(this);
}

TInt CAppView::CMyBaseTimer::RunError(TInt aError) {
	LOGD("CMyBaseTimer::RunError %i\n", aError);
	if(aError == KErrCancel)
		return KErrNone;
	return aError;
}

//***************************************************************************
//CWaitTimer
//***************************************************************************

CAppView::CWaitTimer::CWaitTimer(CAppView& appView) :
	mSequence(0), mAppView(appView)
{}

void CAppView::CWaitTimer::RunL() {
	LOGD("WTRL\n");
	if(mAppView.iWaitTimerSequence == mSequence) {
		mAppView.iWait = 0;
		mAppView.Resume();
	}
}

//***************************************************************************
//CCloseTimer
//***************************************************************************

void CAppView::CCloseTimer::RunL() {
	_LIT(KCloseMessage,
		"The application failed to respond to the Close Event and will be terminated.");
#ifdef PUBLIC_DEBUG
	writePanicReport(REPORT_TIMEOUT, 0);
#endif
	ShowAknErrorNoteThenExitL(KCloseMessage);
}

//***************************************************************************
//CDelayTimer
//***************************************************************************

void CAppView::SetIdleDelay(int ms) {
	LOGD("setIdleDelay(%i)\n", ms);
	iIdleLoopDelay = ms;
}

CAppView::CDelayTimer::CDelayTimer(CAppView& appView) :
	mAppView(appView)
{}

void CAppView::CDelayTimer::RunL() {
	LOGD("CDelayTimer::RunL()\n");
	if(mAppView.iKeepRunning && mAppView.iIdle) if(!mAppView.iIdle->IsActive()) {
		LOGD("iIdle->Start\n");
		mAppView.iIdle->Start(TCallBack(RunIdle, &mAppView));
	}
}

//***************************************************************************
//Pointer events
//***************************************************************************

void CAppView::HandlePointerEventL(const TPointerEvent& pe) {
	LOGD("PE %i %ix%i\n", pe.iType, pe.iPosition.iX, pe.iPosition.iY);
	MAEvent e;
	switch(pe.iType) {
	case TPointerEvent::EButton1Down:
		e.type = EVENT_TYPE_POINTER_PRESSED;
		break;
	case TPointerEvent::EButton1Up:
		e.type = EVENT_TYPE_POINTER_RELEASED;
		break;
	case TPointerEvent::EDrag:
		e.type = EVENT_TYPE_POINTER_DRAGGED;
		break;
	default:
		return;
	}
	e.pointer.point.x = pe.iPosition.iX;
	e.pointer.point.y = pe.iPosition.iY;
	e.pointer.touchId = 0;
	AddEvent(e);
}

//***************************************************************************
//Key handling
//***************************************************************************

#define QWERTY_KEYS(m)\
	m(MAK_RETURN, EStdKeyEnter)\
	m(MAK_SPACE, EStdKeySpace)\
	m(MAK_LSHIFT, EStdKeyLeftShift)\
	m(MAK_RSHIFT, EStdKeyRightShift)\
	m(MAK_LALT, EStdKeyLeftFunc)\
	m(MAK_RALT, EStdKeyRightFunc)\
	m(MAK_LCTRL, EStdKeyLeftCtrl)\
	m(MAK_RCTRL, EStdKeyRightCtrl)\

// Keycode values >= ASCII_MIN && <= ASCII_MAX
// can be passed directly as ASCII codes.
#define ASCII_MIN 0x20
#define ASCII_MAX 0x5f

// These keycode identifiers do not respond to the proper keys
// on a Swedish E71.
#define BROKEN_KEYS(m)\
	m(MAK_COMMA, EStdKeyComma)\
	m(MAK_PERIOD, EStdKeyFullStop)\
	m(MAK_SLASH, EStdKeyForwardSlash)\
	m(MAK_BACKSLASH, EStdKeyBackSlash)\
	m(MAK_SEMICOLON, EStdKeySemiColon)\
	m(MAK_QUOTE, EStdKeySingleQuote)\

#define STDKEYS(m)\
	m(UP, EStdKeyUpArrow)\
	m(DOWN, EStdKeyDownArrow)\
	m(LEFT, EStdKeyLeftArrow)\
	m(RIGHT, EStdKeyRightArrow)\
	m(STAR, EStdKeyNkpAsterisk)\
	m(STAR, 0x2a/*hack*/)\
	m(POUND, EStdKeyHash)\
	m(1, EStdKeyNkp1)\
	m(2, EStdKeyNkp2)\
	m(3, EStdKeyNkp3)\
	m(4, EStdKeyNkp4)\
	m(5, EStdKeyNkp5)\
	m(6, EStdKeyNkp6)\
	m(7, EStdKeyNkp7)\
	m(8, EStdKeyNkp8)\
	m(9, EStdKeyNkp9)\
	m(0, EStdKeyNkp0)\
	m(1, '1')\
	m(2, '2')\
	m(3, '3')\
	m(4, '4')\
	m(5, '5')\
	m(6, '6')\
	m(7, '7')\
	m(8, '8')\
	m(9, '9')\
	m(0, '0')\
	m(CLEAR, EStdKeyBackspace)\
	m(SOFTLEFT, EStdKeyDevice0)\
	m(SOFTRIGHT, EStdKeyDevice1)\

TKeyResponse CAppView::OfferKeyEventL(const TKeyEvent& aKeyEvent, TEventCode aType) {
	bool down;
	int scancode = aKeyEvent.iScanCode;
	switch(aType) {
	case EEventKeyUp:
		LOGD("KeyUp 0x%02x %i\n", scancode, aKeyEvent.iCode);
		down = false;
		break;
	case EEventKeyDown:
		LOGD("KeyDown 0x%02x %i\n", scancode, aKeyEvent.iCode);
		down = true;
		break;
	case EEventKey:
		LOGD("Key 0x%02x %i\n", scancode, aKeyEvent.iCode);
		if(iEventBuffer.Count() < EVENT_BUFFER_SIZE) {
			MAEvent event;
			event.type = EVENT_TYPE_CHAR;
			event.character = aKeyEvent.iCode;
			AddEvent(event);
		}
		return EKeyWasConsumed;
	default:
		LOG("KeyEvent %i\n", aType);
		return EKeyWasNotConsumed;
	}

#define SCANTOMAKB_CASE(name, num)\
	case num: makey = MAK_##name; makb = MAKB_##name; break;

#define SCANTOMAK_CASE(name, num)\
	case num: makey = name; break;

	int makey;
	int makb = 0;

	// TODO: handle Unicode input, which is received in aKeyEvent.iCode
	// when aType == EEventKey.
	// Will probably need a new MoSync EVENT_TYPE_CHAR for this.

#define ASCII_MIN 0x20
#define ASCII_MAX 0x5f
	if(scancode >= ASCII_MIN && scancode <= ASCII_MAX) {
		makey = scancode;
	} else switch(scancode) {
		STDKEYS(SCANTOMAKB_CASE);
		QWERTY_KEYS(SCANTOMAK_CASE);
		BROKEN_KEYS(SCANTOMAK_CASE);

	case EStdKeyDevice3:	//Middle joystick button?
#ifndef QWERTY_KEYS
	case EStdKeyLeftCtrl: //emu only
	case EStdKeyRightCtrl:  //emu only
#endif
		makey = MAK_FIRE;
		makb = MAKB_FIRE;
		break;
#ifndef QWERTY_KEYS
	case EStdKeyRightShift:
		makey = MAK_PEN;
		break;
#endif
	default:
		LOG("Unknown scancode: %i\n", scancode);
		makey = MAK_UNKNOWN;
	}
	switch(aKeyEvent.iCode) {
#define DO_NUMBER(n) case ('0' + n): makey = MAK_##n; break;
		DO_NUMBER(0);
		DO_NUMBER(1);
		DO_NUMBER(2);
		DO_NUMBER(3);
		DO_NUMBER(4);
		DO_NUMBER(5);
		DO_NUMBER(6);
		DO_NUMBER(7);
		DO_NUMBER(8);
		DO_NUMBER(9);
	}

	if(down)
		iKeys |= makb;
	else
		iKeys &= ~makb;

	if(iEventBuffer.Count() < EVENT_BUFFER_SIZE) {
		MAEvent event;
		event.type = down ? EVENT_TYPE_KEY_PRESSED : EVENT_TYPE_KEY_RELEASED;
		event.ked.key = makey;
		event.ked.nativeKey = scancode;
		AddEvent(event);
	}

	return EKeyWasConsumed;
}

bool CAppView::AddEvent(const MAEvent& event) {
	if(iWait) {
		iWait = 0;
		Resume();
	}
	TInt res = iEventBuffer.Add(&event);
	if(res == 0) {
		LOGD("EB full!\n");
		return false;
	} else if(res != 1) {
		LOG("EB.Add error %i\n", res);
		BIG_PHAT_ERROR(SYMERR_EVENT_BUFFER_FULL);
	}
	return true;
}

int CAppView::GetEvent(MAEvent* ep) {
	int res = iEventBuffer.Remove(ep);
	if(res == 0)
		return 0;
	else {

	//TODO: clean up using static const table instead of macro.
#define HANDLE_CUSTOM_EVENT(eventType, dataType) if(ep->type == eventType) { \
	memcpy(Core::GetCustomEventPointer(iCore), (void*)ep->data, sizeof(dataType)); \
	delete (dataType*)ep->data; \
	ep->data = (int(Core::GetCustomEventPointer(iCore)) - int(iCore->mem_ds)); }

		CUSTOM_EVENTS(HANDLE_CUSTOM_EVENT);
		return 1;
	}
}

bool CAppView::HasEvent() {
	return iEventBuffer.Count() != 0;
}

int CAppView::GetKeys() {
	return iKeys;
}

class CMyAknQueryControl : CAknQueryControl {
public:
	CEikEdwin* getEdwin() { return iEdwin; }
};

class CMyAknTextQueryDialog : public CAknTextQueryDialog {
public:
	CMyAknTextQueryDialog(TDes& outText) : CAknTextQueryDialog(outText) {}
	// Inherited from CAknQueryDialog.
	virtual void UpdateLeftSoftKeyL() {
		MakeLeftSoftkeyVisible(ETrue); // Even if there is no text, shows the LSK.
	}
#if 1
	TKeyResponse OfferKeyEventL(const TKeyEvent &aKeyEvent, TEventCode aType) {
		LOGD("CMyAknTextQueryDialog::OfferKeyEventL(%i, %i)\n", aKeyEvent.iCode, aType);
		if(aKeyEvent.iCode == EKeyEnter) {
			LOGD("Enter 7\n");
			// offer this event to only the edit box.
			// this prevents it from closing the dialog.
			CEikEdwin* e = ((CMyAknQueryControl*)QueryControl())->getEdwin();
			DEBUG_ASSERT(e != NULL);
			//e->ClearSelectionL();
			return e->OfferKeyEventL(aKeyEvent, aType);
		}
		return CAknTextQueryDialog::OfferKeyEventL(aKeyEvent, aType);
	}
#endif
#if 0	// doesn't work (on X6 at least)
	TKeyResponse OfferKeyEventL(const TKeyEvent &aKeyEvent, TEventCode aType) {
		LOGD("CMyAknTextQueryDialog::OfferKeyEventL(%i, %i)\n", aKeyEvent.iCode, aType);
		if(aKeyEvent.iCode == EKeyEnter) {
			LOGD("Enter 5\n");
			CAknQueryDialog::OfferKeyEventL(aKeyEvent, aType);
			return EKeyWasNotConsumed;
		}
		return CAknTextQueryDialog::OfferKeyEventL(aKeyEvent, aType);
	}
	virtual TBool NeedToDismissQueryL(const TKeyEvent& aKeyEvent) {
		LOGD("CMyAknTextQueryDialog::NeedToDismissQueryL(%i)\n", aKeyEvent.iCode);
		if(aKeyEvent.iCode == EKeyEnter) {
			LOG("Enter 1\n");
			return false;
		}
		return CAknTextQueryDialog::NeedToDismissQueryL(aKeyEvent);
	}
#endif
};

int CAppView::TextBox(const TDesC& title, const TDesC& inText, TDes& outText, int constraints) {
	int resourceId;
	int type = constraints & 0xFFF;
	switch(type) {
	case MA_TB_TYPE_ANY:
		resourceId = R_TEXTBOX_QUERY;
		break;
	case MA_TB_TYPE_EMAILADDR:
	case MA_TB_TYPE_NUMERIC:
	case MA_TB_TYPE_PHONENUMBER:
	case MA_TB_TYPE_URL:
	case MA_TB_TYPE_DECIMAL:
	case MA_TB_TYPE_SINGLE_LINE:
		resourceId = R_TEXTBOX_QUERY_SINGLE_LINE;
		break;
	default:
		return MA_TB_RES_TYPE_UNAVAILABLE;
	}

	if(iEngine->IsDrawing())
		iEngine->StopDrawing();

	outText.Copy(inText);
	CAknTextQueryDialog* dlg = new (ELeave) CMyAknTextQueryDialog(outText);
	CleanupStack::PushL(dlg);
	dlg->SetPromptL(title);
	dlg->SetMaxLength(outText.MaxLength());

	if(type == MA_TB_TYPE_NUMERIC || type == MA_TB_TYPE_PHONENUMBER || type == MA_TB_TYPE_DECIMAL)
		dlg->SetDefaultInputMode(EAknEditorNumericInputMode);

	bool predict = (constraints & (MA_TB_FLAG_PASSWORD | MA_TB_FLAG_SENSITIVE |
		MA_TB_FLAG_NON_PREDICTIVE)) == 0;
	if(!(type == MA_TB_TYPE_ANY || type == MA_TB_TYPE_SINGLE_LINE))
		predict = false;
	dlg->SetPredictiveTextInputPermitted(predict);

	TBool answer = dlg->ExecuteLD(resourceId);
	CleanupStack::Pop(dlg);
	return answer ? 1 : 0;
}

void CAppView::HomeScreenEventsOn() {
	RWindowGroup& wg(CCoeEnv::Static()->RootWin());
	LHEL(wg.EnableFocusChangeEvents());
}

void CAppView::HomeScreenEventsOff() {
	RWindowGroup& wg(CCoeEnv::Static()->RootWin());
	wg.DisableFocusChangeEvents();
}
