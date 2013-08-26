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

#include "Platform.h"

#ifdef SUPPORT_MOSYNC_SERVER

#include "../server/ClientServer.h"
#include <apgcli.h> // link against apgrfx.lib
#include <apacmdln.h> // link against apparc.lib

_LIT(KMoSyncServerExe, "20037EBF.exe");

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
			"A component is missing. Please reinstall the application.");
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
	int res = CreateSession(KMoSyncServer, Version(), 4);
	if(res == KErrNone) {
		mConnected = true;
	}
	return res;
}

TVersion RMoSyncServerSession::Version() const {
	LOG("Client version: %i.%i.%i\n", KMoSyncMajorVersionNumber,
		KMoSyncMinorVersionNumber, KMoSyncBuildVersionNumber);
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
	if(!mConnected)
		return 0;
	return Send(EMoSyncLocationStop);
}

int RMoSyncServerSession::AutostartOn() {
	return SendReceive(EMoSyncAutostartOn);
}

int RMoSyncServerSession::AutostartOff() {
	return SendReceive(EMoSyncAutostartOff);
}

int RMoSyncServerSession::GetNetworkStatus(
	CTelephony::TNetworkRegistrationV1& nr)
{
	CTelephony::TNetworkRegistrationV1Pckg pckg(nr);
	TIpcArgs args(&pckg);
	return SendReceive(EMoSyncGetNetworkStatus, args);
}

void RMoSyncServerSession::GetNetworkStatusChange(
	CTelephony::TNetworkRegistrationV1Pckg& nr, TRequestStatus& aStatus)
{
	TIpcArgs args(&nr);
	SendReceive(EMoSyncGetNetworkStatusChange, args, aStatus);
}

void RMoSyncServerSession::CancelNetworkStatusChange() {
	Send(EMoSyncCancelNetworkStatusChange);
}

#endif	//SUPPORT_MOSYNC_SERVER
