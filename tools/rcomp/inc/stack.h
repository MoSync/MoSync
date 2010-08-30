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


#ifndef __STACK_H__
#define __STACK_H__

#include "LINKLIST.H"

class StackItem : public ListItem
	{
protected:
	StackItem();
	~StackItem();
	};

class Stack : public LinkedList
	{
protected:
	Stack();
	StackItem* Pop();
	void Push(StackItem* pNewItem);
	StackItem* Peek();
	};

#endif
