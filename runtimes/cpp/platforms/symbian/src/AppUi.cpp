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
