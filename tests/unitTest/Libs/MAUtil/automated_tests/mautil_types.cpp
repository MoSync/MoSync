/* Copyright (C) 2009 Mobile Sorcery AB

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

#include <MAUtil/String.h>
#include <MAUtil/Vector.h>
#include <MAUtil/Set.h>
#include <MAUtil/Map.h>
#include <MAUtil/HashMap.h>
#include <conprint.h>

#include "common.h"

#define TEST_CAPACITY 16

static const int srcData[] = {1, 2, 3};

using namespace MAUtil;

class MAUtilTypesTest : public MATest::TestCase {
public:
	MAUtilTypesTest() : MATest::TestCase("MAUtilTypes") {}

	void start() {
		vector();
		string();
		set();
		hashMap();
		map();
		/*
		list();
		*/
		printf("MAUtilTypes test complete, press OK to continue\n");
		suite->runNextCase();
	}

	void vector() {
		Vector<int> v(TEST_CAPACITY);
		assert("Vector::capacity()", v.capacity() == TEST_CAPACITY);

		v.add(4);
		assert("Vector::add(type)", v.size() == 1 && v[0] == 4);

		v.add(srcData, sizeof(srcData) / sizeof(int));
		assert("Vector::add(array)", v.size() == 4 && v[1] == 1 && v[3] == 3);

		v.remove(2);
		assert("Vector::remove()", v.size() == 3 && v[2] == 3);

		v.insert(2, 5);
		assert("Vector::insert()", v.size() == 4 && v[2] == 5);

		v.resize(6);
		assert("Vector::resize()", v.size() == 6);

		v.reserve(TEST_CAPACITY + 1);
		assert("Vector::reserve()", v.capacity() == TEST_CAPACITY + 1);

		v.clear();
		assert("Vector::clear()", v.capacity() == TEST_CAPACITY + 1 && v.size() == 0);
		assert("Vector::empty()", v.size() == 0);

		assert("Vector::begin()", v.begin() == &v[0]);
		assert("Vector::end()", v.end() == ((&v[0])+v.size()));
	}

	void string() {
		MAUtil::String str = "test";
		assert("String::==", str == "test");
		assert("String::!=", str != "fest");
		assert("String::<", !(str < "fest") && (MAUtil::String("fest") < str));
		assert("String::>", !(MAUtil::String("fest") > str) && (str > "fest"));
		assert("String::<=", str <= "test" && str <= "west");
		assert("String::>=", str >= "test" && str >= "fest");
		assert("String::+", (str + "ing") == "testing");
		str+="ing";
		assert("String::+=", str == "testing");
		assert("String::find()", str.find("ing") == 4 && str.find("1") == MAUtil::String::npos);
		str+=" string";
		assert("String::findLastOf()", str.findLastOf('g') == 13 && str.findLastOf('1') == MAUtil::String::npos);
		assert("String::findFirstOf()", str.findFirstOf('g') == 6 && str.findFirstOf('1') == MAUtil::String::npos);
		assert("String::findFirstNotOf()", str.findFirstNotOf('t') == 1 && str.findFirstNotOf('1') == 0);
		str.insert(7, " MAUtil::");
		assert("String::insert(string)", str == "testing MAUtil:: string");

		str.remove(16, 2);
		assert("String::remove()", str == "testing MAUtil::tring");

		str.insert(16, 'S');
		assert("String::insert(char)", str == "testing MAUtil::String");

		assert("String::substr()", str.substr(8, 6) == "MAUtil");

		assert("String::size(), length()", str.size() == 22 && str.length() == 22);

		str.reserve(32);
		assert("String::reserve()", str == "testing MAUtil::String" && str.size() == 22);
		assert("String::capacity()", str.capacity() == 32);

		str.clear();
		assert("String::clear()", str.size() == 0 && str == "");
	}

	void set() {
		Set<String> s;

		//insert
		assert("Set::insert()", s.insert("foo").second);
		assert("Set::insert()", s.insert("bar").second);
		assert("Set::insert()", s.insert("baz").second);
		assert("Set::size()", s.size() == 3);

		//dupe insert
		Pair<Set<String>::Iterator, bool> res = s.insert("foo");
		assert("Set::insert()", !res.second);
		assert("Set::insert()", res.first != s.end());
		assert("Set::insert()", *res.first == "foo");

		//find
		Set<String>::Iterator itr = s.find("bar");
		assert("Set::find()", itr != s.end());
		assert("Set::find()", *itr == "bar");

		//iterate
		itr = s.begin();
		assert("Set::begin()", itr != s.end());
		assert("Set::Iterator", *itr == "bar");
		++itr;
		assert("Set::Iterator", *itr == "baz");
		++itr;
		assert("Set::Iterator", *itr == "foo");
		++itr;
		assert("Set::Iterator", itr == s.end());

		//iterator
		--itr;
		assert("Set::Iterator--", itr != s.end());
		assert("Set::Iterator", itr->size() == 3);

		//const iterator
		Set<String>::ConstIterator citr = s.find("bar");
		assert("cSet::find()", citr != s.end());
		assert("cSet::find()", *citr == "bar");

		citr = s.begin();
		assert("cSet::begin()", citr != s.end());
		assert("cSet::Iterator", *citr == "bar");
		++citr;
		assert("cSet::Iterator", *citr == "baz");
		++citr;
		assert("cSet::Iterator", *citr == "foo");
		++citr;
		assert("cSet::Iterator", citr == s.end());

		--citr;
		assert("cSet::Iterator--", citr != s.end());
		assert("cSet::Iterator", citr->size() == 3);

		//erase
		assert("Set::erase()", s.erase("bar"));
		assert("Set::erase()", s.size() == 2);

		//copy constuctor
		Set<String> copy(s);
		assert("Set::Set(Set)", copy.size() == 2);
		
		//clear
		s.clear();
		assert("Set::clear()", s.size() == 0);
		itr = s.find("baz");
		assert("Set::clear()", itr == s.end());
		assert("Set::clear()", s.begin() == s.end());

		//assignment operator
		s = copy;
		assert("Set::operator=()", s.size() == 2);
	}
	
	void hashMap() {
		HashMap<String, String> m;

		//insert
		assert("HashMap::insert()", m.insert("foo", "F00").second);
		assert("HashMap::insert()", m.insert("bar", "B4R").second);
		assert("HashMap::insert()", m.insert("baz", "B4Z").second);
		assert("HashMap::size()", m.size() == 3);

		//dupe insert
		Pair<HashMap<String, String>::Iterator, bool> res = m.insert("foo", "whatev");
		assert("HashMap::insert()", !res.second);
		assert("HashMap::insert()", res.first != m.end());
		assert("HashMap::insert()", res.first->first == "foo");
		assert("HashMap::insert()", res.first->second == "F00");

		//find
		HashMap<String, String>::Iterator itr = m.find("bar");
		assert("HashMap::find()", itr != m.end());
		assert("HashMap::find()", itr->first == "bar");
		assert("HashMap::find()", itr->second == "B4R");

		//iterate
		itr = m.begin();
		assert("HashMap::begin()", itr != m.end());
		++itr;
		++itr;
		++itr;
		assert("HashMap::Iterator", itr == m.end());

		//const iterator
		HashMap<String, String>::ConstIterator citr = m.find("baz");
		assert("cHashMap::find()", citr != m.end());
		assert("cHashMap::find()", citr->first == "baz");
		assert("cHashMap::find()", citr->second == "B4Z");

		citr = m.begin();
		assert("cHashMap::begin()", citr != m.end());
		++citr;
		++citr;
		++citr;
		assert("cHashMap::Iterator", citr == m.end());

		//erase
		assert("HashMap::erase()", m.erase("bar"));
		assert("HashMap::erase()", m.size() == 2);

		//copy constuctor
		HashMap<String, String> copy(m);
		assert("HashMap::HashMap(HashMap)", copy.size() == 2);
		
		//erase by iterator
		m.erase(m.find("baz"));
		assert("HashMap::erase(Iterator)", m.size() == 1);

		//clear
		m.clear();
		assert("HashMap::clear()", m.size() == 0);
		itr = m.find("baz");
		assert("HashMap::clear()", itr == m.end());
		assert("HashMap::clear()", m.begin() == m.end());

		//assignment operator
		m = copy;
		assert("HashMap::operator=", m.size() == 2);
		
		//square bracket operator
		m["norg"] = "N07G";
		assert("HashMap::operator[]", m["foo"] == "F00");
	}

	void map() {
		Map<String, String> m;
		m["Abraham"] = "Lincoln";
		m["Benjamin"] = "Franklin";
		m["George"] = "Washington";
		const Vector<String>& keys = m.getKeySet();
		assert("Map::getKeySet().size()", keys.size()==3);
		assert("Map::getKeySet()",
				keys[0] == "Abraham" &&
				keys[1] == "Benjamin" &&
				keys[2] == "George"
		);

		String* s = m.get("Benjamin");
		assert("Map::get()!=NULL", s!=NULL && *s=="Franklin");
		s = m.get("Edgar");
		assert("Map::get()==NULL", s==NULL);

		m.remove("Benjamin");
		s = m.get("Benjamin");
		assert("Map::remove()", s==NULL);

		m.clear();
		s = m.get("Abraham");
		assert("Map::clear()", keys.size()==0 && s==NULL);
	}
};

void addMAUtilTypeTests(MATest::TestSuite* suite);
void addMAUtilTypeTests(MATest::TestSuite* suite) {
	suite->addTestCase(new MAUtilTypesTest);
}
