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


#include "stringar.h"

// StringArray

StringArray::StringArray()
	{}
	
StringArray& StringArray::operator= ( const StringArray& Source)
	{
	return (StringArray&) Array::operator= ( Source);
	}
	
void StringArray::Add( String * pNewItem)
	{
	Array::Add( pNewItem);
	}

// StringArrayIterator

StringArrayIterator::StringArrayIterator(const StringArray& c):
	ArrayIterator(c)
	{}

String* StringArrayIterator::operator() ()
	{
	return ( String *) ArrayIterator::operator() ();
	}

// ostream functions

ostream& operator<< ( ostream & os, StringArray & s)
	{
	if (s.Size() == 0)
		return ( os << "<none>");
	StringArrayIterator next( s);
	String * p;
	// Ouput first string.
	if ( ( p = next() ) != NULL)
		os << *p;
	// Output remaining strings with tab before each.
	while ( ( p = next() ) != NULL)
		os << "\t" << * p;
	return os;
	}

