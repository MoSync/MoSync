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

/** \file maths.cpp
*
* This file contains mathematical support functions and macros.
*
* \author Patrick Broman and Niklas Nummelin
*
*/

#include "maths.h"
#include "madmath.h"

Vector::Vector() : mX(0), mY(0), mZ(0) {
}

Vector::Vector(int x, int y, int z) : mX(x), mY(y), mZ(z) {
}

int sinTable[TRIG_LOOKUP_SIZE];
#define TRIG_LOOKUP_RANGE ((1<<TRIG_LOOKUP_BITS)-1)

void initTrigTables() {

	double ang = 0;
	double ang_delta = (2*PI)/TRIG_LOOKUP_SIZE;
	for(int i = 0; i < TRIG_LOOKUP_SIZE; i++) {
		sinTable[i] = (int)(sin(ang)*(double)TRIG_LOOKUP_RANGE);
		ang+=ang_delta;
	}
}
