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


#ifndef __MEM_H__
#define __MEM_H__

class MemCheckControl
	{
public:
	static unsigned long iNumBlocksAllocated;  // Count of blocks allocated.
	static unsigned short iLogMemory;
	static unsigned short iOutputCalls;
	};

#define MOFF	MemCheckControl::iLogMemory=0
#define MON		MemCheckControl::iLogMemory=logmemorysetting

extern unsigned short logmemorysetting;

#endif //__MEM_H__
