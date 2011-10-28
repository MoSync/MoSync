/*
 * membench.cpp
 *
 * Created on: 13 jul 2011
 *      Author: Alex
 *
 * A Memory Benchmark to test the performance of
 * malloc/free, new/delete and access of different kind of datatypes/structures
 *
 * Assumptions: It should be as fast to access a member (of a certain type) of a
 * struct and a class. Not always true in the real world.
 *
 * It should be as fast to allocate 1 byte of heap space as 1000 bytes.
 * May depend on what strategy is used to allocate heap memory.
 *
 *
 */

#include "membench.h"

MemBench::MemBench() {

}

MemBench::~MemBench() {

}

void MemBench::bench() {

	int time;
	int i;

	printf("Memory Benchmark started");

	//double the amount of test runs until the test take more than RUNNING_TIME to perform
	for(i = 1; (time = this->heapBench(i, 10, MAUTIL_STRING)) < RUNNING_TIME; i*=2);
	printf("allocating/freeing %d MAUtil::Strings of capacity 10 ", ALOT*i);
	printf("Time: %d msecs, %1.2f KMEMOPS.\n", time, (float) (ALOT*i)/(float) time);

	for(i = 1; (time = this->heapBench(i, 100, MAUTIL_STRING)) < RUNNING_TIME; i*=2);
	printf("allocating/freeing %d MAUtil::Strings of capacity: 100 ", ALOT*i);
	printf("Time: %d msecs, %1.2f KMEMOPS.\n", time, (float) (ALOT*i)/(float) time);

	for(i = 1; (time = this->heapBench(i, 1, MALLOC)) < RUNNING_TIME; i*=2);
	printf("allocating/freeing %d heap blocks of size 1 using malloc() ", ALOT*i);
	printf("Time: %d msecs, %1.2f KMEMOPS.\n", time, (float) (ALOT*i)/(float) time);

	for(i = 1; (time = this->heapBench(i, 100, MALLOC)) < RUNNING_TIME; i*=2);
	printf("allocating/freeing %d heap blocks of size 100 using malloc() ", ALOT*i);
	printf("Time: %d msecs, %1.2f KMEMOPS.\n", time, (float) (ALOT*i)/(float) time);

	for(i = 1; (time = this->heapBench(i, 1000, MALLOC)) < RUNNING_TIME; i*=2);
	printf("allocating/freeing %d heap blocks of size 1000 using malloc() ", ALOT*i);
	printf("Time: %d msecs, %1.2f KMEMOPS.\n", time, (float) (ALOT*i)/(float) time);

	for(i = 1; (time = this->heapBench(i, 100, DUMMY)) < RUNNING_TIME; i*=2);
	printf("allocating/freeing %d dummy objects ", ALOT*i);
	printf("Time: %d msecs, %1.2f KMEMOPS.\n", time, (float) (ALOT*i)/(float) time);

	for(i = 1; (time = this->heapBench(i, 100, DUMMY_STRUCT)) < RUNNING_TIME; i*=2);
	printf("allocating/freeing %d dummy structs ", ALOT*i);
	printf("Time: %d msecs, %1.2f KMEMOPS.\n", time, (float) (ALOT*i)/(float) time);

	for(i = 1; (time = this->heapBench(i, 100, DUMMY_MIX)) < RUNNING_TIME; i*=2);
	printf("allocating/freeing %d larger dummy objects ", ALOT*i);
	printf("Time: %d msecs, %1.2f KMEMOPS.\n", time, (float) (ALOT*i)/(float) time);

	mArray = (char*) malloc(100 * sizeof(char));
	for(i = 1; (time = this->memAccess(i, 100, ARRAY)) < RUNNING_TIME; i*=2);
	printf("accessing each element in a char array[100] %d times ", ALOT*i);
	printf("Time: %d msecs, %1.2f KMEMOPS.\n", time, (float) (ALOT*i*100)/(float) time);
	free(mArray);

	for(i = 1; (time = this->memAccess(i, 100, VECTOR_ACCESS)) < RUNNING_TIME; i*=2);
	printf("accessing 100 int:s in a MAUtil::vector<int> %d times ", ALOT*i);
	printf("Time: %d msecs, %1.2f KMEMOPS.\n", time, (float) (ALOT*i*100)/(float) time);

	for(i = 1; (time = this->memAccess(i, 100, VECTOR_ADD)) < RUNNING_TIME; i*=2);
	printf("adding 100 int:s to a MAUtil::vector<int> %d times ", ALOT*i);
	printf("Time: %d msecs, %1.2f KMEMOPS.\n", time, (float) (ALOT*i*100)/(float) time);

	for(i = 1; (time = this->memAccess(i, 1, DUMMY_ACCESS)) < RUNNING_TIME; i*=2);
	printf("DUMMY_ACCESS %d times ", ALOT*i);
	printf("Time: %d msecs, %1.2f KMEMOPS.\n", time, (float) (ALOT*i)/(float) time);

	for(i = 1; (time = this->memAccess(i, 1, DUMMY_STRUCT_ACCESS)) < RUNNING_TIME; i*=2);
	printf("DUMMY_STRUCT_ACCESS %d times ", ALOT*i);
	printf("Time: %d msecs, %1.2f KMEMOPS.\n", time, (float) (ALOT*i)/(float) time);

	for(i = 1; (time = this->memAccess(i, 1, DUMMY_MIX_ACCESS)) < RUNNING_TIME; i*=2);
	printf("DUMMY_MIX_ACCESS %d times ", ALOT*i);
	printf("Time: %d msecs, %1.2f KMEMOPS.\n", time, (float) (ALOT*i)/(float) time);


	FREEZE;

}

int MemBench::heapBench(int numRuns, int size, int testType) {

	int startTime = currTime();

	switch(testType){

	case MAUTIL_STRING:
		MAUtil::String *str;

		for(int i = 0; i < numRuns; ++i){
			for(int j = 0; j < ALOT; ++j){
				str = new MAUtil::String(size);
				delete str;
			}
		}
		break;

	case DUMMY:
		DummyObject* dummy;

		for(int i = 0; i < numRuns; ++i){
			for(int j = 0; j < ALOT; ++j){
				dummy = new DummyObject();
				delete dummy;
			}
		}
		break;

	case DUMMY_MIX:
		DummyObjectMix* dummyMix;

		for(int i = 0; i < numRuns; ++i){
			for(int j = 0; j < ALOT; ++j){
				dummyMix = new DummyObjectMix();
				delete dummyMix;
			}
		}
		break;

	case DUMMY_STRUCT:
		DummyStruct* dummyStruct;

		for(int i = 0; i < numRuns; ++i){
			for(int j = 0; j < ALOT; ++j){
				dummyStruct = new DummyStruct();
				delete dummyStruct;
			}
		}
		break;

	case MALLOC:
		void *mem;

		for(int i = 0; i < numRuns; ++i){
			for(int j = 0; j < ALOT; ++j){
				mem = malloc(size);
				free(mem);
			}
		}
		break;

	}
	return currTime() - startTime;

}

int MemBench::memAccess(int numRuns, int size, int testType) {

	int startTime = currTime();

	switch(testType){

	case ARRAY:

		for(int i = 0; i < numRuns; ++i){
			for(int j = 0; j < ALOT; ++j){
				for(int k = 0; k < size-1; ++k){
					mArray[k] = mArray[k+1];
				}
			}
		}
		break;

	case VECTOR_ADD:

		for(int i = 0; i < numRuns; ++i){
			for(int j = 0; j < ALOT; ++j){
				mVector.clear();
				for(int k = 0; k < size; ++k){
					mVector.add(k);
				}
			}
		}
		break;

	case VECTOR_ACCESS:

		for(int i = 0; i < numRuns; ++i){
			for(int j = 0; j < ALOT; ++j){
				for(int k = 0; k < mVector.size()-1; ++k){
					mVector[k] = mVector[k+1];
				}
			}
		}
		break;

	case DUMMY_ACCESS:
		for(int i = 0; i < numRuns; ++i){
			for(int j = 0; j < ALOT; ++j){
				mDo.a = j;
				mDo.b = mDo.c;
				mDo.c = i;
			}
		}
		break;

	case DUMMY_STRUCT_ACCESS:
		for(int i = 0; i < numRuns; ++i){
			for(int j = 0; j < ALOT; ++j){
				mDs.a = j;
				mDs.b = mDs.c;
				mDs.c = i;
			}
		}
		break;

	case DUMMY_MIX_ACCESS:
		int tmp;
		float ftmp;
		MAUtil::String tmpStr;
		MAUtil::Vector<MAUtil::String> tmpStrVec;
		mDom.str_vec = MAUtil::Vector<MAUtil::String>();
		for(int i = 0; i < numRuns; ++i){
			mDom.a = rand(); mDom.b = rand(); mDom.c = rand(); mDom.d = rand(); mDom.e = rand(); mDom.f = rand(); mDom.g = rand();
			mDom.str = MAUtil::String("kebab");
			for(int j = 0; j < ALOT; ++j){
				mDom.str_vec.add("owlbowl");
				tmp = mDom.getA() + mDom.getB() + mDom.getC();
				ftmp = mDom.getD() + mDom.getE() + mDom.getF() + mDom.getG();
				tmpStr = mDom.getStr();
				tmpStrVec = mDom.getStrVec();
			}
			mDom.str_vec.clear();
		}
		break;

	}
	return currTime() - startTime;

}

/*
 * Wrapper around mosync syscall so that we can have the same call on different platforms
 */
int MemBench::currTime() {
	return maGetMilliSecondCount();
}


