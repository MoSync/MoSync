// LINKLIST.H
//
// Copyright (c) 1997-1999 Symbian Ltd.  All rights reserved.
//

#ifndef __LINKLIST_H__
#define __LINKLIST_H__

class LinkedListIterator;
class LinkedList;

class ListItem
{
	friend class LinkedList;
	friend class LinkedListIterator;
public:
	ListItem();
	virtual ~ListItem();
private:
	ListItem* iNext;
};

class LinkedList
{
	friend class LinkedListIterator;
public:
	LinkedList();
	~LinkedList();
	void AddToHead(ListItem* aNewItem);
	void AddToTail(ListItem* aNewItem);
	void AddAfter(ListItem* aBeforeItem,ListItem* aNewItem);
	void RemoveItem(ListItem* aItem);
	ListItem* TailItem();
	ListItem* Previous(ListItem* aItem);
	void DeleteAll();
	int IsEmpty();
private:
	ListItem*	iHead;
	ListItem* iTail;
};

class LinkedListIterator
{
public:
	LinkedListIterator(const LinkedList& aList);
	LinkedListIterator(ListItem* aItem);
	ListItem* operator()();
	void Reset();
private:
	const LinkedList* iList;
	ListItem* iCurrentItem;
};

#endif
