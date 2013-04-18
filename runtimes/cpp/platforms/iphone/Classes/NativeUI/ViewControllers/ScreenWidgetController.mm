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
#import "../Screens/StackScreenWidget.h"
#import "../Screens/TabScreenWidget.h"
#include "Platform.h"
#include <helpers/CPP_IX_WIDGET.h>

@interface ScreenWidgetController(hidden)

    /**
     * Send a widget orientation changed event.
     * @param eventType One of the following:
     * MAW_EVENT_SCREEN_ORIENTATION_WILL_CHANGE or MAW_EVENT_SCREEN_ORIENTATION_DID_CHANGE.
     * screenHandle The handle of the screen that will get notified.
     * @param screenOrientation The new orientation value for the specified screen.
     */
    - (void)sendOrientationEvent:(const int)eventType
					   forWidget:(const int)screenHandle
						 toValue:(const int)orientation;
@end

@implementation ScreenWidgetController

/**
 * Return a boolean value indicating whether the view controller supports the specified orientation.
 * Deprecated in iOS 6.0.
 * @param interfaceOrientation The orientation of the app’s user interface after the rotation.
 * @return YES if the view controller auto-rotates its view to the specified orientation, otherwise NO.
 */
- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation
{
    return [[ScreenOrientation getInstance] isInterfaceOrientationSupported:interfaceOrientation];
}

/**
 * Returns whether the view controller’s contents should auto rotate.
 * Available in iOS 6.0 and later.
 * @return YES.
 */
-(BOOL)shouldAutorotate
{
  return YES;
}

/**
 * Returns all of the interface orientations that the view controller supports.
 * Available in iOS 6.0 and later.
 * @return A mask with supported orientations.
 */
- (NSUInteger)supportedInterfaceOrientations
{
	UIInterfaceOrientationMask orientations = [[ScreenOrientation getInstance] supportedInterfaceOrientations];
    return orientations;
}

/**
 * Called before performing a one-step user interface rotation.
 * @param interfaceOrientation The new orientation for the user interface.
 * @param duration The duration of the pending rotation, measured in seconds.
 */
- (void)willAnimateRotationToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation
										 duration:(NSTimeInterval)duration
{
    // Store the new screen orientation.
    [[ScreenOrientation getInstance] currentOrientationChanged:interfaceOrientation];

    // Get current screen handle.
    MoSyncUI* mosyncUI = getMoSyncUI();
    IWidget* currentScreen = [mosyncUI getCurrentlyShownScreen];
    int screenHandle = [currentScreen handle];

    // Set size and coordinate values for each child widget of the current screen
    // according with the new orientation.
    [currentScreen layout];

    // Send MoSync Widget event notifying that the screen will change its orientation.
    [self sendOrientationEvent:MAW_EVENT_SCREEN_ORIENTATION_WILL_CHANGE forWidget:screenHandle toValue:0];

    // For StackScreen or TabScreen, notify all the children screens as well.
    if ([currentScreen class] == [StackScreenWidget class] ||
        [currentScreen class] == [TabScreenWidget class])
    {
        for(IWidget *widgetScreen in [currentScreen children])
        {
            [self sendOrientationEvent:MAW_EVENT_SCREEN_ORIENTATION_WILL_CHANGE
                             forWidget:[widgetScreen handle]
                               toValue:0];
        }
    }

    // Send MoSync event also, to be handled outside the NativeUI.
    MAEvent event;
    event.type = EVENT_TYPE_ORIENTATION_WILL_CHANGE;
    Base::gEventQueue.put(event);
}

/**
 * Called after the user interface rotates.
 * @param fromInterfaceOrientation The old orientation of the user interface.
 */
- (void)didRotateFromInterfaceOrientation:(UIInterfaceOrientation)fromInterfaceOrientation
{
    // Get current screen handle.
    MoSyncUI* mosyncUI = getMoSyncUI();
    IWidget* currentScreen = [mosyncUI getCurrentlyShownScreen];
    int screenHandle = [currentScreen handle];

    // Get the current screen orientation.
    int screenOrientation = [[ScreenOrientation getInstance] getCurrentScreenOrientation];

    // Send MoSync Widget event notifying that the screen changed its orientation.
    [self sendOrientationEvent:MAW_EVENT_SCREEN_ORIENTATION_DID_CHANGE
                     forWidget:screenHandle
                       toValue:screenOrientation];

    // For StackScreen or TabScreen, notify all the children screens as well.
    if ([currentScreen class] == [StackScreenWidget class] ||
        [currentScreen class] == [TabScreenWidget class])
    {
        for(IWidget *widgetScreen in [currentScreen children])
        {
            [self sendOrientationEvent:MAW_EVENT_SCREEN_ORIENTATION_DID_CHANGE
                             forWidget:[widgetScreen handle]
                               toValue:screenOrientation];
        }
    }

    // Send MoSync event notifying that a screen changed its orientation.
    MAEvent event;
    event.type = EVENT_TYPE_ORIENTATION_DID_CHANGE;
    event.orientation = screenOrientation;
    Base::gEventQueue.put(event);
}

- (void)dealloc
{
    [super dealloc];
}


@end

/**
 * Implementation for hidden methods/functions.
 */
@implementation ScreenWidgetController(hidden)

/**
 * Send a widget orientation changed event.
 * @param eventType One of the following:
 * MAW_EVENT_SCREEN_ORIENTATION_WILL_CHANGE or MAW_EVENT_SCREEN_ORIENTATION_DID_CHANGE.
 * screenHandle The handle of the screen that will get notified.
 * @param screenOrientation The new orientation value for the specified screen.
 */
- (void)sendOrientationEvent:(const int)eventType
				   forWidget:(const int)screenHandle
					 toValue:(const int)orientation
{
	MAEvent event;
	event.type = EVENT_TYPE_WIDGET;
	MAWidgetEventData *eventData = new MAWidgetEventData;
	eventData->eventType = eventType;
	eventData->widgetHandle = screenHandle;
    if (eventType == MAW_EVENT_SCREEN_ORIENTATION_DID_CHANGE )
    {
        eventData->screenOrientation = orientation;
    }
	event.data = (int)eventData;
	Base::gEventQueue.put(event);
}

@end
