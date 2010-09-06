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

const TUid KUidTgb18030 = { 0x10287187 };

//             The function is called by the UI framework to ask for the
//             application's UID. The returned value is defined by the
//             constant KUidTgb18030 and must match the second value
//             defined in the project definition file.
//
TUid CTgb18030Application::AppDllUid() const
	{
	return KUidTgb18030;
	}

//             This function is called by the UI framework at
//             application start-up. It creates an instance of the
//             document class.
//
CApaDocument* CTgb18030Application::CreateDocumentL()
	{
	return new (ELeave) CTgb18030Document(*this);
	}
