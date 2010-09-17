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
*
*/


#include "qualifar.h"

QualifiedString::QualifiedString(const String& aEntry, const String * aFileName, int aLineNumber)
	:iEntry(aEntry),
	iFileName((*aFileName)),
	iLineNumber(aLineNumber)
	{
	}

QualifiedString::~QualifiedString()
	{
	}

String QualifiedString::GetEntry()
	{
	return iEntry;
	}

String QualifiedString::GetFileName()
	{
	return iFileName;
	}

int QualifiedString::GetLineNumber()
	{
	return iLineNumber;
	}


QualifiedStringArray::QualifiedStringArray()
	{
	}

void QualifiedStringArray::Add(QualifiedString * pNewItem)
	{
	Array::Add(pNewItem);
	}

QualifiedStringArrayIterator::QualifiedStringArrayIterator(const QualifiedStringArray & c)
	:ArrayIterator(c)
	{}

QualifiedString* QualifiedStringArrayIterator::operator() ()
	{
	return (QualifiedString *) ArrayIterator::operator() ();
	}

ostream& operator<< ( ostream & aOs, QualifiedStringArray & aQualifiedStringArray)
	{
	if(aQualifiedStringArray.Size() == 0)
		{
		return ( aOs << "<none>");
		}
	QualifiedStringArrayIterator next(aQualifiedStringArray);
	QualifiedString * pQualifiedString;
	//Output first QualifiedString
	if( ( pQualifiedString = next() ) != NULL)
		{
		aOs << (* pQualifiedString).GetEntry();
		aOs << ":";
		aOs << (* pQualifiedString).GetFileName();
		aOs << ":";
		aOs << (* pQualifiedString).GetLineNumber();
		}
	// Output remaining with newline before each
	while( ( pQualifiedString = next() ) != NULL)
		{
		aOs << "\n";
		aOs << (* pQualifiedString).GetEntry();
		aOs << ":";
		aOs << (* pQualifiedString).GetFileName();
		aOs << ":";
		aOs << (* pQualifiedString).GetLineNumber();
		}
	return aOs;
	}


