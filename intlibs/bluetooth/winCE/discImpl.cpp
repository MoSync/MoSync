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

#include "../config_bluetooth.h"

#include <Winsock2.h>
#include <Ws2bth.h>
#include <vector>

#include <bthapi.h>
#include <bt_api.h>
#include <bt_sdp.h>
#include <ws2bth.h>

//#define WSASERVICE_NOT_FOUND             10108L

#include <helpers/helpers.h>
#include <helpers/array.h>
#include <helpers/CriticalSection.h>

#include "../discInternal.h"
using namespace Bluetooth;
#include <MAUtil/mauuid.h>

#include <base_errors.h>
using namespace MoSyncError;

#if 1

class MySdpWalk : public ISdpWalk {
public:
	//could be set up to return CONNERR codes, but we don't need that yet.
	MySdpWalk(BtService& serv) : mServ(serv), expectBtPort(false) {}

	HRESULT WalkNode(NodeData* pData, ULONG state) {
		static int level = 0;
		for(int i=0; i<level; i++) {
			LOGBT(" ");
		}
		LOGBT("WalkNode 0x%08X, %i. type: 0x%x sType: 0x%x\n", pData, state,
			pData->type, pData->specificType);

		if(expectBtPort) {
			expectBtPort = false;
			if(pData->type == SDP_TYPE_UINT) {
				//we got it :)
				switch(pData->specificType) {
				case SDP_ST_UINT8:
					mServ.port = pData->u.uint8;
					break;
				case SDP_ST_UINT16:
					mServ.port = pData->u.uint16;
					break;
				default:
					LOG("Unhandled SDP RFCOMM port sType: %i\n", pData->specificType);
					return E_FAIL;
				}
			} else {
				LOG("Bad SDP data type: %i, %i\n", pData->type, pData->specificType);
				return E_FAIL;
			}
		}

		if(pData->type == SDP_TYPE_UUID) {
			MAUUID mauuid = Bluetooth_Base_MAUUID;
			switch(pData->specificType) {
				case SDP_ST_UUID16:
					mauuid.i[0] = pData->u.uuid16;
					break;
				case SDP_ST_UUID32:
					mauuid.i[0] = pData->u.uuid32;
					break;
				case SDP_ST_UUID128:
					mauuid = GUID2MAUUID(pData->u.uuid128);
					break;
				default:
					LOG("Bad SDP UUID sType: %i\n", pData->specificType);
					return E_FAIL;
			}
			mServ.uuids.push_back(mauuid);
			if(memcmp(&mauuid, &RFCOMM_PROTOCOL_MAUUID, sizeof(MAUUID)) == 0) {
				expectBtPort = true;
			}
		} else if(pData->type == SDP_TYPE_CONTAINER) {
			if(state)
				level++;
			else if(level > 0)
				level--;
		}
		return TRUE;
	}

	HRESULT WalkStream(UCHAR elementType, ULONG elementSize, UCHAR* pStream) {
		BIG_PHAT_ERROR(ERR_INTERNAL);
	}

	HRESULT QueryInterface(REFIID riid, void **ppvInt) {
		BIG_PHAT_ERROR(ERR_FUNCTION_UNIMPLEMENTED);
	}

	ULONG AddRef() {
		return ++m_nRefCount;
	}

	ULONG Release() {
		if (--m_nRefCount)
			return m_nRefCount;
		delete this;
		return 0;
	}
private:
	ULONG m_nRefCount;
	BtService& mServ;
	bool expectBtPort;
};


int handleSdpResponse(std::vector<BtService>* services, LPWSAQUERYSET pQs) {
	//set up SdpStream

	HRESULT hres;
	ISdpStream *pIStream = NULL;
	//ISdpRecord **pSdpRecords = NULL;
	// Create a stream object.
	HRES(CoCreateInstance(__uuidof(SdpStream),NULL,CLSCTX_INPROC_SERVER,
		__uuidof(ISdpStream), (LPVOID *)&pIStream));

	BYTE* pSDPStream = pQs->lpBlob->pBlobData;
	ULONG streamSize = pQs->lpBlob->cbSize;

	DUMPHEX(pSDPStream);
	DUMPHEX(streamSize);
	DUMPHEX(pIStream);

	// Ensure that the stream is valid and is well formed.
	hres = pIStream->Validate(pSDPStream, streamSize, NULL);
	if(FAILED(hres)) {
		LOG("SdpStream validation failed: 0x%08X\n", hres);
		return CONNERR_GENERIC;
	}

	//get list of Records

	int numRecords;
	hres = pIStream->VerifySequenceOf(pSDPStream, streamSize,
		SDP_TYPE_SEQUENCE, NULL, (ULONG*)&numRecords);
	LOGBT("SdpStream verification result: 0x%08X\n", hres);
	if(FAILED(hres) || numRecords<=0) {
		LOG("SdpStream verification failed: 0x%08X, numRecords: %d\n", hres, numRecords);
		return CONNERR_GENERIC;
	}
	LOGBT("%i SDP records\n", numRecords);

	//pSdpRecords = (ISdpRecord **) CoTaskMemAlloc(sizeof(ISdpRecord*) * (numRecords));

	Array<ISdpRecord*> pSdpRecords(numRecords);
	MYASSERT(pSdpRecords != NULL, ERR_OOM);

	// Retrieve the SDP records from the stream.
	hres = pIStream->RetrieveRecords(pSDPStream, streamSize, pSdpRecords.p(), (ULONG *) &numRecords);
	if(FAILED(hres)) {
		LOG("SdpStream retrieval failed: 0x%08X\n", hres);
		return CONNERR_GENERIC;
	}

	//one Service per Record
	//Walk
	//store results in *services

	for(int i = 0; i < numRecords; i++) {
		LOGBT("SDP Walk %i\n", i);
		BtService serv;
		MySdpWalk walk(serv);
		hres = pSdpRecords[i]->Walk(&walk);
		if(FAILED(hres)) {
			LOG("SdpStream walk failed: 0x%08X\n", hres);
			return CONNERR_GENERIC;
		}
		services->push_back(serv);
	}

	return 1;
}

#else

class SdpWalkListener : public ISdpWalk {
public:
	SdpWalkListener(std::vector<MAUUID>* pUUIDs) :
		m_nRefCount(1), 
		pUUIDs(pUUIDs) {

	  }

	  HRESULT WalkNode(NodeData* pData, ULONG state) {
			static int level = 0;
			for(int i=0; i<level; i++) {
				LOGBT(" ");
			}
			LOGBT("WalkNode 0x%08X, %i. type: 0x%x sType: 0x%x\n", pData, state,
				pData->type, pData->specificType);
		  MAUUID mauuid = Bluetooth_Base_MAUUID;
		  if(pData->type == SDP_TYPE_UUID) {
				switch(pData->specificType) {
				case SDP_ST_UUID16:
				  mauuid.i[0] = pData->u.uuid16;
					break;
				case SDP_ST_UUID32:
				  mauuid.i[0] = pData->u.uuid32;
					break;
				case SDP_ST_UUID128:
				  mauuid = GUID2MAUUID(pData->u.uuid128);
					break;
				default:
					LOG("Bad SDP UUID sType: %i\n", pData->specificType);
					return FALSE;
			  }
				pUUIDs->push_back(mauuid);
			} else if(pData->type == SDP_TYPE_CONTAINER) {
				level++;
				//pData->u.container->Walk(this);
				//pData->u.container-
				ULONG nc;
				pData->u.container->GetNodeCount(&nc);
				LOGBT("%i nodes\n", nc);
				level--;
			}
		  return TRUE;
	  }

	  HRESULT WalkStream(UCHAR elementType, ULONG elementSize, UCHAR* pStream) {
			LOGBT("WalkStream 0x%02X, %i, 0x%08X\n", elementType, elementSize, pStream);
		  return TRUE;
	  }

	  HRESULT QueryInterface(REFIID riid, void **ppvInt) {
		BIG_PHAT_ERROR(ERR_FUNCTION_UNIMPLEMENTED);
		  /*
		  if (riid == IID_IHello || riid == IID_IName)
			  *ppvInt = this;
		  else
			  return E_NOINTERFACE;
		  AddRef();
		  */
		  return S_OK;
	  }

	  ULONG AddRef() {
		  return ++m_nRefCount;
	  }

	  ULONG Release() {
		  if (--m_nRefCount)
			  return m_nRefCount;
		  delete this;
		  return 0;
	  }


private:
	ULONG m_nRefCount;
	std::vector<MAUUID>* pUUIDs;
};

int MASdpEnumAttrs(LPBYTE pSDPStream, ULONG streamSize, std::vector<MAUUID>* pUUIDs) {
	//should fill serv.uuids with what we're looking for, if everything went well.
	//GLE(BluetoothSdpEnumAttributes(pSDPStream, streamSize,
	//	BtEnumAttributesCallback, pUUIDs));

	HRESULT hres;
	ISdpStream *pIStream = NULL;
	ISdpRecord **pSdpRecords = NULL;
	// Create a stream object.
	HRES(CoCreateInstance(__uuidof(SdpStream),NULL,CLSCTX_INPROC_SERVER,
		__uuidof(ISdpStream),(LPVOID *) &pIStream));

	// Ensure that the stream is valid and is well formed.
	hres = pIStream->Validate(pSDPStream,streamSize,NULL);
	if(FAILED(hres)) {
		LOG("SdpStream validation failed: 0x%08X\n", hres);
		//not good; we should return a proper MoSync error code, but the current MASdpEnumAttrs
		//interface doesn't allow it.
		return false;
	}

	SdpWalkListener walker(pUUIDs);
#if 0
	HRES(pIStream->Walk(pSDPStream, streamSize, &walker));
	//ISdpNodeContainer::CreateFromStream(
	return true;
#endif

	// Ensure that the sequence of the stream is valid and is well formed.
#if 1
	ULONG numRecords;
	hres = pIStream->VerifySequenceOf(pSDPStream,streamSize,
		SDP_TYPE_SEQUENCE,NULL,&numRecords);
	LOGBT("%i SDP records\n", numRecords);

	if (SUCCEEDED(hres) && numRecords > 0) {
		//Allocate memory for the SDP record buffer.
		pSdpRecords = (ISdpRecord **) CoTaskMemAlloc(sizeof(ISdpRecord*) * (numRecords));
		if (pSdpRecords != NULL) {
			// Retrieve the SDP records from the stream.
			hres = pIStream->RetrieveRecords(pSDPStream, 
				streamSize,pSdpRecords,&numRecords);

			if(SUCCEEDED(hres)) {
				//SdpWalkListener *walkListener = new SdpWalkListener(pUUIDs);
				for(unsigned int i = 0; i < numRecords; i++) {
					LOGBT("SDP Walk %i\n", i);
					pSdpRecords[i]->Walk(&walker);//walkListener);
				}
			}

			CoTaskMemFree(pSdpRecords);
			pSdpRecords = NULL;
			numRecords = 0;
		}
		else 
		{
			LOGBT("Bluetooth Service discovery: Out of Memory\n");
			hres = E_OUTOFMEMORY;
		}	
	} else {
		LOGBT("pIStream->VerifySequenceOf failed\n");
	}
#endif

	return SUCCEEDED(hres);
}
#endif	//0
