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

#import "AppDelegate.h"
#import "GLView.h"
#import "StatsView.h"

@implementation AppDelegate

@synthesize window;
@synthesize glView;
@synthesize statsView;

- (void)applicationDidFinishLaunching:(UIApplication *)application {

	//allocate the view
	self.statsView = [StatsView alloc];

	glView.animationInterval = 1.0 / kRenderingFrequency;
	[glView startAnimation];

	//add the view controller's view to the window

	[window addSubview:self.statsView];

	//[window makeKeyAndVisible];
}


- (void)applicationWillResignActive:(UIApplication *)application {
	glView.animationInterval = 1.0 / kInactiveRenderingFrequency;
}


- (void)applicationDidBecomeActive:(UIApplication *)application {
	glView.animationInterval = 1.0 / 60.0;
}

- (void)dealloc {
	[window release];
	[glView release];
	[statsView release];
	[super dealloc];
}

@end
