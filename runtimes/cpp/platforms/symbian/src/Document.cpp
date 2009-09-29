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
