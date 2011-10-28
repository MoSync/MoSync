/*
 * membench.h
 *
 *  Created on: 13 jul 2011
 *      Author: Alex
 */

#ifndef MEMBENCH_H_
#define MEMBENCH_H_

#include <MAUtil/String.h>
#include <ma.h>
#include <conprint.h>
#include <maassert.h> //give access to FREEZE macro
#include <mastdlib.h> //give acces to rand()
#include <maheap.h>

#define ALOT 1024 //used as number of iterations when we want to do a lot of operations
#define	RUNNING_TIME 1000 //running time per test in msecs

/* Test types */
#define MAUTIL_STRING 0
#define MALLOC 1
#define ARRAY 2
#define VECTOR_ADD 3
#define VECTOR_ACCESS 4
#define DUMMY 5
#define DUMMY_MIX 6
#define DUMMY_STRUCT 7
#define DUMMY_ACCESS 8
#define DUMMY_MIX_ACCESS 9
#define DUMMY_STRUCT_ACCESS 10

struct DummyStruct {
	int a;
	int b;
	int c;
};

class DummyObject {
public:
	int a;
	int b;
	int c;
};

class DummyObjectMix {
public:
	inline int getA(){return a;};
	inline int getB(){return b;};
	inline int getC(){return c;};
	inline float getD(){return d;};
	inline float getE(){return e;};
	inline float getF(){return f;};
	inline float getG(){return g;};

	inline MAUtil::Vector<MAUtil::String> & getStrVec(){return str_vec;};
	inline MAUtil::String & getStr(){return str;};

	int a;
	int b;
	int c;
	float d;
	float e;
	float f;
	float g;
	MAUtil::Vector<MAUtil::String> str_vec;
	MAUtil::String str;

};

class MemBench {
public:
	MemBench();
	~MemBench();

	/* start benching! */
	void bench();

private:

	int heapBench(int numRuns, int size, int testType);
	int memAccess(int numRuns, int size, int testType);

	/* Timer functions */
	int currTime();

	/* member variables */
	char *mArray;
	MAUtil::Vector<int> mVector;
	DummyObject mDo;
	DummyObjectMix mDom;
	DummyStruct mDs;

};



#endif /* MEMBENCH_H_ */
