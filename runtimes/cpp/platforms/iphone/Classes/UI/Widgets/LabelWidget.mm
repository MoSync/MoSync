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

#import "LabelWidget.h"

@implementation LabelWidget

- (id)init {	
	UILabel* label = [[[UILabel alloc] initWithFrame:CGRectMake(0, 0, 100, 60)] retain];
	label.numberOfLines = 0;
	label.opaque = NO;
	view = label;		
	
	return [super init];	
}

- (void)addChild: (IWidget*)child {
	[super addChild:child];
}

- (void)removeChild: (IWidget*)child {
}

- (int)setPropertyWithKey: (NSString*)key toValue: (NSString*)value {
	if([key isEqualToString:@"text"]) {
		UILabel* label = (UILabel*) view;
		[label setText: value];
		[label sizeToFit];
	} 
	else if([key isEqualToString:@"numberOfLines"]) {
		UILabel* label = (UILabel*) view;
		label.numberOfLines = [value intValue];
	} 
	else if([key isEqualToString:@"textHorizontalAlignment"]) {
		UILabel* label = (UILabel*) view;
		if([value isEqualToString:@"left"]) {
			label.textAlignment = UITextAlignmentLeft;
		}
		else if([value isEqualToString:@"center"]) {
			label.textAlignment = UITextAlignmentCenter;
		}
		else if([value isEqualToString:@"right"]) {
			label.textAlignment = UITextAlignmentRight;
		}		
	}
	else if([key isEqualToString:@"textVerticalAlignment"]) {
		// This isn't trivial on iphone.
		
		//UILabel* label = (UILabel*) view;
		//if([value isEqualToString:@"center"]) {
		//}
	}	
	else if([key isEqualToString:@"fontColor"]) {
		UILabel* label = (UILabel*) view;	
		label.textColor = [UIColor colorWithHexString:value];
	}	
	else if([key isEqualToString:@"fontSize"]) {
		UILabel* label = (UILabel*) view;	
		float fontSize = [value floatValue];
		label.font = [UIFont boldSystemFontOfSize:fontSize];
	}
	else {
		return [super setPropertyWithKey:key toValue:value];
	}
	return MA_WIDGET_OK;	
}

- (NSString*)getPropertyWithKey: (NSString*)key {
	
	return [super getPropertyWithKey:key];
}

@end
