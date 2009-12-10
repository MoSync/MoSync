// LINKLIST.CPP
//
// Copyright (c) 1997-1999 Symbian Ltd.  All rights reserved.
//

#include <stdlib.h>
#include <assert.h>
#include "linklist.h"

// LinkedListIterator

LinkedListIterator::LinkedListIterator(ListItem* aItem):
iCurrentItem(aItem)
{}

LinkedListIterator::LinkedListIterator(const LinkedList& aList):
iCurrentItem(aList.iHead)
{}

ListItem* LinkedListIterator::operator() ()
{
	ListItem* p = iCurrentItem;
	if(iCurrentItem)
		iCurrentItem = iCurrentItem->iNext;
	return p;
}

void LinkedListIterator::Reset()
{
	iCurrentItem = iList->iHead;
}

// ListItem

ListItem::ListItem():
iNext(NULL)
{}

ListItem::~ListItem()
{}

// LinkedList

LinkedList::LinkedList():
iHead(NULL),
iTail(NULL)
{}

LinkedList::~LinkedList()
{}

void LinkedList::AddToHead(ListItem* aNewItem)
{
	if(iHead)
		aNewItem->iNext = iHead;
	else
		iTail = aNewItem;
	iHead = aNewItem;
}

void LinkedList::AddToTail(ListItem* aNewItem)
{
	if(iTail)
		iTail->iNext = aNewItem;
	else
		iHead = aNewItem;
	iTail = aNewItem;
}

void LinkedList::DeleteAll()
{
	ListItem* item = iHead; 
	while(item)
	{
		ListItem* p = item;
		item=item->iNext;
		delete p;
	}
	iHead = NULL;
	iTail = NULL;
}

void LinkedList::AddAfter(ListItem* aExistingItem,ListItem* aNewItem)
{
	if(aExistingItem==NULL)
		AddToHead(aNewItem);
	else if(aExistingItem == iTail)
		AddToTail(aNewItem);
	else
	{
		aNewItem->iNext = aExistingItem->iNext;
		aExistingItem->iNext = aNewItem;
	}
}

ListItem* LinkedList::Previous(ListItem* aItem)
{
	assert(aItem != NULL);
	assert(iHead != NULL);
	ListItem* p = iHead;
	if(p == aItem)
		return NULL;
	while(p->iNext != aItem && p->iNext != NULL)
		p = p->iNext;
	assert(p->iNext == aItem);
	return p;		
}

void LinkedList::RemoveItem(ListItem* aItem)
{
	assert(aItem != NULL);
	assert(iHead != NULL);
	// If item to remove is at head of list.	
	if (iHead == aItem)
	{
		if(iTail == aItem)	// Item being removed is only item in list
			iTail = NULL;	
		iHead = aItem->iNext;
		return;
	}
	// Search through the list for the item.
	ListItem* p = iHead;
	while( p && (p->iNext!=aItem))
		p = p->iNext;
	assert(p!=NULL);
	if (iTail == p->iNext)
		iTail = p;
	p->iNext = p->iNext->iNext;
}

ListItem* LinkedList::TailItem()
{
	return iTail;
}

int LinkedList::IsEmpty()
{
	return(iHead==NULL);
}
