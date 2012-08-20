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
		CActive(EPriorityStandard), mPckg((TUint8*)&data, sizeof(T), sizeof(T)), mCancellationCode(cr), mTelephony(aTelephony) {}
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
		CActive(EPriorityStandard), CTelephonyHandler(aTelephony, CTelephony::EGetCurrentNetworkInfoCancel, mInfo) {}
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
		CActive(EPriorityStandard), CTelephonyHandler(aTelephony, CTelephony::EGetNetworkRegistrationStatusCancel, mNR) {}
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
		CActive(EPriorityStandard), CTelephonyHandler(aTelephony, CTelephony::ENetworkRegistrationStatusChangeCancel, mNR) {}
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
