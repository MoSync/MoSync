//
//  ImageWidget.mm
//  nativeuitest
//
//  Created by Niklas Nummelin on 11/26/10.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

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
	return [super init];
}

- (void)addChild: (IWidget*)child {
	[super addChild:child];
}

- (void)removeChild: (IWidget*)child {
}

- (int)setPropertyWithKey: (NSString*)key toValue: (NSString*)value {
	if([key isEqualToString:@"image"]) {
		int imageHandle = [value intValue];
		#ifndef NATIVE_TEST
		Surface* imageResource = Base::gSyscall->resources.get_RT_IMAGE(imageHandle);
		UIImage* image = [UIImage imageWithCGImage:imageResource->image];
		if(leftCapWidth != 0 || topCapHeight != 0) {
			image = [image stretchableImageWithLeftCapWidth:leftCapWidth topCapHeight:topCapHeight];
		}
		if(imageView != nil)
			[imageView removeFromSuperview];

		imageView = [[UIImageView alloc] initWithImage:image];		
		[view addSubview:imageView];
		view.frame = imageView.frame;
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
	else if ([key isEqualToString:@"width"]) {
		int ret = [super setPropertyWithKey:key toValue:value];
		float width = [value floatValue];
		if(width == -1) {
			width = view.frame.size.width;
		}		
		[imageView setFrame:CGRectMake(imageView.frame.origin.x, imageView.frame.origin.y, width, imageView.frame.size.height)];
		return ret;
	}
	else if ([key isEqualToString:@"height"]) {
		int ret = [super setPropertyWithKey:key toValue:value];
		float height = [value floatValue];
		if(height == -1) {
			height = view.frame.size.height;
		}		
		[imageView setFrame:CGRectMake(imageView.frame.origin.x, imageView.frame.origin.y, imageView.frame.size.width, height)];
		return ret;
	}
	else {
		return [super setPropertyWithKey:key toValue:value];
	}
	return WIDGET_OK;
}

- (NSString*)getPropertyWithKey: (NSString*)key {
	
	return [super getPropertyWithKey:key];
}

@end
