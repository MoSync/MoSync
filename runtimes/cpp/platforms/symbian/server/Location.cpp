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

#include <lbs.h>

#include "MoSyncServer.h"
#include "ActiveEnder.h"
#include "Log.h"

#define ULIE User::LeaveIfError

class CPositionNotifier : public CActiveLink {
public:
	CPositionNotifier(const RMessage2& aMessage, RPositioner& aPositioner)
		: mPositioner(aPositioner), mMessage(aMessage), mWaiting(true) {}
	virtual ~CPositionNotifier() {}
	
	void start() {
		LOG("mPositioner.NotifyPositionUpdate\n");
		mPositioner.NotifyPositionUpdate(mInfo, iStatus);
		SetActive();
	}
	
	void RunL() {
		LOG("CPositioner:RunL %i\n", iStatus.Int());
		TPosition p;
		TPckg<TPosition> pckg(p);
		mInfo.GetPosition(p);
		
		LOG("lat: %g\n", p.Latitude());
		if(iStatus == KErrNone) {
			LOG("mMessage.WriteL %i\n", pckg.Size());
			TRAPD(err, mMessage.WriteL(0, pckg));
			LOG("mMessage.WriteL res %i\n", err);
		}
		//if(iStatus != KErrCancel)
		if(mWaiting) {
			mMessage.Complete(iStatus.Int());
			mWaiting = false;
		}
		LOG("Message completed\n");
		lazyDelete(mMessage, this);
		LOG("CPositioner:RunL complete\n");
	}
	
	void DoCancel() {	//should never be called.
		LOG("CPositioner:DoCancel\n");
#if 0
		int res = mPositioner.CancelRequest(EPositionerNotifyPositionUpdate);
		if(res != KErrNone) {
			LOG("Positioner.CancelRequest() failed with %i", res);
		}
		//lazyDelete(mMessage, this);
#endif
	}
	
private:
	RPositioner& mPositioner;
	TPositionInfo mInfo;
public:
	RMessage2 mMessage;
	bool mWaiting;
};

void CMoSyncSession::LocationStop() {
	if(mPositionerOpen)
		mPositioner.CancelRequest(EPositionerNotifyPositionUpdate);
	mPositioner.Close();
	mPositionServer.Close();
	mPositionerOpen = false;
}

//todo: avoid calling new() all the time; make the notifier a singleton.
//don't do more than one of these at once!
void CMoSyncSession::LocationGetL(const RMessage2& aMessage) {
	LOG("LocationGetL\n");
	if(!mPositionerOpen) {
		LocationStop();
		LOG("mPositionServer.Connect\n");
		ULIE(mPositionServer.Connect());
		LOG("mPositioner.Open\n");
		ULIE(mPositioner.Open(mPositionServer));
		LOG("mPositioner.SetRequestor\n");
		ULIE(mPositioner.SetRequestor(CRequestor::ERequestorService,
			CRequestor::EFormatApplication, KMoSyncServer));
		mPositionerOpen = true;
	}
	mPn = new (ELeave) CPositionNotifier(aMessage, mPositioner);
	mPn->start();
	LOG("LocationGetL done\n");
}


//will cause any active LocationGetL request to cancel silently

//finishes immediately, from the server's point of view,
//but the client runs in another thread, so:
//don't do more than one of these at once!

void CMoSyncSession::LocationStopL(const RMessage2& aMessage) {
	LocationStop();
	if(mPn) if(mPn->mWaiting) {
		mPn->mMessage.Complete(KErrCancel);
		mPn->mWaiting = false;
	}
	aMessage.Complete(KErrNone);
}
