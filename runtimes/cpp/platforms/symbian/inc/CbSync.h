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

#ifndef __CBSYNCHRONIZER_H__
#define __CBSYNCHRONIZER_H__

#include "symbian_helpers.h"

class TSyncCallback {
public:
	typedef void (*PSyncFunction)(TAny*, TInt);
	TSyncCallback(PSyncFunction aFunc, TAny* aPtr) : iFunc(aFunc), iPtr(aPtr) {}

	void CallbackL(TInt aResult) { iFunc(iPtr, aResult); }
protected:

	PSyncFunction iFunc;
	TAny* iPtr;
};

class CCBSynchronizer : public CPublicActive {
public:
	CCBSynchronizer(const TSyncCallback& aCallback) : mCallback(aCallback) {}
	TRequestStatus* Status() { return &iStatus; }
	virtual ~CCBSynchronizer() {
		this->Cancel();
	}
protected: //CActive
	void RunL() { mCallback.CallbackL(iStatus.Int()); }
	void DoCancel() {}

	TSyncCallback mCallback;
};

template<class T> class CClassSynchronizer : public CPublicActive {
public:
	typedef void (T::* Callback)(TInt);
	CClassSynchronizer(T* that, Callback cb) : mThat(that), mCb(cb) {}
	TRequestStatus* Status() { return &iStatus; }
	virtual ~CClassSynchronizer() {
		LOG("~CClassSynchronizer\n");
		this->Cancel();
		LOG("~CClassSynchronizer done\n");
	}
protected: //CActive
	void RunL() { (mThat->*mCb)(iStatus.Int()); }
	void DoCancel() { LOG("CClassSynchronizer:DoCancel\n"); }
	
	T* mThat;
	Callback mCb;
};

#endif  //__CBSYNCHRONIZER_H__
