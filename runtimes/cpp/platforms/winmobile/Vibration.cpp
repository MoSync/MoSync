#include "Vibration.h"	
#include <helpers/cpp_defs.h>
#include <windows.h>

#ifdef WIN32_PLATFORM_WFSP
#include <vibrate.h>
#endif

#include <ceconfig.h>

#include "config_platform.h"
#include <helpers/helpers.h>
#include <helpers/CriticalSection.h>
using namespace MoSyncError;

#ifndef VibrateStop
HRESULT VibrateStop() {
	return S_FALSE;
}
#endif

/****** hack for nled on pocket pc:s using smartphone runtime (htc touch for instance). ***********/
//#include <NLed.h> 
const int NLED_COUNT_INFO_ID = 0;
const int NLED_SETTINGS_INFO_ID = 2;
struct NLED_SETTINGS_INFO {
  UINT LedNum;
  INT OffOnBlink;
  LONG TotalCycleTime;
  LONG OnTime;
  LONG OffTime;
  INT MetaCycleOn;
  INT MetaCycleOff; 
};
struct NLED_COUNT_INFO {
  UINT cLeds; 
};
extern "C" { BOOL NLedGetDeviceInfo(INT nID, PVOID pOutput); BOOL NLedSetDevice(INT nID, PVOID pOutput); } 
/*************************************************************************************************/

// only used on Pocket PC devices 
#define VIB_MODE_PPC 0
#define VIB_MODE_SP 1
	int vibMode;
	int GetLedCount() {
		NLED_COUNT_INFO nci; 
		int wCount = 0; 
		if(NLedGetDeviceInfo(NLED_COUNT_INFO_ID, (PVOID) &nci)) 
			wCount = (int) nci.cLeds; 
		return wCount; 
	} 

	HRESULT SetLedStatus(int wLed, int wStatus) { 
		NLED_SETTINGS_INFO nsi; 
		nsi.LedNum = (INT) GetLedCount()-1; 
		nsi.OffOnBlink = (INT) wStatus; 
		BOOL r = NLedSetDevice(NLED_SETTINGS_INFO_ID, &nsi); 
		if(r == TRUE) return S_OK;
		else return S_FALSE;
	} 

namespace Base {
	CRITICAL_SECTION vibrationCS;
	UINT_PTR vibrationId = NULL;
	extern HWND g_hwndMain;
	
	VOID CALLBACK VibrationStopCallback(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime) {
		CriticalSectionHandler cs(&vibrationCS);
		if(vibMode == VIB_MODE_PPC)	
			SetLedStatus(0, 0);
		else
			VibrateStop();
		vibrationId = NULL;
	}

	int VibrationStop() {
#ifdef WIN32_PLATFORM_WFSP
		if(vibrationId != NULL) {
			GLE(KillTimer(g_hwndMain, vibrationId));
			vibrationId = NULL;	
			if(vibMode == VIB_MODE_PPC)
				return SetLedStatus(0, 0);
			else 
				return VibrateStop() == S_OK;
		}
		return TRUE;
#else 
		return IOCTL_UNAVAILABLE;
#endif
	}

	int VibrationStart(int ms) 
	{
#ifdef WIN32_PLATFORM_WFSP
		CriticalSectionHandler cs(&vibrationCS);
		vibMode = VIB_MODE_SP;
		int ret = 0;
		// hack to support vibration using smartphone runtime on pocket pc phones (htc touch)
		if((ret=VibrateGetDeviceCaps(VDC_AMPLITUDE))<=0) {
			vibMode = VIB_MODE_PPC;
		}
		if(ms==0) {	
			return VibrationStop();
		} else {
			VibrationStop();
			
			HRESULT res;

			if(vibMode == VIB_MODE_PPC)
				res = SetLedStatus(0, 1);
			else
				res = Vibrate(0, NULL, 0, INFINITE);

			if(res == S_OK) {
				GLE(vibrationId=SetTimer(g_hwndMain, 1, ms, VibrationStopCallback));
				return 1;
			} else {
				return 0;
			}
		}
#else
		return IOCTL_UNAVAILABLE;
#endif
	}

} // namespace Base