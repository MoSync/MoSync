#ifndef _DEVICESKIN_H_
#define _DEVICESKIN_H_

#include <string>
#include <vector>

#include "DeviceProfile.h"

namespace MoRE {

	/**
	 * An abstract interface for the presentation and input handling
	 * of a device being "emulated". 
	 */
	class DeviceSkin {
	public:
		DeviceSkin(DeviceProfile* profile) : mProfile(profile) {}
		virtual int getWindowWidth() = 0;
		virtual int getWindowHeight() = 0;
		virtual void drawDevice() = 0;
		virtual void rotateCW() = 0;
		virtual void rotateCCW() = 0;
		virtual void mouseMoved(int x, int y) = 0;
		virtual void mousePressed(int x, int y, int button) = 0;
		virtual void mouseReleased(int x, int y, int button) = 0;
	protected:
		DeviceProfile* mProfile;
	};
}
#endif // _DEVICEPSKIN_H_