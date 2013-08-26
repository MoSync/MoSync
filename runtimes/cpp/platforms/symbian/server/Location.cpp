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
		mPositioner.NotifyPositionUpdate(mInfo, iStatus);
		SetActive();
	}

	void RunL() {
		//LOG("CPositioner:RunL %i\n", iStatus.Int());
		TPosition p;
		TPckg<TPosition> pckg(p);
		mInfo.GetPosition(p);

		if(iStatus == KErrNone) {
			TRAPD(err, mMessage.WriteL(0, pckg));
		}
		//if(iStatus != KErrCancel)
		if(mWaiting) {
			mMessage.Complete(iStatus.Int());
			mWaiting = false;
		}
		lazyDelete(mMessage, this);
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
	if(!mPositionerOpen) {
		LocationStop();
		ULIE(mPositionServer.Connect());
		ULIE(mPositioner.Open(mPositionServer));
		ULIE(mPositioner.SetRequestor(CRequestor::ERequestorService,
			CRequestor::EFormatApplication, KMoSyncServer));
		mPositionerOpen = true;
	}
	mPn = new (ELeave) CPositionNotifier(aMessage, mPositioner);
	mPn->start();
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
