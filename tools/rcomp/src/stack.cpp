// STACK.CPP
//
// Copyright (c) 1997-1999 Symbian Ltd.  All rights reserved.
//

#include "stack.h"

StackItem::StackItem()
{}

StackItem::~StackItem()
{}

Stack::Stack()
{}

StackItem * Stack::Pop()
{
	StackItem* item=(StackItem*)LinkedList::TailItem();
	LinkedList::RemoveItem(item);
	return item;
}

void Stack::Push(StackItem* aNewItem)
{
	LinkedList::AddToTail(aNewItem);
}

StackItem * Stack::Peek()
{
	return (StackItem*)LinkedList::TailItem();
}
