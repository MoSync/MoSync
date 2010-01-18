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

#ifndef _DEVICEPROFILE_H_
#define _DEVICEPROFILE_H_

#include <string>
#include <vector>

namespace MoRE {

	/**
	 * A class representing a device profile, specialized to support
	 * rendering and interaction.
	 */
	class DeviceProfile {
	public:
		enum KeyboardType {
			DKT_NONE,
			DKT_JOYSTICK,
			DKT_KEYPAD,
			DKT_QWERTY
		};

		DeviceProfile() : 
			mKeyboardType(DKT_NONE),
			mModel(""),
			mVendor(""),
			mScreenWidth(0),
			mScreenHeight(0) {
		}

		DeviceProfile(const std::string& model, const std::string& vendor)
			: 
			mKeyboardType(DKT_NONE),
			mModel(model),
			mVendor(vendor),
			mScreenWidth(0),
			mScreenHeight(0)
		{}

		KeyboardType mKeyboardType;
		std::string mModel;
		std::string mVendor;
		int mScreenWidth;
		int mScreenHeight;
	};
}

#endif // _DEVICEPROFILE_H_