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

#include "Document.h"
#include "AppUi.h"
#include "Application.h"
#include "config_platform.h"
#include "symbian_helpers.h"


CDocument::~CDocument( void ) 
{
	LOG("EOF\n");
}

CDocument* CDocument::NewL( CApplication& aApp ) 
{    
	CDocument* self = NewLC( aApp );
	CleanupStack::Pop( self );
	return self;
}

CDocument* CDocument::NewLC( CApplication& aApp ) 
{    
	// Symbian 2-phase constructor.
	// Calls both the default C++ constructor and Symbian ConstructL methods.
	CDocument* self = new (ELeave) CDocument( aApp );
	CleanupStack::PushL( self );
	return self;
}

CEikAppUi* CDocument::CreateAppUiL( void ) 
{
	LOGD("ui\n");
	return (static_cast <CEikAppUi*> (new (ELeave) CAppUi( *this )));
	LOGD("Created ui\n");
}

CDocument::CDocument( CApplication& aApp ) 
: CEikDocument(aApp), iApp(aApp)
{
	//SetAppFileMode(EFileRead | EFileShareReadersOnly);	//no effect
}

#if defined(PHONE_RELEASE) && defined(__WINSCW__)
CFileStore *CDocument::OpenFileL(TBool DEBUG_ARG(aDoOpen), const TDesC &aFilename, RFs &) {
	HBufC8* buf = CreateHBufC8FromDesC16LC(aFilename);
	LOGD("CDocument::OpenFileL %i \"%S\"\n", aDoOpen, buf);
	CleanupStack::PopAndDestroy(buf);
	//return CEikDocument::OpenFileL(aDoOpen, aFilename, aFs);
	return NULL;
}
#endif
