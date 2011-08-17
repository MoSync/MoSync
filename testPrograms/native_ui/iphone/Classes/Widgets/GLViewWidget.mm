/*
Copyright (C) 2010 MoSync AB

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

//
//  LabelWidget.mm
//  nativeuitest
//
//  Created by Niklas Nummelin on 11/26/10.
//

#import "GLViewWidget.h"
#import "MoSyncGLView.h"

@implementation GLViewWidget

- (id)init {
	view = [[[MoSyncGLView alloc] initWithFrame:CGRectMake(0, 0, 100, 100)] retain]; // TODO: do have to do this (retain)??
	return [super init];	
}

- (void)addChild: (IWidget*)child {
	[super addChild:child];
}

- (void)removeChild: (IWidget*)child {
}

- (int)setPropertyWithKey: (NSString*)key toValue: (NSString*)value {
	if([key isEqualToString:@"bind"]) {
		[view bindContext];
	}
	else if([key isEqualToString:@"invalidate"]) {
		[view renderContext];
	}
	else {
		return [super setPropertyWithKey:key toValue:value];
	}
}

- (NSString*)getPropertyWithKey: (NSString*)key {
	
	return [super getPropertyWithKey:key];
}

@end
