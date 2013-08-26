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

#import "MoSyncLocation.h"
#import "Platform.h"

static MoSyncCLController *sLocationController = NULL;

MoSyncCLController* getLocationController()
{
	if (!sLocationController)
	{
		sLocationController = [[MoSyncCLController alloc] init];
	}
	return sLocationController;
}

// TODO Redo this so that it returns true availability
// (depending on the users accept of sharing locaation).
int maLocationStart()
{
    [[NSOperationQueue mainQueue] addOperationWithBlock:^ {
        MoSyncCLController* locationController = getLocationController();
        [locationController.locationManager startUpdatingLocation];
    }];
	return MA_LPS_AVAILABLE;
}

int maLocationStop()
{
    [[NSOperationQueue mainQueue] addOperationWithBlock:^ {
        MoSyncCLController* locationController = getLocationController();
        [locationController.locationManager stopUpdatingLocation];
    }];
	return 0;
}

@implementation MoSyncCLController

@synthesize locationManager;

- (id) init {
    self = [super init];
    if (self != nil) {
            self.locationManager = [[[CLLocationManager alloc] init] autorelease];
            [self.locationManager setDelegate:self];
    }
    return self;
}

- (void)locationManager:(CLLocationManager *)manager
    didUpdateToLocation:(CLLocation *)newLocation
           fromLocation:(CLLocation *)oldLocation
{
	MAEvent event;
	event.type = EVENT_TYPE_LOCATION;
	MALocation* location = new MALocation;
	event.data = (int)location;

	location->state = MA_LOC_QUALIFIED;
	location->lat = newLocation.coordinate.latitude;
	location->lon = newLocation.coordinate.longitude;
	location->horzAcc = newLocation.horizontalAccuracy;
	location->vertAcc = newLocation.verticalAccuracy;
	location->alt = newLocation.altitude;
	Base::gEventQueue.put(event);
}

- (void)locationManager:(CLLocationManager *)manager
	   didFailWithError:(NSError *)error
{
	//NSLog(@"Error: %@", [error description]);
	MAEvent event;
	event.type = EVENT_TYPE_LOCATION_PROVIDER;

	if(error.code == kCLErrorLocationUnknown) {
		event.state = MA_LPS_TEMPORARILY_UNAVAILABLE;

	} else if(error.code == kCLErrorDenied) {
		event.state = MA_LPS_OUT_OF_SERVICE;
	} else if(error.code == kCLErrorHeadingFailure) {
		return;
	}
	Base::gEventQueue.put(event);
}

- (void)dealloc {
    [self.locationManager release];
    [super dealloc];
}

@end