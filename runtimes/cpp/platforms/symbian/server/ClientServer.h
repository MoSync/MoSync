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

#ifndef CLIENTSERVER_H
#define CLIENTSERVER_H

#include <e32base.h>

// server name
_LIT(KMoSyncServer, "MoSync Server 2");

// A version must be specified when creating a session with the server
const TUint KMoSyncMajorVersionNumber=1;
const TUint KMoSyncMinorVersionNumber=5;
const TUint KMoSyncBuildVersionNumber=0;

// Function codes (opcodes) used in message passing between client and server
enum TMoSyncServRqst
{
	EMoSyncGetNetworkInfo = 1,	//CTelephony::GetCurrentNetworkInfo
	EMoSyncGetSubscriberId,	//CTelephony::GetSubscriberId
	EMoSyncLocationGet,
	EMoSyncLocationStop,
	EMoSyncAutostartOn,
	EMoSyncAutostartOff,
	EMoSyncAutostartTimer,
	EMoSyncGetNetworkStatus,
	EMoSyncGetNetworkStatusChange,
	EMoSyncCancelNetworkStatusChange,
};

// reasons for server panic
enum TMoSyncPanic
{
	EBadRequest = 1,
	//EBadDescriptor,
	EMainSchedulerError,
	ESvrCreateServer,
	ESvrStartServer,
	ECreateTrapCleanup,
	EInternal,
	//ENotImplementedYet,
};

#endif	//CLIENTSERVER_H
