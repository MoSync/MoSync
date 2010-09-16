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
