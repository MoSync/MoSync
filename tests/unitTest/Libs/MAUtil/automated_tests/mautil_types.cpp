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
		printf("MAUtil tests\n");
		printf("-vector\n");
		vector();
		printf("-string\n");
		string();
		printf("-set\n");
		set();
		printf("-hashMap\n");
		hashMap();
		printf("-map\n");
		map();
		printf("-setInt\n");
		setInt();
		printf("-hashMapInt\n");
		hashMapInt();
		printf("-mapInt\n");
		mapInt();
		/*
		list();
		*/
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
		itr--;
		itr++;
		itr++;
		assert("Set::Iterator++", itr == s.end());
		itr--;
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
		citr--;
		citr++;
		citr++;
		assert("cSet::Iterator++", citr == s.end());
		citr--;
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

	void setInt() {
		Set<int> s;

		//insert
		assert("Set::insert()", s.insert(2).second);
		assert("Set::insert()", s.insert(3).second);
		assert("Set::insert()", s.insert(4).second);
		assert("Set::size()", s.size() == 3);

		//dupe insert
		Pair<Set<int>::Iterator, bool> res = s.insert(2);
		assert("Set::insert()", !res.second);
		assert("Set::insert()", res.first != s.end());
		assert("Set::insert()", *res.first == 2);

		//find
		Set<int>::Iterator itr = s.find(3);
		assert("Set::find()", itr != s.end());
		assert("Set::find()", *itr == 3);

		//iterate
		itr = s.begin();
		assert("Set::begin()", itr != s.end());
		assert("Set::Iterator", *itr == 2);
		++itr;
		assert("Set::Iterator", *itr == 3);
		++itr;
		assert("Set::Iterator", *itr == 4);
		++itr;
		assert("Set::Iterator", itr == s.end());

		//iterator
		--itr;
		assert("Set::Iterator--", itr != s.end());
		itr--;
		itr++;
		itr++;
		assert("Set::Iterator++", itr == s.end());
		itr--;
		assert("Set::Iterator--", itr != s.end());

		//const iterator
		Set<int>::ConstIterator citr = s.find(3);
		assert("cSet::find()", citr != s.end());
		assert("cSet::find()", *citr == 3);

		citr = s.begin();
		assert("cSet::begin()", citr != s.end());
		assert("cSet::Iterator", *citr == 2);
		++citr;
		assert("cSet::Iterator", *citr == 3);
		++citr;
		assert("cSet::Iterator", *citr == 4);
		++citr;
		assert("cSet::Iterator", citr == s.end());

		--citr;
		assert("cSet::Iterator--", citr != s.end());
		citr--;
		citr++;
		citr++;
		assert("cSet::Iterator++", citr == s.end());
		citr--;
		assert("cSet::Iterator--", citr != s.end());

		//erase
		assert("Set::erase()", s.erase(3));
		assert("Set::erase()", s.size() == 2);

		//copy constuctor
		Set<int> copy(s);
		assert("Set::Set(Set)", copy.size() == 2);
		
		//clear
		s.clear();
		assert("Set::clear()", s.size() == 0);
		itr = s.find(4);
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
		itr++;
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
		citr++;
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

	void hashMapInt() {
		HashMap<int, int> m;

		//insert
		assert("HashMap::insert()", m.insert(2, 5).second);
		assert("HashMap::insert()", m.insert(3, 6).second);
		assert("HashMap::insert()", m.insert(4, 7).second);
		assert("HashMap::size()", m.size() == 3);

		//dupe insert
		Pair<HashMap<int, int>::Iterator, bool> res = m.insert(4, 8);
		assert("HashMap::insert()", !res.second);
		assert("HashMap::insert()", res.first != m.end());
		assert("HashMap::insert()", res.first->first == 4);
		assert("HashMap::insert()", res.first->second == 7);

		//find
		HashMap<int, int>::Iterator itr = m.find(3);
		assert("HashMap::find()", itr != m.end());
		assert("HashMap::find()", itr->first == 3);
		assert("HashMap::find()", itr->second == 6);

		//iterate
		itr = m.begin();
		assert("HashMap::begin()", itr != m.end());
		++itr;
		itr++;
		++itr;
		assert("HashMap::Iterator", itr == m.end());

		//const iterator
		HashMap<int, int>::ConstIterator citr = m.find(2);
		assert("cHashMap::find()", citr != m.end());
		assert("cHashMap::find()", citr->first == 2);
		assert("cHashMap::find()", citr->second == 5);

		citr = m.begin();
		assert("cHashMap::begin()", citr != m.end());
		++citr;
		citr++;
		++citr;
		assert("cHashMap::Iterator", citr == m.end());

		//erase
		assert("HashMap::erase()", m.erase(3));
		assert("HashMap::erase()", m.size() == 2);

		//copy constuctor
		HashMap<int, int> copy(m);
		assert("HashMap::HashMap(HashMap)", copy.size() == 2);
		
		//erase by iterator
		m.erase(m.find(2));
		assert("HashMap::erase(Iterator)", m.size() == 1);

		//clear
		m.clear();
		assert("HashMap::clear()", m.size() == 0);
		itr = m.find(4);
		assert("HashMap::clear()", itr == m.end());
		assert("HashMap::clear()", m.begin() == m.end());

		//assignment operator
		m = copy;
		assert("HashMap::operator=", m.size() == 2);
		
		//square bracket operator
		m[9] = 10;
		assert("HashMap::operator[]", m[2] == 5);
	}

	void map() {
		Map<String, String> m;

		//operator[] and insert
		m["Abraham"] = "Lincoln";
		m["Benjamin"] = "Franklin";
		m.insert("George", "Washington");
		assert("Map::size()", m.size() == 3);

		//iterate
		{
			Map<String, String>::ConstIterator itr = m.begin();
			assert("Map::begin()", itr->first == "Abraham" && itr->second == "Lincoln");
			++itr;
			assert("Map::ConstIterator()", itr->first == "Benjamin" && itr->second == "Franklin");
			itr++;
			assert("Map::ConstIterator()", itr->first == "George" && itr->second == "Washington");
			itr++;
			assert("Map::end()", itr == m.end());
		}

		//find
		Map<String, String>::Iterator itr = m.find("Benjamin");
		assert("Map::find()", itr != m.end() && itr->second == "Franklin");
		itr = m.find("Edgar");
		assert("Map::find()", itr == m.end());

		//erase
		m.erase("Benjamin");
		itr = m.find("Benjamin");
		assert("Map::erase()", itr == m.end());

		//clear
		m.clear();
		itr = m.find("Abraham");
		assert("Map::clear()", m.size()==0 && itr == m.end());
	}

	void mapInt() {
		Map<int, int> m;

		//operator[] and insert
		m[2] = 5;
		m[3] = 6;
		m.insert(4, 7);
		assert("Map::size()", m.size() == 3);

		//iterate
		{
			Map<int, int>::ConstIterator itr = m.begin();
			assert("Map::begin()", itr->first == 2 && itr->second == 5);
			++itr;
			assert("Map::ConstIterator()", itr->first == 3 && itr->second == 6);
			itr++;
			assert("Map::ConstIterator()", itr->first == 5 && itr->second == 7);
			itr++;
			assert("Map::end()", itr == m.end());
		}

		//find
		Map<int, int>::Iterator itr = m.find(2);
		assert("Map::find()", itr != m.end() && itr->second == 5);
		itr = m.find(9);
		assert("Map::find()", itr == m.end());

		//erase
		m.erase(2);
		itr = m.find(2);
		assert("Map::erase()", itr == m.end());

		//clear
		m.clear();
		itr = m.find(3);
		assert("Map::clear()", m.size()==0 && itr == m.end());
	}
};

void addMAUtilTypeTests(MATest::TestSuite* suite);
void addMAUtilTypeTests(MATest::TestSuite* suite) {
	suite->addTestCase(new MAUtilTypesTest);
}
