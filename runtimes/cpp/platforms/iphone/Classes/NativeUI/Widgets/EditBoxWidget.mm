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

#import <QuartzCore/QuartzCore.h>
#include <helpers/cpp_defs.h>
#include <helpers/CPP_IX_WIDGET.h>
#include <base/Syscall.h>

#import "EditBoxWidget.h"
#import "UIColorExpanded.h"
#import "WidgetUtils.h"
#include "Platform.h"

@implementation EditBoxWidget

- (id)init
{
	self = [super init];
    if (self)
    {
        _textField = [[UITextField alloc] initWithFrame:CGRectMake(0, 10, 100, 30)];
        _textField.borderStyle = UITextBorderStyleRoundedRect;

        self.view = _textField;
        [self.view setUserInteractionEnabled:YES];
        self.view.contentMode = UIViewContentModeRedraw;
        self.view.autoresizesSubviews = NO;
        [_textField setOpaque:NO];

        self.autoSizeWidth = WidgetAutoSizeWrapContent;
        self.autoSizeHeight = WidgetAutoSizeWrapContent;
        _textField.delegate = self;
        [_textField addTarget:self action:@selector(textChanged) forControlEvents:UIControlEventEditingChanged];
        _maxTextLength = INT_MAX;
    }

	return self;
}

/**
 * Set a widget property value.
 * @param key Widget's property name that should be set.
 * @param value Widget's proeprty value that should be set.
 * @return One of the following values:
 * - MAW_RES_OK if the property was set.
 * - MAW_RES_INVALID_PROPERTY_NAME if the property name was invalid.
 * - MAW_RES_INVALID_PROPERTY_VALUE if the property value was invalid.
 */
- (int)setPropertyWithKey:(NSString*)key toValue:(NSString*)value
{
	if ([key isEqualToString:@MAW_EDIT_BOX_TEXT])
    {
		_textField.text = value;
	}
	else if ([key isEqualToString:@MAW_EDIT_BOX_PLACEHOLDER])
    {
		_textField.placeholder = value;
	}
	else if ([key isEqualToString:@MAW_EDIT_BOX_SHOW_KEYBOARD])
    {
		if ([value isEqualToString:kWidgetTrueValue])
        {
			[_textField becomeFirstResponder];
		}
        else
        {
			[_textField resignFirstResponder];
		}
	}
	else if([key isEqualToString:@"autocorrectionEnabled"])
    {
		if([value isEqualToString:kWidgetTrueValue])
        {
			_textField.autocorrectionType = UITextAutocorrectionTypeYes;
		}
        else
        {
			_textField.autocorrectionType = UITextAutocorrectionTypeNo;
		}
	}
	else if([key isEqualToString:@MAW_LABEL_TEXT_HORIZONTAL_ALIGNMENT])
    {
		if([value isEqualToString:@"left"])
        {
			_textField.textAlignment = UITextAlignmentLeft;
		}
		else if([value isEqualToString:@"center"])
        {
			_textField.textAlignment = UITextAlignmentCenter;
		}
		else if([value isEqualToString:@"right"]) {
			_textField.textAlignment = UITextAlignmentRight;
		}
	}
	else if([key isEqualToString:@MAW_LABEL_FONT_COLOR])
    {
		UIColor* color = [UIColor colorWithHexString:value];
		if(!color) return MAW_RES_INVALID_PROPERTY_VALUE;
		_textField.textColor = color;
	}
	else if([key isEqualToString:@MAW_EDIT_BOX_FONT_COLOR])
    {
		float fontSize = [value floatValue];
		_textField.font = [UIFont boldSystemFontOfSize:fontSize];
	}
	else if([key isEqualToString:@MAW_EDIT_BOX_EDIT_MODE])
    {

		if ([value isEqualToString:@"password"])
		   _textField.secureTextEntry = YES;
		else
		   _textField.secureTextEntry = NO;
	}
    else if([key isEqualToString:@MAW_EDIT_BOX_INPUT_MODE])
    {
        int type = [value intValue];
        int typeFound = true;
        switch (type)
        {
            case MAW_EDIT_BOX_TYPE_EMAILADDR:
                _textField.keyboardType = UIKeyboardTypeEmailAddress;
                break;
            case MAW_EDIT_BOX_TYPE_NUMERIC:
                _textField.keyboardType = UIKeyboardTypeNumbersAndPunctuation;
                break;
            case MAW_EDIT_BOX_TYPE_PHONENUMBER:
                _textField.keyboardType = UIKeyboardTypePhonePad;
                break;
            case MAW_EDIT_BOX_TYPE_URL:
                _textField.keyboardType = UIKeyboardTypeURL;
                break;
            case MAW_EDIT_BOX_TYPE_DECIMAL:
                _textField.keyboardType = UIKeyboardTypeDecimalPad;
                break;
            case MAW_EDIT_BOX_TYPE_SINGLE_LINE:
                _textField.keyboardType = UIKeyboardTypeDefault;
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
                _textField.secureTextEntry = YES;
                break;
            case MAW_EDIT_BOX_FLAG_INITIAL_CAPS_WORD:
                _textField.autocapitalizationType = UITextAutocapitalizationTypeWords;
                break;
            case MAW_EDIT_BOX_FLAG_INITIAL_CAPS_SENTENCE:
                _textField.autocapitalizationType = UITextAutocapitalizationTypeSentences;
                break;
            case MAW_EDIT_BOX_FLAG_INITIAL_CAPS_ALL_CHARACTERS:
                _textField.autocapitalizationType = UITextAutocapitalizationTypeAllCharacters;
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
        _maxTextLength = maxTextLength;
    }
	else {
		return [super setPropertyWithKey:key toValue:value];
	}

	return MAW_RES_OK;
}

/**
 * Get a widget property value.
 * @param key Widget's property name.
 * @return The property value, or nil if the property name is invalid.
 * The returned value should not be autoreleased. The caller will release the returned value.
 */
- (NSString*)getPropertyWithKey:(NSString*)key
{
	if([key isEqualToString:@MAW_EDIT_BOX_TEXT])
    {
        NSString* text;
        if (_textField.text)
        {
            text = [[NSString alloc] initWithString:_textField.text];
        }
        else
        {
            text = [[NSString alloc] initWithString:@""];
        }
		return text;
	}
    else if ([key isEqualToString:@MAW_EDIT_BOX_MAX_LENGTH])
    {
        return [[NSString alloc] initWithFormat:@"%d", _maxTextLength];
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
    if ([string length] > 0 && [uiTextField.text length] >= _maxTextLength)
    {
        uiTextField.text = [uiTextField.text substringToIndex:_maxTextLength];
        return NO;
    }
    return YES;
}

/**
 * Dealloc method.
 */
-(void) dealloc
{
    [_textField release];
    [super dealloc];
}

@end