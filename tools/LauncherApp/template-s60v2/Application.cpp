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

#include <Eikapp.h>
//#include "Document.h"

#define KLauncherUid TUid::Uid(0x%s)

class CApplication : public CEikApplication {
	//inherited from CEikApplication
public:
	TUid AppDllUid() const {
		return KLauncherUid;
	}
protected:
	CApaDocument* CreateDocumentL();
};

GLDEF_C TInt E32Dll(TDllReason) {
	return KErrNone;
}

EXPORT_C CApaApplication* NewApplication() {
	return new CApplication;
}

void StartMoSyncRuntimeL();

CApaDocument* CApplication::CreateDocumentL() {
	//return (static_cast<CApaDocument*>(CDocument::NewL(*this)));
	StartMoSyncRuntimeL();
	User::Exit(0);
	return NULL;
}
