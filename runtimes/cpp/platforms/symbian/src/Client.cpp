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

#include "Platform.h"

#ifdef SUPPORT_MOSYNC_SERVER

#include "../server/ClientServer.h"
#include <apgcli.h> // link against apgrfx.lib
#include <apacmdln.h> // link against apparc.lib

_LIT(KMoSyncServerExe, "MoSyncServer.exe");

//does nothing if server is already started
//todo: don't die if server can't be started
static void StartServer() {
	TFindServer findServer(KMoSyncServer);
	TFullName name;

  // Need to check that the server exists.
	if(findServer.Next(name)==KErrNone) {
		LOG("Server already started.\n");
		return;
	}
	LOG("Starting server...\n");

/*	TThreadId app_threadid;
	CApaCommandLine* cmdLine; 
	cmdLine=CApaCommandLine::NewLC();
	cmdLine->SetExecutableNameL(KMoSyncServerExe);
	cmdLine->SetCommandL(EApaCommandRun);
	RApaLsSession ls;
	LHEL(ls.Connect());
	TInt err = ls.StartApp(*cmdLine, app_threadid);
	ls.Close();
	CleanupStack::PopAndDestroy(cmdLine);
	LHEL(err);*/
	
	RProcess proc;
	TRequestStatus rendezvousStatus;
	
	//if this fails, display a friendly "reinstall" message.
	TInt res = proc.Create(KMoSyncServerExe, KNullDesC);
	if(IS_SYMBIAN_ERROR(res)) {
		LOG("MoSync server proc.Create error: %i\n", res);
		_LIT(KErrorMessage,
			"MoSyncServer is missing. Please reinstall the application.");
#ifdef PUBLIC_DEBUG
		writePanicReport(REPORT_PLATFORM_CODE, res);
#endif
		ShowAknErrorNoteThenExitL(KErrorMessage);
	}
	
	proc.Rendezvous(rendezvousStatus);
	proc.Resume();
	User::WaitForRequest(rendezvousStatus);
	proc.Close(); // Closes the handle, not the process.
	LHEL(rendezvousStatus.Int());
}

int RMoSyncServerSession::Connect() {
	StartServer();
	return CreateSession(KMoSyncServer, Version(), 4);
}

TVersion RMoSyncServerSession::Version() const {
	return TVersion(KMoSyncMajorVersionNumber, KMoSyncMinorVersionNumber,
		KMoSyncBuildVersionNumber);
}

int RMoSyncServerSession::GetNetworkInfo(TDes8& aPckg) {
	TIpcArgs args(&aPckg);
	return SendReceive(EMoSyncGetNetworkInfo, args);
}

void RMoSyncServerSession::LocationGet(TRequestStatus& aStatus) {
	TIpcArgs args(&mPositionPckg);
	SendReceive(EMoSyncLocationGet, args, aStatus);
}
int RMoSyncServerSession::LocationStop() {
	return Send(EMoSyncLocationStop);
}

#endif	//SUPPORT_MOSYNC_SERVER
