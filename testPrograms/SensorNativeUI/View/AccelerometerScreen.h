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

/**
 * @file AccelerometerScreen.h
 * @author Bogdan Iusco
 * @date 1 Mar 2012
 *
 * @brief Screen used to show data received from accelerometer.
 */

#ifndef SENSOR_ACCELEROMETER_SCREEN_H_
#define SENSOR_ACCELEROMETER_SCREEN_H_

#include <MAUtil/String.h>

#include <NativeUI/Screen.h>
#include <NativeUI/Button.h>
#include <NativeUI/ButtonListener.h>
#include <NativeUI/Label.h>
#include <NativeUI/VerticalLayout.h>

#include "ISensorScreen.h"
#include "../Controller/IApplicationController.h"

namespace SensorNativeUI
{

	/**
	 * @brief Screen used to display accelerometer data.
	 * Also it contains buttons for starting and stopping the
	 * accelerometer sensor.
	 */
	class AccelerometerScreen :
		public NativeUI::Screen,
		public NativeUI::ButtonListener,
		public ISensorScreen
	{
	public:
		/**
		 * Constructor.
		 * @param applicationController Interface to controller.
		 */
		AccelerometerScreen(IApplicationController& applicationController);

		/**
		 * Destructor.
		 */
		~AccelerometerScreen();

		/**
		 * Set the new acceleration data.
		 * @param x Acceleration x value.
		 * @param y Acceleration y value.
		 * @param z Acceleration z value.
		 */
		void setAccelerationData(const float x, const float y, const float z);

		/**
		 * Stop the sensor.
		 * Called by application controller to stop the sensor when user
		 * changes the screen from tab screen.
		 * Screens should update the UI(e.g. enable the start sensor button and
		 * disable the stop sensor button).
		 */
		virtual void stopSensor();

	private:
		/**
		 * Create widgets and add them to screen.
		 */
		void createUI();

		/**
		 * Create a horizontal layout that will contain a first label with a
		 * given text and a seconds label given as argument. Layout will be
		 * added to screen.
		 * @param dataLabel Second label that will be added to layout.
		 * Object's ownership is passed to this method.
		 * @param firstLabelText Text that will be set to the first label.
		 */
		void createHorizontalWidgetsRow(NativeUI::Label* dataLabel,
			const MAUtil::String& firstLabelText);

		/**
		 * This method is called if the touch-up event was inside the
		 * bounds of the button.
		 * Platform: iOS, Android, Windows Phone.
		 * @param button The button object that generated the event.
		 */
		virtual void buttonClicked(NativeUI::Widget* button);

	private:
		/**
		 * Interface to controller.
		 */
		IApplicationController& mApplicationController;

		/**
		 * Screen's main layput.
		 * All contained widgets will be added to this layout.
		 */
		NativeUI::VerticalLayout* mMainLayout;

		/**
		 * Label that contains x accelerometer value.
		 */
		NativeUI::Label* mValueXLabel;

		/**
		 * Label that contains y accelerometer value.
		 */
		NativeUI::Label* mValueYLabel;

		/**
		 * Label that contains z accelerometer value.
		 */
		NativeUI::Label* mValueZLabel;

		/**
		 * Button used to start reading accelerometer data.
		 */
		NativeUI::Button* mStartAccelerometerButton;

		/**
		 * Button used to stop reading accelerometer data.
		 */
		NativeUI::Button* mStopAccelerometerButton;
	};
} // end of SensorNativeUI

#endif /* SENSOR_ACCELEROMETER_SCREEN_H_ */
