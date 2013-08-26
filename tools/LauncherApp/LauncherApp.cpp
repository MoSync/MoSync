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
#include <apgcli.h>
#include <apgicnfl.h>
#include <bautils.h>
#include <apacmdln.h>


_LIT(KApplicationName, "Launcher Application");

#ifdef __SERIES60_3X__
_LIT(KParameters, "E1223344");
static const TUid KUidMoSyncRuntime = {0xA0006FC7};
#else //Series 60, 2nd Ed.
_LIT(KParameters, "01223344");
static const TUid KUidMoSyncRuntime = {0x20015BDC};
#endif	//__SERIES60_3X__

void StartMoSyncRuntimeL()
{

	RApaLsSession ls;
	User::LeaveIfError(ls.Connect());
	CleanupClosePushL(ls);

	TThreadId threadId;
	int result = ls.StartDocument(KParameters, KUidMoSyncRuntime, threadId);
	User::LeaveIfError(result);

	CleanupStack::PopAndDestroy(1);

	//doesn't work.
	RFs rfs;
	User::LeaveIfError(rfs.Connect());
	CleanupClosePushL(rfs);
	rfs.Delete(KParameters);
	CleanupStack::PopAndDestroy(1);
}


#ifdef __SERIES60_3X__

#include <eikstart.h>
GLDEF_C TInt E32Main()
{
	CTrapCleanup* cleanup = CTrapCleanup::New();
	__ASSERT_ALWAYS( cleanup != NULL, User::Panic( KApplicationName, KErrNoMemory ) );

	TRAPD( error, StartMoSyncRuntimeL() );
	__ASSERT_ALWAYS( !error, User::Panic( KApplicationName, error ) );

	delete cleanup;
	return 0;
}

#endif	//__SERIES60_3X__
