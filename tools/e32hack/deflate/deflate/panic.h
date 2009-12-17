// e32tools\petran\Szip\panic.h
//
// Copyright (c) 1998-2004 Symbian Software Ltd. All rights reserved.
//
#ifndef __PANIC_H__
#define __PANIC_H__

#include <e32std.h>

enum TPanic
{
	EHuffmanTooManyCodes=0,
	EHuffmanInvalidCoding=1,
	EHuffmanBufferOverflow=2,
	EHuffmanOutOfMemory=3,
	EHuffmanCorruptFile=4,

};

void ATTRIBUTE(noreturn, Panic(TPanic aPanic));

#endif
