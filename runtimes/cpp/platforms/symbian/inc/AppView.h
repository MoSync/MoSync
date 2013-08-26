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

#ifndef __CAPPVIEW_H__
#define __CAPPVIEW_H__

#include <coecntrl.h>
#include <e32base.h>

#include "config_platform.h"

#include <base/FileStream.h>

class CAppUi;
class CDocument;
class CDirScrAccEng;
class CIdle;

namespace Core {
	class VMCore;
}
namespace Base {
	class Syscall;
}

using namespace Base;
using namespace Core;

class CAppView : public CCoeControl {
public:
	~CAppView();
	static CAppView* NewL(CAppUi& aAppUi);
	static CAppView* NewLC(CAppUi& aAppUi);

	void Stop();

	int GetKeys();
	int GetEvent(MAEvent* ep);
	bool HasEvent();
	void Wait(int timeout);
	void SetReload(MAHandle data, bool reload);
	bool AddEvent(const MAEvent& event);
	void DoCloseEventL();
	const Core::VMCore* GetCore() const { return iCore; }
	Base::Syscall* GetSyscall() { return iSyscall; }
	int TextBox(const TDesC& title, const TDesC& inText, TDes& outText, int constraints);
	void SetIdleDelay(int ms);

#ifdef __SERIES60_3X__
	int WindowGroupId() const { return Window().WindowGroupId(); }
#endif
	void HomeScreenEventsOn();
	void HomeScreenEventsOff();

	//CCoeControl
public:
	TKeyResponse OfferKeyEventL(const TKeyEvent& aKeyEvent, TEventCode aType);
	void HandlePointerEventL(const TPointerEvent&);
protected:
	void FocusChanged(TDrawNow aDrawNow);
	void HandleResourceChange(TInt aType);

private:
	CAppView(CAppUi& aAppUi);
	void ConstructL();

	void Draw(const TRect& aRect) const;
	static TInt RunIdle(TAny*);
	TInt RunL();
	static TInt StartStuff(TAny*);

	void CreateCoreL();
	void LoadProgramL();
#ifndef MOSYNC_COMMERCIAL
public:
	void ShowSplashScreen();
#endif

private:
	CDirScrAccEng* iEngine;
	CCirBuf<MAEvent> iEventBuffer;
	Syscall* iSyscall;
	VMCore* iCore;
	CIdle* iIdle;
	bool iKeepRunning, iStopForever;
	int iReloadHandle, iKeys;

#if defined(__MARM_ARMI__) || defined(__MARM_THUMB__)	//buggy compiler
public:
#endif
	bool iWait;
	int iWaitTimerSequence;
	void Resume();

private:
	class CMyBaseTimer : public CTimer {
	public:
		CMyBaseTimer();
		void ConstructL() { CTimer::ConstructL(); }
		TInt RunError(TInt aError);
	};

	class CWaitTimer : public CMyBaseTimer {
	public:
		CWaitTimer(CAppView& appView);
		void RunL();
		int mSequence;
	private:
		CAppView& mAppView;
	};	//DO NOT INSTANTIATE C-TYPE CLASSES AS MEMBERS! THEY WILL FUCK YOU UP! :)
	CWaitTimer* iWaitTimer;

	class CCloseTimer : public CMyBaseTimer {
		void RunL();
	};
	CCloseTimer* iCloseTimer;

	class CDelayTimer : public CMyBaseTimer {
	public:
		CDelayTimer(CAppView& appView);
		void RunL();
	private:
		CAppView& mAppView;
	};
	CDelayTimer* iDelayTimer;
	int iIdleLoopDelay;	// milliseconds
	friend class CDelayTimer;

	CAppUi& iAppUi;
};

#endif // __CAPPVIEW_H__
