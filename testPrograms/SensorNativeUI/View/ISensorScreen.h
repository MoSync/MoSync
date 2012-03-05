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
