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


#include <stdio.h>
#include <assert.h>
#include "array.h"

Array::Array():
	iData(NULL),
	iItemCount(0),
	iItemAllocCount(0)
	{}

Array::~Array()
	{
	delete [] iData;
	}

Array::Array(const Array& aSource)
	{
	iItemCount=aSource.iItemCount;
	iItemAllocCount=aSource.iItemAllocCount;
	assert(iItemAllocCount>=iItemCount);
	if (iItemAllocCount>0)
		{
		iData=new ArrayItem* [iItemAllocCount];
		assert(iData!=0);
		for(int i=0;i<iItemCount;i++)
			{
			iData[i]=aSource.iData[i];
			}
		}
	}

void Array::Empty()
	{
	delete [] iData;
	iData = NULL;
	iItemCount = 0;
	iItemAllocCount = 0;
	}

Array& Array::operator= (const Array& aSource)
	{
	if(&aSource==this)
		return *this;
	DeleteAll();
	iItemCount=aSource.iItemCount;
	iItemAllocCount=aSource.iItemAllocCount;
	assert(iItemAllocCount>=iItemCount);
	if (iItemAllocCount>0)
		{
		iData=new ArrayItem* [iItemAllocCount];
		assert(iData!=0);
		for(int i=0;i<iItemCount;i++)
			{
			iData[i]=aSource.iData[i];
			}
		}
	return *this;
	}

ArrayItem*& Array::operator[](int aIndex) const
	{
	assert(aIndex>=0);
	assert(aIndex<iItemCount);
	return iData[aIndex];
	}

int Array::Size() const
	{
	return iItemCount;
	}

void Array::Add(ArrayItem* aNewItem)
	{
	Add(iItemCount, aNewItem);
	}

void Array::Add(int aIndex, ArrayItem* aNewItem)
	{
	assert(aIndex>=0);
	assert(aIndex<=iItemCount);
	assert(iItemCount<=iItemAllocCount);
	if (iItemCount<iItemAllocCount)
		{
		++iItemCount;
		for (int i=iItemCount-1;i>aIndex;--i)
			{
			iData[i]=iData[i-1];
			}
		iData[aIndex]=aNewItem;
		}
	else
		{
		iItemAllocCount+=4;
		ArrayItem** const data=new ArrayItem* [iItemAllocCount];
		assert(data!=0);
		++iItemCount;
		int i;
		for (i=iItemCount-1;i>aIndex;--i)
			{
			data[i]=iData[i-1];
			}
		data[aIndex]=aNewItem;
		for (i=0;i<aIndex;++i)
			{
			data[i]=iData[i];
			}
		delete [] iData;
		iData = data;
		}
	}

void Array::Discard(ArrayItem* aItem)
	{
	// Find index of item to remove.
	for(int i=0;i<iItemCount;++i)
		{
		if(iData[i]==aItem)
			{
			Discard(i);
			break;
			}
		}
	}

void Array::Discard(int aIndex)
	{
	assert(aIndex>=0);
	assert(aIndex<iItemCount);
	delete iData[aIndex];
	--iItemCount;
	for (int i=aIndex;i<iItemCount;++i)
		{
		iData[i]=iData[i+1];
		}
	}

void Array::DeleteAll()
	{
	for(int i=0;i<iItemCount;i++)
		delete iData[i];
	delete [] iData;
	iData=NULL;
	iItemCount = 0;
	iItemAllocCount = 0;
	}	

ArrayIterator::ArrayIterator(const Array& aArray)
	{
	iArray=&aArray;
	iCurrentIndex=0;
	}

ArrayItem * ArrayIterator::operator()()
	{
	if(iCurrentIndex>=iArray->Size())
		return NULL;
	ArrayItem* p=(*iArray)[iCurrentIndex++];
	return p;
	}

void ArrayIterator::Reset()
	{
	iCurrentIndex=0;
	}

