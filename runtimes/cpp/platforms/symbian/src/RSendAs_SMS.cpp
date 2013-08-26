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

#include <smut.h>

#include "RSendAs_SMS.h"


CRSendAsSender* CRSendAsSender::NewL(MRSendSMSObserver& aObserver, const TDesC& aMessage,
																		 const TDesC& aRecipients)
{
	CRSendAsSender* self = new(ELeave)CRSendAsSender(aObserver);
	self->ConstructL(aMessage,aRecipients);
	return self;
}

CRSendAsSender::CRSendAsSender(MRSendSMSObserver& aObserver) : CActive(EPriorityStandard),
iObserver(aObserver)
{
}

CRSendAsSender::~CRSendAsSender()
{
	Cancel();
	iSendAsMessage.Close();
	iSendAs.Close();
}

void CRSendAsSender::ConstructL(const TDesC& aMessage,const TDesC& aRecipients)
{
	CActiveScheduler::Add(this);

	User::LeaveIfError(iSendAs.Connect());
	iSendAsMessage.CreateL(iSendAs, KUidMsgTypeSMS);

	iSendAsMessage.AddRecipientL(aRecipients, RSendAsMessage::ESendAsRecipientTo);
	iSendAsMessage.SetBodyTextL(aMessage);

	iSendAsMessage.SendMessage(iStatus);
	SetActive();
}

void CRSendAsSender::DoCancel()
{
	iSendAsMessage.Cancel();
	iObserver.MessageSentL(this, KErrCancel);
}

void CRSendAsSender::RunL()
{	
	iObserver.MessageSentL(this, iStatus.Int());
}
