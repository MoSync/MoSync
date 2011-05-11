/* Copyright (C) 2010 MoSync AB
 
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

/**
 * Sensor types.
 */
#define SENSOR_TYPE_ACCELEROMETER 1

/**
 * Sensor update interval general values
 */
#define SENSOR_RATE_FASTEST 0
#define SENSOR_RATE_GAME   -1
#define SENSOR_RATE_NORMAL -2
#define SENSOR_RATE_UI     -3

/**
 * Sensor update interval values in milliseconds
 */
#define SENSOR_RATE_FASTEST_IOS 20
#define SENSOR_RATE_GAME_IOS    50
#define SENSOR_RATE_NORMAL_IOS  70
#define SENSOR_RATE_UI_IOS      100

/**
 * Error codes for sensor.
 */
#define SENSOR_ERROR_NONE              0
#define SENSOR_ERROR_NOT_AVAILABLE    -2
#define SENSOR_ERROR_INTERVAL_NOT_SET -3
#define SENSOR_ERROR_ALREADY_ENABLED  -4
#define SENSOR_ERROR_NOT_ENABLED      -5


#import <UIKit/UIKit.h>

/**
 * A class for handling sensors.
 */
@interface MoSyncSensor : NSObject<UIAccelerometerDelegate> {
    /**
     * Accelerometer object.
     */
    UIAccelerometer *accelerometer;
}

/**
 * Start a sensor.
 * @param sensorType What type of sensor to start.
 * @param value Update interval value.
 * @return SENSOR_ERROR_NONE if the sensor has been started, or a code error otherwise(see the above error list).
 */
-(int) startSensor: (int)sensorType interval:(int)value;

/**
 * Stop a sensor.
 * @param sensorType What sensor to stop.
 * @return SENSOR_ERROR_NONE if the sensor has been stopped, or a code error otherwise(see the above error list).
 */
-(int) stopSensor: (int)sensorType;

/**
 * Start the accelerometer sensor.
 * @param interval Update interval value.
 * @return SENSOR_ERROR_NONE if the sensor has been started, or a code error otherwise(see the above error list).
 */
-(int) startAccelerometer:(int)interval;

/**
 * Stop the accelerometer sensor.
 * @return SENSOR_ERROR_NONE if the sensor has been stopped, or a code error otherwise(see the above error list).
 */
-(int) stopAccelerometer;

/**
 * Delivers the latest acceleration data.
 * @param accelerometer The application's accelerometer object.
 * @param acceleration The most recent acceleration data.
 */
- (void) accelerometer:(UIAccelerometer *)accelerometer didAccelerate:(UIAcceleration *)acceleration;

@end

