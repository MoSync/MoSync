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

#include <e32base.h>
#include <F32FILE.H>
#include <BADESCA.H>

#include <rsendasmessage.h>
#include <rsendas.h>
#include <MTCLREG.H>

class CRSendAsSender;

class MRSendSMSObserver {
public:
	virtual void MessageSentL(CRSendAsSender* aSender, TInt aError) = 0;
};

class CRSendAsSender : public CActive {
public:
	static CRSendAsSender* NewL(MRSendSMSObserver& aObserver, const TDesC& aMessage,
		const TDesC& aRecipients);
	~CRSendAsSender();
	
	TDblQueLink mLink;
protected:
	void DoCancel();
	void RunL();
private:
	CRSendAsSender(MRSendSMSObserver& aObserver);
	void ConstructL(const TDesC& aMessage, const TDesC& aRecipients);	
private:
	MRSendSMSObserver& iObserver;
	RSendAs iSendAs;
	RSendAsMessage iSendAsMessage;
};
