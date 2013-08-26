/* Copyright 2013 David Axmark

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

	http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

/*
 * stropbench.h
 *
 *  Created on: 7 jul 2011
 *      Author: Alex
 */

#ifndef STROPBENCH_H_
#define STROPBENCH_H_

#include <MAUtil/String.h>
#include <ma.h>
#include <conprint.h>
#include <maassert.h> //give access to FREEZE macro
#include <mastdlib.h> //give acces to rand()
#include <string>

#define ALOT 256 //used as number of iterations when we want to do a lot of operations
#define MAUTIL_STRING 0
#define STD_STRING 1
#define	RUNNING_TIME 1000 //running time per test in msecs

class StropBencher {
public:

	StropBencher();
	~StropBencher();

	void bench();

private:

	/* Benchmark functions, returns STROPS */
	int appender(int numRuns, int strType, int func);
	int copy(int numRuns, int strType);
	int find(int numRuns, int strType, int func);
	int insert(int numRuns, int strType, int insType);
	int substr(int numRuns, int strType);
	int remove(int numRuns, int strType);
	int resize(int numRuns, int strType);
	int compare(int numRuns, int strType, int cmpType);

	/* Timer functions */
	int currTime();

	/* Private members */
	MAUtil::String * mMaStr;


	//std::String * mStdStr;

};

#endif /* STROPBENCH_H_ */
