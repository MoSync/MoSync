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

#include "config_platform.h"
#include "syscall.h"
#include "AppView.h"
#include "core.h"
#include <bluetooth/bt_errors.h>

#ifdef __SERIES60_3X__
#include <centralrepository.h>
#include <BTServerSDKCRKeys.h>
#else	// 2nd edition
#include <SettingInfo.h>
#endif	//__SERIES60_3X__

using namespace MoSyncError;

//***************************************************************************
//Defines
//***************************************************************************
#define SEARCH_TRAP(statement) LTRAP_BASE(statement,\
	BtHandleServiceSearchError(lhel); return )

//***************************************************************************
//Structors
//***************************************************************************

void Syscall::ClearBluetoothVariables() {
	gBtState = eError;
	gBtSynchronizer = NULL;
	gBtSdpa = NULL;
	gBtServiceSearchPattern = NULL;
	gBtAttrMatchList = NULL;
	gBtService = NULL;
	gBtInquirySockAddr.SetIAC(KGIAC);
}

Syscall::BtState Syscall::BtCheckPowerState() {
	// check power state
	// if error while checking
	// gBtState = eError;
	// else
	// gBtState = eTurnedOff;
	TInt btMode = -1;
#ifdef __SERIES60_3X__
	Smartie<CRepository> crep(CRepository::NewL(KCRUidBluetoothPowerState));
	TInt err = crep->Get(KBTPowerState, btMode);
#else	// 2nd edition
	Smartie<CSettingInfo> si(CSettingInfo::NewL(NULL));
	TInt err = si->Get(SettingInfo::EBluetoothPowerMode, btMode);
#endif	//__SERIES60_3X__
	LOG("BtCheckPowerState: err %i, mode %i\n", err, btMode);
	if(IS_SYMBIAN_ERROR(err) || btMode < 0) {
		LOG_FAILURE(err);
		return eError;
	}
	if(btMode == 0)
		return eTurnedOff;
	LOG("Mysterious BluetoothPowerMode: %i\n", btMode);
	return eError;
}

void Syscall::ConstructBluetoothL() {
	TProtocolDesc pInfo;
	_LIT(KL2Cap, "BTLinkManager");	//symbian magic
	int res;
	res = gSocketServ.FindProtocol(KL2Cap(), pInfo);
	if(res == KErrHardwareNotAvailable) {
		LOG("Bt: couldn't find protocol!\n");
		return;
	}
	LHEL(res);
	
	res = gBtResolver.Open(gSocketServ, pInfo.iAddrFamily, pInfo.iProtocol);
	if(res == KErrPermissionDenied) {
		LOG("BtResolver Open error %i\n", res);
		gBtState = eForbidden;
		return;
	}
	if(res == KErrHardwareNotAvailable || res == KErrNotSupported) {
		LOG("BtResolver Open error %i\n", res);
		gBtState = BtCheckPowerState();
		return;
	} else {
		LHEL(res);
	}

	TSyncCallback scb(BtSyncCallbackL, this);
	gBtSynchronizer = new (ELeave) CCBSynchronizer(scb);

	gBtServiceSearchPattern = CSdpSearchPattern::NewL();

	//magic
	gBtAttrMatchList = CSdpAttrIdMatchList::NewL();
	gBtAttrMatchList->AddL(TAttrRange(KSdpAttrIdProtocolDescriptorList));
	gBtAttrMatchList->AddL(TAttrRange(KSdpAttrIdServiceClassIDList));
	gBtAttrMatchList->AddL(TAttrRange(KSdpAttrIdServiceID));
	gBtAttrMatchList->AddL(TAttrRange(KSdpAttrIdBluetoothProfileDescriptorList));
	gBtAttrMatchList->AddL(TAttrRange(KSdpAttrIdBasePrimaryLanguage +
		KSdpAttrIdOffsetServiceName));
	
	LHEL(gBtSdp.Connect());
	LHEL(gBtSdpDB.Open(gBtSdp));

	gBtState = eAvailable;
}

void Syscall::DestructBluetooth() {
	if(gBtState == eAvailable)
		gBtResolver.Cancel();
	gBtResolver.Close();
	SAFE_DELETE(gBtSynchronizer);
	gBtDeviceArray.Close();

	SAFE_DELETE(gBtSdpa);
	SAFE_DELETE(gBtServiceSearchPattern);
	SAFE_DELETE(gBtAttrMatchList);
	SAFE_DELETE(gBtService);
	gBtServiceArray.ResetAndDestroy();
	
	gBtSdpDB.Close();
	gBtSdp.Close();
}

//***************************************************************************
//Helpers
//***************************************************************************
static void CopyAddr(const TBTSockAddr& aAddr, void* dst) {
	const TBTDevAddr& devAddr(aAddr.BTAddr());
	const TPtrC8 devPtrC8(devAddr.Des());
	DEBUG_ASSERT(devPtrC8.Length() == BTADDR_LEN);
	memcpy(dst, devPtrC8.Ptr(), BTADDR_LEN);
}

//***************************************************************************
//Device
//***************************************************************************
void Syscall::BtSyncCallbackL(TAny* aPtr, TInt aResult) {
	LOGBT("BtSyncCallbackL %i\n", aResult);
	((Syscall*)aPtr)->BtRunL(aResult);
}

void Syscall::BtRunL(TInt aResult) {
	int result;
	if(aResult == KErrHostResNoMoreResults) {	//time to end this
		result = 1;
	} else if(aResult == KErrCancel) {
		result = CONNERR_CANCELED;
	} else if(aResult) {
		result = CONNERR_GENERIC;
	} else {
		result = 0;
		gBtDeviceArray.AppendL(gBtDevice);
		gBtResolver.Next(gBtDevice, *gBtSynchronizer->Status());
		gBtSynchronizer->SetActive();
	}
	MAEvent event;
	event.type = EVENT_TYPE_BT;
	event.state = result;
	DEBUG_ASSERT(gAppView.AddEvent(event));
}

//***************************************************************************
//Service
//***************************************************************************
void Syscall::BtHandleServiceSearchError(TInt /*aError*/) {
	MAEvent event;
	event.type = EVENT_TYPE_BT;
	event.state = CONNERR_GENERIC;
	DEBUG_ASSERT(gAppView.AddEvent(event));
	SAFE_DELETE(gBtSdpa);
}

void Syscall::NextRecordRequestComplete(TInt aError,
	TSdpServRecordHandle aHandle, TInt aTotalRecordsCount)
{	//Leaving is not allowed.
	LOGBT("NRRC e%i h%i c%i\n", aError, aHandle, aTotalRecordsCount);
	DEBUG_ASSERT(gBtSdpa != NULL);
	DEBUG_ASSERT(gBtService == NULL);

	MAEvent event;
	event.type = EVENT_TYPE_BT;
	if(aError == KErrEof) {	//discovery complete
		event.state = gBtServiceArray.Count() + 1;
	} else if(aError != KErrCancel) {
		event.state = CONNERR_CANCELED;
	} else if(aError != 0) {
		event.state = CONNERR_GENERIC;
	}
	if(aError != 0) {
		DEBUG_ASSERT(gAppView.AddEvent(event));
		SAFE_DELETE(gBtSdpa);
	}

	if(aError != 0 || aTotalRecordsCount == 0)
		return;
	SEARCH_TRAP(gBtSdpa->AttributeRequestL(aHandle, *gBtAttrMatchList));

	gBtService = new CBtService(aHandle);
	MYASSERT(gBtService != NULL, ERR_OOM);
}

void Syscall::AttributeRequestResult(TSdpServRecordHandle aHandle,
	TSdpAttributeID LOGBT_ARG(aAttrID), CSdpAttrValue* aAttrValue)
{	//Leaving is not allowed.
	LOGBT("ARR h%i a%i\n", aHandle, aAttrID);
	DEBUG_ASSERT(gBtSdpa != NULL);
	DEBUG_ASSERT(gBtService != NULL);
	DEBUG_ASSERT(gBtService->mHandle == aHandle);

	Smartie<CSdpAttrValue> deleter(aAttrValue);
	SEARCH_TRAP(aAttrValue->AcceptVisitorL(*gBtService));	//should be synchronous
}

void Syscall::AttributeRequestComplete(TSdpServRecordHandle aHandle,
	TInt aError)
{	//Leaving is not allowed.
	LOGBT("ARC h%i e%i\n", aHandle, aError);
	DEBUG_ASSERT(gBtSdpa != NULL);
	DEBUG_ASSERT(gBtService != NULL);
	DEBUG_ASSERT(gBtService->mHandle == aHandle);

	if(aError) {
		BtHandleServiceSearchError(aError);
		return;
	}

	gBtServiceArray.AppendL(gBtService);
	gBtService = NULL;

	MAEvent event;
	event.type = EVENT_TYPE_BT;
	event.state = 0;
	DEBUG_ASSERT(gAppView.AddEvent(event));

	SEARCH_TRAP(gBtSdpa->NextRecordRequestL());
}

#ifdef BLUETOOTH_DEBUGGING_MODE
static void ParseUUID(TDes8& aBuf, TUUID aUuid) {
	TPtrC8 uuid(aUuid.ShortestForm());
	const TUint8* ptr = uuid.Ptr();

	switch(uuid.Length()) {
	case 2:
		aBuf.Format(_L8("0x%04X"), BigEndian::Get16(ptr));
		break;
	case 4:
		aBuf.Format(_L8("0x%08X"), BigEndian::Get32(ptr));
		break;
	case 16:
		aBuf.Format(_L8("%08X-%08X-%08X-%08X"), BigEndian::Get32(ptr),
			BigEndian::Get32(ptr + 4), BigEndian::Get32(ptr + 8),
			BigEndian::Get32(ptr + 12));
		break;
	default:
		DEBIG_PHAT_ERROR;
	}
}
#endif

//***************************************************************************
//CBtService
//***************************************************************************
Syscall::CBtService::CBtService(TSdpServRecordHandle aHandle) :
	mPort(-1), mName(NULL), mUuids(8), mHandle(aHandle)
{
	LOGBT("CBtService(%i)\n", mHandle);
}

Syscall::CBtService::~CBtService() {
	SAFE_DELETE(mName);
	mUuids.Close();
	LOGBT("~CBtService(%i)\n", mHandle);
}

void Syscall::CBtService::VisitAttributeValueL(CSdpAttrValue& aValue,
	TSdpElementType aType)
{
	LOGBT("VAV h%i t%i ", mHandle, aType);
	switch(aType) {
	case ETypeNil:
		LOGBT("Nil\n");
		break;
	case ETypeUint:
		LOGBT("Uint %i\n", aValue.Uint());
		//The Uint following the RFCOMM UUID is supposed to be the service's port.
		if(mUuids.Count() > 0 && mPort == -1) {
			if(mUuids[mUuids.Count() - 1] == TUUID(KRFCOMM)) {
				mPort = aValue.Uint();
				LOGBT("Port found: %i\n", mPort);
			}
		}
		break;
	case ETypeInt:
		LOGBT("Int %i\n", aValue.Int());
		break;
	case ETypeUUID:
		mUuids.AppendL(aValue.UUID());
#ifdef BLUETOOTH_DEBUGGING_MODE
		{
			TBuf8<64> buf;
			ParseUUID(buf, aValue.UUID());
			LOGBT("UUID %S\n", &buf);
		}
#endif
		break;
	case ETypeString: {
		const TDesC8& des = aValue.Des();
		LOGBT("String %S\n", &des);
		if(mName == NULL) {
			mName = HBufC8::NewL(des.Length());
			*mName = des;
		}
		break;
	}
	case ETypeBoolean:
		LOGBT("Bool %i\n", aValue.Bool());
		break;
	case ETypeDES:
		LOGBT("DES %i\n", aValue.DataSize());
		break;
	case ETypeDEA:
		LOGBT("DEA %i\n", aValue.DataSize());
		break;
#ifdef BLUETOOTH_DEBUGGING_MODE
	case ETypeURL: {
		const TDesC8& des = aValue.Des();
		LOGBT("URL %S\n", &des);
		break;
	}
#endif
	case ETypeEncoded:
		LOGBT("Encoded %i\n", aValue.DataSize());
		break;
	default:
		LOGBT("Unknown %i\n", aValue.DataSize());
	}
}

void Syscall::CBtService::StartListL(CSdpAttrValueList& LOGBT_ARG(aList)) {
	LOGBT("StartList %i\n", aList.DataSize());
}
void Syscall::CBtService::EndListL() {
	LOGBT("EndList\n");
}


//***************************************************************************
//External functions
//***************************************************************************
void Syscall::SBTmaBtStartDeviceDiscovery(bool names) {
	LOGBT("SBTmaBtStartDeviceDiscovery\n");
	MYASSERT(!gBtSynchronizer->IsActive(), BTERR_DISCOVERY_IN_PROGRESS);
	MYASSERT(gBtSdpa == NULL, BTERR_DISCOVERY_IN_PROGRESS);
	gBtDeviceArray.Reset();
	gBtNextDevice = 0;
	gBtNames = names;
	gBtInquirySockAddr.SetAction(KHostResInquiry | (names ? KHostResName : 0) |
		KHostResIgnoreCache);
	gBtResolver.GetByAddress(gBtInquirySockAddr, gBtDevice, *gBtSynchronizer->Status());
	gBtSynchronizer->SetActive();
	LOGBT("SBTmaBtStartDeviceDiscovery done\n");	//fixes instability in emulator?
}

int Syscall::SBTmaBtCancelDiscovery() {
	LOGBT("SBTmaBtCancelDiscovery\n");
	if(!gBtSynchronizer->IsActive())
		return 0;
	if(gBtSdpa) {	//service
		gBtSdpa->Cancel();
	} else {	//device
		gBtResolver.Cancel();
	}
	//hopefully, this will allow the active object to be cancelled before
	//another discovery is started.
	VM_Yield();

	return 1;
}

int Syscall::SBTmaBtGetNewDevice(MABtDeviceNative* dst) {
	if(gBtNextDevice >= gBtDeviceArray.Count())
		return 0;

	TNameEntry& src(gBtDeviceArray[gBtNextDevice++]);
	const TBTSockAddr& addr(TBTSockAddr::Cast(src().iAddr));

	CopyAddr(addr, &dst->address);

	if(gBtNames) {
		//copy device name
		byte* nameDst = (byte*)dst->name;
		const THostName& nameSrc(src().iName);
		const int nameLen = MIN(dst->nameBufSize - 1, nameSrc.Length());
		TPtr8 nameDst8(nameDst, nameLen);
		const TPtrC16 nameSrc16(nameSrc.Left(nameLen));

		nameDst8.Copy(nameSrc16);
		nameDst[nameLen] = 0;
		
		return nameSrc.Length();
	} else {
		return 1;
	}
}

//Takes a device address and the UUID of the service class to search for.
//For example, pass the RFCOMM UUID and you'll get all connectable services.
//Or pass the OBEX Object Push UUID and you'll get only that service.
void Syscall::SBTmaBtStartServiceDiscovery(const MABtAddr* address, const MAUUID* uuid) {
	LOGBT("SBTmaBtStartServiceDiscovery\n");
	MYASSERT(!gBtSynchronizer->IsActive(), BTERR_DISCOVERY_IN_PROGRESS);
	MYASSERT(gBtSdpa == NULL, BTERR_DISCOVERY_IN_PROGRESS);

	gBtServiceArray.ResetAndDestroy();
	gBtNextService = 0;

	TPtrC8 ap(address->a, BTADDR_LEN);
	TBTDevAddr addr(ap);
	gBtSdpa = CSdpAgent::NewL(*this, addr);
	gBtServiceSearchPattern->Reset();
	gBtServiceSearchPattern->AddL(TUUID(uuid->i[0], uuid->i[1], uuid->i[2], uuid->i[3]));
	gBtSdpa->SetRecordFilterL(*gBtServiceSearchPattern);
	gBtSdpa->NextRecordRequestL();
	LOGBT("SBTmaBtStartServiceDiscovery done\n");
};

//returns >0 on success.
int Syscall::SBTmaBtGetNewService(MABtServiceNative* dst) {
	if(gBtNextService == gBtServiceArray.Count()) {
		return 0;
	}
	MYASSERT(dst->nameBufSize >= 0, BTERR_NEGATIVE_BUFFER_SIZE);
	const CBtService* serv = gBtServiceArray[gBtNextService++];
	dst->port = serv->mPort;
	
	if(dst->nameBufSize > 0) {
		if(serv->mName == NULL) {
			dst->name[0] = 0;
		} else {
			int servNameLen = MIN(dst->nameBufSize - 1, serv->mName->Length());
			memcpy(dst->name, serv->mName->Ptr(), servNameLen);
			dst->name[servNameLen] = 0;
		}
	}
	
	for(int i=0; i<serv->mUuids.Count(); i++) {
		//dst->uuids[i] = serv->mUuids[i];
		//memcpy(&dst->uuids[i], serv->mUuids[i].LongForm().Ptr(), 16);	//bad endian
		const byte* ptr = serv->mUuids[i].LongForm().Ptr();
		for(int j=0; j<4; j++) {
			dst->uuids[i].i[j] = BigEndian::Get32(ptr);
			ptr += 4;
		}
	}
	return 1;
}

//does not remove a service from the queue. returns >0 on success.
int Syscall::SBTmaBtGetNextServiceSize(MABtServiceSize* dst) {
	if(gBtNextService == gBtServiceArray.Count()) {
		return 0;
	}
	const CBtService* serv = gBtServiceArray[gBtNextService];
	if(serv->mName == NULL) {
		dst->nameBufSize = -1;
	} else {
		dst->nameBufSize = serv->mName->Length() + 1;
	}
	dst->nUuids = serv->mUuids.Count();
	return 1;
}
