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
