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

#import "EditBoxWidget.h"
#import <QuartzCore/QuartzCore.h>

#ifndef NATIVE_TEST
#include "Platform.h"
#include <helpers/cpp_defs.h>
#include <helpers/CPP_IX_WIDGET.h>
#include <base/Syscall.h>
#endif


@implementation EditBoxWidget

- (BOOL)textFieldShouldReturn:(UITextField*)textField {
#ifndef NATIVE_TEST
	MAEvent event;
	event.type = EVENT_TYPE_WIDGET;
	MAWidgetEventData *eventData = new MAWidgetEventData;
	eventData->eventType = WIDGET_EVENT_CLICKED;
	eventData->widgetHandle = handle;
//	eventData->searchBarButton = 0;
	event.data = eventData;
	Base::gEventQueue.put(event);
#endif
	return YES;
}

- (id)init {
	textField = [[[UITextField alloc] initWithFrame:CGRectMake(0, 10, 100, 30)] retain];
	
	/*
	 //	textView = [[[UITextView alloc] initWithFrame:CGRectMake(0, 10, 100, 30)] retain];	 
	[textView setTextColor:[UIColor blackColor]];
	[textView.layer setBackgroundColor: [[UIColor whiteColor] CGColor]];
    [textView.layer setBorderColor: [[UIColor grayColor] CGColor]];
    [textView.layer setBorderWidth: 1.0];
    [textView.layer setCornerRadius:8.0f];
    //[textView.layer setMasksToBounds:YES];
	*/
	textField.borderStyle = UITextBorderStyleRoundedRect;
	
	view = textField;			
	id ret = [super init];
	textField.delegate = self;
	return ret;
}

- (void)addChild: (IWidget*)child {
	[super addChild:child];
}

- (void)removeChild: (IWidget*)child {
}

- (int)setPropertyWithKey: (NSString*)key toValue: (NSString*)value {
	if([key isEqualToString:@"text"]) {
		textField.text = value;
	}
	else if([key isEqualToString:@"placeholder"]) {
		textField.placeholder = value;
	}	
	else if([key isEqualToString:@"showKeyboard"]) {
		if([value isEqualToString:@"true"]) {
			[textField becomeFirstResponder];
		} else {
			[textField resignFirstResponder];
		}
	}
	else if([key isEqualToString:@"autocorrectionEnabled"]) {
		if([value isEqualToString:@"true"]) {
			textField.autocorrectionType = UITextAutocorrectionTypeYes;
		} else {
			textField.autocorrectionType = UITextAutocorrectionTypeNo;
		}
	}	
	else if([key isEqualToString:@"horizontalAlignment"]) {
		if([value isEqualToString:@"left"]) {
			textField.textAlignment = UITextAlignmentLeft;
		}
		else if([value isEqualToString:@"center"]) {
			textField.textAlignment = UITextAlignmentCenter;
		}
		else if([value isEqualToString:@"right"]) {
			textField.textAlignment = UITextAlignmentRight;
		}		
	}
	else if([key isEqualToString:@"verticalAlignment"]) {
		// This isn't trivial on iphone.
		
		//UILabel* label = (UILabel*) view;
		//if([value isEqualToString:@"center"]) {
		//}
	}	
	else if([key isEqualToString:@"fontColor"]) {	
		textField.textColor = [UIColor colorWithHexString:value];
	}	
	else if([key isEqualToString:@"fontSize"]) {
		float fontSize = [value floatValue];
		textField.font = [UIFont boldSystemFontOfSize:fontSize];
	}
	else {
		return [super setPropertyWithKey:key toValue:value];
	}
}

- (NSString*)getPropertyWithKey: (NSString*)key {
	if([key isEqualToString:@"text"]) {
		return textField.text;
	}	
	
	return [super getPropertyWithKey:key];
}

@end