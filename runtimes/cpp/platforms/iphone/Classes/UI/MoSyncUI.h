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

/*
 * This is the main obj-c interface for the native ui api. It should be executing on the main thread, so 
 * everything calling it must use performSelectorOnMainThread when invoking methods on an instance.
 * Most things can be performed directly on the widget instances themselves. But they still need to be 
 * performed on the main thread. See MoSyncUISyscalls.* for more information.
 */

#import <Foundation/Foundation.h>
#import "IWidget.h"

@interface MoSyncUI : NSObject {

}

// Parameters are the main MoSync window and UIViewController.
- (id)initWithWindow: (UIWindow*) window andController: (UIViewController*)controller;

// close the api.
- (void)close;

// returns increasing handles beginning at 0
- (int)createWidget: (NSString*)name;

// get a pointer to a widget instance by passing a handle.
- (IWidget*)getWidget: (int) handle;

- (IWidget*)getCurrentlyShownScreen;

// destroy a widget instance by passing a pointer to it.
- (int) destroyWidgetInstance:(IWidget*)widget;

// shows a screen.
- (int)show: (IWidget*) handle;

// shows a modal view controller
- (void) showModal: (UIViewController*)controller;

// hides a view controller that was shown modally
- (void) hideModal;

@end
