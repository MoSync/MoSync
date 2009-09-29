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

#include "WifiPeek.h"

//////////////////////////////////////////////////////////////////////////
CWifiPeek::CWifiPeek()
{
	m_hNDUIO=NULL;
	InitializeCriticalSection(&m_Lock);
	OpenDriver();
}

//////////////////////////////////////////////////////////////////////////
CWifiPeek::~CWifiPeek()
{
	if(m_hNDUIO != NULL)
	{
		CloseDriver();
	}
	DeleteCriticalSection(&m_Lock);
}

//////////////////////////////////////////////////////////////////////////
//get list of usable adapters
bool CWifiPeek::GetAdapters(LPWSTR pDest, DWORD &dwBufSizeBytes)
{
HANDLE hFile;
BYTE Buffer[2048];
void *pvBuf;
WCHAR TmpBuf[1024];
WCHAR *pszOut;
DWORD dwRet;
bool retval;

	if(pDest == NULL || dwBufSizeBytes < sizeof(WCHAR))
	{
		dwBufSizeBytes=0;
		return false;
	}

	EnterCriticalSection(&m_Lock);
	retval=false;
	//open NDIS driver
	hFile=CreateFile(L"NDS0:", GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, INVALID_HANDLE_VALUE);
	if(hFile != INVALID_HANDLE_VALUE)
	{
		pvBuf=(void *)(&Buffer[0]);
		dwRet=sizeof(Buffer);
		if(DeviceIoControl(hFile, IOCTL_NDIS_GET_ADAPTER_NAMES, NULL, 0, pvBuf, sizeof(Buffer), &dwRet, NULL))
		{
			//adapter list ok.
			LPWSTR pszStr;
			dwRet=0;
			pszOut=TmpBuf;

			//no string classes used, so no MFC or ATL dependency.
			for(pszStr=(LPWSTR)pvBuf; *pszStr; pszStr+=wcslen(pszStr)+1)
			{
				//check if adapter name is ok, skip infrared, gprs, ActiveSync etc.
				if(wcsicmp(pszStr, L"ASYNCMAC1") && \
					wcsicmp(pszStr, L"IRSIR1") && \
					wcsicmp(pszStr, L"L2TP1") && \
					wcsicmp(pszStr, L"PPTP1") && \
					wcsicmp(pszStr, L"RNDISFN1") && \
					wcsicmp(pszStr, L"WWAN1") && \
					wcsicmp(pszStr, L"XSC1_IRDA1"))
				{						
					//not the first adapter?
					if(pszOut != TmpBuf)
					{
						//append separator
						wcscat(pszOut, L",");
						pszOut++;
						dwRet+=sizeof(WCHAR);
					}
					wcscpy(pszOut, pszStr);
					pszOut+=wcslen(pszStr);
					dwRet+=sizeof(WCHAR) * wcslen(pszStr);
				}
			}
			//return required buffer size anyway
			dwBufSizeBytes=dwRet;

			//if buffer large enough, copy adapter names
			if(dwBufSizeBytes >= dwRet)
			{
				wcsncpy(pDest, TmpBuf, dwRet);
				retval=true;
			}
			//otherwise will return false
			//..
		}
		CloseHandle(hFile);
	}
	LeaveCriticalSection(&m_Lock);
	return retval;
}

//////////////////////////////////////////////////////////////////////////
//open the NDISUIO driver
bool CWifiPeek::OpenDriver()
{
HANDLE hDev;

	//already opened?
	if(m_hNDUIO != NULL)
	{
		return true;
	}

	hDev=CreateFile(NDISUIO_DEVICE_NAME, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, INVALID_HANDLE_VALUE);
	if(hDev == INVALID_HANDLE_VALUE)
	{
		return false;
	}
	else
	{
		m_hNDUIO=hDev;
		return true;
	}
}

//////////////////////////////////////////////////////////////////////////
//at the end, close NDISUIO handle
void CWifiPeek::CloseDriver()
{
	CloseHandle(m_hNDUIO);
	m_hNDUIO=NULL;
}

//////////////////////////////////////////////////////////////////////////
//initiate station scanning
bool CWifiPeek::RefreshBSSIDs(LPWSTR pAdapter)
{
NDISUIO_SET_OID nso;
DWORD dwBytesRet;
bool retval;

	EnterCriticalSection(&m_Lock);
	nso.ptcDeviceName = pAdapter;
	nso.Oid = OID_802_11_BSSID_LIST_SCAN;

	dwBytesRet=0;
	if(!DeviceIoControl(m_hNDUIO, IOCTL_NDISUIO_SET_OID_VALUE, (void *)&nso, sizeof(NDISUIO_SET_OID), NULL, 0, &dwBytesRet, NULL))
	{
		retval=false;
	}
	else
	{
		retval=true;
	}
	LeaveCriticalSection(&m_Lock);
	return retval;
}

//////////////////////////////////////////////////////////////////////////
//get a list of currently visible stations
bool CWifiPeek::GetBBSIDs(LPWSTR pAdapter, struct BSSIDInfo *pDest, DWORD &dwBufSizeBytes, DWORD &dwReturnedItems)
{
PNDISUIO_QUERY_OID pNQO;
DWORD dwBytesRet;
BYTE Buffer[8192], *pByte;
PNDIS_802_11_BSSID_LIST pList;
size_t i;

	if(pDest == NULL)
	{
		dwReturnedItems=0;
		return false;
	}

	EnterCriticalSection(&m_Lock);	
	pNQO=(PNDISUIO_QUERY_OID)Buffer;

	pNQO->ptcDeviceName = pAdapter;
	pNQO->Oid=OID_802_11_BSSID_LIST;

	// Run query
	dwBytesRet = 0;
	if(!DeviceIoControl(m_hNDUIO, IOCTL_NDISUIO_QUERY_OID_VALUE, (void *)pNQO, 8192, (void *)pNQO, 8192, &dwBytesRet, NULL))
	{
		HRESULT hr = GetLastError();
		LeaveCriticalSection(&m_Lock);
		return false;
	}

	pList=(PNDIS_802_11_BSSID_LIST)&pNQO->Data;
	dwReturnedItems=pList->NumberOfItems;

	//first item in array
	PNDIS_WLAN_BSSID pItem=pList->Bssid;

	for(i=0; i<pList->NumberOfItems; i++)
	{
		memcpy(pDest[i].BSSID, pItem->MacAddress, sizeof(pItem->MacAddress));
		mbstowcs(pDest[i].SSID, (char *)pItem->Ssid.Ssid, sizeof(pItem->Ssid.Ssid));
		pDest[i].RSSI=pItem->Rssi;
		if(pItem->Configuration.DSConfig > 14)
		{
			pDest[i].Channel=(pItem->Configuration.DSConfig - 2407000) / 5000;
		}
		else
		{
			pDest[i].Channel=pItem->Configuration.DSConfig;
		}
		pDest[i].Auth=pItem->Privacy;
		pDest[i].Infastructure=pItem->InfrastructureMode;

		//some pointer magic...actually pItem->Length was not sizeof(NDIS_WLAN_BSSID)
		//so use returned length
		pByte=(BYTE *)pItem;
		pByte+=pItem->Length;
		pItem=(PNDIS_WLAN_BSSID)pByte;
	}//for

	LeaveCriticalSection(&m_Lock);
	return true;
}
