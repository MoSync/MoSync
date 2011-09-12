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
#include "UIColor-Expanded.h"

@implementation EditBoxWidget

- (id)init {
	textField = [[UITextField alloc] initWithFrame:CGRectMake(0, 10, 100, 30)];
	textField.borderStyle = UITextBorderStyleRoundedRect;
	view = textField;			
	id ret = [super init];
	[self setAutoSizeParamX:WRAP_CONTENT andY:WRAP_CONTENT];
	textField.delegate = self;
    [textField addTarget:self action:@selector(textChanged) forControlEvents:UIControlEventEditingChanged];
	return ret;
}

- (int)setPropertyWithKey: (NSString*)key toValue: (NSString*)value {
	if([key isEqualToString:@MAW_EDIT_BOX_TEXT]) {
		textField.text = value;
	}
	else if([key isEqualToString:@MAW_EDIT_BOX_PLACEHOLDER]) {
		textField.placeholder = value;
	}	
	else if([key isEqualToString:@MAW_EDIT_BOX_SHOW_KEYBOARD]) {
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
	else if([key isEqualToString:@MAW_LABEL_TEXT_HORIZONTAL_ALIGNMENT]) {
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
	else if([key isEqualToString:@MAW_LABEL_TEXT_VERTICAL_ALIGNMENT]) {
		// This isn't trivial on iphone.

		//UILabel* label = (UILabel*) view;
		//if([value isEqualToString:@"center"]) {
		//}
        return MAW_RES_INVALID_PROPERTY_VALUE;
	}
	else if([key isEqualToString:@MAW_LABEL_FONT_COLOR]) {
		UIColor* color = [UIColor colorWithHexString:value];
		if(!color) return MAW_RES_INVALID_PROPERTY_VALUE;
		textField.textColor = color;
	}
	else if([key isEqualToString:@MAW_LABEL_FONT_SIZE]) {
		float fontSize = [value floatValue];
		textField.font = [UIFont boldSystemFontOfSize:fontSize];
	}
	else if([key isEqualToString:@MAW_EDIT_BOX_EDIT_MODE]) {

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
	if([key isEqualToString:@MAW_EDIT_BOX_TEXT]) {
		return [textField.text retain];
	}

	return [super getPropertyWithKey:key];
}

/**
 * Delegate for editing began action for this widget.
 * @param textField The text field for which an editing session began.
 */
- (void)textFieldDidBeginEditing:(UITextField *)textField
{
    [super sendEvent:MAW_EVENT_EDIT_BOX_EDITING_DID_BEGIN];
}

/**
 * Delegate for editing stopped action for this widget.
 * @param textField The text field for which editing ended.
 */
- (void)textFieldDidEndEditing:(UITextField *)textField
{
    [super sendEvent:MAW_EVENT_EDIT_BOX_EDITING_DID_END];
}

/**
 * Delegate for processing the pressing of the return button.
 * @param textField The text field whose return button was pressed.
 */
- (BOOL)textFieldShouldReturn:(UITextField*)textField
{
    [super sendEvent:MAW_EVENT_EDIT_BOX_RETURN];
	return YES;
}

/**
 * Called each time when the text field's text has changed.
 */
- (void) textChanged
{
    [super sendEvent:MAW_EVENT_EDIT_BOX_TEXT_CHANGED];
}

@end