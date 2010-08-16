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

#include "Platform.h"
#if defined(__SERIES60_3X__) && defined(WLAN)

#include "syscall.h"
#include "AppView.h"
#include "core.h"
#include "CBSync.h"

using namespace MoSyncError;

//***************************************************************************
//Structors
//***************************************************************************

void Syscall::ClearWlanVariables() {
	gWlanInfo = NULL;
	gWlanClient = NULL;
	gWlanSynchronizer = NULL;
	gWlanAvailable = false;
}
void Syscall::ConstructWlanL() {
	LOGW("ConstructWlanL\n");

	TSyncCallback scb(WlanSyncCallbackL, this);
	gWlanSynchronizer = new (ELeave) CCBSynchronizer(scb);

	TRAPD(err, gWlanInfo=CWlanScanInfo::NewL());
	if(err != KErrNone)
		return;
	TRAP(err, gWlanClient=CWlanMgmtClient::NewL());	//leaves with KErrNotFound on emu
	if(err == KErrNone)
		gWlanAvailable = true;

#if 0
	//test
	LHEL(client->GetScanResults(*scanInfo));
	LOGW("sISize: %i\n", scanInfo->Size());
	for(scanInfo->First(); !scanInfo->IsDone(); scanInfo->Next()) {
		TWlanBssid mac;
		scanInfo->Bssid(mac);
		LOGW("%02X%02X%02X%02X%02X%02X\n", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
		LOGW("RX: %i\n", scanInfo->RXLevel());
	}
	delete client;
	delete scanInfo;
#endif
}
void Syscall::DestructWlan() {
	SAFE_DELETE(gWlanClient);
	SAFE_DELETE(gWlanInfo);
	SAFE_DELETE(gWlanSynchronizer);
}

void Syscall::WlanSyncCallbackL(TAny* aPtr, TInt aResult) {
	LOGW("WlanSyncCallbackL %i\n", aResult);
	Syscall* self = (Syscall*)aPtr;
	//reset iterator
	self->gWlanInfo->First();
	//send event
	MAEvent event;
	event.type = EVENT_TYPE_WLAN;
	event.state = aResult == KErrNone ? 1 : CONNERR_GENERIC;
	DEBUG_ASSERT(self->gAppView.AddEvent(event));
}

int Syscall::maWlanStartDiscovery() {
	if(!gWlanAvailable)
		return IOCTL_UNAVAILABLE;
	gWlanClient->GetScanResults(*gWlanSynchronizer->Status(), *gWlanInfo);
	gWlanSynchronizer->SetActive();
	return 0;
}

int Syscall::maWlanGetNewAp(MAWlanAp* dst) {
	char* vmName = dst->name;
	dst->name = (char*)GetValidatedMemRange((int)dst->name, dst->nameBufSize);
	int res = WlanGetNewAp(dst);
	dst->name = vmName;
	return res;	
}

int Syscall::WlanGetNewAp(MAWlanAp* dst) {
	if(gWlanSynchronizer->IsActive())
		return 0;
	if(gWlanInfo->IsDone())
		return 0;

	TWlanBssid mac;
	gWlanInfo->Bssid(mac);
	memcpy(dst->address.a, mac.Ptr(), WLAN_ADDR_LEN);

	dst->signalStrength = - gWlanInfo->RXLevel();	//dBm

	
	TUint8 ieLen = 0;
	const TUint8* ieData = NULL;
	// Information Element ID for SSID as specified in 802.11.
	const TUint8 KWlan80211SsidIE = 0;
	TInt ret = gWlanInfo->InformationElement(KWlan80211SsidIE, ieLen, &ieData);
	if(ret == KErrNone) {
		// ieData now points to the beginning of the 802.11 SSID payload data
		// (i.e. the header is bypassed). ieLen contains the length of payload
		// data.
		if(ieLen < dst->nameBufSize) {
			memcpy(dst->name, ieData, ieLen);
			dst->name[ieLen] = 0;
		}
	} else {
		dst->name[0] = 0;
	}

	gWlanInfo->Next();

	return 1;
}

#endif	//WLAN
