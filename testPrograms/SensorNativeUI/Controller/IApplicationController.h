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
 * @file IApplicationController.h
 * @author Bogdan Iusco
 * @date 1 Mar 2012
 *
 * @brief Interface for application controller.
 */

#ifndef SENSOR_IAPPLICATION_CONTROLLER_H_
#define SENSOR_IAPPLICATION_CONTROLLER_H_

namespace SensorNativeUI
{

	/**
	 * Interface for controller.
	 */
	class IApplicationController
	{
	public:
		/**
		 * Tell controller to start reading a given sensor data.
		 * @param sensorType One of the SENSOR_TYPE_ constants.
		 */
		virtual void startSensor(const int sensorType) = 0;

		/**
		 * Tell controller to stop reading a given sensor data.
		 * @param sensorType One of the SENSOR_TYPE_ constants.
		 */
		virtual void stopSensor(const int sensorType) = 0;

	};

} // end of SensorNativeUI

#endif /* SENSOR_IAPPLICATION_CONTROLLER_H_ */
