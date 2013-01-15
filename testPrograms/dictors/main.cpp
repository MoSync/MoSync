// Test that MAUtil::Map, Set, HashMap and HashSet calls
// constructors and destructors of keys and values properly.

#include <maassert.h>
#include <conprint.h>
#include <MAUtil/Map.h>
#include <MAUtil/Set.h>
#include <MAUtil/HashMap.h>
#include <MAUtil/HashSet.h>

static int mDestructions, mConstructions, mCopyConstructions, mAssigns;

class Counter {
public:
	int mId;
	Counter(int id) : mId(id) {
		mConstructions++;
	}
	Counter(const Counter& c) {
		mId = c.mId;
		mConstructions++;
		mCopyConstructions++;
	}
	Counter& operator=(const Counter& c) {
		mId = c.mId;
		mAssigns++;
		return *this;
	}
	~Counter() {
		mDestructions++;
	}
	bool operator<(const Counter& c) const {
		return mId < c.mId;
	}
	bool operator==(const Counter& c) const {
		return mId == c.mId;
	}
};

namespace MAUtil {
	template<> hash_val_t THashFunction(const Counter& c) {
		return THashFunction(c.mId);
	}
}

static void t(int d, int c, int cc, int a, int line) {
#define TEST(n, m) if(n != m) { printf("%s @ %i: %i != %i\n", #n, line, n, m); FREEZE; }
	TEST(mDestructions, d);
	TEST(mConstructions, c);
	TEST(mCopyConstructions, cc);
	TEST(mAssigns, a);
}
#define T(...) t(__VA_ARGS__, __LINE__)

template<template<class> class Set>
static void testSet() {
	mDestructions = mConstructions = mCopyConstructions = mAssigns = 0;
	{
		Set<Counter> set;
		T(0,0,0,0);
		set.insert(Counter(1));
		T(2,3,2,0);
		set.erase(Counter(1));
		T(4,4,2,0);
	}
	T(4,4,2,0);
}

template<template<class, class> class Map>
static void testMap() {
	mDestructions = mConstructions = mCopyConstructions = mAssigns = 0;
	{
		Map<Counter, Counter> map;
		T(0,0,0,0);
		map.insert(Counter(1), Counter(2));
		T(6,8,6,0);
		map.erase(Counter(1));
		T(9,9,6,0);
	}
	T(9,9,6,0);
}

int MAMain() GCCATTRIB(noreturn);
int MAMain() {
	printf("HashSet\n");
	testSet<MAUtil::HashSet>();
	printf("HashMap\n");
	testMap<MAUtil::HashMap>();
	printf("Set\n");
	testSet<MAUtil::Set>();
	printf("Map\n");
	testMap<MAUtil::Map>();
	printf("done\n");
	FREEZE;
}
