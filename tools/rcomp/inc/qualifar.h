/*
* Copyright (c) 2005-2009 Nokia Corporation and/or its subsidiary(-ies).
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
* A QualifiedString is a String which has a file name and line 
* number attached to it. The QualifiedStringArray and 
* QualifiedStringArrayIterator classes are inherited from
* the Array type base classes and implement the simplest 
* functionality
*
*/



#ifndef __QUALIFAR_H__
#define __QUALIFAR_H__

#include "stringar.h"
#include "astring.h" 

class QualifiedString : public ArrayItem
	{
public:
	QualifiedString(const String& aEntry, const String * aFileName, int aLineNumber);
	~QualifiedString();
	String GetEntry();
	String GetFileName();
	int GetLineNumber();
private:
	String iEntry;
	String iFileName;
	int iLineNumber;
	};

class QualifiedStringArray : public Array
	{
	friend ostream& operator<<(ostream & os, QualifiedStringArray & s);
public:
	QualifiedStringArray();
	void Add( QualifiedString * pNewItem);
	};

class QualifiedStringArrayIterator : public ArrayIterator
	{
public:
	QualifiedStringArrayIterator(const QualifiedStringArray & c);
	QualifiedString * operator()();
	};
	

#endif

