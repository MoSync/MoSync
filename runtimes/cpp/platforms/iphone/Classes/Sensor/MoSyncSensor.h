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

#import <UIKit/UIKit.h>
#import <CoreMotion/CoreMotion.h>
#import <CoreLocation/CoreLocation.h>

/**
 * A class for handling sensors.
 */
@interface MoSyncSensor : NSObject<UIAccelerometerDelegate> {
    /**
     * Accelerometer object.
     */
    UIAccelerometer *accelerometer;

    /**
     * Used to access motion data.
     */
    CMMotionManager* motionManager;

    /**
     * Used a timer for reading gyro data at a given time interval.
     */
    dispatch_source_t motionManagerTimer;

    /**
     * Used to acces data from the magnetometer(compass) sensor.
     */
    CLLocationManager *locationManager;

    /**
     * CLLocationManager does not offer a way of setting an update interval.
     * Used a timer for reading location data at a given time interval.
     * This timer is used for the magnetic field sensor
     */
    dispatch_source_t locationManagerTimer;

    /**
     * CLLocationManager does not offer a way of setting an update interval.
     * Used a timer for reading location data at a given time interval.
     * This timer is used for the compass sensor
     */
    dispatch_source_t compassManagerTimer;

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

    /**
     * The flag is set is the heading sensor is started.
     */
    BOOL isCompassRunning;
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
 * @param interval How fast to read data(time interval in milliseconds).
 * @return SENSOR_ERROR_NONE if the sensor has been started, or a code error otherwise.
 */
-(int)startMagnetometer:(const int)interval;

/**
 * Stop the magnetometer sensor.
 * @return SENSOR_ERROR_NONE if the sensor has been stopped, or a code error otherwise.
 */
-(int) stopMagnetometer;

/**
 * Start the heading sensor.
 * @param interval How fast to read data(time interval in milliseconds).
 * @return SENSOR_ERROR_NONE if the sensor has been started, or a code error otherwise.
 */
-(int)startCompass:(const int)interval;

/**
 * Stop the compass.
 * @return SENSOR_ERROR_NONE if the sensor has been stopped, or a code error otherwise.
 */
-(int) stopCompass;

/**
 * Get the update interval associated with a rate constant.
 * @param rate One of the next constants:
 * - SENSOR_RATE_FASTEST
 * - SENSOR_RATE_GAME
 * - SENSOR_RATE_NORMAL
 * - SENSOR_RATE_UI
 * @return The update interval associated with the rate, or rate parameter
 * if it's not one of the above constants.
 */
-(int) getUpdateIntervalFromRate:(const int) rate;

@end
