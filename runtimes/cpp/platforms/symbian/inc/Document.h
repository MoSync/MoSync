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
