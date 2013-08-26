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

#include "ActiveEnder.h"

CActiveEnder::CActiveEnder() : CActive(CActive::EPriorityStandard), mLast(NULL) {
	CActiveScheduler::Add(this);
}

CActiveEnder::~CActiveEnder() {
	Cancel();
}

void CActiveEnder::move(CActiveLink* aObject) {
	aObject->iPrev = mLast;
	mLast = aObject;

	//this code causes RunL to be called later.
	if(!IsActive()) {
		iStatus = KRequestPending;
		SetActive();
		TRequestStatus* rsp = &iStatus;
		User::RequestComplete(rsp, KErrNone);
	}
}

void CActiveEnder::RunL() { clean(); }
void CActiveEnder::DoCancel() { clean(); }

void CActiveEnder::clean() {
	while(mLast != NULL) {
		CActiveLink* object = mLast;
		mLast = object->iPrev;
		delete object;
	}
}
