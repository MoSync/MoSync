/* Copyright 2013 David Axmark

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

	http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

//*********************************************************************************************
//				  			  	Array Class Routines
// 						   		Written by A.R.Hartley
//*********************************************************************************************

#include "compile.h"

//***************************************
//
//***************************************

void BucketArrayInit(BucketArray *theArray, uint listSize)
{
	ArrayInit(&theArray->BucketList,	sizeof(BucketListEntry), 0);
	ArrayInit(&theArray->Buckets,		sizeof(BucketEntry), 0);

	theArray->listSize = listSize;
	theArray->free = 1;
}

//***************************************
//
//***************************************

void BucketArrayDispose(BucketArray *theArray)
{
	ArrayDispose(&theArray->BucketList);
	ArrayDispose(&theArray->Buckets);
	theArray->listSize = 0;
	theArray->free = 0;
}


//***************************************
//
//***************************************

void BucketArrayClear(BucketArray *theArray)
{
	ArrayClear(&theArray->BucketList);
	ArrayClear(&theArray->Buckets);
	theArray->free = 1;
}

//***************************************
//			Add an entry
//***************************************

void BucketArrayAdd(BucketArray *theArray, uint entry, uint data)
{
	BucketListEntry *thisEntry;
	BucketEntry *thisBucket;

	uint NewBucket;
	uint LastBucket;

	if (theArray->listSize)
		if (entry >= theArray->listSize)
			return;

	// Get a new bucket

	NewBucket = theArray->free++;

	// Get the list entry at @entry

	thisEntry = (BucketListEntry *) ArrayPtr(&theArray->BucketList, entry);

	if (!thisEntry)
		return;

	LastBucket = thisEntry->last;					// Save the old previous bucket entry
	thisEntry->last = NewBucket;			// Get new bucket

	if (thisEntry->first == 0)				// Is this the first ID entry
		thisEntry->first = NewBucket;		// Fill in the top entry

	// Fill in the NewBucket

	thisBucket = (BucketEntry *) ArrayPtr(&theArray->Buckets, NewBucket);

	thisBucket->data	= data;				// Fill in bucket entry
	thisBucket->next = 0;					// set the next link to zero

	// is this the first last entry

	if (LastBucket == 0)
		return;
	
	// if so, set its ptr to this bucket

	thisBucket = (BucketEntry *) ArrayPtr(&theArray->Buckets, LastBucket);
	thisBucket->next = NewBucket;
}

//***************************************
//
//***************************************

int BucketArraySearch(BucketArray *theArray, uint entry, int elem, uint *rptr)
{
	BucketListEntry *thisEntry;
	BucketEntry *thisBucket;

	if (theArray->listSize)
		if (entry >= theArray->listSize)
			return -1;

	if (elem == -1)
	{
		thisEntry = (BucketListEntry *) ArrayPtr(&theArray->BucketList, entry);

		if (!thisEntry)
			return -1;

		elem = thisEntry->first;
		
		if (elem == 0)
			return -1;
	}
	
	thisBucket = (BucketEntry *) ArrayPtr(&theArray->Buckets, elem);
	
	if (!thisBucket)
		return -1;

	*rptr = thisBucket->data;	
	return thisBucket->next;
}

//***************************************
//
//***************************************

void BucketArrayPrint(BucketArray *theArray, uint entry)
{
	int elem = -1;
	uint data = 0;

	printf("Entry %d:", entry);
	
	while(1)
	{
		elem = BucketArraySearch(theArray, entry, elem, &data);

		if (elem == -1)
			break;

		printf("(%d)=%d ", elem, data);

		if (elem == 0)
			break;

	}

	printf("\n");
}


//******************************************************************************
// 								BucketArray Tests
//******************************************************************************

#if 0

void BucketTest()
{
	BucketArray testArray;
	int n,m;
	int c = 0;
	int j;
	
	BucketArrayInit(&testArray, 10);

	for (m=0;m<20;m++)
	{
		j = (rand() & 511) + 1;
		for (n=1;n<j;n++)
		{
			BucketArrayAdd(&testArray, m, c++);
		}
	}

	for (n=0;n<20;n++)
		BucketArrayPrint(&testArray, n);
		
}

#endif

//******************************************************************************
//  						Structured Array Class
//******************************************************************************

#if 0
//***************************************
//
//***************************************

void TreeArrayInit(TreeArray *theArray)
{
	ArrayInit(&theArray->RootTreeList, sizeof(TreeEntry), 0);

	theArray->current = &theArray->RootTreeList;
	theArray->up = 0;
}

//***************************************
//
//***************************************

void TreeArrayDispose(TreeArray *theArray)
{
	ArrayDispose(&theArray->RootTreeList);
}

//***************************************
//
//***************************************

void TreeArrayAddItem(TreeArray *theArray, int item)
{
	TreeEntry *thisTree = theArray->current;
	ArrayAppend((ArrayStore *) thisTree, item);
}

#endif	//0
