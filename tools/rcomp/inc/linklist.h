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
