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


#if !defined(__FILEACC_H__)
#define __FILEACC_H__
#if !defined(__ASTRING_H__)	
#include "astring.h"
#endif

class FileAccess
	{
public:
	static String FullPath(const String& aPartialPath);
	static String GetDriveAndDirectory(const String& aFullFileName);
	};

#endif //__FILEACC_H__
