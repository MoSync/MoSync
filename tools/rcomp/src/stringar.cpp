// STRINGAR.CPP
//
// Copyright (c) 1997-1999 Symbian Ltd.  All rights reserved.
//

#include "stringar.h"

using namespace std;

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

