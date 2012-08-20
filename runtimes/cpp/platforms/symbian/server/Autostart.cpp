/* Copyright (C) 2010 MoSync AB

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

#include "MoSyncServer.h"
#include "ActiveEnder.h"
#include "Log.h"
#include <e32base.h>
#include <e32std.h>
#include <w32std.h>
#include <APACMDLN.H>
#include <APGCLI.H>
#include <APGTASK.H>

_LIT(KDatabaseFileName, "autostart.dat");

#define PIE(func) { int _res = (func); if(_res < 0) { LOG("Error in %s on %i: %i\n", __FILE__, __LINE__, _res); User::Leave(_res); } }

static void autostartProgramL(int uid, CApaCommandLine* cmdLine, RApaLsSession& ls, RWsSession& wss);

//***************************************************************************
// File system server connection management
//***************************************************************************

class MyRFs {
public:
	MyRFs() {}
	void Connect() {
		int res = rfs.Connect();
		if(res < 0)
			User::Leave(res);
	}
	~MyRFs() {
		rfs.Close();
	}
	RFs& operator()() { return rfs; }
private:
	RFs rfs;
};
#define FSS (myrfs())

//***************************************************************************
// Server functions
//***************************************************************************

void CMoSyncServer::writeAutostartDbL() {
	MyRFs myrfs;
	myrfs.Connect();
	RFile file;
	TFileName path;
	PIE(FSS.CreatePrivatePath(EDriveC));
	PIE(FSS.PrivatePath(path));
	path.Append(KDatabaseFileName);
	PIE(file.Replace(FSS, path, EFileStream | EFileShareExclusive | EFileWrite));
	CArrayFixFlat<TInt>& as(*mAutostart);
	for(int i=0; i<as.Count(); i++) {
		TPtrC8 ptr((const TUint8 *)&as[i], 4);
		PIE(file.Write(ptr));
	}
	PIE(file.Flush());
	file.Close();
}

void CMoSyncServer::doAutostartL() {
	LOG("doAutostartL\n");
	if(mAutostart != NULL) {
		LOG("mAutostart != NULL\n");
		PanicServer(EInternal);
	}
	//LOG("doAutostartL 1\n");
	mAutostart = new (ELeave) CArrayFixFlat<TInt>(4);
	CArrayFixFlat<TInt>& as(*mAutostart);
	//LOG("doAutostartL 2\n");

	// read database file, if it exists.
	MyRFs myrfs;
	myrfs.Connect();
	RFile file;
	int res = file.Open(FSS, KDatabaseFileName, EFileStream | EFileShareExclusive | EFileRead);
	if(res == KErrNotFound || res == KErrPathNotFound) {
		// no database. no need to read anything.
		LOG("Autostart database not found.\n");
		return;
	}
	//LOG("doAutostartL 3\n");
	PIE(res);
	int size;
	PIE(file.Size(size));
	//LOG("doAutostartL 4\n");
	if((size % 4) != 0) {
		LOG("Autostart database corrupted! Size: %i Not loading it.\n", size);
		return;
	}
	int count = size / 4;
	as.ResizeL(count);
	//LOG("doAutostartL 5\n");
	TPtr8 ptr((TUint8 *)&as[0], size);
	//LOG("doAutostartL 6\n");
	PIE(file.Read(ptr));
	if(ptr.Length() != size) {
		LOG("Autostart database couldn't be completely read! Length: %i\n", ptr.Length());
		as.Reset();
		return;
	}
	//LOG("doAutostartL 7\n");

	LOG("Autostart database read successfully. Launching %i programs...\n", as.Count());

	CApaCommandLine* cmdLine = CApaCommandLine::NewLC();
	RApaLsSession ls;
	PIE(ls.Connect());
	RWsSession wss;
	PIE(wss.Connect());
	for(int i=0; i<as.Count(); i++) {
		autostartProgramL(as[i], cmdLine, ls, wss);
	}
	wss.Close();
	ls.Close();
	CleanupStack::PopAndDestroy(cmdLine);

	LOG("Autostart complete.\n");
}

static void autostartProgramL(int uid, CApaCommandLine* cmdLine, RApaLsSession& ls, RWsSession& wss) {
	LOG("autostart: 0x%08X\n", uid);
	TUid tuid(TUid::Uid(uid));

	// check if the app is already running.
	TApaTaskList apataskList(wss);
	TApaTask apatask = apataskList.FindApp(tuid);
	bool isRunning = apatask.Exists();
	if(isRunning) {
		LOG("Already running.\n");
		return;
	}

	TApaAppInfo appInfo;
	int res = ls.GetAppInfo(appInfo, tuid);
	if(res < 0) {
		// If the program doesn't exist anymore (maybe it got uninstalled)
		// we don't want to cause a fuss, so just fail silently with logging.
		LOG("GetAppInfo failed: %i\n", res);
		return;
	}
	cmdLine->SetExecutableNameL(appInfo.iFullName);
	cmdLine->SetCommandL(EApaCommandBackground);
	res = ls.StartApp(*cmdLine);
	if(res < 0) {
		// The program may already be running.
		LOG("StartApp failed: %i\n", res);
	}
}

//***************************************************************************
// Session functions
//***************************************************************************

void CMoSyncSession::AutostartOnL(const RMessage2& aMessage) {
	// get the uid of the calling client.
	int uid = aMessage.SecureId().iId;
	LOG("AutostartOnL(0x%08X)\n", uid);

	CMoSyncServer* server = (CMoSyncServer*)Server();
	CArrayFixFlat<TInt>& as(*server->mAutostart);
	// search for the uid.
	for(int i=0; i<as.Count(); i++) {
		if(as[i] == uid) {
			aMessage.Complete(0);
			return;
		}
	}

	// not found, so add it.
	as.AppendL(uid);
	server->writeAutostartDbL();

	aMessage.Complete(1);
}

void CMoSyncSession::AutostartOffL(const RMessage2& aMessage) {
	// get the uid of the calling client.
	int uid = aMessage.SecureId().iId;
	LOG("AutostartOffL(0x%08X)\n", uid);

	CMoSyncServer* server = (CMoSyncServer*)Server();
	CArrayFixFlat<TInt>& as(*server->mAutostart);
	// search for the uid.
	for(int i=0; i<as.Count(); i++) {
		if(as[i] == uid) {	// gotcha.
			as.Delete(i);
			server->writeAutostartDbL();
			aMessage.Complete(1);
			return;
		}
	}

	// not found, so we can't remove it.
	aMessage.Complete(0);
}
