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

namespace MegaDemoEngine {

	void Demo::addPart(Part* dp, int startTime, int endTime) {
		instances.add(new PartInstance(dp, startTime, endTime));
	}
	
	
	void Demo::render(int time) {
		Vector_each(PartInstance*, p, instances) {
			if(time >= (*p)->startTime && time <= (*p)->endTime) {
				int iTime = time - (*p)->startTime;
				(*p)->part->render(iTime, iTime*0.001);
			}
		}
	}


}