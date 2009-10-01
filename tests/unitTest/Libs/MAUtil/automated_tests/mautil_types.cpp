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
#include <conprint.h>

#include "common.h"

#define TEST_CAPACITY 16

static const int srcData[] = {1, 2, 3};

class MAUtilTypesTest : public MATest::TestCase {
public:
	MAUtilTypesTest() : MATest::TestCase("MAUtilTypes") {}

	void start() {
		vector();
		string();
		set();
		map();
		/*
		list();
		*/
		printf("MAUtilTypes test complete, press OK to continue\n");
		suite->runNextCase();
	}

	void vector() {
		MAUtil::Vector<int> v(TEST_CAPACITY);
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
		MAUtil::Set<MAUtil::String> s;

		//insert
		assert("Set::insert()", s.insert("foo").first);
		assert("Set::insert()", s.insert("bar").first);
		assert("Set::insert()", s.insert("baz").first);
		assert("Set::size()", s.size() == 3);

		//dupe insert
		Pair<bool, MAUtil::Set<MAUtil::String>::Iterator> res = s.insert("foo");
		assert("Set::insert()", !res.first);
		assert("Set::insert()", res.second != s.end());
		assert("Set::insert()", *res.second == "foo");

		//find
		MAUtil::Set<MAUtil::String>::Iterator itr = s.find("bar");
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

		//erase
		assert("Set::erase()", s.erase("bar"));
		assert("Set::erase()", s.size() == 2);

		//copy constuctor
		MAUtil::Set<MAUtil::String> copy(s);
		assert("Set::Set(Set)", copy.size() == 2);
		
		//clear
		s.clear();
		assert("Set::clear()", s.size() == 0);
		itr = s.find("baz");
		assert("Set::clear()", itr == s.end());
		assert("Set::clear()", s.begin() == s.end());
	}

	void map() {
		MAUtil::Map<MAUtil::String, MAUtil::String> m;
		m["Abraham"] = "Lincoln";
		m["Benjamin"] = "Franklin";
		m["George"] = "Washington";
		const MAUtil::Vector<MAUtil::String>& keys = m.getKeySet();
		assert("Map::getKeySet().size()", keys.size()==3);
		assert("Map::getKeySet()",
				keys[0] == "Abraham" &&
				keys[1] == "Benjamin" &&
				keys[2] == "George"
		);

		MAUtil::String* s = m.get("Benjamin");
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

void addMAUtilTypeTests(MATest::TestSuite* suite) {
	suite->addTestCase(new MAUtilTypesTest);
}
