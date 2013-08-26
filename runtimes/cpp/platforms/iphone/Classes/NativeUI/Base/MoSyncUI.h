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

// shows a screen with a transition.
- (int)show: (IWidget*) widget withTransitionType: (NSNumber*) transitionType
                            andTransitionDuration: (NSNumber*) transitionDuration;

// shows a modal view controller
- (void) showModal: (UINavigationController*)controller;

// hides a view controller that was shown modally
- (void) hideModal;

@end
