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


#ifndef __STRINGAR_H__
#define __STRINGAR_H__

#include "array.h"
#include "astring.h"

class StringArray : public Array
	{
	friend ostream& operator<<(ostream & os, StringArray & s);
	friend RCBinaryStream& operator<<(RCBinaryStream & os, StringArray & s);
public:
	StringArray();
	StringArray& operator=(const StringArray& Source);
	void Add( String * pNewItem);
	};

class StringArrayIterator : public ArrayIterator
	{
public:
	StringArrayIterator(const StringArray & c);
	String * operator()();
	};
	

#endif
