/*
* Copyright (c) 2008-2009 Nokia Corporation and/or its subsidiary(-ies).
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


#ifndef __WIDE_H__
#define __WIDE_H__

/*
   When processing UTF16 we need these types to 
   ensure that everything works as expected.
   whar_t is not useful because it is UTF32 on
   linux and UTF16 on windows.  
*/
typedef unsigned short UTF16;
typedef unsigned long UTF32;

#endif
