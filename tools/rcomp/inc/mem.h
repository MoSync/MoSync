// MEM.H
//
// Copyright (c) 1997-1999 Symbian Ltd.  All rights reserved.
//

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
