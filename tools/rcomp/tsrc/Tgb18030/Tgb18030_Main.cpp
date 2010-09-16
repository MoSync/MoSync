/*
* Copyright (c) 1997-2009 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of the License "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description: 
*
*/



#include "Tgb18030.h"

//             The entry point for the application code. It creates
//             an instance of the CApaApplication derived
//             class, CTgb18030Application.
//

#if defined(EKA2)

#include <eikstart.h>
LOCAL_C CApaApplication* NewApplication()
	{
	return new CTgb18030Application;
	}
	
GLDEF_C TInt E32Main()
	{
	return EikStart::RunApplication(NewApplication);
	}
	
#endif

#if defined(__WINS__) && !defined(EKA2)


EXPORT_C CApaApplication* NewApplication()
	{
	return new CTgb18030Application;
	}

GLDEF_C TInt E32Dll(TDllReason)
	{
	return KErrNone;
	}
	
EXPORT_C TInt WinsMain(TDesC* aCmdLine)
	{
	return EikStart::RunApplication(NewApplication, aCmdLine);
	}
	
#endif
