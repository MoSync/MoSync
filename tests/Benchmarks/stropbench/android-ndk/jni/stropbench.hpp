/*
 * stropbench.h
 *
 *  Created on: 7 jul 2011
 *      Author: Alex
 */

#ifndef STROPBENCH_H_
#define STROPBENCH_H_

//#include <MAUtil/String.h>
//#include <ma.h>
//#include <conprint.h>
//#include <maassert.h> //give access to FREEZE macro
#include <stdio.h>
#include <cstdlib> //give acces to rand()
#include <string>
#include <time.h>
#include <jni.h>

using namespace std;

#define ALOT 256 //used as number of iterations when we want to do a lot of operations
#define MAUTIL_STRING 0
#define STD_STRING 1
#define	RUNNING_TIME 1.0f //running time per test in msecs

static JNIEnv  *glob_env;
static jobject *glob_obj;

class StropBencher {
public:

	StropBencher();
	~StropBencher();

	void bench();

private:

	/* Benchmark functions, returns STROPS */
	float appender(int numRuns, int strType, int func);
	float copy(int numRuns, int strType);
	float find(int numRuns, int strType, int func);
	float insert(int numRuns, int strType, int insType);
	float substr(int numRuns, int strType);
	float remove(int numRuns, int strType);
	float resize(int numRuns, int strType);
	float compare(int numRuns, int strType, int cmpType);

	/* Timer functions */
	float currTime();


	//std::String * mStdStr;

};

#endif /* STROPBENCH_H_ */
