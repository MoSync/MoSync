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
