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
#include "Platform.h"
#include <helpers/cpp_defs.h>
#include <helpers/CPP_IX_WIDGET.h>
#include <base/Syscall.h>

@implementation EditBoxWidget

- (BOOL)textFieldShouldReturn:(UITextField*)textField {
	MAEvent event;
	event.type = EVENT_TYPE_WIDGET;
	MAWidgetEventData *eventData = new MAWidgetEventData;
	eventData->eventType = MAW_EVENT_CLICKED;
	eventData->widgetHandle = handle;
	event.data = eventData;
	Base::gEventQueue.put(event);
	return YES;
}

- (id)init {
	textField = [[[UITextField alloc] initWithFrame:CGRectMake(0, 10, 100, 30)] retain];
	textField.borderStyle = UITextBorderStyleRoundedRect;
	view = textField;			
	id ret = [super init];
	[self setAutoSizeParamX:WRAP_CONTENT andY:WRAP_CONTENT];
	textField.delegate = self;
	return ret;
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
	else if([key isEqualToString:@"textHorizontalAlignment"]) {
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
	else if([key isEqualToString:@"textVerticalAlignment"]) {
		// This isn't trivial on iphone.
		
		//UILabel* label = (UILabel*) view;
		//if([value isEqualToString:@"center"]) {
		//}
	}	
	else if([key isEqualToString:@"fontColor"]) {
		UIColor* color = [UIColor colorWithHexString:value];
		if(!color) return MAW_RES_INVALID_PROPERTY_VALUE;
		textField.textColor = color;
	}	
	else if([key isEqualToString:@"fontSize"]) {
		float fontSize = [value floatValue];
		textField.font = [UIFont boldSystemFontOfSize:fontSize];
	}
	else if([key isEqualToString:@"editMode"]) {
		
		if(
		   [value isEqualToString:@"password"])
		   textField.secureTextEntry = YES;
		else 
		   textField.secureTextEntry = NO;
		
	}
	else {
		return [super setPropertyWithKey:key toValue:value];
	}
	
	return MAW_RES_OK;
}

- (NSString*)getPropertyWithKey: (NSString*)key {
	if([key isEqualToString:@"text"]) {
		return textField.text;
	}	
	
	return [super getPropertyWithKey:key];
}

@end