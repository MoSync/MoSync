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
	UIImageView* imageView = [[[UIImageView alloc] initWithFrame:CGRectMake(0, 0, 100, 60)] retain];
	view = imageView;			
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
		UITableViewCell* cell = (UITableViewCell*) view;
		UIImageView* imageView = cell.imageView;
		#ifndef NATIVE_TEST
		Surface* imageResource = Base::gSyscall->resources.get_RT_IMAGE(imageHandle);		
		imageView.image = [UIImage imageWithCGImage:imageResource->image];
		#endif
		
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
