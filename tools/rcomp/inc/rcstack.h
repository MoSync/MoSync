// RCSTACK.H
//
// Copyright (c) 1997-1999 Symbian Ltd.  All rights reserved.
//

#include "stack.h"
#include "resource.h"
#include "structst.h"

class ResourceItemArrayStack : public Stack
{
public:
	ResourceItemArrayStack();
	~ResourceItemArrayStack();
	ResourceItemArray* Pop();
	void Push(ResourceItemArray* aNewItem);
	ResourceItemArray* Peek();
};

class StructResourceItemStack : public Stack
{
public:
	StructResourceItemStack();
	~StructResourceItemStack();
	ResourceItem* Pop();
	void Push(ResourceItem* aNewItem);
	ResourceItem* Peek();
};
