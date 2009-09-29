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
