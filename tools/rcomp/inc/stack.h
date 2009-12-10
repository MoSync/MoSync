// STACK.H
//
// Copyright (c) 1997-1999 Symbian Ltd.  All rights reserved.
//

#ifndef __STACK_H__
#define __STACK_H__

#include "linklist.h"

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
