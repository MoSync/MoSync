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

#ifndef __CAPPVIEW_H__
#define __CAPPVIEW_H__

#include <coecntrl.h>
#include <e32base.h>

#include "config_platform.h"

#include <base/FileStream.h>

#include "AppUi.h"

struct MAEVENT;

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

	//CCoeControl
public:
	TKeyResponse OfferKeyEventL(const TKeyEvent& aKeyEvent, TEventCode aType);
	void HandlePointerEventL(const TPointerEvent&);
protected:
	void FocusChanged(TDrawNow aDrawNow);

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
	
	CAppUi& iAppUi;
};

#endif // __CAPPVIEW_H__
