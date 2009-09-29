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

#include <eikon.hrh>
#include <apgcli.h>
#include <aknutils.h>

#include "Application.h"
#include "AppUi.h"
#include "MoSync.pan"
#include "MoSync.hrh"
#include "AppView.h"
#include "Document.h"
#include "config_platform.h"
#include <helpers/helpers.h>
#include "symbian_helpers.h"
#include "symbian_errors.h"

CAppUi::~CAppUi()
{
	LOGD("~CAppUi\n");
	if(iAppView) {
		RemoveFromStack(iAppView);
		delete iAppView;
	}
	SAFE_DELETE(iBuf);
	LOGD("~CAppUi ends\n");
}

void CAppUi::ConstructL( void ) 
{    
	BaseConstructL();               // initialize appUI with standard values, read standard resource files
	SetKeyBlockMode(ENoKeyBlock); //important
	LOGD("view\n");
	iAppView = CAppView::NewL(*this); // construct appView
	LOGD("Created view\n");
	iAppView->SetMopParent( this ); // allow object to be in network of object providers (see MObjectProvider)
	AddToStackL( iAppView );        // view now gets key press events
	//DebugMarkStart();
	LOG("Initialization complete.\n");
}

CAppUi::CAppUi( CDocument& aDocument ) 
: iDocument(aDocument), iReload(false), iAppView(NULL), iExiting(false), iBuf(NULL)
{    
}

void CAppUi::HandleCommandL( TInt aCommand ) 
{    
	switch(aCommand) {
		case EEikCmdExit:
			LOG("EEikCmdExit\n");
			iAppView->DoCloseEventL();
			break;
//#if 0
		case EAknSoftkeyExit:
			LOG("EAknSoftkeyExit\n");
			//iAppView->DoCloseEventL();
			break;
		case EAknSoftkeyBack:
			LOG("EAknSoftkeyBack\n");
			//iAppView->DoCloseEventL();
			break;
//#endif
		default:
			LOG("Unknown command %i\n", aCommand);
			Panic( EMoSyncUi );
			break;
	}
}

#if 0
TBool CAppUi::ProcessCommandParametersL(TApaCommand aCommand,
	TFileName &aDocumentName, const TDesC8& LOG_ARG(aTail))
{
	//iAppView->ShowSplashScreen();
#ifdef PHONE_RELEASE
	if(aDocumentName.Length() != 8)	{
#endif	//PHONE_RELEASE
#ifdef LOGGING_ENABLED
		iBuf = CreateHBufC8FromDesC16L(aDocumentName);
		LOG("PCPa %i \"%S\" \"%S\"\n", aCommand, iBuf, &aTail);
		SAFE_DELETE(iBuf);
#endif	//LOGGING_ENABLED
#ifdef PHONE_RELEASE
		BIG_PHAT_ERROR(SYMERR_BAD_COMMAND_LINE);
	}
#else
	TBool res = CEikAppUi::ProcessCommandParametersL(aCommand, aDocumentName);
	return res;
#endif	//PHONE_RELEASE

	TLex lex(aDocumentName);
	TUint32 callerUid;
	LHEL(lex.Val(callerUid, EHex));

#ifdef __SERIES60_3X__
	// \\resource\\apps\\<UID>_.comb
	_LIT(KBasePath, "\\resource\\apps\\");
	_LIT(KFilename, ".comb");
#else	//Series 60, 2nd edition
	// \\system\\apps\\<UID>\\program.comb
	_LIT(KBasePath, "\\system\\apps\\");
	_LIT(KFilename, "\\program.comb");
#endif	//__SERIES60_3X__

	RApaLsSession als;
	LHEL(als.Connect());
	TApaAppInfo ai;
	LHEL(als.GetAppInfo(ai, TUid::Uid(callerUid)));
	als.Close();

	TBuf16<KMaxFileName> fileName16;
	fileName16.Append(ai.iFullName.Left(2));	//drive letter and colon
	fileName16.Append(KBasePath);
	fileName16.Append(aDocumentName);
	fileName16.Append(KFilename);
	LHEL(CompleteWithAppPath(fileName16));
	
	iBuf = HBufC8::NewL(fileName16.Length() + 1);	//the +1 is so we can call PtrZ later.
	TPtr8 bufPtr = iBuf->Des();
	bufPtr.Copy(fileName16);

	LOGD("PCPb %i 0x%08X \"%S\" \"%S\"\n", aCommand, callerUid, &bufPtr, &aTail);

#ifdef PHONE_RELEASE
#if 0
//set caption
	RApaLsSession als;
	LHEL(als.Connect());
	//GetAppInfo(calling app)
	TApaAppInfo ai;
	LHEL(als.GetAppInfo(ai, TUid::Uid(callerUid)));
	//setCap(calling app's name)
	//LHEL(als.SetAppShortCaption(ai.iShortCaption, ELangNone, KUidMoSyncApp));
	als.Close();
#endif

	iAppView->SetProgramL(CCP bufPtr.PtrZ());
#endif	//PHONE_RELEASE

	SAFE_DELETE(iBuf);

	return false;
}
#endif	//0

void CAppUi::Stop() {
	LOGD("CAppUi::Stop()\n");
	if(iAppView)
		iAppView->Stop();
}

const VMCore* CAppUi::GetCore() const {
	if(!iAppView)
		return NULL;
	return iAppView->GetCore();
}

void CAppUi::Exit() {
	//DebugMarkEnd();
	LOG("CAppUi::Exit()\n");
	if(iExiting) {
		//Panic(EMoSyncUnloggedPanic);
		LOG("Already Exiting!!!\n");
		return;
	}
	iExiting = true;
	Stop();
	
	//HACK-test, recompiler USER-EXEC 3
	//LOG("calling User::Leave()\n");
	//User::Leave(KLeaveMoSyncExit);
	
	LOG("calling CAknAppUi::Exit()\n");
	CAknAppUi::Exit();
	LOG("CAppUi::Exit() ends\n");
}
