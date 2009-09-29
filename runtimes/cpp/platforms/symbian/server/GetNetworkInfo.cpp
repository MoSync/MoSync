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

class CNetworkInfoGetter : public CActiveLink {
public:
	CNetworkInfoGetter(const RMessage2& aMessage) : mMessage(aMessage), mPckg(mInfo) {}
	virtual ~CNetworkInfoGetter() {}
	
	void start(CTelephony* aTelephony) {
		mTelephony = aTelephony;
		LOG("GetCurrentNetworkInfo\n");
		mTelephony->GetCurrentNetworkInfo(iStatus, mPckg);
		SetActive();
	}
	
	void RunL() {
		LOG("CNetworkInfoGetter:RunL %i\n", iStatus.Int());
		if(iStatus == KErrNone)
			mMessage.WriteL(0, mPckg);
		mMessage.Complete(iStatus.Int());
		lazyDelete(mMessage, this);
	}
	
	void DoCancel() {
		LOG("CNetworkInfoGetter:DoCancel\n");
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
