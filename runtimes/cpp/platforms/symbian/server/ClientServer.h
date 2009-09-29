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

#ifndef CLIENTSERVER_H
#define CLIENTSERVER_H

#include <e32base.h>

// server name
_LIT(KMoSyncServer, "MoSync Server");

// A version must be specified when creating a session with the server
const TUint KMoSyncMajorVersionNumber=1;
const TUint KMoSyncMinorVersionNumber=2;
const TUint KMoSyncBuildVersionNumber=0;

// Function codes (opcodes) used in message passing between client and server
enum TMoSyncServRqst
{
	EMoSyncGetNetworkInfo = 1,	//CTelephony::GetCurrentNetworkInfo
	EMoSyncGetSubscriberId,	//CTelephony::GetSubscriberId
	EMoSyncLocationGet,
	EMoSyncLocationStop,
};

// reasons for server panic
enum TMoSyncPanic
{
	EBadRequest = 1,
	//EBadDescriptor,
	EMainSchedulerError,
	ESvrCreateServer,
	ESvrStartServer,
	ECreateTrapCleanup
	//ENotImplementedYet,
};

#endif	//CLIENTSERVER_H
