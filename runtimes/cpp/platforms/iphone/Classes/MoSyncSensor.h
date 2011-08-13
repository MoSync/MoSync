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

#import <UIKit/UIKit.h>
#import <CoreMotion/CoreMotion.h>
#import <CoreLocation/CoreLocation.h>
 
/**
 * A class for handling sensors.
 */
@interface MoSyncSensor : NSObject<UIAccelerometerDelegate, CLLocationManagerDelegate> {
    /**
     * Accelerometer object.
     */
    UIAccelerometer *accelerometer;
    
    /**
     * Used to access motion data.
     */
    CMMotionManager* motionManager;
    
    /**
     * Used by the gyroscope sensor to process data.
     */
    NSOperationQueue *operationQueue;
    
    /**
     * Used to acces data from the magnetometer(compass) sensor.
     */
    CLLocationManager *locationManager;
    
    /**
     * The flag is set is the proximity sensor is started.
     */
    BOOL isProximitySensorRunning;
    
    /**
     * The flag is set is the orientation sensor is started.
     */
    BOOL isOrientationSensorRunning;
    
    /**
     * The flag is set is the magnetometer sensor is started.
     */
    BOOL isMagnetometerSensorRunning;
}

/**
 * Init function.
 */
-(id) init;

/**
 * Start a sensor.
 * @param sensorType What type of sensor to start.
 * @param value Update interval value.
 * @return SENSOR_ERROR_NONE if the sensor has been started, or a code error otherwise.
 */
-(int) startSensor: (int)sensorType interval:(int)value;

/**
 * Stop a sensor.
 * @param sensorType What sensor to stop.
 * @return SENSOR_ERROR_NONE if the sensor has been stopped, or a code error otherwise.
 */
-(int) stopSensor: (int)sensorType;

/**
 * Start the accelerometer sensor.
 * @param interval Update interval value.
 * @return SENSOR_ERROR_NONE if the sensor has been started, or a code error otherwise.
 */
-(int) startAccelerometer:(int)interval;

/**
 * Stop the accelerometer sensor.
 * @return SENSOR_ERROR_NONE if the sensor has been stopped, or a code error otherwise.
 */
-(int) stopAccelerometer;

/**
 * Start the gyroscope sensor.
 * @param interval Update interval value.
 * @return SENSOR_ERROR_NONE if the sensor has been started, or a code error otherwise.
 */
-(int)startGyroscope:(int)interval;

/**
 * Stop the gyroscope sensor.
 * @return SENSOR_ERROR_NONE if the sensor has been stopped, or a code error otherwise.
 */
-(int) stopGyroscope;

/**
 * Start the proximity sensor.
 * @return SENSOR_ERROR_NONE if the sensor has been started, or a code error otherwise.
 */
-(int)startProximity;

/**
 * Stop the proximity sensor.
 * @return SENSOR_ERROR_NONE if the sensor has been stopped, or a code error otherwise.
 */
-(int) stopProximity;

/**
 * Start the proximity sensor.
 * @return SENSOR_ERROR_NONE if the sensor has been started, or a code error otherwise.
 */
-(int)startOrientation;

/**
 * Stop the orientation sensor.
 * @return SENSOR_ERROR_NONE if the sensor has been stopped, or a code error otherwise.
 */
-(int) stopOrientation;

/**
 * Start the magnetometer sensor.
 * @return SENSOR_ERROR_NONE if the sensor has been started, or a code error otherwise.
 */
-(int)startMagnetometer;

/**
 * Stop the magnetometer sensor.
 * @return SENSOR_ERROR_NONE if the sensor has been stopped, or a code error otherwise.
 */
-(int) stopMagnetometer;

/**
 * Delivers the latest acceleration data.
 * @param accelerometer The application's accelerometer object.
 * @param acceleration The most recent acceleration data.
 */
- (void) accelerometer:(UIAccelerometer *)accelerometer didAccelerate:(UIAcceleration *)acceleration;

@end

