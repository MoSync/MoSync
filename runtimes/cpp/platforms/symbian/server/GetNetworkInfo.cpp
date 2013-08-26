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

#include <Etel3rdParty.h>

#include "MoSyncServer.h"
#include "ActiveEnder.h"
#include "Log.h"

class CNetworkInfoGetter : public CActiveLink {
public:
	CNetworkInfoGetter(const RMessage2& aMessage) : mMessage(aMessage), mPckg(mInfo) {}
	virtual ~CNetworkInfoGetter() {}

	void start(CTelephony* aTelephony) {
		mTelephony = aTelephony;
		mTelephony->GetCurrentNetworkInfo(iStatus, mPckg);
		SetActive();
	}

	void RunL() {
		if(iStatus == KErrNone)
			mMessage.WriteL(0, mPckg);
		mMessage.Complete(iStatus.Int());
		lazyDelete(mMessage, this);
	}

	void DoCancel() {
		mTelephony->CancelAsync(CTelephony::EGetCurrentNetworkInfoCancel);
		lazyDelete(mMessage, this);
	}

private:
	const RMessage2& mMessage;
	CTelephony::TNetworkInfoV1 mInfo;
	TPckg<CTelephony::TNetworkInfoV1> mPckg;
	CTelephony* mTelephony;
};

//todo: avoid calling new() all the time; make the getter a singleton.
//don't do more than one of these at once!
void CMoSyncSession::GetNetworkInfoL(const RMessage2& aMessage) {
	if(mTelephony == NULL) {
		mTelephony = CTelephony::NewL();
	}
	CNetworkInfoGetter* nig = new (ELeave) CNetworkInfoGetter(aMessage);
	nig->start(mTelephony);
}
