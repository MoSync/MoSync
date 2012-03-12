/* Copyright (C) 2011 MoSync AB

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

#import "ScreenWidgetController.h"
#import "ScreenOrientation.h"
#import "MoSyncUI.h"
#import "MoSyncUISyscalls.h"
#include "Platform.h"
#include <helpers/CPP_IX_WIDGET.h>

@implementation ScreenWidgetController

/*
 // The designated initializer.  Override if you create the controller programmatically and want to perform customization that is not appropriate for viewDidLoad.
- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil {
    if ((self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil])) {
        // Custom initialization
    }
    return self;
}
*/

/*
// Implement loadView to create a view hierarchy programmatically, without using a nib.
- (void)loadView {
}
*/

/*
// Implement viewDidLoad to do additional setup after loading the view, typically from a nib.
- (void)viewDidLoad {
    [super viewDidLoad];
}
*/

// Override to allow orientations other than the default portrait orientation.
- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation
{
    // Return YES for supported orientations
    return [[ScreenOrientation getInstance] isInterfaceOrientationSupported:interfaceOrientation];
}


- (void)didReceiveMemoryWarning {
    // Releases the view if it doesn't have a superview.
    [super didReceiveMemoryWarning];

    // Release any cached data, images, etc that aren't in use.
}

- (void)viewDidUnload {
    [super viewDidUnload];
    // Release any retained subviews of the main view.
    // e.g. self.myOutlet = nil;
}

/**
 * Called before performing a one-step user interface rotation.
 * @param interfaceOrientation The new orientation for the user interface.
 * @param duration The duration of the pending rotation, measured in seconds.
 */
- (void)willAnimateRotationToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation duration:(NSTimeInterval)duration
{
    // Store the new screen orientation.
    [[ScreenOrientation getInstance] currentOrientationChanged:interfaceOrientation];

    // Get current screen handle.
    MoSyncUI* mosyncUI = getMoSyncUI();
    IWidget* currentScreen = [mosyncUI getCurrentlyShownScreen];
    int screenHandle = [currentScreen getWidgetHandle];

    // Set size and coordinate values for each child widget of the current screen
    // according with the new orientation.
    [currentScreen layout];

    // Send MoSync Widget event notifying that the screen will change its orientation.
    MAEvent event;
	event.type = EVENT_TYPE_WIDGET;
	MAWidgetEventData *eventData = new MAWidgetEventData;
	eventData->eventType = MAW_EVENT_SCREEN_ORIENTATION_WILL_CHANGE;
	eventData->widgetHandle = screenHandle;
	event.data = (int)eventData;
	Base::gEventQueue.put(event);
}

- (void)dealloc {
    [super dealloc];
}


@end
