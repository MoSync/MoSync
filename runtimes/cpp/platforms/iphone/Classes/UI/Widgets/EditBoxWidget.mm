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
	self = [super init];
    if (self)
    {
        textField = [[UITextField alloc] initWithFrame:CGRectMake(0, 10, 100, 30)];
        textField.borderStyle = UITextBorderStyleRoundedRect;

        view = textField;
        [view setUserInteractionEnabled:YES];
        view.contentMode = UIViewContentModeRedraw;
        view.autoresizesSubviews = NO;
        [textField setOpaque:NO];

        [self setAutoSizeParamX:WRAP_CONTENT andY:WRAP_CONTENT];
        textField.delegate = self;
        [textField addTarget:self action:@selector(textChanged) forControlEvents:UIControlEventEditingChanged];
        mMaxTextLength = INT_MAX;
    }

	return self;
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
	else if([key isEqualToString:@MAW_EDIT_BOX_FONT_COLOR]) {
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
    else if([key isEqualToString:@MAW_EDIT_BOX_INPUT_MODE])
    {
        int type = [value intValue];
        int typeFound = true;
        switch (type)
        {
            case MAW_EDIT_BOX_TYPE_EMAILADDR:
                textField.keyboardType = UIKeyboardTypeEmailAddress;
                break;
            case MAW_EDIT_BOX_TYPE_NUMERIC:
                textField.keyboardType = UIKeyboardTypeNumbersAndPunctuation;
                break;
            case MAW_EDIT_BOX_TYPE_PHONENUMBER:
                textField.keyboardType = UIKeyboardTypePhonePad;
                break;
            case MAW_EDIT_BOX_TYPE_URL:
                textField.keyboardType = UIKeyboardTypeURL;
                break;
            case MAW_EDIT_BOX_TYPE_DECIMAL:
                textField.keyboardType = UIKeyboardTypeDecimalPad;
                break;
            case MAW_EDIT_BOX_TYPE_SINGLE_LINE:
                textField.keyboardType = UIKeyboardTypeDefault;
                break;
            default:
                typeFound = false;
        }
        if (!typeFound)
        {
            return MAW_RES_INVALID_PROPERTY_VALUE;
        }
    }
    else if([key isEqualToString:@MAW_EDIT_BOX_INPUT_FLAG])
    {
        int flag = [value intValue];
        bool flagFound = true;
        switch (flag)
        {
            case MAW_EDIT_BOX_FLAG_PASSWORD:
                textField.secureTextEntry = YES;
                break;
            case MAW_EDIT_BOX_FLAG_INITIAL_CAPS_WORD:
                textField.autocapitalizationType = UITextAutocapitalizationTypeWords;
                break;
            case MAW_EDIT_BOX_FLAG_INITIAL_CAPS_SENTENCE:
                textField.autocapitalizationType = UITextAutocapitalizationTypeSentences;
                break;
            case MAW_EDIT_BOX_FLAG_INITIAL_CAPS_ALL_CHARACTERS:
                textField.autocapitalizationType = UITextAutocapitalizationTypeAllCharacters;
                break;
            default:
                flagFound = false;
        }
        if (!flagFound)
        {
            return MAW_RES_INVALID_PROPERTY_VALUE;
        }
    }
    else if ([key isEqualToString:@MAW_EDIT_BOX_MAX_LENGTH])
    {
        int maxTextLength = [value intValue];
        if (maxTextLength < 0)
        {
            return MAW_RES_INVALID_PROPERTY_VALUE;
        }
        mMaxTextLength = maxTextLength;
    }
	else {
		return [super setPropertyWithKey:key toValue:value];
	}

	return MAW_RES_OK;
}

- (NSString*)getPropertyWithKey: (NSString*)key {
	if([key isEqualToString:@MAW_EDIT_BOX_TEXT]) {
        NSString* text;
        if (textField.text)
        {
            text = [[NSString alloc] initWithString:textField.text];
        }
        else
        {
            text = [[NSString alloc] initWithString:@""];
        }
		return text;
	}
    else if ([key isEqualToString:@MAW_EDIT_BOX_MAX_LENGTH])
    {
        return [[NSString alloc] initWithFormat:@"%d", mMaxTextLength];
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

/**
 * Delegate method called before the text has been changed.
 * @param uiTextField The text field containing the text.
 * @param range The range of characters to be replaced.
 * @param string The replacement string.
 * @return YES if the specified text range should be replaced; otherwise, NO to keep the old text.
 */
- (BOOL)textField:(UITextField *)uiTextField shouldChangeCharactersInRange:(NSRange)range
replacementString:(NSString *)string
{
    if ([string length] > 0 && [uiTextField.text length] >= mMaxTextLength)
    {
        uiTextField.text = [uiTextField.text substringToIndex:mMaxTextLength];
        return NO;
    }
    return YES;
}

@end