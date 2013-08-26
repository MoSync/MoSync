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

#include "Demo.h"
#include <ma.h>

struct Planet {
	int x, y, z;
	int startz;
	int type;
};

#define NUM_PLANET_BUCKETS 64
#define NUM_PLANETS 32

class Part2 : public MegaDemoEngine::Part {
public:
	Part2();
	void render(int time, double dtime);
private:
	// image->zoom
	Handle planetImages[4][4];
};
