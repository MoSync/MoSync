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

#import <UIKit/UIKit.h>


@interface ScreenWidgetController : UIViewController {

    UIInterfaceOrientation mOrientation;
}

/**
 * Send a widget orientation changed event.
 * @param eventType One of the following:
 * MAW_EVENT_SCREEN_ORIENTATION_WILL_CHANGE or MAW_EVENT_SCREEN_ORIENTATION_DID_CHANGE.
 * screenHandle The handle of the screen that will get notified.
 * @param screenOrientation The new orientation value for the specified screen.
 */
- (void)sendOrientationEvent:(const int)eventType forWidget:(const int)screenHandle toValue:(const int)screenOrientation;

@end
