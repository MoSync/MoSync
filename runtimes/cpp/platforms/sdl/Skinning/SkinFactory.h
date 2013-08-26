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

#ifndef _SKINFACTORY_H_
#define _SKINFACTORY_H_

#include "DeviceSkin.h"

namespace MoRE {

	/**
	 * An abstract interface for skin factories, i.e. classes that 
	 * evaluate how suitable their associated skin is for 
	 * representing a certain device. This is dubbed the "fitness" 
	 * of the skin, and comes in three levels - NONE, COMPATIBLE
	 * and EXACT. 'NONE' means the skin is totally unsuitable for 
	 * representing the device in question. COMPATIBLE means that
	 * it can be used, but does not provide visual fidelity with 
	 * respect to the physical device being modeled. EXACT
	 * means it is designed specifically for representing the
	 * device in question. It is also capable of creating instances
	 * of said associated skin.
	 */
	class SkinFactory {
	public:
		enum Fitness {
			SKIN_FITNESS_NONE = 0,
			SKIN_FITNESS_COMPATIBLE,
			SKIN_FITNESS_EXACT
		};
		virtual DeviceSkin* createInstanceFor(const DeviceProfile* dp) const = 0;
		virtual Fitness getFitnessFor(const DeviceProfile* dp) const = 0;
	};

}

#endif // _SKINFACTORY_H_
