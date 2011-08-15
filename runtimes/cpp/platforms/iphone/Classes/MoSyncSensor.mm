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


// sensor update interval values in milliseconds
#define SENSOR_RATE_FASTEST_IOS 20
#define SENSOR_RATE_GAME_IOS    50
#define SENSOR_RATE_NORMAL_IOS  70
#define SENSOR_RATE_UI_IOS      100

// used for converting milliseconds in seconds
#define SECOND 1000.0

#import "MoSyncSensor.h"

#include "MosyncMain.h"
#include <helpers/cpp_defs.h>

@implementation MoSyncSensor

/**
 * Init function.
 */
-(id) init {
    operationQueue = [[NSOperationQueue alloc] init];
    motionManager = [[CMMotionManager alloc] init];
    locationManager = [[CLLocationManager alloc] init];
    
    isProximitySensorRunning = FALSE;
    isOrientationSensorRunning = FALSE;
    isMagnetometerSensorRunning = FALSE;
    
    return [super init];
}

/**
 * Start a sensor.
 * @param sensorType What type of sensor to start.
 * @param value Update interval value.
 * @return SENSOR_ERROR_NONE if the sensor has been started, or a code error otherwise.
 */
-(int) startSensor: (int)sensorType  interval:(int)value {
    int result = SENSOR_ERROR_NONE;
    switch (sensorType) {
        case SENSOR_TYPE_ACCELEROMETER:
            result = [self startAccelerometer:value];
            break;  
        case SENSOR_TYPE_GYROSCOPE:
            result = [self startGyroscope:value];
            break;
        case SENSOR_TYPE_PROXIMITY:
            result = [self startProximity];
            break;    
        case SENSOR_TYPE_ORIENTATION:
            result = [self startOrientation];
            break;    
        case SENSOR_TYPE_MAGNETIC_FIELD:
            result = [self startMagnetometer];
            break;    
        default:
            result = SENSOR_ERROR_NOT_AVAILABLE;
    }
   
    return result;
}

/**
 * Stop a sensor.
 * @param sensorType What sensor to stop.
 * @return SENSOR_ERROR_NONE if the sensor has been stopped, or a code error otherwise.
 */
-(int) stopSensor: (int)sensorType {
    int result = SENSOR_ERROR_NONE;
    switch (sensorType) {
        case SENSOR_TYPE_ACCELEROMETER:
            result = [self stopAccelerometer];
            break;      
        case SENSOR_TYPE_GYROSCOPE:
            result = [self stopGyroscope];
            break;
        case SENSOR_TYPE_PROXIMITY:
            result = [self stopProximity];
            break;    
        case SENSOR_TYPE_ORIENTATION:
            result = [self stopOrientation];
            break;  
        case SENSOR_TYPE_MAGNETIC_FIELD:
            result = [self stopMagnetometer];
            break;     
        default:
            result = SENSOR_ERROR_NOT_AVAILABLE;
    }
    
    return result;
}

/**
 * Start the accelerometer sensor.
 * @param interval Update interval value.
 * @return SENSOR_ERROR_NONE if the sensor has been started, or a code error otherwise.
 */
-(int) startAccelerometer:(int)interval {
    if(nil != accelerometer) {
        return SENSOR_ERROR_ALREADY_ENABLED;
    }
    
    if(SENSOR_RATE_UI > interval) {
        return SENSOR_ERROR_INTERVAL_NOT_SET;
    }
    
    float updateValue = 0;
    switch (interval) {
        case SENSOR_RATE_FASTEST:
            updateValue = SENSOR_RATE_FASTEST_IOS;
            break;
        case SENSOR_RATE_GAME:
            updateValue = SENSOR_RATE_GAME_IOS;
            break;
        case SENSOR_RATE_NORMAL:
            updateValue = SENSOR_RATE_NORMAL_IOS;
            break;
        case SENSOR_RATE_UI:
            updateValue = SENSOR_RATE_UI_IOS;
            break;    
        default:
            updateValue = interval;
    }
    
    accelerometer = [UIAccelerometer sharedAccelerometer];
    
    // set the update interval(the value must be in seconds so we need to convert it from milliseconds).
    accelerometer.updateInterval = updateValue / SECOND;
    accelerometer.delegate = self;
    
    return SENSOR_ERROR_NONE;
}

/**
 * Stop the accelerometer sensor.
 * @return SENSOR_ERROR_NONE if the sensor has been stopped, or a code error otherwise.
 */
-(int) stopAccelerometer {
    if(nil == accelerometer) {
        return SENSOR_ERROR_NOT_ENABLED;
    }
    
    accelerometer.delegate = nil;
	[accelerometer release];
    accelerometer = nil;
    
    return SENSOR_ERROR_NONE;
}

/**
 * Start the gyroscope sensor.
 * @param interval Update interval value.
 * @return SENSOR_ERROR_NONE if the sensor has been started, or a code error otherwise.
 */
-(int) startGyroscope:(int)interval {
    if([motionManager isGyroActive]) {
        return SENSOR_ERROR_ALREADY_ENABLED;
    }
    
    if(SENSOR_RATE_UI > interval) {
        return SENSOR_ERROR_INTERVAL_NOT_SET;
    }
    
    // check if gyroscope is available.
    if([motionManager isGyroAvailable]) {
        
        // set the update interval(the value must be in seconds so we need to convert it from milliseconds).
        float updateValue = interval;
        motionManager.gyroUpdateInterval = updateValue / SECOND;
        
        // start gyroscope sensor.
        [motionManager startGyroUpdatesToQueue:operationQueue
                                   withHandler: ^(CMGyroData *gyroData, NSError *error) 
                                                  {
                                                      CMRotationRate rotate = gyroData.rotationRate;
                                                    
                                                      MAEvent event;
                                                      event.type = EVENT_TYPE_SENSOR;
                                                      event.sensor.type = SENSOR_TYPE_GYROSCOPE;
                                                      
                                                      event.sensor.values[0] = rotate.x;
                                                      event.sensor.values[1] = rotate.y;
                                                      event.sensor.values[2] = rotate.z;
                                                      
                                                      Base::gEventQueue.put(event);
                                                  }];
        
        
    } else {
        return SENSOR_ERROR_NOT_AVAILABLE;
    }
    
    return SENSOR_ERROR_NONE;
}

/**
 * Stop the gyroscope sensor.
 * @return SENSOR_ERROR_NONE if the sensor has been stopped, or a code error otherwise.
 */
-(int) stopGyroscope {
    if(![motionManager isGyroActive]) {
        return SENSOR_ERROR_NOT_ENABLED;
    }
    
    [motionManager stopGyroUpdates];    
    return SENSOR_ERROR_NONE;
}

/**
 * Start the proximity sensor.
 * @return SENSOR_ERROR_NONE if the sensor has been started, or a code error otherwise.
 */
-(int)startProximity {
    UIDevice *device = [UIDevice currentDevice];
    
    if(isProximitySensorRunning) {
        return SENSOR_ERROR_ALREADY_ENABLED;
    }   
    
    // start the proximity sensor
    device.proximityMonitoringEnabled = YES;
    
    // check if the proximity sensor is available
    if(YES == device.proximityMonitoringEnabled) {
        [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(proximityChanged) name:UIDeviceProximityStateDidChangeNotification object:nil];
    } else {
        return SENSOR_ERROR_NOT_AVAILABLE;
    }
    
    isProximitySensorRunning = TRUE;
    return SENSOR_ERROR_NONE;
}

/**
 * Stop the proximity sensor.
 * @return SENSOR_ERROR_NONE if the sensor has been stopped, or a code error otherwise.
 */
-(int)stopProximity {
    UIDevice *device = [UIDevice currentDevice];
    
    if(isProximitySensorRunning) {
        device.proximityMonitoringEnabled = NO;
        [[NSNotificationCenter defaultCenter] removeObserver:self name:UIDeviceProximityStateDidChangeNotification object:nil];
        isProximitySensorRunning = FALSE;
    } else {
        return SENSOR_ERROR_NOT_ENABLED;
    }    
    
    return SENSOR_ERROR_NONE;
}


/**
 * Start the orientation sensor.
 * @return SENSOR_ERROR_NONE if the sensor has been started, or a code error otherwise.
 */
-(int)startOrientation {
    
    if(isOrientationSensorRunning) {
        return SENSOR_ERROR_ALREADY_ENABLED;
    }   
    
    // start the orientation sensor
    UIDevice *device = [UIDevice currentDevice];
    [device beginGeneratingDeviceOrientationNotifications];
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(orientationChanged) name:UIDeviceOrientationDidChangeNotification object:nil];
    isOrientationSensorRunning = TRUE;
    
    return SENSOR_ERROR_NONE;
}

/**
 * Stop the orientation sensor.
 * @return SENSOR_ERROR_NONE if the sensor has been stopped, or a code error otherwise.
 */
-(int)stopOrientation {
    UIDevice *device = [UIDevice currentDevice];
    
    if(isOrientationSensorRunning) {
        [device endGeneratingDeviceOrientationNotifications];
        [[NSNotificationCenter defaultCenter] removeObserver:self name:UIDeviceOrientationDidChangeNotification object:nil];
        isOrientationSensorRunning = FALSE;
    } else {
        return SENSOR_ERROR_NOT_ENABLED;
    }    
    
    return SENSOR_ERROR_NONE;
}

/**
 * Start the magnetometer sensor.
 * @return SENSOR_ERROR_NONE if the sensor has been started, or a code error otherwise.
 */
-(int)startMagnetometer {
    if(![CLLocationManager headingAvailable]) {
        return SENSOR_ERROR_NOT_AVAILABLE;
    }
    
    if(isMagnetometerSensorRunning) {
        return SENSOR_ERROR_ALREADY_ENABLED;
    }   
    
    // start the magnetometer sensor
    locationManager.delegate = self;
    [locationManager startUpdatingHeading];
    isMagnetometerSensorRunning = TRUE;
    
    return SENSOR_ERROR_NONE;
}

/**
 * Stop the magnetometer sensor.
 * @return SENSOR_ERROR_NONE if the sensor has been stopped, or a code error otherwise.
 */
-(int)stopMagnetometer {
    if(isMagnetometerSensorRunning) {
        locationManager.delegate = self;
        [locationManager stopUpdatingHeading];
        isMagnetometerSensorRunning = FALSE;
    } else {
        return SENSOR_ERROR_NOT_ENABLED;
    }
    
    return SENSOR_ERROR_NONE;
}

/**
 * Delivers the latest acceleration data.
 * @param accelerometer The application's accelerometer object.
 * @param acceleration The most recent acceleration data.
 */
- (void) accelerometer:(UIAccelerometer *)accelerometer didAccelerate:(UIAcceleration *)acceleration {
	MAEvent event;
	event.type = EVENT_TYPE_SENSOR;
	event.sensor.type = SENSOR_TYPE_ACCELEROMETER;
    
	event.sensor.values[0] = acceleration.x;
	event.sensor.values[1] = acceleration.y;
	event.sensor.values[2] = acceleration.z;
    
	Base::gEventQueue.put(event);
}

/**
 * Delivers the latest data from proximity sensor.
 */
-(void)proximityChanged {
    UIDevice *device = [UIDevice currentDevice];
    
    float sensorValue;
    if([device proximityState]) {
        sensorValue = SENSOR_PROXIMITY_VALUE_NEAR;
    } else {
        sensorValue = SENSOR_PROXIMITY_VALUE_FAR;    
    }
    
    MAEvent event;
	event.type = EVENT_TYPE_SENSOR;
	event.sensor.type = SENSOR_TYPE_PROXIMITY;
	event.sensor.values[0] = sensorValue;
	Base::gEventQueue.put(event);
}
 
/**
 * Delivers the latest data from orientation sensor.
 */
-(void)orientationChanged {
    UIDevice *device = [UIDevice currentDevice];
    float sensorValue = [device orientation];
    
    MAEvent event;
	event.type = EVENT_TYPE_SENSOR;
	event.sensor.type = SENSOR_TYPE_ORIENTATION;
	event.sensor.values[0] = sensorValue;
	Base::gEventQueue.put(event);
}

/**
 * This delegate method is invoked when the magnetometer sensor has new data.
 * @param manager The location manager object.
 * @param heading The new heading data.
 */
 - (void)locationManager:(CLLocationManager *)manager didUpdateHeading:(CLHeading *)heading {
    MAEvent event;
	event.type = EVENT_TYPE_SENSOR;
    event.sensor.type = SENSOR_TYPE_MAGNETIC_FIELD;
     
	event.sensor.values[0] = heading.x;
    event.sensor.values[1] = heading.y;
    event.sensor.values[2] = heading.z;
	
    Base::gEventQueue.put(event);
}    

/**
 * Release all the objects.
 */
- (void) dealloc {
    // stop all sensors
    [self stopAccelerometer];
    [self stopGyroscope];
    [self stopProximity];
    [self stopOrientation];
    [self stopMagnetometer];
    
    [accelerometer release];
    [motionManager release];
    [operationQueue release];
    [locationManager release];
    
    [super dealloc];
 }

@end
