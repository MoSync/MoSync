#import "MoSyncCLController.h"
#include "MoSyncMain.h"

@implementation MoSyncCLController

@synthesize locationManager;

- (id) init {
    self = [super init];
    if (self != nil) {
        self.locationManager = [[[CLLocationManager alloc] init] autorelease];
        self.locationManager.delegate = self; // send loc updates to myself
    }
    return self;
}

- (void)locationManager:(CLLocationManager *)manager
    didUpdateToLocation:(CLLocation *)newLocation
           fromLocation:(CLLocation *)oldLocation
{
    NSLog(@"Location: %@", [newLocation description]);
	
	MAEvent event;
	event.type = EVENT_TYPE_LOCATION;
	MALocation* location = new MALocation;
	event.data = location;
	
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