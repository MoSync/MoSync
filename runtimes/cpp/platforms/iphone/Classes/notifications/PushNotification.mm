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
 * @file PushNotification.mm
 * @author Bogdan Iusco
 * @date 1 Nov 2011
 *
 * @brief Store the push notification JSON data received from APNS.
 */

#import "PushNotification.h"

@implementation PushNotification

@synthesize alertMessage = mAlertMessage;
@synthesize soundFileName = mSoundFileName;
@synthesize badgeIcon = mBadgeIcon;

/**
 * Init function.
 */
- (id)init
{
    return [super init];
}

/**
 * Deallocate member objects.
 */
-(void) dealloc
{
    [mAlertMessage release];
    [mSoundFileName release];
    [mBadgeIcon release];

    [super dealloc];
}

@end
