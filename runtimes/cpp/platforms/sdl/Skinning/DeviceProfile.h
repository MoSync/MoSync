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
		DeviceProfile(const std::string& name)
			: 
			mKeyboardType(DKT_NONE),
			mName(name), 
			mScreenWidth(0),
			mScreenHeight(0)
		{}

		KeyboardType mKeyboardType;
		std::string mName;
		int mScreenWidth;
		int mScreenHeight;
	};
}

#endif // _DEVICEPROFILE_H_