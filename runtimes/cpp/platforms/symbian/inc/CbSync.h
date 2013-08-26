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
