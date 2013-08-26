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
 * @file ISensorScreen.h
 * @author Bogdan Iusco
 * @date 2 Mar 2012
 *
 * @brief Interface for sensor screens.
 */

#ifndef SENSOR_ISENSOR_SCREEN_H_
#define SENSOR_ISENSOR_SCREEN_H_

namespace SensorNativeUI
{

	/**
	 * Interface for sensor screens.
	 */
	class ISensorScreen
	{
	public:
		/**
		 * Stop the sensor.
		 * Called by application controller to stop the sensor when user
		 * changes the screen from tab screen.
		 * Screens should update the UI(e.g. enable the start sensor button and
		 * disable the stop sensor button).
		 */
		virtual void stopSensor() = 0;
	};
}


#endif /* SENSOR_ISENSOR_SCREEN_H_ */
