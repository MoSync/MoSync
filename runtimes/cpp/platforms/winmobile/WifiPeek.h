//////////////////////////////////////////////////////////////////////////
//
// CWifiPeek - helper class for Access Point listing
//
// Coded by dzolee
// http://dzolee.blogspot.com
//
//////////////////////////////////////////////////////////////////////////

#include <Windows.h>
#include <winioctl.h>
#include <NtDDNdis.h>
#include <nuiouser.h>

#if _MSC_VER > 1000
#pragma once
#endif

//basid BSSID info struct
struct BSSIDInfo
{
	BYTE BSSID[6];	//mac
	WCHAR SSID[32];
	int RSSI;
	int Channel;
	int Infastructure;
	int Auth;
};

class CWifiPeek
{
public:
	CWifiPeek();
	~CWifiPeek();

	bool GetAdapters(LPWSTR pDest, DWORD &dwBufSizeBytes);
	bool RefreshBSSIDs(LPWSTR pAdapter);
	bool GetBBSIDs(LPWSTR pAdapter, struct BSSIDInfo *pDest, DWORD &dwBufSizeBytes, DWORD &dwReturnedItems);	

private:
	bool OpenDriver();
	void CloseDriver();

	CRITICAL_SECTION m_Lock;
	HANDLE m_hNDUIO;
};

