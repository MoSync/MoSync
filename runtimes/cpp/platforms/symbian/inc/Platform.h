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

#ifndef _PLATFORM_H_
#define _PLATFORM_H_

#include <e32base.h>
#include <in_sock.h>
#include <MdaAudioOutputStream.h>
#include <Mda\Common\Audio.h>
#include <mmf/server/MmfCodec.h>
#include <bt_sock.h>
#include <btsdp.h>
#include <vibractrl.h>
#include <aknkeylock.h>
#include <MdaAudioSamplePlayer.h>
#include <MdaAudioSampleEditor.h>
#include <mmf/common/mmfcontroller.h>
#include <ecam.h>

#include "config_platform.h"
#define SYMBIAN 1

#ifdef __SERIES60_3X__
#define WLAN
#else	//2nd edition
#undef WLAN
#undef CALL
#undef CELLID
#undef SUPPORT_MOSYNC_SERVER
#endif

#include <helpers/cpp_defs.h>

#ifndef DONT_SUPPORT_RELOAD
#define SUPPORT_RELOAD
#endif


#ifdef GUIDO

#define CALL

#ifdef __SERIES60_3X__
//#include <babile/badesca.h>
//#define BABILE
#ifdef BABILE2
#include <bb_def.h>
#include <bb_types.h>
#include <bb_mem.h>
#include <i_babile.h>
#include <io_babile.h>
#include <if_babile.h>
#else
#define ARM_LE_SYMBIAN
#include <bb_def.h>
#include <bb_types.h>
#include <bb_mem.h>
#include <i_babile.h>
#include <io_babile.h>
#include <if_babile.h>
#endif
#else	//2nd edition
#define ARM_LE_SYMBIAN
#define BABILE
#include <i_babile.h>
#include <io_babile.h>
#include <if_babile.h>
#endif	//__SERIES60_3X__
#endif	//GUIDO


#include <helpers/smartie.h>
#include <helpers/types.h>

#ifdef WLAN
#include <helpers/CPP_IX_WLAN.H>
#include <wlanmgmtclient.h>
#include <wlanscaninfo.h>
#endif

#include <helpers/CPP_IX_CELLID.h>

#include <VideoPlayer.h>
#include <helpers/CPP_IX_STREAMING.H>

#include <lbs.h>

#include <helpers/CPP_IX_AUDIOBUFFER.h>

#include <helpers/CPP_IX_PIM.h>
#include <cntdb.h>

//TODO: add support for s60v2fp3.
#if defined(__SERIES60_3X__)
#define TELEPHONY
#endif

#if (defined(CELL) || defined(CALL)) && !defined(TELEPHONY)
#error No telephony!
#endif


#ifdef TELEPHONY
#include <Etel3rdParty.h>
#endif

#include "CbSync.h"
#include "hashmap/hashmap.h"

#ifdef __SERIES60_3X__
#include "RSendAs_SMS.h"
#include "pim.h"
#else	//Series 60, 2nd Ed.
#include "RCondVar.h"
#include "CSmsHandler.h"
#endif

// Bluetooth
#ifdef __SERIES60_3X__
#include <centralrepository.h>
#else	//Series 60, 2nd Ed.
#include <settinginfo.h>
#endif

class CBaseSocket;
class CSocket;
class CHttpConnection;
class CServerSocket;
class CMySecureSocket;

class CConnection : public CBase {
protected:
	CConnection() : state(0), connErr(0), errorOverride(0) {}
public:
	virtual ~CConnection() { LOGST("~CConnection"); }

	virtual void RecvOneOrMoreL(TDes8& aDes, CPublicActive& op) = 0;

	//returns false if socket is write-protected
	virtual bool Write(const TDesC8& aDesc, CPublicActive& op) = 0;

	virtual void CancelAll() = 0;

	virtual void GetAddr(MAConnAddr* addr) = 0;

	virtual CHttpConnection* http() { return NULL; }
	virtual CServerSocket* server() { return NULL; }
	virtual bool isBluetooth() const { return false; }

	int state;
	int connErr;
	int errorOverride;
};

#ifdef SUPPORT_MOSYNC_SERVER
class RMoSyncServerSession : public RSessionBase {
public:
	RMoSyncServerSession() : mPositionPckg(mPosition), mConnected(false) {}

	int Connect();
	TVersion Version(void) const;

	int GetNetworkInfo(TDes8& aPckg);

	void LocationGet(TRequestStatus& aStatus);
	int LocationStop();

	const TPosition& Position() const { return mPosition; }

	int AutostartOn();
	int AutostartOff();

	int GetNetworkStatus(CTelephony::TNetworkRegistrationV1&);
	void GetNetworkStatusChange(CTelephony::TNetworkRegistrationV1Pckg&, TRequestStatus&);
	void CancelNetworkStatusChange();
private:
	TPosition mPosition;
	TPckg<TPosition> mPositionPckg;
	bool mConnected;
};
#endif	//SUPPORT_MOSYNC_SERVER

#if defined(SUPPORT_MOSYNC_SERVER) || defined(__S60_50__)
#define MA_PROF_SUPPORT_LOCATIONAPI	//for custom events
#endif

class CDirScrAccEng;
class CAppView;

class FileList {
public:
	virtual ~FileList() {}
	virtual int next(char* nameBuf, int bufSize) = 0;
};

namespace Core {
	class VMCore;
}
using namespace Core;

#define BLUETOOTH(func) SBT##func

#if defined(__SERIES60_3X__)// && !defined(__S60_50__)
#define MMF
#endif

#endif	//_PLATFORM_H_
