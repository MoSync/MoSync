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

#ifndef __SERIES60_3X__	//2nd edition
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

#include <helpers/CPP_IX_CONNSERVER.H>

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
#else	//Series 60, 2nd Ed.
#include "RCondVar.h"
#include "CSmsHandler.h"
#endif

class CSocket;
class CHttpConnection;
class CServerSocket;

class CConnection : public CBase {
protected:
	CConnection() : state(0), connErr(0) {}
public:
	virtual ~CConnection() { LOGST("~CConnection"); }

	virtual void RecvOneOrMoreL(TDes8& aDes, CPublicActive& op) = 0;

	//returns false if socket is write-protected
	virtual bool Write(const TDesC8& aDesc, CPublicActive& op) = 0;

	virtual void CancelAll() = 0;
	
	virtual void GetAddr(MAConnAddr* addr) = 0;

	virtual CHttpConnection* http() { return NULL; }
	virtual CServerSocket* server() { return NULL; }

	int state;
	int connErr;
};

#ifdef SUPPORT_MOSYNC_SERVER
class RMoSyncServerSession : public RSessionBase {
public:
	RMoSyncServerSession() : mPositionPckg(mPosition) {}

	int Connect();
	TVersion Version(void) const;

	int GetNetworkInfo(TDes8& aPckg);
	
	void LocationGet(TRequestStatus& aStatus);
	int LocationStop();
	
	const TPosition& Position() const { return mPosition; }
private:
	TPosition mPosition;
	TPckg<TPosition> mPositionPckg;
};

#define MA_PROF_SUPPORT_LOCATIONAPI	//for custom events
#endif	//SUPPORT_MOSYNC_SERVER

class CDirScrAccEng;
class CAppView;
class PimItem;

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

#if defined(__SERIES60_3X__)
#define MMF
#endif

#endif	//_PLATFORM_H_
