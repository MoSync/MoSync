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
//  LabelWidget.mm
//  nativeuitest
//
//  Created by Niklas Nummelin on 11/26/10.
//

#import "LabelWidget.h"


@implementation LabelWidget

- (id)init {	
	UILabel* label = [[[UILabel alloc] initWithFrame:CGRectMake(0, 0, 100, 60)] retain];
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
	else if([key isEqualToString:@"horizontalAlignment"]) {
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
	else if([key isEqualToString:@"verticalAlignment"]) {
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
