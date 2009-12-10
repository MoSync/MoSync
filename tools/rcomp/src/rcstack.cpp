// RCSTACK.CPP
//
// Copyright (c) 1997-1999 Symbian Ltd.  All rights reserved.
//

#include <assert.h>
#include "rcstack.h"

// ResourceItemArrayStack

ResourceItemArrayStack::ResourceItemArrayStack()
{}

ResourceItemArrayStack::~ResourceItemArrayStack()
{
	assert( IsEmpty());
}

ResourceItemArray * ResourceItemArrayStack::Pop()
{
	return ( ResourceItemArray *) Stack::Pop();
}

void ResourceItemArrayStack::Push( ResourceItemArray * pNewItem)
{
	Stack::Push( pNewItem);
}

ResourceItemArray * ResourceItemArrayStack::Peek()
{
	return ( ResourceItemArray *) Stack::Peek();
}

// StructResourceItemStack

StructResourceItemStack::StructResourceItemStack()
{}

StructResourceItemStack::~StructResourceItemStack()
{
	assert(IsEmpty());
}

ResourceItem * StructResourceItemStack::Pop()
{
	return (ResourceItem*)Stack::Pop();
}

void StructResourceItemStack::Push( ResourceItem * pNewItem)
{
	Stack::Push( pNewItem);
}

ResourceItem * StructResourceItemStack::Peek()
{
	return ( ResourceItem *) Stack::Peek();
}
