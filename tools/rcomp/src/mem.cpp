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


#if defined(__MSVCDOTNET__) || defined(__TOOLS2__)
#include <iostream>
#else //!__MSVCDOTNET__
#include <iostream.h>
#endif //__MSVCDOTNET__

#include <stdlib.h>
#include "mem.h"

unsigned long MemCheckControl::iNumBlocksAllocated = 0;  // Count of blocks allocated.
unsigned short MemCheckControl::iLogMemory = 0;
unsigned short MemCheckControl::iOutputCalls = 0;

