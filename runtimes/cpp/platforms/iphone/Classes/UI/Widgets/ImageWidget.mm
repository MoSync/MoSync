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

#import "ImageWidget.h"
#include <helpers/cpp_defs.h>
#include <helpers/CPP_IX_WIDGET.h>
#include "Platform.h"
#include <base/Syscall.h>

@implementation ImageWidget

- (id)init {
    
    // temporarily use a UIView placeholder until we have an image..
	view = [[[UIView alloc] initWithFrame:CGRectMake(0, 0, 100, 60)] retain];
	imageView = nil;
	leftCapWidth = 0;
	topCapHeight = 0;
	id ret = [super init];
	[self setAutoSizeParamX:WRAP_CONTENT andY:WRAP_CONTENT];
    view.contentMode = UIViewContentModeCenter;
	return ret;
}

- (int)setPropertyWithKey: (NSString*)key toValue: (NSString*)value {
	if([key isEqualToString:@"image"]) {
		int imageHandle = [value intValue];
		if(imageHandle<=0) return MAW_RES_INVALID_PROPERTY_VALUE;
		Surface* imageResource = Base::gSyscall->resources.get_RT_IMAGE(imageHandle);
		UIImage* image = [UIImage imageWithCGImage:imageResource->image];
		if(leftCapWidth != 0 || topCapHeight != 0) {
			image = [image stretchableImageWithLeftCapWidth:leftCapWidth topCapHeight:topCapHeight];
		}
		if(imageView != nil)
			[imageView removeFromSuperview];

		imageView = [[UIImageView alloc] initWithImage:image];	
		
        //[view addSubview:imageView];
        
        [view.superview insertSubview:imageView belowSubview:view];
        [view removeFromSuperview];
        imageView.contentMode = view.contentMode;
        [view release];
        view = imageView;
        //view.contentMode = UIViewContentModeCenter;
        
		[self layout];
	}
	else if([key isEqualToString:@"leftCapWidth"]) {
		int newLeftCapWidth = [value intValue];
		if(imageView != nil) {
			UIImage* image = [imageView.image stretchableImageWithLeftCapWidth:newLeftCapWidth topCapHeight:topCapHeight];
			imageView.image = image;
		}
		leftCapWidth = newLeftCapWidth;
	}
	else if ([key isEqualToString:@"topCapHeight"]) {
		int newTopCapHeight = [value intValue];
		if(imageView != nil) {
			UIImage* image = [imageView.image stretchableImageWithLeftCapWidth:leftCapWidth topCapHeight:newTopCapHeight];
			imageView.image = image;
		}
		topCapHeight = newTopCapHeight;
	}
    else if ([key isEqualToString:@"scaleMode"]) {
        // none
        // scaleXY
        // scalePreserveAspect
        
        // maybe have these later?
        // scaleX
        // scaleY
        
        if([value isEqualToString:@"none"]) view.contentMode = UIViewContentModeCenter;
        else if([value isEqualToString:@"scaleXY"]) view.contentMode = UIViewContentModeScaleToFill;
        else if([value isEqualToString:@"scalePreserveAspect"]) view.contentMode = UIViewContentModeScaleAspectFit;        
    }
	else {
		return [super setPropertyWithKey:key toValue:value];
	}
	return MAW_RES_OK;
}

- (NSString*)getPropertyWithKey: (NSString*)key {
	
	return [super getPropertyWithKey:key];
}

@end
