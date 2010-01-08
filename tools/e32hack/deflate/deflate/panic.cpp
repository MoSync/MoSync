// e32tools\petran\Szip\panic.cpp
//
// Copyright (c) 1998-2004 Symbian Software Ltd. All rights reserved.
//

#include "panic.h"
#include "h_utl.h"
#include <stdlib.h>

const char* HuffmanError[]=	{
	"Huffman: Too many codes\n",
	"Huffman: Invalid coding\n",
	"Huffman: Buffer overflow\n",
	"Huffman: Out Of Memory\n",
	"Huffman: Corrupt File\n",
};

const TInt KHuffmanErrorBase=700;

void Panic(TPanic aPanic)
{
	Print(ESevereError, "%s", HuffmanError[TInt(aPanic)]);
	exit(KHuffmanErrorBase+TInt(aPanic));
}
