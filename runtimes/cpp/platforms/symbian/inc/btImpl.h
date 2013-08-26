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

#ifndef BTIMPL_H
#error "Bad include!"
#endif	//BTIMPL_H

//This file is designed to be included in the private section of class Syscall.

//***************************************************************************
//Classes
//***************************************************************************

class CBtService : public CBase, public MSdpAttributeValueVisitor {
public:
	int mPort;
	HBufC8* mName;
	RArray<TUUID> mUuids;
	const TSdpServRecordHandle mHandle;

	CBtService(TSdpServRecordHandle aHandle);
	~CBtService();

	//MSdpAttributeValueVisitor
	virtual void VisitAttributeValueL(CSdpAttrValue& aValue, TSdpElementType aType);
	virtual void StartListL(CSdpAttrValueList&);
	virtual void EndListL();
};

//***************************************************************************
//Variables
//***************************************************************************

enum BtState {
	eAvailable, eTurnedOff, eForbidden, eError
} gBtState;

CCBSynchronizer* gBtSynchronizer;

RHostResolver gBtResolver;
TInquirySockAddr gBtInquirySockAddr;
TNameEntry gBtDevice;	//temporary
RArray<TNameEntry> gBtDeviceArray;
int gBtNextDevice;
bool gBtNames;

CSdpAgent* gBtSdpa;
CSdpSearchPattern* gBtServiceSearchPattern;
CSdpAttrIdMatchList* gBtAttrMatchList;
CBtService* gBtService;	//temporary
RPointerArray<CBtService> gBtServiceArray;
int gBtNextService;

RSdp gBtSdp;
RSdpDatabase gBtSdpDB;

//***************************************************************************
//Power
//***************************************************************************

#ifdef __SERIES60_3X__
CClassSynchronizer<Syscall>* gBtPowerSync;
CRepository* gBtRepo;
void BtPowerRunL(TInt aResult);
#else	// 2nd edition
CSettingInfo* gBtInfo;
void HandleNotificationL(SettingInfo::TSettingID aID, const TDesC& aNewValue);
#endif	//__SERIES60_3X__

//***************************************************************************
//Functions
//***************************************************************************

void ClearBluetoothVariables();
void ConstructBluetoothL();
void InitBluetoothL();
void CancelBluetoothConnections();
void CloseBluetooth();
void DestructBluetooth();

// can only return eError or eTurnedOff.
BtState BtCheckPowerState();

static void BtSyncCallbackL(TAny* aPtr, TInt aResult);
void BtRunL(TInt aResult);

void BtHandleServiceSearchError(TInt aError);

//MSdpAgentNotifier
virtual void NextRecordRequestComplete(TInt aError, TSdpServRecordHandle aHandle,
	TInt aTotalRecordsCount);
virtual void AttributeRequestResult(TSdpServRecordHandle aHandle,
	TSdpAttributeID aAttrID, CSdpAttrValue* aAttrValue);
virtual void AttributeRequestComplete(TSdpServRecordHandle aHandle, TInt aError);


void SBTmaBtStartDeviceDiscovery(bool names);
int SBTmaBtGetNewDevice(MABtDeviceNative* dst);

int SBTmaBtCancelDiscovery();

//Takes a device address and the UUID of the service class to search for.
//For example, pass the RFCOMM UUID and you'll get all connectable services.
//Pass the OBEX Object Push UUID and you'll get only that service.
void SBTmaBtStartServiceDiscovery(const MABtAddr* address, const MAUUID* uuid);

//returns >0 on success.
int SBTmaBtGetNewService(MABtServiceNative* dst);

//does not remove a service from the queue. returns >0 on success.
int SBTmaBtGetNextServiceSize(MABtServiceSize* dst);
