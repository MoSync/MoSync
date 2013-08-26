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

//
//  MoSyncUI.h
//  nativeuitest
//
//  Created by Niklas Nummelin on 11/26/10.
//

#import <Foundation/Foundation.h>
#import "IWidget.h"

@interface MoSyncUI : NSObject {

}

- (id)initWithWindow: (UIWindow*) window andController: (UIViewController*)controller;
- (void)close;
- (void)createWidget: (NSString*)name; // increasing handles beginning at 0
- (IWidget*)getWidget: (int) handle;
- (void)removeWidget: (IWidget*) handle;
//- (void)addChild: (IWidget*)child toParent:(IWidget*)parent;
- (void)show: (IWidget*) handle;

@end
