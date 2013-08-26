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

#ifndef __CDOCUMENT_H__
#define __CDOCUMENT_H__

#include <Eikdoc.h>

#include "config_platform.h"

#include "Application.h"

class CAppUi;
class CEikApplication;
class CDocument : public CEikDocument {
public:
	~CDocument();
	static CDocument* NewL(CApplication& aApp);
	static CDocument* NewLC(CApplication& aApp);
	
#if defined(PHONE_RELEASE) && defined(__WINSCW__)
	virtual CFileStore *OpenFileL(TBool aDoOpen, const TDesC &aFilename, RFs &aFs);
#endif

	CEikAppUi* CreateAppUiL();
	
	CApplication& iApp;
private:
	CDocument(CApplication& aApp);
};

#endif // __CDOCUMENT_H__
