/* Copyright (c) 2003, Nokia. All rights reserved */


#ifndef __CSMSHANDLER_H__
#define __CSMSHANDLER_H__

#ifndef __SERIES60_3X__	//Series 60, 2nd Ed.

//  INCLUDES
#include <e32base.h>
#include <msvapi.h>
#include <mtuireg.h>

// CONSTANTS
const TInt KBfrLength = 20;
const TInt EMaxTelephoneNumberLength = 15;		// max telephone number length
const TInt EMaxMessageLength = 160;				// max SMS message length (standard)

// FORWARD DECLARATIONS
class CClientMtmRegistry;
class CSmsClientMtm;

class MSmsObserver {
public:
	virtual void MovedToOutbox() = 0;
	virtual void ServerSessionEstablished() = 0;
	virtual void ServerSessionError() = 0;
	virtual void ServerDown(MMsvSessionObserver::TMsvSessionEvent aEvent) = 0;
	virtual void NoServiceCenter() = 0;
	virtual void MessageReceived() = 0;
	virtual void NoMoreUnread() = 0;
};

// CLASS DECLARATION
/**
* CSmsHandler application engine class.
* Takes care of sending and receiveing SMS messages using the SMS client MTM.
* Interacts with the application UI class.
*/
class CSmsHandler : public CActive, public MMsvSessionObserver
{
public: // Constructors and destructor

	/**
	* NewL.
	* Two-phased constructor.
	* @param aSmsAppUi Pointer to AppUi instance.
	* @return Pointer to the created instance of CSmsHandler.
	*/
	static CSmsHandler* NewL( MSmsObserver* aSmsObserver );

	/**
	* NewLC.
	* Two-phased constructor.
	* @param aSmsAppUi Pointer to AppUi instance.
	* @return Pointer to the created instance of CSmsHandler.
	*/
	static CSmsHandler* NewLC( MSmsObserver* aSmsObserver );

	/**
	* ~CSmsHandler
	* Destructor.
	*/
	virtual ~CSmsHandler();

public: // New functions

	/**
	* SendL.
	* Starts the process of creating and sending an SMS message.
	* @param aRecipientNumber The number of the recipent.
	* @param aMessageText The message text.
	* @return ETrue if successful, EFalse if not.
	*/
	TBool SendL( const char* aRecipientNumber,
		const char* aMessageText );

	/**
	* ViewL.
	* Displays a received SMS message.
	*/
	void ViewL();

public: // Functions from base classes

	/**
	* From MMsvSessionObserver, HandleSessionEventL.
	* Handles notifications of events from the Message Server.
	* @param aEvent The event that has taken place
	* @param aArg1 Event type-specific argument value
	* @param aArg2 Event type-specific argument value
	* @param aArg3 Event type-specific argument value
	*/
	void HandleSessionEventL( TMsvSessionEvent aEvent, TAny* aArg1,
		TAny* aArg2, TAny* aArg3 );

protected: // Functions from base classes

	/**
	* From CActive, DoCancel.
	* Cancels any outstanding requests.
	*/
	void DoCancel();

	/**
	* From CActive, RunL.
	* Handles an active object’s request completion event.
	*/
	void RunL();

private: // Constructors

	/**
	* CSmsHandler.
	* C++ default constructor.
	* @param aSmsAppUi Pointer to AppUi instance.
	*/
	CSmsHandler( MSmsObserver* aSmsObserver );

	/**
	* ConstructL.
	* 2nd phase constructor.
	*/
	void ConstructL();

private: // New functions

	/**
	* AccessMtmL.
	* Access the MTM Registry and create an SMS specific Client MTM instance.
	*/
	void AccessMtmL();

	/**
	* CreateMsgL.
	* Create an SMS message.
	* @return ETrue if successful, EFalse is unsuccessful.
	*/
	TBool CreateMsgL();

	/**
	* ScheduleL.
	* Schedule an SMS message for sending.
	*/
	void ScheduleL();

	/**
	* MessageReceivedL.
	* Handles a received SMS message.
	* @param aEntryId The message server id of the received message.
	*/
	void MessageReceivedL( TMsvId aEntryId );

	/**
	* ValidateL.
	* Validate an SMS message.
	* @return ETrue if successful, EFalse is unsuccessful.
	*/
	TBool ValidateL();

private: // Enumeration

	/**
	* TState, enumeration for the state of the handler, used by RunL().
	*/
	enum TState
	{
		EWaitingForMoving = 1,
		EWaitingForScheduling
	};

private: // Data

	/**
	* iState, the state of the handler.
	*/
	TState iState;

	/**
	* iSession, the contact database.
	* Owned by CSmsHandler object.
	*/
	CMsvSession* iSession;

	/**
	* iMtmRegistry, client MTM registry.
	* Owned by CSmsHandler object.
	*/
	CClientMtmRegistry* iMtmRegistry;

	/**
	* iSmsMtm, SMS specific Client MTM.
	* Owned by CSmsHandler object.
	*/
	CSmsClientMtm* iSmsMtm;

	/**
	* iOperation, the current message server operation.
	* Owned by CSmsHandler object.
	*/
	CMsvOperation* iOperation;

	/**
	* iRecipientNumber, telephone number of the recipient.
	*/
	TBuf<EMaxTelephoneNumberLength> iRecipientNumber;

	/**
	* iMessageText, SMS message text.
	*/
	TBuf<EMaxMessageLength> iMessageText;

	/**
	* iSmsObserver, observer of CSmsHandler object
	* Not owned by CSmsHandler object.
	*/
	MSmsObserver* iSmsObserver;

	/**
	* iMtmUiRegistry, User Interface MTM Registry.
	* Owned by CSmsHandler object.
	*/
	CMtmUiRegistry* iMtmUiRegistry;

	/**
	* iSelection, entry selection to hold received messages.
	* Owned by CSmsHandler object.
	*/
	CMsvEntrySelection* iSelection;

	/**
	* iNextUnread, index of the next unread message in iSelection.
	*/
	TInt iNextUnread;
};

#endif	//__SERIES60_3X__

#endif // __CSMSHANDLER_H__
