/* Copyright (C) 2009 Mobile Sorcery AB

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License, version 2, as published by
the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

You should have received a copy of the GNU General Public License
along with this program; see the file COPYING.  If not, write to the Free
Software Foundation, 59 Temple Place - Suite 330, Boston, MA
02111-1307, USA.
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
