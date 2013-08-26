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

#ifndef _PART1_H_
#define _PART1_H_

#include "Demo.h"
#include <MAUtil/Vector.h>

using namespace MAUtil;

struct GridPoint {
	
	void set(double x, double y, double z) {
		this->x = x;
		this->y = y;
		this->z = z;
	}

	double x, y, z;
	double transx, transy, transz;
	int projx, projy;
};

class Part1 : public MegaDemoEngine::Part {
public:
	Part1();
	void render(int time, double dtime);
private:
	GridPoint* points;
	int screenWidth;
	int screenHeight;
};

#endif