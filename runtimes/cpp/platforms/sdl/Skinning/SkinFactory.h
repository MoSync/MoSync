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
		virtual DeviceSkin* createInstanceFor(DeviceProfile* dp) const = 0;
		virtual Fitness getFitnessFor(DeviceProfile* dp) const = 0;
	};

}

#endif // _SKINFACTORY_H_