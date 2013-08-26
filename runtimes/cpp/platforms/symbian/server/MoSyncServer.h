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

#ifndef MOSYNCSERVER_H
#define MOSYNCSERVER_H

#include <e32base.h>
#include <lbs.h>

#include "ClientServer.h"

class CActiveLink;
class CActiveEnder;
class CTelephony;

//**********************************
//CMoSyncServer
//**********************************
/**
Our server class - an active object - and therefore derived ultimately from CActive.
It accepts requests from client threads and forwards
them to the client session to be dealt with. It also handles the creation
of the server-side client session.
*/
class CMoSyncServer : public CServer2
{
public:
	// Creates a new session with the server; the function
	// implements the pure virtutal function
	// defined in class CServer2
	CSession2* NewSessionL(const TVersion& aVersion, const RMessage2& aMessage) const;
public:

	static void PanicServer(TMoSyncPanic aPanic);

	CMoSyncServer(CActive::TPriority aActiveObjectPriority);
	virtual ~CMoSyncServer();

	// Load autostart database and start any programs therein.
	void doAutostartL();
	void writeAutostartDbL();

	CArrayFixFlat<TInt>* mAutostart;
};

//**********************************
//CMoSyncSession
//**********************************
class CPositionNotifier;
class CNetworkInfoGetter;
class CNetworkStatusGetter;
class CNetworkStatusChangeGetter;

/**
This class represents a session with the  server.
Functions are provided to respond appropriately to client messages.
*/
class CMoSyncSession : public CSession2
{
public:
	CMoSyncSession();
	virtual ~CMoSyncSession();

	void ConstructL();

	//service request
	void ServiceL(const RMessage2& aMessage);
	void DispatchMessageL(const RMessage2& aMessage);

	void LazyDelete(CActiveLink* aObject);

	//individual service calls
	template<class T> void GetTelephonyInfoL(const RMessage2& aMessage, T*&);
	void CancelNetworkStatusChange(const RMessage2& aMessage);

	void LocationStop();

	void LocationGetL(const RMessage2& aMessage);
	void LocationStopL(const RMessage2& aMessage);

	void AutostartOnL(const RMessage2& aMessage);
	void AutostartOffL(const RMessage2& aMessage);

protected:
	// panic the client, but leave the server running
	void PanicClient(const RMessage2& aMessage,TInt aPanic) const;

private:
	CActiveEnder* mEnder;

	CTelephony* mTelephony;
	CNetworkInfoGetter* mNetworkInfo;
	CNetworkStatusGetter* mNetworkStatus;
	CNetworkStatusChangeGetter* mNetworkStatusChange;

	RPositionServer mPositionServer;
	RPositioner mPositioner;
	CPositionNotifier* mPn;	//no need to delete this in destructor
	bool mPositionerOpen;
};

#endif	//MOSYNCSERVER_H
