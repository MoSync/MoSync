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