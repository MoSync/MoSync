// MEM.CPP
//
// Copyright (c) 1997-1999 Symbian Ltd.  All rights reserved.
//

#include <iostream>
#include <stdlib.h>
#include "mem.h"

unsigned long MemCheckControl::iNumBlocksAllocated = 0;  // Count of blocks allocated.
unsigned short MemCheckControl::iLogMemory = 0;
unsigned short MemCheckControl::iOutputCalls = 0;

