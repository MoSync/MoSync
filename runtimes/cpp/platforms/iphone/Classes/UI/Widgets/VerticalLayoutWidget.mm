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

#import "VerticalLayoutWidget.h"
#import "LayoutManagers.h"

MAKE_UIWRAPPER_LAYOUTING_IMPLEMENTATION(VLayoutView)

@implementation VerticalLayoutWidget

- (id)init {
	//view = [[VLayoutView alloc] init];
	
	view = [[MoSyncVLayoutView alloc]  initWithFrame:CGRectZero spacing:0
									leftMargin:0 rightMargin:0 topMargin:0 bottomMargin:0
									hAlignment:UIControlContentHorizontalAlignmentLeft
									vAlignment:UIControlContentVerticalAlignmentTop];
	
	[view setWidget:self];
	
	return [super init];
}

- (void)addChild: (IWidget*)child {
	[super addChild:child];
	[view setSize];	
}

- (void)removeChild: (IWidget*)child {
}

- (int)setPropertyWithKey: (NSString*)key toValue: (NSString*)value {
	if([key isEqualToString:@"childVerticalAlignment"]) {
		VLayoutView* vlv = (VLayoutView*)view;
		if([value isEqualToString:@"top"])
			[vlv setVerticalAlignment:UIControlContentVerticalAlignmentTop];
		else if([value isEqualToString:@"center"])
			[vlv setVerticalAlignment:UIControlContentVerticalAlignmentCenter];
		else if([value isEqualToString:@"bottom"])
			[vlv setVerticalAlignment:UIControlContentVerticalAlignmentBottom];		
	} else if([key isEqualToString:@"childHorizontalAlignment"]) {
		VLayoutView* vlv = (VLayoutView*)view;
		if([value isEqualToString:@"left"])
			[vlv setHorizontalAlignment:UIControlContentHorizontalAlignmentLeft];
		else if([value isEqualToString:@"center"])
			[vlv setHorizontalAlignment:UIControlContentHorizontalAlignmentCenter];
		else if([value isEqualToString:@"right"])
			[vlv setHorizontalAlignment:UIControlContentHorizontalAlignmentRight];		
	} else {
		return [super setPropertyWithKey:key toValue:value];
	}
	return MA_WIDGET_OK;	
}

- (NSString*)getPropertyWithKey: (NSString*)key {
	
	return [super getPropertyWithKey:key];
}

@end
