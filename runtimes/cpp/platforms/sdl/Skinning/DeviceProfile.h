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
