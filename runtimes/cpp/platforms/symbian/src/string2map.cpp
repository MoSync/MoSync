/* Copyright (C) 2010 MoSync AB

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License, version 2, as published by
the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

You should have received a copy of the GNU General Public License
along with this program; see the file COPYING.  If not, write to the Free
Software Foundation, 59 Temple Place - Suite 330, Boston, MA
02111-1307, USA.
*/

#include "string2map.h"
#include "symbian_helpers.h"

//******************************************************************************
//String2Map
//******************************************************************************

void String2Map::close() {
	HashMap<HBufC8>::close();
}

void String2Map::dispose(BasePair& pair) {
	RMyString string(mPool, pair.key);
	string.Close();
	delete (HBufC8*)pair.value;
}

const TDesC8& String2Map::insert(const TDesC8& key, const TDesC8& value) {
	HBufC8* buf = HBufC8::NewLC(value.Length());
	*buf = value;
	RMyString string(mPool.OpenStringL(key));
	CleanupClosePushL(string);
	HashMap<HBufC8>::insert(string.Val(), buf);
	CleanupStack::Pop();	//string
	CleanupStack::Pop(buf);
	return *buf;
}

const TDesC8* String2Map::find(const TDesC8& key) const {
	RMyString string(mPool.OpenStringL(key));
	int val = string.Val();
	string.Close();
	return HashMap<HBufC8>::find(val);
}

void String2Map::erase(const TDesC8& key) {
	RMyString string(mPool.OpenStringL(key));
	int val = string.Val();
	string.Close();
	HashMap<HBufC8>::erase(val);
}

//******************************************************************************
//String2Map::TIteratorC
//******************************************************************************

String2Map::Pair String2Map::TIteratorC::next() {
	Pair result = { RMyString(mPool, mPos->key).DesC(),
		*(HBufC8*)mPos->value };
	proceed();
	return result;
}
