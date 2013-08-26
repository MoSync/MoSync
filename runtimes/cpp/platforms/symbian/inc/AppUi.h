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

#ifndef __CAPPUI_H__
#define __CAPPUI_H__

#include <aknappui.h>
#ifdef __SERIES60_3X__
#include <remconcoreapi.h>
#include <remconcoreapitarget.h>
#include <remconcoreapitargetobserver.h>
#include <remconinterfaceselector.h>
#endif

#include "MoSync.pan"
#include "config_platform.h"

class CDocument;
class CAppView;

namespace Core {
	class VMCore;
}
namespace Base {
	class Syscall;
}

class CAppUi : public CAknAppUi
#ifdef __SERIES60_3X__
,public MRemConCoreApiTargetObserver
#endif
{
public:
	~CAppUi();
	void ConstructL();
	CAppUi(CDocument& aDocument);
	void Stop();
	void Exit();

	void HandleCommandL(TInt aCommand);
	TBool isExiting() const { return iExiting; }
	const Core::VMCore* GetCore() const;
	Base::Syscall* GetSyscall();
	CAppView* GetAppView() { return iAppView; }

#if 0
	//CEikAppUi
	TBool ProcessCommandParametersL(TApaCommand aCommand,
		TFileName &aDocumentName, const TDesC8 &aTail);
#endif
	CDocument& iDocument;
	TBool iReload;
private:
	CAppView* iAppView;
	TBool iExiting;
	HBufC8* iBuf;

#ifdef __SERIES60_3X__
	RWsSession& iWsSession;
	CApaWindowGroupName* iGn;
	bool iHomeScreenShown;

	CRemConInterfaceSelector* iInterfaceSelector;
	CRemConCoreApiTarget* iCoreTarget;

	void MrccatoCommand(TRemConCoreApiOperationId aOperationId,
		TRemConCoreApiButtonAction aButtonAct);
#endif

	void HandleWsEventL(const TWsEvent& aEvent, CCoeControl* aDestination);
};

#endif // __CAPPUI_H__
