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

#include <e32base.h>
#include <apgcli.h>
#include <apgicnfl.h>
#include <bautils.h>
#include <apacmdln.h>


_LIT( KApplicationName, "MoSyncLauncher" );
_LIT(KParameters, "%s");

_LIT( KMoSyncPath, "\\sys\\bin\\MoSync.exe" );

static const TUid KUidMoSyncRuntime = {0xA0006FC7};


void StartMoSyncRuntimeL()
{
	RApaLsSession ls;
	User::LeaveIfError(ls.Connect());
	CleanupClosePushL(ls);

	TThreadId threadId;
	User::LeaveIfError(ls.StartDocument(KParameters, KUidMoSyncRuntime, threadId));

	CleanupStack::PopAndDestroy(1);
}

GLDEF_C TInt E32Main()
{
	CTrapCleanup* cleanup = CTrapCleanup::New();
	__ASSERT_ALWAYS( cleanup != NULL, User::Panic( KApplicationName, KErrNoMemory ) );

	TRAPD( error, StartMoSyncRuntimeL() );
	__ASSERT_ALWAYS( !error, User::Panic( KApplicationName, error ) );

	delete cleanup;
	return 0;
}
