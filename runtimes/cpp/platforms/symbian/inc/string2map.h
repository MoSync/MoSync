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

#ifndef STRING2MAP_H
#define STRING2MAP_H

#include <hashmap/hashmap.h>
#include <stringpool.h>

class String2Map : protected HashMap<HBufC8> {
public:
	struct Pair {
		const TDesC8& key;
		const TDesC8& value;
	};
	
	class TIteratorC : protected HashMapBase::TIteratorC {
	public:
		bool hasMore() const { return HashMapBase::TIteratorC::hasMore(); }
		Pair next();
	protected:
		friend class String2Map;
		TIteratorC(const RStringPool& pool, HashMapBase::TIteratorC baseItr)
		: HashMapBase::TIteratorC(baseItr), mPool(pool) {}
		
		const RStringPool& mPool;
	};
	
	String2Map(RStringPool& pool) : mPool(pool) {}
	
	TIteratorC begin() { return TIteratorC(mPool, HashMapBase::begin()); }

	//returns the value
	const TDesC8& insert(const TDesC8& key, const TDesC8& value);
	const TDesC8* find(const TDesC8& key) const;

	void erase(const TDesC8& key);
	void close();
	uint size() const { return HashMap<HBufC8>::size(); }
protected:
	virtual void dispose(BasePair& pair);
private:
	RStringPool& mPool;
};

#endif	//STRING2MAP_H
