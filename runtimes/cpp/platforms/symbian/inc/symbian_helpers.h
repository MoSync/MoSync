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

#ifndef __SYMBIAN_HELPERS_H__
#define __SYMBIAN_HELPERS_H__

#include <EIKENV.H>
#include <stringpool.h>

#include "MoSync.pan"
#include "config_platform.h"

#include <helpers/helpers.h>
#include <helpers/smartie.h>
#include <helpers/cpp_defs.h>

using namespace MoSyncError;

_LIT(KMoSync, "MoSync");

#if 0//defined(PHONE_RELEASE) && defined(GUIDO)
#error unsupported package!
#endif

#ifdef __SERIES60_3X__
_LIT8(KMAStorePath8, "C:\\Data\\MAStore\\");
_LIT16(KMAStorePath16, "C:\\Data\\MAStore\\");
#else  //Series 60, 2nd Ed.
_LIT8(KMAStorePath8, "C:\\MAStore\\");
_LIT16(KMAStorePath16, "C:\\MAStore\\");
#endif  //__SERIES60_3X__

//***************************************************************************
//Debugging
//***************************************************************************

void AppendToFile(const TDesC& aFileName, const TDesC8& aText);

void LogAvailableMemory();

void __declspec(noreturn) ShowAknErrorNoteThenExitL(const TDesC& text);

void __declspec(noreturn) ShowErrorCodeThenExitL(TInt aError);

void PanicIfError(TInt code);

#ifdef PUBLIC_DEBUG
void writePanicReport(int rt, int code);
void writePanicReport(int rt, int code, const TDesC& text);
#endif

const char* FileNameFromPath(const char* path);

namespace Base {
	void StopEverything();
}

//***************************************************************************
//CBufFlat
//***************************************************************************

CBufFlat* NewCBufFlatL(TInt size);

inline TPtr8 CBufFlatPtr(CBufFlat* buf) {
	return TPtr8((byte*)buf->Ptr(0).Ptr(), buf->Size(), buf->Capacity());
}

//***************************************************************************
//Resources and dumping
//***************************************************************************

CFbsBitmap* LoadOwnEncodedImageL(const TPtrC &aFilename);
TSize operator/(TSize a, TInt b);
//void DoErrorL(TInt aMessageResource, TInt aCode = ESPGPCodelessError);
//void DoErrorL(const TDesC& aMessage);
void DumpToFileL(const TDesC& aFileName, const TDesC8& aData);
TDesC* LoadResourceStringLC(TInt aResourceId);
HBufC8* LoadFileLC(const TDesC& aFileName);

class CFbsBitmap;
class TAlphaBitmap {
public:
	TAlphaBitmap(CFbsBitmap* aColor, CFbsBitmap* aAlpha) :
		mColor(aColor), mAlpha(aAlpha), mDevice(NULL), mClipRect(aColor->SizeInPixels()) {}
	~TAlphaBitmap() {
		if(mColor)
			delete mColor;
		if(mAlpha)
			delete mAlpha;
		if(mDevice)
			delete mDevice;
	}
	CFbsBitmap* Color() { return mColor; }
	CFbsBitmap* Alpha() { return mAlpha; }
	CFbsBitmapDevice* Device();
	const TRect& ClipRect() { return mClipRect; }
	void SetClipRect(const TRect& aRect);
private:
	TAlphaBitmap(const TAlphaBitmap&);
	TAlphaBitmap& operator=(const TAlphaBitmap&);

	CFbsBitmap* mColor;
	CFbsBitmap* mAlpha;
	CFbsBitmapDevice* mDevice;
	TRect mClipRect;
};

//Returns a RES code. Stores the address of a new TAlphaBitmap in *ptab.
int LoadEncodedImageWAlphaL(const TDesC8& aEncodedData, TAlphaBitmap** ptab);

//#define DUMPINT(i) { _LIT8(KTemp, #i); DumpInt(KTemp, i); }
//#define DUMPINT64(i) { _LIT8(KTemp, #i); DumpInt64(KTemp, i); }
//#define DUMPHEX(i) { _LIT8(KTemp, #i); DumpHex(KTemp, i); }
void DumpInt(const TDesC8& str, TInt i);
void DumpInt64(const TDesC8& str, TInt64 i);
void DumpHex(const TDesC8& str, TUint h);

//***************************************************************************
//Error management and logging macros
//***************************************************************************

#define KLeaveMoSyncExit 1002
#define KLeaveMoSyncReload 1001

#define IS_SYMBIAN_ERROR(code) ((code) < 0)


#define LHEL_BASE(status_code, end) {\
	int lhel = (status_code); if(IS_SYMBIAN_ERROR(lhel)) { LOGT("Error");\
	LOG("In %s on line %d: %d\n", FileNameFromPath(__FILE__), __LINE__, lhel);\
	end; } }

#define LHEL(status_code) LHEL_BASE(status_code, ShowErrorCodeThenExitL(lhel))

#define LHEL_OOM(status_code) LHEL_BASE(status_code, return RES_OUT_OF_MEMORY)

/*#define LPANIC(panic_code) { LogTimeL(_L8("Panic")); TBuf8<256> buffer;\
buffer.Format(_L8("In %s on line %d: Panic(%s);\n"), __FILE__, __LINE__,\
#panic_code); Log(buffer); User::Panic(KMoSync, panic_code); }*/

#define LTRAP_BASE(statement, end) { TRAPD(_err, statement); LHEL_BASE(_err, end) }

#define LTRAP(statement) { TRAPD(_err, statement); LHEL(_err) }
#define RVTRAP(statement) LTRAP_BASE(statement, return)
#define ITRAP(statement) LTRAP_BASE(statement, )

//assert that a op b
#define ASSERT_OP(a, op, b, errorCode) { if(!((a) op (b))) {\
	LOGT("Assert"); LOG("In " __FILE__ " on line %d: %d %s %d\n",\
	__LINE__, a, #op, b); MoSyncErrorExit(errorCode); } }

//assert that a != b
#define ASSERT_NEQ(a, b, errorCode) ASSERT_OP(a, !=, b, errorCode)

#define TEST_SYMBIAN(a) { int test_s = (a); if(IS_SYMBIAN_ERROR(test_s)) FAIL_VAL(test_s) }

#define SYMTESTFAIL(a) { int test_s = (a); if(IS_SYMBIAN_ERROR(test_s)) {\
	LOG_VAL(test_s) return test_s; } }

#define LOG_FAILURE(a) { LOG("Failure %i ", a); IN_FILE_ON_LINE; }

// Test Symbian No Return
#define TSNR(a) { int test_s = (a); if(IS_SYMBIAN_ERROR(test_s)) LOG_FAILURE(test_s); }

#define DUMP_RECT(name) LOG("%s %i,%i, %i,%i\n", #name,\
	name.iTl.iX, name.iTl.iY,\
	name.iBr.iX, name.iBr.iY)

//***************************************************************************
//String macros
//***************************************************************************
#define SSTREQ(base, lit) (base.Left(lit().Length()).Compare(lit) == 0)

//***************************************************************************
//Standard functions
//***************************************************************************

inline void DebugMarkStart() {
	//User::__DbgMarkStart(false);
}
inline void DebugMarkEnd() {
	//User::__DbgMarkEnd(false, 0);
}

#ifndef __SERIES60_3X__
inline void* memset(void* dst, int c, uint len) {
	Mem::Fill(dst, len, (TChar)c);
	return dst;
}
#endif

int unixTime(const TTime& tt);

// returns 0 on success.
int _mkdir(const char* name);
int _rmdir(const char* name);
int remove(const char* name);

//***************************************************************************
//Text conversion to/from unicode
//***************************************************************************

inline HBufC16* CreateHBufC16FromDesC8LC(const TDesC8& desc8) {
	HBufC16* self = HBufC16::NewLC(desc8.Length());
	self->Des().Copy(desc8);
	return self;
}

inline HBufC16* CreateHBufC16FromDesC8L(const TDesC8& desc8) {
	HBufC16* self = HBufC16::NewL(desc8.Length());
	self->Des().Copy(desc8);
	return self;
}

inline HBufC16* CreateHBufC16FromCStringLC(const char* str) {
	TPtrC8 ptrc8(CBP str);
	return CreateHBufC16FromDesC8LC(ptrc8);
}

inline HBufC16* CreateHBufC16FromCStringL(const char* str) {
	TPtrC8 ptrc8(CBP str);
	return CreateHBufC16FromDesC8L(ptrc8);
}

inline HBufC8* CreateHBufC8FromDesC16L(const TDesC16& desc16) {
	HBufC8* self = HBufC8::NewL(desc16.Length());
	self->Des().Copy(desc16);
	return self;
}

inline HBufC8* CreateHBufC8FromDesC16LC(const TDesC16& desc16) {
	HBufC8* self = HBufC8::NewLC(desc16.Length());
	self->Des().Copy(desc16);
	return self;
}

inline HBufC8* CreateHBufC8FromDesC8L(const TDesC8& desc8) {
	HBufC8* self = HBufC8::NewL(desc8.Length());
	self->Des().Copy(desc8);
	return self;
}

inline HBufC8* CreateHBufC8FromDesC8LC(const TDesC8& desc8) {
	HBufC8* self = HBufC8::NewLC(desc8.Length());
	self->Des().Copy(desc8);
	return self;
}

inline HBufC8* CreateHBufC8FromCStringL(const char* str) {
	TPtrC8 ptrc8(CBP str);
	return CreateHBufC8FromDesC8L(ptrc8);
}

inline HBufC8* CreateHBufC8FromCStringLC(const char* str) {
	TPtrC8 ptrc8(CBP str);
	return CreateHBufC8FromDesC8LC(ptrc8);
}

inline void Append(TDes16& ptr, const TDesC8& desc) {
	int off = ptr.Length();
	ptr.SetLength(ptr.Length() + desc.Length());
	for(int i=0; i<desc.Length(); i++) {
		ptr[off++] = desc[i];
	}
}

inline void Append(TDes8& ptr, const TDesC16& desc) {
	int off = ptr.Length();
	ptr.SetLength(ptr.Length() + desc.Length());
	for(int i=0; i<desc.Length(); i++) {
		ptr[off++] = (byte)desc[i];
	}
}


//***************************************************************************
//An array of mutex-protected objects.
//***************************************************************************

template<int Length, class T> class MutexArray {
public:
	void ConstructL() {
		for(int i=0; i<Length; i++) {
			LHEL(arr[i].mutex.CreateLocal());
		}
	}
	T& Wait(int index) {
		DEBUG_ASSERT(index < Length);
		T& ref = arr[index];
		ref.mutex.Wait();
		return ref;
	}
	T& Get(int index) {	//unprotected, dangerous
		DEBUG_ASSERT(index < Length);
		return arr[index];
	}
	~MutexArray() {
		for(int i=0; i<Length; i++) {
			arr[i].mutex.Close();
		}
	}
private:
	T arr[Length];
};

//***************************************************************************
//File system server connection management
//***************************************************************************

class MyRFs {
public:
	MyRFs() {}
	void Connect() {
		PanicIfError(rfs.Connect());
	}
	~MyRFs() {
		rfs.Close();
	}
	RFs& operator()() { return rfs; }
private:
	RFs rfs;
};
#define FSS (myrfs())

//***************************************************************************
//Fifo queue designed for C-type objects.
//***************************************************************************

class FifoQueBase;
class FifoQueLink {
public:
	virtual ~FifoQueLink() {
		LOGD("Link destruct. Next=0x%08X\n", (uint)iNext);
	}
	friend class FifoQueBase;
private:
	FifoQueLink* iNext;
};
class FifoQueBase {
protected:
	FifoQueBase() : iFirst(NULL), iLast(NULL) {}
	~FifoQueBase() {
		Clear();
	}
	void Push(FifoQueLink* obj) { //takes ownership of obj
		if(iFirst == NULL) {
			iFirst = obj;
		} else {
			DEBUG_ASSERT(iLast != NULL);
			iLast->iNext = obj;
		}
		obj->iNext = NULL;
		iLast = obj;
		Test();
	}
	FifoQueLink& First() {
		DEBUG_ASSERT(iFirst != NULL);
		Test();
		return *iFirst;
	}
public:
	TBool IsEmpty() {
		return iFirst == NULL;
	}
	void Pop() {
		DEBUG_ASSERT(iFirst != NULL);
		Test();
		FifoQueLink* link = iFirst->iNext;
		delete iFirst;
		iFirst = link;
		if(iFirst == NULL)
			iLast = NULL;
		else
			Test();
	}
	void Clear() {
		//unoptimized
		while(!IsEmpty()) {
			Pop();
		}
	}
private:
	void Test() {
		//MYASSERT(((uint)iFirst->iNext) < ((uint)0xC0000000)); //HACK
	}
	FifoQueLink* iFirst;
	FifoQueLink* iLast;
};
template<class T> class FifoQue : public FifoQueBase {
public:
	void Push(T* obj) { FifoQueBase::Push(obj); }
	T& First() { return (T&)FifoQueBase::First(); }
};

//***************************************************************************
//An array that calls the destructors of its objects before it removes them.
//***************************************************************************

template<class T> class RDeletingArray : public RArray<T> {
public:
	RDeletingArray(TInt aGranularity, TInt aKeyOffset) :
		RArray<T>(aGranularity, aKeyOffset) {}

	void Close() {
		DestructAllObjects();
		RArray<T>::Close();
	}
	void Remove(TInt index) {
		Destruct(index);
		RArray<T>::Remove(index);
	}
	void Reset() {
		DestructAllObjects();
		RArray<T>::Reset();
	}
private:
	void DestructAllObjects() {
		for(TInt i=0; i<RArray<T>::Count(); i++) {
			Destruct(i);
		}
	}
	void Destruct(TInt index) {
		(*this)[index].~T();
	}
};

//***************************************************************************
//A synchronizer for active objects
//***************************************************************************

#if 0
#define MyPrioIdle (CActive::EPriorityHigh - 1)
#define MyPrioStandard (MyPrioIdle + 1)
#define MyPrioHigh (MyPrioIdle + 2)
#else
#define MyPrioIdle (CActive::EPriorityIdle)
#define MyPrioStandard (CActive::EPriorityStandard)
#define MyPrioHigh (CActive::EPriorityHigh)
#endif

class CLocalSynchronizer : public CActive {
public:
	CLocalSynchronizer() : CActive(MyPrioHigh) {
		CActiveScheduler::Add(this);
		mWait = new (ELeave) CActiveSchedulerWait;
	}
	TRequestStatus* Status() { return &iStatus; }
	void Wait() {
		CActive::SetActive();
		mWait->Start();
	}
	virtual ~CLocalSynchronizer() {
		if(IsActive())
			this->Cancel();
		if(mWait)
			delete mWait;
	}
protected: //CActive
	void RunL() { mWait->AsyncStop(); }
	void DoCancel() { mWait->AsyncStop(); }

	CActiveSchedulerWait* mWait;
};

//***************************************************************************
//CPublicActive
//***************************************************************************

class CPublicActive : public CActive {
public:
	CPublicActive() : CActive(MyPrioStandard) {
		CActiveScheduler::Add(this);
	}
	void SetActive() {
		CActive::SetActive();
	}
};

//***************************************************************************
//A semi-smart pointer designed to PopAndDestroy itself when it goes out of scope.
//Note: It does NOT PushL anything.
//***************************************************************************

template<class T> class TCleaner : public SmartieBase<T> {
public:
	TCleaner(T* p) : SmartieBase<T>(p) {}
	~TCleaner() {
		if(this->ptr)
			CleanupStack::PopAndDestroy(this->ptr);
	}
	TCleaner& operator=(T* other) {
		if(this->ptr)
			CleanupStack::PopAndDestroy(this->ptr);
		this->ptr = other;
		return *this;
	}
	void pop() {
		if(this->ptr) {
			CleanupStack::Pop(this->ptr);
			this->ptr = NULL;
		}
	}
	void popAndDestroy() {
		if(this->ptr) {
			CleanupStack::PopAndDestroy(this->ptr);
			this->ptr = NULL;
		}
	}
private:
	TCleaner(const TCleaner&);
	TCleaner& operator=(const TCleaner&);
};

//***************************************************************************
//A TPtr8 with a default constructor.
//***************************************************************************

class TMyPtr8 : public TPtr8 {
public:
	TMyPtr8() : TPtr8(NULL, 0) {}
	TPtr8 MyTPtr(TInt pos) {
		return TPtr8(iPtr + pos, Length() - pos, MaxLength() - pos);
	}
};

//***************************************************************************
//An RString with the representative TUint32 accessible.
//***************************************************************************

class RMyString : public RString {
public:
	RMyString(const RString& other) : RString(other) {
	}
	RMyString(RStringPool pool, int val) {
		iPool = pool;
		iVal = val;
	}
	TUint32 Val() { return iVal; }
};

//***************************************************************************
//An active object to end all active objects.
//***************************************************************************

class CActiveEnder : public CActive {
public:
	//offset = _FOFF(ConnOp,iLink);
	CActiveEnder(TInt aOffset) : CActive(MyPrioStandard), mQue(aOffset),
		mOffset(aOffset)
	{
		CActiveScheduler::Add(this);
	}
	~CActiveEnder() {
		LOGD("~CActiveEnder()\n");
		Cancel();
	}
	void move(CActive* aObject) {
		LOGD("CAE move 0x%08X\n", aObject);
		TDblQueLink* link = (TDblQueLink*)(((byte*)aObject) + mOffset);
		link->Deque();
		mQue.AddLast(*aObject);
		if(!IsActive()) {
			iStatus = KRequestPending;
			SetActive();
			TRequestStatus* rsp = &iStatus;
			User::RequestComplete(rsp, KErrNone);
		}
	}
protected:	//CActive
	void RunL() { clean(); }
	void DoCancel() { clean(); }
private:
	TDblQue<CActive> mQue;
	const TInt mOffset;

	void clean() {
		LOGD("CActiveEnder.clean %i\n", iStatus.Int());
		while(!mQue.IsEmpty()) {
			CActive* object = mQue.First();
			LOGD("CAE deque/lete 0x%08X\n", object);
			TDblQueLink* link = (TDblQueLink*)(((byte*)object) + mOffset);
			link->Deque();
			delete object;
		}
	}
};

#endif  //__SYMBIAN_HELPERS_H__
