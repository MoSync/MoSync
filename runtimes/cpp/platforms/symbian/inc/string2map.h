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
