/* Copyright (C) 2009 Mobile Sorcery AB

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License, version 2, as published by
the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

You should have received a copy of the GNU General Public License
along with MoSync; see the file COPYING.  If not, write to the Free
Software Foundation, 59 Temple Place - Suite 330, Boston, MA
02111-1307, USA.
*/

// MoSyncServer.cpp
//
// Created: 01/09/09
// Copyright (c) Mobile Sorcery. All rights reserved.
//

#include <e32base.h>
#include <Etel3rdParty.h>

#include "MoSyncServer.h"
#include "ClientServer.h"
#include "ActiveEnder.h"
#include "Log.h"

//**********************************
//CMoSyncServer - implementations
//**********************************

/**
Constructor takes the server priority value. 

The server is an active object, and the priority value is the priority
of this active object.

It passes the priority value to the base class in the Ctor list.
By default, the session is not sharable, which is what we want here
so no second parameter is passed to the CServer2 constructor.
*/
CMoSyncServer::CMoSyncServer(CActive::TPriority aActiveObjectPriority)
: CServer2(aActiveObjectPriority)
{
}


/**
Creates a new session with the server.
*/
CSession2* CMoSyncServer::NewSessionL(const TVersion& aVersion,
	const RMessage2& /*aMessage*/) const
{
	// Check that the version is OK
	TVersion v(KMoSyncMajorVersionNumber, KMoSyncMinorVersionNumber,
		KMoSyncBuildVersionNumber);
	if (!User::QueryVersionSupported(v,aVersion))
		User::Leave(KErrNotSupported);	
	
	// CAN USE THE aMessage argument to check client's security and identity
	// can make use of this later but for now ignore. AH 4/5/05
	// the connect message is delivered via the RMessage2 object passed. 
	
	// Create the session.
	CMoSyncSession* s = new (ELeave) CMoSyncSession;
	CleanupStack::PushL(s);
	s->ConstructL();
	CleanupStack::Pop(s);
	return s;
}


/**
A utility function to panic the server.
*/
void CMoSyncServer::PanicServer(TMoSyncPanic aPanic)
{
	_LIT(KTxtServerPanic,"MoSync Server panic");
	User::Panic(KTxtServerPanic, aPanic);
}


//***********************************
//CMoSyncSession - implementations
//***********************************

CMoSyncSession::CMoSyncSession()
: mEnder(NULL), mTelephony(NULL), mPn(NULL), mPositionerOpen(false)
{
}

CMoSyncSession::~CMoSyncSession() {
	LOG("~CMoSyncSession 1\n");
	LocationStop();
	LOG("~CMoSyncSession 2\n");
	if(mTelephony != NULL)
		delete mTelephony;
	LOG("~CMoSyncSession 3\n");
	if(mEnder != NULL)
		delete mEnder;
	LOG("~CMoSyncSession 4\n");
}

void CMoSyncSession::ConstructL() {
	mEnder = new (ELeave) CActiveEnder;
}

/**
Services a client request.
*/
void CMoSyncSession::ServiceL(const RMessage2& aMessage)
{
	TRAPD(err, DispatchMessageL(aMessage));
	if(err != KErrNone)
		aMessage.Complete(err);
}

/**
Called by ServiceL()

It tests the function code and then delegates to
the appropriate function.
*/
void CMoSyncSession::DispatchMessageL(const RMessage2& aMessage)
	{
	switch (aMessage.Function())
	{
	case EMoSyncGetNetworkInfo:
		GetNetworkInfoL(aMessage);
		return;
	case EMoSyncLocationGet:
		LocationGetL(aMessage);
		return;
	case EMoSyncLocationStop:
		LocationStopL(aMessage);
		return;
		
	 //  Requests that we don't understand at all are a different matter.
	 //  This is considered a client programming error, so we panic the 
	 //  client - this also completes the message.
	default:
		PanicClient(aMessage, EBadRequest);
		return;
	}
}

/**
Panics the client
*/
void CMoSyncSession::PanicClient(const RMessage2& aMessage, TInt aPanic) const
{
	_LIT(KTxtServer,"MoSync server");
	aMessage.Panic(KTxtServer, aPanic);
}

void CMoSyncSession::LazyDelete(CActiveLink* aObject) {
	mEnder->move(aObject);
}

//**********************************
//Global functions
//**********************************

void lazyDelete(const RMessage2& aMessage, CActiveLink* aObject) {
	CMoSyncSession* session = (CMoSyncSession*)aMessage.Session();
	session->LazyDelete(aObject);
}

LOCAL_C void runServerL();

GLDEF_C TInt E32Main() // Symbian EXE file entry point
{
	__UHEAP_MARK;
	CTrapCleanup* cleanup=CTrapCleanup::New(); // get clean-up stack
	TRAPD(error, runServerL());
	__ASSERT_ALWAYS(!error, User::Panic(KMoSyncServer, error));
	delete cleanup; // destroy clean-up stack
	__UHEAP_MARKEND;
	return 0; // and return
}


LOCAL_C void runServerL()
{ 
	// Create the server, if one with this name does not already exist.
	TFindServer findServer(KMoSyncServer);
	TFullName name;

	// If the server exists already, we must shut down.
	if (findServer.Next(name) == KErrNone) {
		return;
	}

	InitLog();

	// create an active scheduler and server
	CActiveScheduler *pA = new (ELeave) CActiveScheduler;
	CMoSyncServer *pS = new (ELeave) CMoSyncServer(CActive::EPriorityStandard);

	//Install the active scheduler
	CActiveScheduler::Install(pA);
	
	LOG("CActiveScheduler Installed.\n");

	// Start the server
	TInt err = pS->Start(KMoSyncServer);
	if (err != KErrNone)
	{
		CMoSyncServer::PanicServer(ESvrStartServer);
	}
	LOG("Server Started.\n");


	// Let everyone know that we are ready to
	// deal with requests.
	RProcess::Rendezvous(KErrNone);
	LOG("RProcess Rendezvous'd.\n");

	// And start fielding requests from client(s).
	CActiveScheduler::Start();
	LOG("CActiveScheduler Started.\n");

	// Tidy up... 	
	delete pS;
	delete pA;

	LOG("Server and Scheduler deleted. Closing down...\n");

	// ...although we should never get here!
	return;
}
