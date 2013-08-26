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
