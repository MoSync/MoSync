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
