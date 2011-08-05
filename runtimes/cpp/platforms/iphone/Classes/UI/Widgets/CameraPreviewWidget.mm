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

#import "CameraPreviewWidget.h"
#import "MoSyncViewController.h"
#import "MoSyncAppDelegate.h"

#include "Platform.h"
#include <helpers/cpp_defs.h>
#include <helpers/CPP_IX_WIDGET.h>
#include <base/Syscall.h>

@implementation CameraPreviewWidget

@synthesize previewLayer;

- (id)init {
	
	view=[[UIPreview alloc] initWithCameraWidget:self];
	
	return [super init];	
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
	
	return @"";
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

@end

