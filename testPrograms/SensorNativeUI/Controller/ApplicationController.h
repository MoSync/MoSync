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
 * @file ApplicationController.h
 * @author Bogdan Iusco
 * @date 1 Mar 2012
 *
 * @brief Application's controller.
 */

#ifndef SENSOR_APPLICATION_CONTROLLER_H_
#define SENSOR_APPLICATION_CONTROLLER_H_

#include <MAUtil/Environment.h>

#include <NativeUI/TabScreen.h>
#include <NativeUI/TabScreenListener.h>

#include "IApplicationController.h"
#include "../View/ISensorScreen.h"

namespace SensorNativeUI
{

	// Forward declarations
	class AccelerometerScreen;
	class GyroscopeScreen;
	class MagneticFieldScreen;
	class CompassScreen;
	class ProximityScreen;
	class OrientationScreen;

	/**
	 * @brief Create views.
	 * Listen for sensor events and pass them to the appropriate view.
	 */
	class ApplicationController:
		public IApplicationController,
		public MAUtil::SensorListener,
		public NativeUI::TabScreenListener
	{
	public:
		/**
		 * Constructor.
		 */
		ApplicationController();

		/**
		 * Destructor.
		 */
		virtual ~ApplicationController();

private:
		/**
		 * Tell controller to start reading a given sensor data.
		 * @param sensorType One of the SENSOR_TYPE_ constants.
		 */
		virtual void startSensor(const int sensorType);

		/**
		 * Tell controller to stop reading a given sensor data.
		 * @param sensorType One of the SENSOR_TYPE_ constants.
		 */
		virtual void stopSensor(const int sensorType);

		/**
		 * Called when a sensor has new values.
		 * @param sensorData Sensor's values.
		 */
		virtual void sensorEvent(MASensor sensorData);

		/**
		 * This method is called when a tab screen has changed to a new tab.
		 * @param tabScreen The tab screen object that generated the event.
		 * @param tabScreenIndex The index of the new tab.
		 */
		virtual void tabScreenTabChanged(
			NativeUI::TabScreen* tabScreen,
			const int tabScreenIndex);

		/**
		 * Create all contained screens.
		 */
		void createScreens();

		/**
		 * Add all contained screens to tab screen.
		 * Note: make sure that the screen objects are created before calling
		 * this method.
		 */
		void addScreensToTabScreen();

	private:
		/**
		 * Application's tab screen.
		 * All screens will be added into tab screen.
		 */
		NativeUI::TabScreen* mTabScreen;

		/**
		 * Used to display acceleration values.
		 * 1st screen in tab screen.
		 */
		AccelerometerScreen* mAccelerometerScreen;

		/**
		 * Used to display gyroscope values.
		 * 2nd screen in tab screen.
		 */
		GyroscopeScreen* mGyroscopeScreen;

		/**
		 * Used to display magnetic field sensor values.
		 * 3rd screen in tab screen.
		 */
		MagneticFieldScreen* mMagneticFieldScreen;

		/**
		 * Used to display compass sensor value.
		 * 4th screen in tab screen.
		 */
		CompassScreen* mCompassScreen;

		/**
		 * Used to display proximity sensor value.
		 * 5th screen in tab screen.
		 */
		ProximityScreen* mProximityScreen;

		/**
		 * Used to display orientation sensor value.
		 * 6th screen in tab screen.
		 */
		OrientationScreen* mOrientationScreen;

		/**
		 * Pointer to current screen interface.
		 * Used to notify the screen that it should stop the sensor because
		 * another become active.
		 * Only the current screen can have active sensor.
		 */
		ISensorScreen* mCurrentScreen;
	};

} // end of SensorNativeUI namespace

#endif /* SENSOR_APPLICATION_CONTROLLER_H_ */
