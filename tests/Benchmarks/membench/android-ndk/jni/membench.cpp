/*
 * membench.cpp
 *
 * Created on: 13 jul 2011
 *      Author: Alex
 */

#include "membench.h"

extern "C" {

/* Calls a java function to print a message stored in buf */
void pass_jni_msg(char *buf) {
	/* Callback to Java to print buf */
	jstring msg2java = ((*glob_env).NewStringUTF(buf));
	jclass cls = ((*glob_env).GetObjectClass(*glob_obj));
	jmethodID mid = (*glob_env).GetMethodID(cls, "printMsg", "(Ljava/lang/String;)V");
	if(mid == 0) /* Could not find the method */
		return;
	//    (*glob_env)->ExceptionClear();
	(*glob_env).CallVoidMethod(*glob_obj, mid, msg2java);
}

/* entry point callable from the Dalvik VM */
JNIEXPORT jstring JNICALL Java_com_mosync_MemBenchNativeAndroidActivity_invokeNativeFunction(JNIEnv* env, jobject javaThis) {
	glob_env = env; //save the java environment
	glob_obj = &javaThis; //save the object that called us!
	MemBench * mb = new MemBench();
	mb->bench();
	delete mb;
	//	return (*env)->NewStringUTF(env, "Done!");
}

MemBench::MemBench() {

}

MemBench::~MemBench() {

}

void MemBench::bench() {

	float time;
	int i;
	char buf[120];

	sprintf(buf, "Memory Benchmark started!\n");
	pass_jni_msg(buf);

	//double the amount of test runs until the test take more than RUNNING_TIME to perform
	for(i = 1; (time = this->heapBench(i, 10, MAUTIL_STRING)) < RUNNING_TIME; i*=2);
	sprintf(buf, "allocating/freeing %d std::strings of capacity 10 ", ALOT*i);
	pass_jni_msg(buf);
	sprintf(buf, "Time: %1.2f msecs, %1.2f KMEMOPS.\n", time, (float) (ALOT*i)/(float) time);
	pass_jni_msg(buf);

	for(i = 1; (time = this->heapBench(i, 100, MAUTIL_STRING)) < RUNNING_TIME; i*=2);
	sprintf(buf, "allocating/freeing %d std::strings of capacity: 100 ", ALOT*i);
	pass_jni_msg(buf);
	sprintf(buf, "Time: %1.2f msecs, %1.2f KMEMOPS.\n", time, (float) (ALOT*i)/(float) time);
	pass_jni_msg(buf);

	for(i = 1; (time = this->heapBench(i, 1, MALLOC)) < RUNNING_TIME; i*=2);
	sprintf(buf, "allocating/freeing %d heap blocks of size 1 using malloc() ", ALOT*i);
	pass_jni_msg(buf);
	sprintf(buf, "Time: %1.2f msecs, %1.2f KMEMOPS.\n", time, (float) (ALOT*i)/(float) time);
	pass_jni_msg(buf);

	for(i = 1; (time = this->heapBench(i, 100, MALLOC)) < RUNNING_TIME; i*=2);
	sprintf(buf, "allocating/freeing %d heap blocks of size 100 using malloc() ", ALOT*i);
	pass_jni_msg(buf);
	sprintf(buf, "Time: %1.2f msecs, %1.2f KMEMOPS.\n", time, (float) (ALOT*i)/(float) time);
	pass_jni_msg(buf);

	for(i = 1; (time = this->heapBench(i, 1000, MALLOC)) < RUNNING_TIME; i*=2);
	sprintf(buf, "allocating/freeing %d heap blocks of size 1000 using malloc() ", ALOT*i);
	pass_jni_msg(buf);
	sprintf(buf, "Time: %1.2f msecs, %1.2f KMEMOPS.\n", time, (float) (ALOT*i)/(float) time);
	pass_jni_msg(buf);

	for(i = 1; (time = this->heapBench(i, 100, DUMMY)) < RUNNING_TIME; i*=2);
	sprintf(buf, "allocating/freeing %d dummy objects ", ALOT*i);
	pass_jni_msg(buf);
	sprintf(buf, "Time: %1.2f msecs, %1.2f KMEMOPS.\n", time, (float) (ALOT*i)/(float) time);
	pass_jni_msg(buf);

	for(i = 1; (time = this->heapBench(i, 100, DUMMY_STRUCT)) < RUNNING_TIME; i*=2);
	sprintf(buf, "allocating/freeing %d dummy structs ", ALOT*i);
	pass_jni_msg(buf);
	sprintf(buf, "Time: %1.2f msecs, %1.2f KMEMOPS.\n", time, (float) (ALOT*i)/(float) time);
	pass_jni_msg(buf);

	for(i = 1; (time = this->heapBench(i, 100, DUMMY_MIX)) < RUNNING_TIME; i*=2);
	sprintf(buf, "allocating/freeing %d larger dummy objects ", ALOT*i);
	pass_jni_msg(buf);
	sprintf(buf, "Time: %1.2f msecs, %1.2f KMEMOPS.\n", time, (float) (ALOT*i)/(float) time);
	pass_jni_msg(buf);


	mArray = (char*) malloc(100 * sizeof(char));
	for(i = 1; (time = this->memAccess(i, 100, ARRAY)) < RUNNING_TIME; i*=2);
	sprintf(buf, "accessing each element in a char array %d times ", ALOT*i);
	pass_jni_msg(buf);
	sprintf(buf, "Time: %1.2f msecs, %1.2f KMEMOPS.\n", time, (float) (ALOT*i*100)/(float) time);
	pass_jni_msg(buf);
	free(mArray);

	for(i = 1; (time = this->memAccess(i, 100, VECTOR_ADD)) < RUNNING_TIME; i*=2);
	sprintf(buf, "adding 100 int:s to a MAUtil::vector<int> %d times ", ALOT*i);
	pass_jni_msg(buf);
	sprintf(buf, "Time: %1.2f msecs, %1.2f KMEMOPS.\n", time, (float) (ALOT*i*100)/(float) time);
	pass_jni_msg(buf);

	for(i = 1; (time = this->memAccess(i, 100, VECTOR_ACCESS)) < RUNNING_TIME; i*=2);
	sprintf(buf, "accessing 100 int:s in a MAUtil::vector<int> %d times ", ALOT*i);
	pass_jni_msg(buf);
	sprintf(buf, "Time: %1.2f msecs, %1.2f KMEMOPS.\n", time, (float) (ALOT*i*100)/(float) time);
	pass_jni_msg(buf);

	for(i = 1; (time = this->memAccess(i, 1, DUMMY_ACCESS)) < RUNNING_TIME; i*=2);
	sprintf(buf, "DUMMY_ACCESS %d times ", ALOT*i);
	pass_jni_msg(buf);
	sprintf(buf, "Time: %1.2f msecs, %1.2f KMEMOPS.\n", time, (float) (ALOT*i)/(float) time);
	pass_jni_msg(buf);

	for(i = 1; (time = this->memAccess(i, 1, DUMMY_STRUCT_ACCESS)) < RUNNING_TIME; i*=2);
	sprintf(buf, "DUMMY_STRUCT_ACCESS %d times ", ALOT*i);
	pass_jni_msg(buf);
	sprintf(buf, "Time: %1.2f msecs, %1.2f KMEMOPS.\n", time, (float) (ALOT*i)/(float) time);
	pass_jni_msg(buf);

	for(i = 1; (time = this->memAccess(i, 1, DUMMY_MIX_ACCESS)) < RUNNING_TIME; i*=2);
	sprintf(buf, "DUMMY_MIX_ACCESS %d times ", ALOT*i);
	pass_jni_msg(buf);
	sprintf(buf, "Time: %1.2f msecs, %1.2f KMEMOPS.\n", time, (float) (ALOT*i)/(float) time);
	pass_jni_msg(buf);

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
				mem = malloc(size);
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
			printf("mArray[k]: %d", mArray[0]);
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
			printf("mVector[k]: %d", mVector[0]);
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


}
