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

class CTelephonyHandler : public CActiveLink {
protected:
	RMessage2 mMessage;
	TPtr8 mPckg;
	const CTelephony::TCancellationRequest mCancellationCode;
	CTelephony* mTelephony;
public:
	template<class T> CTelephonyHandler(CTelephony* aTelephony, CTelephony::TCancellationRequest cr, T& data) :
		mPckg((TUint8*)&data, sizeof(T), sizeof(T)), mCancellationCode(cr), mTelephony(aTelephony) {}
	virtual ~CTelephonyHandler() {
		if(IsActive())
			Cancel();
	}

	void RunL() {
		//LOG("CTelephonyHandler %i RunL %i 0x%x\n", mCancellationCode, iStatus.Int(), this);
		if(iStatus == KErrNone)
			mMessage.WriteL(0, mPckg);
		mMessage.Complete(iStatus.Int());
	}

	void DoCancel() {
		LOG("CTelephonyHandler %i DoCancel\n", mCancellationCode);
		mTelephony->CancelAsync(mCancellationCode);
		mMessage.Complete(KErrCancel);
	}
};

class CNetworkInfoGetter : public CTelephonyHandler {
public:
	CNetworkInfoGetter(CTelephony* aTelephony) :
		CTelephonyHandler(aTelephony, CTelephony::EGetCurrentNetworkInfoCancel, mInfo) {}
	virtual ~CNetworkInfoGetter() {}

	void start(const RMessage2& aMessage) {
		mMessage = aMessage;
		LOG("GetCurrentNetworkInfo\n");
		mTelephony->GetCurrentNetworkInfo(iStatus, mPckg);
		SetActive();
	}
private:
	CTelephony::TNetworkInfoV1 mInfo;
};

class CNetworkStatusGetter : public CTelephonyHandler {
public:
	CNetworkStatusGetter(CTelephony* aTelephony) :
		CTelephonyHandler(aTelephony, CTelephony::EGetNetworkRegistrationStatusCancel, mNR) {}
	virtual ~CNetworkStatusGetter() {}

	void start(const RMessage2& aMessage) {
		mMessage = aMessage;
		LOG("GetNetworkRegistrationStatus\n");
		mTelephony->GetNetworkRegistrationStatus(iStatus, mPckg);
		SetActive();
	}
private:
	CTelephony::TNetworkRegistrationV1 mNR;
};

class CNetworkStatusChangeGetter : public CTelephonyHandler {
public:
	CNetworkStatusChangeGetter(CTelephony* aTelephony) :
		CTelephonyHandler(aTelephony, CTelephony::ENetworkRegistrationStatusChangeCancel, mNR) {}
	virtual ~CNetworkStatusChangeGetter() {}

	void start(const RMessage2& aMessage) {
		mMessage = aMessage;
		LOG("GetNetworkRegistrationStatusChange\n");
		mTelephony->NotifyChange(iStatus, CTelephony::ENetworkRegistrationStatusChange, mPckg);
		SetActive();
	}
private:
	CTelephony::TNetworkRegistrationV1 mNR;
};

template<class T> void CMoSyncSession::GetTelephonyInfoL(const RMessage2& aMessage, T*& getter) {
	if(mTelephony == NULL) {
		mTelephony = CTelephony::NewL();
	}
	if(getter == NULL) {
		getter = new (ELeave) T(mTelephony);
	}
	getter->start(aMessage);
}
