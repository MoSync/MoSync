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


_LIT(KApplicationName, "MoSyncLauncher");

_LIT(KParameters, "%s");
static const TUid KUidMoSyncRuntime = {0x20015BDC};

void StartMoSyncRuntimeL()
{

	RApaLsSession ls;
	User::LeaveIfError(ls.Connect());
	CleanupClosePushL(ls);

	TThreadId threadId;
	int result = ls.StartDocument(KParameters, KUidMoSyncRuntime, threadId);
	User::LeaveIfError(result);

	CleanupStack::PopAndDestroy(1);
}
