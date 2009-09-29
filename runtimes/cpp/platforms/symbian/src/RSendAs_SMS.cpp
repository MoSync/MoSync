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
