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

#import "CameraPreviewWidget.h"
#import "MoSyncViewController.h"
#import "MoSyncAppDelegate.h"

#include "Platform.h"
#include <helpers/cpp_defs.h>
#include <helpers/CPP_IX_WIDGET.h>
#include <base/Syscall.h>

@implementation CameraPreviewWidget

@synthesize previewLayer;

- (id)init
{
    self = [super init];
    if (self)
    {
        self.view = [[[UIPreview alloc] initWithCameraWidget:self] autorelease];
    }
	return self;
}

- (void)dealloc
{
    [super dealloc];
}

- (int)setPropertyWithKey: (NSString*)key toValue: (NSString*)value {
	if([key isEqualToString:@"someproperty"]) {
	} else {
		return [super setPropertyWithKey:key toValue:value];
	}
	return MAW_RES_OK;
}

- (NSString*)getPropertyWithKey: (NSString*)key {
	if([key isEqualToString:@"someproperty"]) {

	} else {
		return [super getPropertyWithKey:key];
	}

	return [[NSString alloc] initWithString: @""];
}

- (void)showViewAgain{
	//Give the UI system some time to hide the view before wee show it again
	[self.view performSelector:@selector(showAgain) withObject:nil afterDelay:0.1];
}


@end

@implementation UIPreview

- (id)initWithCameraWidget:(CameraPreviewWidget*)widget
{
	mWidget = widget;
	return [super init];
}

//New implementation of the setter method for the frame property
//It will resize the preview layer whenever the frame changes
- (void)setFrame:(CGRect)newFrame
{
	super.frame = newFrame;
	if(mWidget.previewLayer){
		mWidget.previewLayer.frame = super.bounds;
	}
}

- (void)showAgain
{
	self.hidden = NO;
	[self setNeedsDisplay];
}

@end
