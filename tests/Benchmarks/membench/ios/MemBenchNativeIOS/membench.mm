/*
 * membench.cpp
 *
 * Created on: 13 jul 2011
 *      Author: Alex
 */

#include "membench.h"


MemBench::MemBench() {

}

MemBench::~MemBench() {

}

/*
 * Called from obj-C @ runtime to set a pointer for us to do callbacks to update the GUI
 */
void MemBench::reg_objc_obj(id obj) {
    mObjcPtr = obj;
}

/*
 * Callback to objective-C runtime to print results to the GUI
 */
void MemBench::call_objc_obj(char *msg) {
    objc_msgSend(mObjcPtr, sel_getUid("postResult:"), msg);
}

void MemBench::runNextBench() { //run the next benchmark app in the benchmark suite
    objc_msgSend(mObjcPtr, sel_getUid("runNextBench:"), NULL);
}

void MemBench::bench() {

	float time;
    float kmemops;
	int i;
	char buf[120];

	sprintf(buf, "Memory Benchmark started!\n");
	call_objc_obj(buf);

	//double the amount of test runs until the test take more than RUNNING_TIME to perform
	for(i = 1; (time = this->heapBench(i, 10, MAUTIL_STRING)) < RUNNING_TIME; i*=2);
	sprintf(buf, "allocating/freeing %d std::strings of capacity 10 ", ALOT*i);
	call_objc_obj(buf);
	sprintf(buf, "Time: %1.2f seconds, %1.2f KMEMOPS.\n", time, (kmemops = (float) (ALOT*i*0.001f)/(float) time));
	call_objc_obj(buf);
    mBr.alloc_str_10 = kmemops;

	for(i = 1; (time = this->heapBench(i, 100, MAUTIL_STRING)) < RUNNING_TIME; i*=2);
	sprintf(buf, "allocating/freeing %d std::strings of capacity: 100 ", ALOT*i);
	call_objc_obj(buf);
	sprintf(buf, "Time: %1.2f seconds, %1.2f KMEMOPS.\n", time, (kmemops = (float) (ALOT*i*0.001f)/(float) time));
	call_objc_obj(buf);
    mBr.alloc_str_100 = kmemops;

	for(i = 1; (time = this->heapBench(i, 1, MALLOC)) < RUNNING_TIME; i*=2);
	sprintf(buf, "allocating/freeing %d heap blocks of size 1 using malloc() ", ALOT*i);
	call_objc_obj(buf);
	sprintf(buf, "Time: %1.2f seconds, %1.2f KMEMOPS.\n", time, (kmemops = (float) (ALOT*i*0.001f)/(float) time));
	call_objc_obj(buf);
    mBr.alloc_void_1 = kmemops;

	for(i = 1; (time = this->heapBench(i, 100, MALLOC)) < RUNNING_TIME; i*=2);
	sprintf(buf, "allocating/freeing %d heap blocks of size 100 using malloc() ", ALOT*i);
	call_objc_obj(buf);
	sprintf(buf, "Time: %1.2f seconds, %1.2f KMEMOPS.\n", time, (kmemops = (float) (ALOT*i*0.001f)/(float) time));
	call_objc_obj(buf);
    mBr.alloc_void_100 = kmemops;

	for(i = 1; (time = this->heapBench(i, 1000, MALLOC)) < RUNNING_TIME; i*=2);
	sprintf(buf, "allocating/freeing %d heap blocks of size 1000 using malloc() ", ALOT*i);
	call_objc_obj(buf);
	sprintf(buf, "Time: %1.2f seconds, %1.2f KMEMOPS.\n", time, (kmemops = (float) (ALOT*i*0.001f)/(float) time));
	call_objc_obj(buf);
    mBr.alloc_void_1000 = kmemops;

	for(i = 1; (time = this->heapBench(i, 100, DUMMY)) < RUNNING_TIME; i*=2);
	sprintf(buf, "allocating/freeing %d dummy objects ", ALOT*i);
	call_objc_obj(buf);
	sprintf(buf, "Time: %1.2f seconds, %1.2f KMEMOPS.\n", time, (kmemops = (float) (ALOT*i*0.001f)/(float) time));
	call_objc_obj(buf);
    mBr.alloc_dummy = kmemops;

	for(i = 1; (time = this->heapBench(i, 100, DUMMY_STRUCT)) < RUNNING_TIME; i*=2);
	sprintf(buf, "allocating/freeing %d dummy structs ", ALOT*i);
	call_objc_obj(buf);
	sprintf(buf, "Time: %1.2f seconds, %1.2f KMEMOPS.\n", time, (float) (kmemops = (float) (ALOT*i*0.001f)/(float) time));
	call_objc_obj(buf);
    mBr.alloc_dummy_struct = kmemops;

	for(i = 1; (time = this->heapBench(i, 100, DUMMY_MIX)) < RUNNING_TIME; i*=2);
	sprintf(buf, "allocating/freeing %d larger dummy objects ", ALOT*i);
	call_objc_obj(buf);
	sprintf(buf, "Time: %1.2f seconds, %1.2f KMEMOPS.\n", time, (kmemops = (float) (ALOT*i*0.001f)/(float) time));
	call_objc_obj(buf);
    mBr.alloc_dummy_mix = kmemops;


	mArray = (char*) malloc(100 * sizeof(char));
	for(i = 1; (time = this->memAccess(i, 100, ARRAY)) < RUNNING_TIME; i*=2);
	sprintf(buf, "accessing each element in a char array %d times ", ALOT*i);
	call_objc_obj(buf);
	sprintf(buf, "Time: %1.2f seconds, %1.2f KMEMOPS.\n", time, (kmemops = (float) (ALOT*i*100*0.001f)/(float) time));
	call_objc_obj(buf);
    free(mArray);
    mBr.access_array = kmemops;


	for(i = 1; (time = this->memAccess(i, 100, VECTOR_ADD)) < RUNNING_TIME; i*=2);
	sprintf(buf, "adding 100 int:s to a std::vector<int> %d times ", ALOT*i);
	call_objc_obj(buf);
	sprintf(buf, "Time: %1.2f seconds, %1.2f KMEMOPS.\n", time, (kmemops = (float) (ALOT*i*100*0.001f)/(float) time));
	call_objc_obj(buf);
    mBr.add_vector = kmemops;

	for(i = 1; (time = this->memAccess(i, 100, VECTOR_ACCESS)) < RUNNING_TIME; i*=2);
	sprintf(buf, "accessing 100 int:s in a std::vector<int> %d times ", ALOT*i);
	call_objc_obj(buf);
	sprintf(buf, "Time: %1.2f seconds, %1.2f KMEMOPS.\n", time, (kmemops = (float) (ALOT*i*100*0.001f)/(float) time));
	call_objc_obj(buf);
    mBr.access_vector = kmemops;

	for(i = 1; (time = this->memAccess(i, 1, DUMMY_ACCESS)) < RUNNING_TIME; i*=2);
	sprintf(buf, "DUMMY_ACCESS %d times ", ALOT*i);
	call_objc_obj(buf);
	sprintf(buf, "Time: %1.2f seconds, %1.2f KMEMOPS.\n", time, (kmemops = (float) (ALOT*i*0.001f)/(float) time));
	call_objc_obj(buf);
    mBr.access_dummy = kmemops;

	for(i = 1; (time = this->memAccess(i, 1, DUMMY_STRUCT_ACCESS)) < RUNNING_TIME; i*=2);
	sprintf(buf, "DUMMY_STRUCT_ACCESS %d times ", ALOT*i);
	call_objc_obj(buf);
	sprintf(buf, "Time: %1.2f seconds, %1.2f KMEMOPS.\n", time, (kmemops = (float) (ALOT*i*0.001f)/(float) time));
	call_objc_obj(buf);
    mBr.access_dummy_struct = kmemops;

	for(i = 1; (time = this->memAccess(i, 1, DUMMY_MIX_ACCESS)) < RUNNING_TIME; i*=2);
	sprintf(buf, "DUMMY_MIX_ACCESS %d times ", ALOT*i);
	call_objc_obj(buf);
	sprintf(buf, "Time: %1.2f seconds, %1.2f KMEMOPS.\n", time, (kmemops = (float) (ALOT*i*0.001f)/(float) time));
	call_objc_obj(buf);
    mBr.access_dummy_mix = kmemops;

    //finished, upload results
    mBr.benchmark = "membench";
    BenchDBConnector * bdbc = [[BenchDBConnector alloc] init];
    if([bdbc submit:mBr] < 0){
        //submission to benchmark server failed!
        call_objc_obj("!!!ERROR: Failed to send data to the benchmark server, check your connection.");
    }else{
        call_objc_obj("Done! Data successfully sent to the benchmark server.");
    }
    sleep(5); //sleep so that the data can be sent to server before the next benchmark starts
    //runNextBench(); //start the next benchmark app in the benchmark suite.


}

float MemBench::heapBench(int numRuns, int size, int testType) {

	float startTime = currTime();

	switch(testType){

        case MAUTIL_STRING:
            std::string *str;

            for(int i = 0; i < numRuns; ++i){
                for(int j = 0; j < ALOT; ++j){
                    str = new std::string(size, 'a');
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
                    mem = malloc(size); //this gets optimized away on -OX flags :(
                    free(mem);
                }
            }
            break;

	}
	return currTime() - startTime;

}

float MemBench::memAccess(int numRuns, int size, int testType) {

	float startTime = currTime();

	switch(testType){

        case ARRAY:

            for(int i = 0; i < numRuns; ++i){
                for(int j = 0; j < ALOT; ++j){
                    for(int k = 0; k < size-1; ++k){
                        mArray[k] = mArray[k+1];
                    }
                }
                //printf("mArray[k]: %d", mArray[0]);
            }
            break;

        case VECTOR_ADD:

            for(int i = 0; i < numRuns; ++i){
                for(int j = 0; j < ALOT; ++j){
                    mVector.clear();
                    for(int k = 0; k < size; ++k){
                        mVector.push_back(k);
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
                //printf("mVector[k]: %d", mVector[0]);
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
            std::string tmpStr;
            std::vector<std::string> tmpStrVec;
            mDom.str_vec = std::vector<std::string>();
            for(int i = 0; i < numRuns; ++i){
                mDom.a = rand(); mDom.b = rand(); mDom.c = rand(); mDom.d = rand(); mDom.e = rand(); mDom.f = rand(); mDom.g = rand();
                mDom.str = std::string("kebab");
                for(int j = 0; j < ALOT; ++j){
                    mDom.str_vec.push_back("owlbowl");
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
float MemBench::currTime() {
	return ((float)((float)clock()/(float)CLOCKS_PER_SEC));
}