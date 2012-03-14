/*
 Copyright (C) 2012 MoSync AB

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License,
 version 2, as published by the Free Software Foundation.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 MA 02110-1301, USA.
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
