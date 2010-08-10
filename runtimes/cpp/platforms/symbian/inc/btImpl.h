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

bool gBtAvailable;
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
//Functions
//***************************************************************************

void ClearBluetoothVariables();
void ConstructBluetoothL();
void DestructBluetooth();

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
int SBTmaBtGetNewDevice(MABtDevice* dst);

int SBTmaBtCancelDiscovery();

//Takes a device address and the UUID of the service class to search for.
//For example, pass the RFCOMM UUID and you'll get all connectable services.
//Pass the OBEX Object Push UUID and you'll get only that service.
void SBTmaBtStartServiceDiscovery(const MABtAddr* address, const MAUUID* uuid);

//returns >0 on success.
int SBTmaBtGetNewService(MABtService* dst);

//does not remove a service from the queue. returns >0 on success.
int SBTmaBtGetNextServiceSize(MABtServiceSize* dst);
