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
};


//**********************************
//CMoSyncSession
//**********************************
class CPositionNotifier;

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
	void GetNetworkInfoL(const RMessage2& aMessage);
	
	void LocationStop();
	
	void LocationGetL(const RMessage2& aMessage);
	void LocationStopL(const RMessage2& aMessage);

protected:
	// panic the client, but leave the server running
	void PanicClient(const RMessage2& aMessage,TInt aPanic) const;

private:
	CActiveEnder* mEnder;
	
	CTelephony* mTelephony;
	
	RPositionServer mPositionServer;
	RPositioner mPositioner;
	CPositionNotifier* mPn;	//no need to delete this in destructor
	bool mPositionerOpen;
};

#endif	//MOSYNCSERVER_H
