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

#include "config_platform.h"
#include "Application.h"
#include "Document.h"
#include <helpers/log.h>
#include "MoSync.pan"
#include "heapsize.h"

#ifdef __SERIES60_3X__
#define KUidMoSyncApp TUid::Uid(0xE7654321)
#else	//Series 60, 2nd Ed.
#define KUidMoSyncApp TUid::Uid(0x20015BDC)
#endif

#if 0//ndef __SERIES60_3X__	//turn off on 3rd ed.
void ExceptionHandler(TExcType aType)
{
	LOGD("Exception %i\n", aType);
	//Panic(EMoSyncLoggedPanic);
}
#endif

EXPORT_C CApaApplication* NewApplication() {
#ifdef LOGGING_ENABLED
	InitLog();
#endif

#if 0//ndef __SERIES60_3X__	//turn off on 3rd ed.
	//catch KExceptionAbort through KExceptionInteger
#ifdef __SERIES60_3X__
	User::
#else	//Series 60, 2nd Ed.
	RThread().
#endif	//__SERIES60_3X__
	SetExceptionHandler(&ExceptionHandler, 0x3F);
#endif	//0//__SERIES60_3X__

#ifdef LOGGING_ENABLED
	char cBlarf[] = {
		0x80, 0x80, 0x7f
	};
	int i = cBlarf[1];
	LOG("i = 0x%x. c0 = 0x%x. c2 = 0x%x, %i.\n", i, cBlarf[0], cBlarf[2], 42);
#endif

	LOGD("Calling new CApplication...\n");

	CApplication* app = new CApplication;
	return app;
}

#ifdef __SERIES60_3X__

#include <eikstart.h>
GLDEF_C TInt E32Main() {
#if 1
	RHeap* oldHeap = User::SwitchHeap(UserHeap::ChunkHeap(0, 0x300000, HEAP_SIZE));
	oldHeap->Close();
#endif
	TInt result = EikStart::RunApplication(NewApplication);
	//we never get here.
	//RHeap* myHeap = User::SwitchHeap(oldHeap);
	//myHeap->Close();
	return result;
}

#if defined(__WINS__) && defined(__SERIES60_3X__)
TFileName CApplication::ResourceFileName() const {
	_LIT(KResFileName, "z:\\resource\\apps\\MoSync_3rd.RSC");
	return KResFileName();
}
#endif	//__WINS__

#else //Series 60, 2nd Ed.

GLDEF_C TInt E32Dll(TDllReason) {
	return KErrNone;
}

#endif	//__SERIES60_3X__

TUid CApplication::AppDllUid() const {
	LOGD("AppDllUid\n");
	return KUidMoSyncApp;
}

CApaDocument* CApplication::CreateDocumentL() {
	LOGD("doc\n");
	return CDocument::NewL(*this);
	LOGD("Created doc\n");
}
