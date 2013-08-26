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

#include <MAUtil/Moblet.h>
#include <madmath.h>
#include <conprint.h>

#include "Demo.h"

#include "Part1.h"
#include "Part2.h"

using namespace MAUtil;
using namespace MegaDemoEngine;

extern "C" int MAMain() {

	Demo demo;
	
	demo.addPart(new Part1(), 0, 100000);
	
	// add parts here
	
	int startTime = maGetMilliSecondCount();
	
	while(1) {
		int t = maGetMilliSecondCount();
		demo.render(t-startTime);
		EVENT e;
		while(maGetEvent(&e)) {
			if(e.type == EVENT_TYPE_CLOSE) {
				maExit(0);
			}
			else if(e.type == EVENT_TYPE_KEY_PRESSED) {
				if(e.key == MAK_0) {
					maExit(0);
				}
			}
		}
		maUpdateScreen();
		maResetBacklight();
	}
};
