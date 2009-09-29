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

#ifndef __CAPPUI_H__
#define __CAPPUI_H__

#include <aknappui.h>

#include "MoSync.pan"
#include "config_platform.h"

class CDocument;
class CAppView;

namespace Core {
	class VMCore;
}

class CAppUi : public CAknAppUi {
public:
	~CAppUi();
	void ConstructL();
	CAppUi(CDocument& aDocument);
	void Stop();
	void Exit();

	void HandleCommandL(TInt aCommand);
	TBool isExiting() const { return iExiting; }
	const Core::VMCore* GetCore() const;

#if 0
	//CEikAppUi
	TBool ProcessCommandParametersL(TApaCommand aCommand,
		TFileName &aDocumentName, const TDesC8 &aTail);
#endif
	CDocument& iDocument;
	TBool iReload;
private:
	CAppView* iAppView;
	TBool iExiting;
	HBufC8* iBuf;
};

#endif // __CAPPUI_H__
