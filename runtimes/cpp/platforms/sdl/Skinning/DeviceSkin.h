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

#ifndef _DEVICESKIN_H_
#define _DEVICESKIN_H_

#include <string>
#include <vector>
//#include <map>
//#include <utility>

#include "DeviceProfile.h"

namespace MoRE {

	/**
	 * An abstract interface for the presentation and input handling
	 * of a device being "emulated". 
	 */
	class DeviceSkin {
	public:
		class Listener {
		public:
			virtual void onMoSyncKeyPress(int mak) = 0;
			virtual void onMoSyncKeyRelease(int mak) = 0;
			virtual void onMoSyncPointerPress(int x, int y, int touchId) = 0;
			virtual void onMoSyncPointerDrag(int x, int y, int touchId) = 0;
			virtual void onMoSyncPointerRelease(int x, int y, int touchId) = 0;
		};

		DeviceSkin(const DeviceProfile* profile) : mProfile(profile), mListener(0) {}
		virtual ~DeviceSkin() {}
		virtual bool init() = 0;
		virtual int getWindowWidth() const = 0;
		virtual int getWindowHeight() const = 0;
		virtual void drawDevice() const = 0;
		virtual void drawScreen() const = 0;
		/**
		* If activated, draws two markers to simulate multi touch
		*/
		virtual void drawMultiTouchSimulation() const = 0;
		virtual void rotateCW() = 0;
		virtual void rotateCCW() = 0;
		virtual void mouseDragged(int x, int y) = 0;
		virtual void mousePressed(int x, int y) = 0;
		virtual void mouseReleased(int x, int y) = 0;
		/**
		* Adds a second mirrored touch so that multi touch
		* can be tested in MoRE.
		* Should send two events, one for each touch
		*/
		virtual void mouseMultiDragged(int x, int y) = 0;
		/**
		* Adds a second mirrored touch so that multi touch
		* can be tested in MoRE.
		* Should send two events, one for each touch
		*/
		virtual void mouseMultiPressed(int x, int y) = 0;
		/**
		* Adds a second mirrored touch so that multi touch
		* can be tested in MoRE.
		* Should send two events, one for each touch
		*/
		virtual void mouseMultiReleased(int x, int y) = 0;
		virtual int getScreenLeft() const = 0;
		virtual int getScreenTop() const = 0;
		virtual int getScreenWidth() const = 0;
		virtual int getScreenHeight() const = 0;

		// used to update the skin when pressing physical keys
		virtual void keyPressed(int mak) = 0;
		virtual void keyReleased(int mak) = 0;

		void setListener(Listener* listener) { mListener = listener; }
	protected:
		const DeviceProfile* mProfile;
		Listener* mListener;

		/**
		* Containing the last two known positions when simulating
		* multi touch
		*/
		std::pair <int, int> mLastKnownMousePosition[2];

		/**
		*
		*/
		bool mIsSimulatingMultiTouch;
	};
}
#endif // _DEVICESKIN_H_
