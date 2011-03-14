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
#ifndef NATIVE_TEST
#include "Platform.h"
#include <base/Syscall.h>
#endif

@implementation ImageWidget

- (id)init {	
	view = [[[UIView alloc] initWithFrame:CGRectMake(0, 0, 100, 60)] retain];
	imageView = nil;
	leftCapWidth = 0;
	topCapHeight = 0;
	id ret = [super init];
	[self setAutoSizeParamX:WRAP_CONTENT andY:WRAP_CONTENT];
	return ret;
}

- (void)addChild: (IWidget*)child {
	[super addChild:child];
}

- (int)setPropertyWithKey: (NSString*)key toValue: (NSString*)value {
	if([key isEqualToString:@"image"]) {
		int imageHandle = [value intValue];
		if(imageHandle<=0) return MAW_RES_INVALID_HANDLE;
		#ifndef NATIVE_TEST
		Surface* imageResource = Base::gSyscall->resources.get_RT_IMAGE(imageHandle);
		UIImage* image = [UIImage imageWithCGImage:imageResource->image];
		if(leftCapWidth != 0 || topCapHeight != 0) {
			image = [image stretchableImageWithLeftCapWidth:leftCapWidth topCapHeight:topCapHeight];
		}
		if(imageView != nil)
			[imageView removeFromSuperview];

		imageView = [[UIImageView alloc] initWithImage:image];	
		[imageView sizeToFit];
		[view addSubview:imageView];
		//view.frame.size.height = imageView.frame.size.height;
		//view.frame.size.width = imageView.frame.size.width;
		[self layout];
		#endif
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
	/*
	else if ([key isEqualToString:@"width"]) {
		int ret = [super setPropertyWithKey:key toValue:value];
		float width = [value floatValue];
		if(width == -2 || width == -1) {
			autoSizeParamX = width==-2?WRAP_CONTENT:FILL_PARENT;
			//[self layout];
			//return MAW_RES_OK;
			width = (autoSizeParamX==FILL_PARENT)?imageView.superview.frame.size.width:[imageView sizeThatFits:CGSizeZero].width;
			
		} else {
			autoSizeParamX = FIXED_SIZE;
		}
		[imageView setFrame:CGRectMake(imageView.frame.origin.x, imageView.frame.origin.y, width, imageView.frame.size.height)];
		return [super setPropertyWithKey:key toValue:value];
	}
	else if ([key isEqualToString:@"height"]) {
		int ret = [super setPropertyWithKey:key toValue:value];
		float height = [value floatValue];
		if(height == -2 || height == -1) {
			autoSizeParamY = height==-2?WRAP_CONTENT:FILL_PARENT;
			//[self layout];
			height = (autoSizeParamY==FILL_PARENT)?imageView.superview.frame.size.height:[imageView sizeThatFits:CGSizeZero].height;
	
		} else {
			autoSizeParamY = FIXED_SIZE;
		}
		
		[imageView setFrame:CGRectMake(imageView.frame.origin.x, imageView.frame.origin.y, imageView.frame.size.width, height)];
		return [super setPropertyWithKey:key toValue:value];
	}
	*/
	else {
		return [super setPropertyWithKey:key toValue:value];
	}
	return MAW_RES_OK;
}

- (NSString*)getPropertyWithKey: (NSString*)key {
	
	return [super getPropertyWithKey:key];
}

@end
