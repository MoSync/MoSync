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
