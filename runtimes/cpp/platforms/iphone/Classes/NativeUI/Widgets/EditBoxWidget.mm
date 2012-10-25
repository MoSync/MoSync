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

#define DEFAULT_WIDTH 100
#define DEFAULT_HEIGHT 30

/**
 * Constants used to make the text view look like text field.
 */
#define TEXT_VIEW_CORNER_RADIUS 10
#define TEXT_VIEW_BORDER_COLOR_ALPHA 0.5
#define TEXT_VIEW_BORDER_WIDTH 1.0

#import <QuartzCore/QuartzCore.h>
#include <helpers/cpp_defs.h>
#include <helpers/CPP_IX_WIDGET.h>
#include <base/Syscall.h>

#import "EditBoxWidget.h"
#import "UIColorExpanded.h"
#import "NSStringExpanded.h"
#import "WidgetUtils.h"
#include "Platform.h"

/**
 * Private methods/functions for edit box widget.
 */
@interface EditBoxWidget (hidden)

/**
 * Set the edit box text.
 * Setter for MAW_EDIT_BOX_TEXT.
 * @param value Text to set.
 * @return MAW_RES_OK.
 */
- (int)setTextProperty:(NSString*)value;

/**
 * Get the edit box text.
 * Getter for MAW_EDIT_BOX_TEXT.
 * @return The edit box text.
 */
- (NSString*)textProperty;

/**
 * Set the edit box placeholder.
 * Setter for MAW_EDIT_BOX_PLACEHOLDER.
 * @param value Placeholder to set.
 * @return MAW_RES_OK.
 */
- (int)setPlaceholderProperty:(NSString*)value;

/**
 * Show/hide the keyboard.
 * Setter for MAW_EDIT_BOX_SHOW_KEYBOARD.
 * @param value "true" or "false" strings.
 * @return MAW_RES_OK if the value param is as expected, otherwise MAW_RES_INVALID_PROPERTY_VALUE.
 */
- (int)setShowKeyboardProperty:(NSString*)value;

/**
 * Set the edit mode to password or text.
 * Setter for MAW_EDIT_BOX_EDIT_MODE.
 * @param value "password" or "text" strings.
 * @return MAW_RES_OK if the value param is as expected, otherwise MAW_RES_INVALID_PROPERTY_VALUE.
 */
- (int)setEditModeProperty:(NSString*)value;

/**
 * Set the input mode.
 * Those flags are mutual exclusive, so the previous value is always ignored.
 * Setter for MAW_EDIT_BOX_INPUT_MODE.
 * @param value One of the values MAW_EDIT_BOX_TYPE_ANY, MAW_EDIT_BOX_TYPE_EMAILADDR,
 * MAW_EDIT_BOX_TYPE_NUMERIC, MAW_EDIT_BOX_TYPE_PHONENUMBER, MAW_EDIT_BOX_TYPE_URL,
 * MAW_EDIT_BOX_TYPE_DECIMAL, MAW_EDIT_BOX_TYPE_SINGLELINE.
 * @return MAW_RES_OK if the value param is as expected, otherwise MAW_RES_INVALID_PROPERTY_VALUE.
 */
- (int)setInputModeProperty:(NSString*)value;

/**
 * Set the input flag.
 * Those flags are mutual exclusive, so the previous value is always ignored.
 * Setter for MAW_EDIT_BOX_INPUT_TYPE.
 * @param value One of the values: MAW_EDIT_BOX_FLAG_NONE,
 * MAW_EDIT_BOX_FLAG_INITIAL_CAPS_WORD, MAW_EDIT_BOX_FLAG_INITIAL_CAPS_SENTENCE,
 * MAW_EDIT_BOX_FLAG_INITIAL_CAPS_ALL_CHARACTERS,
 * @return MAW_RES_OK if the value param is as expected, otherwise MAW_RES_INVALID_PROPERTY_VALUE.
 */
- (int)setInputFlagProperty:(NSString*)value;

/**
 * Set the text font color.
 * Setter for MAW_EDIT_BOX_FONT_COLOR.
 * @param value A hexadecimal value 0xRRGGBB, where R, G and B are the red, green and blue
 * components respectively.
 * @return MAW_RES_OK if the value param is as expected, otherwise MAW_RES_INVALID_PROPERTY_VALUE.
 */
- (int)setFontColorProperty:(NSString*)value;

/**
 * Set the maximum input lenght of the edit box.
 * Setter for MAW_EDIT_BOX_MAX_LENGTH.
 * @param value A positive integer.
 * @return MAW_RES_OK if the value param is as expected, otherwise MAW_RES_INVALID_PROPERTY_VALUE.
 */
- (int)setMaxLengthProperty:(NSString*)value;

/**
 * Get the maximum input lenght of the edit box.
 * Getter for MAW_EDIT_BOX_MAX_LENGTH.
 * @return The maximum input lenght of the edit box.
 */
- (NSString*)maxLengthProperty;

/**
 * Set the edit box mode.
 * Setter for MAW_EDIT_BOX_MODE.
 * @param value MAW_EDIT_BOX_MODE_SINGLE_LINE or MAW_EDIT_BOX_MULTI_LINE.
 * @return MAW_RES_OK if the value param is as expected, otherwise MAW_RES_INVALID_PROPERTY_VALUE.
 */
- (int)setModeProperty:(NSString*) value;

/**
 * Get the edit box mode.
 * Getter for MAW_EDIT_BOX_MODE.
 * @return MAW_EDIT_BOX_MODE_SINGLE_LINE or MAW_EDIT_BOX_MULTI_LINE.
 */
- (NSString*)modeProperty;

/**
 * Set the auto correction type.
 * Setter for MAW_EDIT_BOX_AUTO_CORRECTION_TYPE.
 * @param value One of the following constants: MAW_EDIT_BOX_AUTOCORRECTION_TYPE_DEAFULT,
 * MAW_EDIT_BOX_AUTOCORRECTION_TYPE_ENABLE, MAW_EDIT_BOX_AUTOCORRECTION_TYPE_DISABLE.
 * @return MAW_RES_OK if the value param is as expected, otherwise MAW_RES_INVALID_PROPERTY_VALUE.
 */
- (int)setAutoCorrectionTypeProperty:(NSString*)value;

/**
 * Get the auto correction type.
 * Getter for MAW_EDIT_BOX_AUTO_CORRECTION_TYPE.
 * @return One of the following constants: MAW_EDIT_BOX_AUTOCORRECTION_TYPE_DEAFULT,
 * MAW_EDIT_BOX_AUTOCORRECTION_TYPE_ENABLE, MAW_EDIT_BOX_AUTOCORRECTION_TYPE_DISABLE.
 */
- (NSString*)autoCorrectionTypeProperty;

/**
 * Set the text horizontal alignment.
 * Setter for MAW_EDIT_BOX_TEXT_HORIZONTAL_ALIGNMENT.
 * @param value One of the following constants: MAW_ALIGNMENT_LEFT, MAW_ALIGNMENT_CENTER
 * or MAW_ALIGNMENT_RIGHT.
 * @return MAW_RES_OK if the value param is as expected, otherwise MAW_RES_INVALID_PROPERTY_VALUE.
 */
- (int)setTextHorizontalAlignmentProperty:(NSString*)value;

/**
 * Get the text horizontal alignment.
 * Getter for MAW_EDIT_BOX_TEXT_HORIZONTAL_ALIGNMENT.
 * @return One of the following constants: MAW_ALIGNMENT_LEFT, MAW_ALIGNMENT_CENTER
 * or MAW_ALIGNMENT_RIGHT.
 */
- (NSString*)textHorizontalAlignmentProperty;

/**
 * Show the placeholder text inside the text view.
 * This method should be called when the text view is empty.
 */
-(void) showPlaceholder;

/**
 * Hide the placeholder from the text view.
 * This method should be called when the user wants to write on the text view.
 */
-(void) hidePlaceholder;

/**
 * Get the keyboard type from string.
 * @param value One of the values MAW_EDIT_BOX_TYPE_ANY, MAW_EDIT_BOX_TYPE_EMAILADDR,
 * MAW_EDIT_BOX_TYPE_NUMERIC, MAW_EDIT_BOX_TYPE_PHONENUMBER, MAW_EDIT_BOX_TYPE_URL,
 * MAW_EDIT_BOX_TYPE_DECIMAL.
 * @return The keyboard type assocciated. If the value param is not one of the above values,
 * UIKeyboardTypeDefault will be returned.
 */
- (UIKeyboardType)keyboardTypeFromString:(NSString*) value;

/**
 * Check if an given input type is valid.
 * @param value One of the values MAW_EDIT_BOX_TYPE_ANY, MAW_EDIT_BOX_TYPE_EMAILADDR,
 * MAW_EDIT_BOX_TYPE_NUMERIC, MAW_EDIT_BOX_TYPE_PHONENUMBER, MAW_EDIT_BOX_TYPE_URL,
 * MAW_EDIT_BOX_TYPE_DECIMAL.
 * @return YES if the value param is one of the above values, NO otherwise.
 */
- (BOOL)isInputTypeValid:(NSString*)value;

/**
 * Get the input type constant from a native UIKeyboardType enun.
 * @param keyboardType The given enun.
 * @return The associated constant.
 */
- (NSString*)getInputType:(UIKeyboardType) keyboardType;

/**
 * Set text field/view delegate to nil.
 */
- (void)removeDelegate;

/**
 * Create an UITextField object and assign it to view.
 * Also set its delegate and properties.
 * Changes edit box mode to MAW_EDIT_BOX_SINGLE_LINE.
 */
- (void)createTextField;

/**
 * Create an UITextView object and assign it to view.
 * Also set its delegate and properties.
 * Changes edit box mode to MAW_EDIT_BOX_MULTI_LINE.
 */
- (void)createTextView;

@end

@implementation EditBoxWidget

@synthesize placeholder = _placeholder;
@synthesize textColor = _textColor;

- (id)init
{
	self = [super init];
    if (self)
    {
        [self createTextField];
        _textColor = [UIColor blackColor];
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
    int returnedValue = MAW_RES_OK;
	if ([key isEqualToString:@MAW_EDIT_BOX_TEXT])
    {
		returnedValue = [self setTextProperty:value];
	}
	else if ([key isEqualToString:@MAW_EDIT_BOX_PLACEHOLDER])
    {
		returnedValue = [self setPlaceholderProperty:value];
	}
	else if ([key isEqualToString:@MAW_EDIT_BOX_SHOW_KEYBOARD])
    {
        returnedValue = [self setShowKeyboardProperty:value];
    }
	else if([key isEqualToString:@MAW_EDIT_BOX_EDIT_MODE])
    {
        returnedValue = [self setEditModeProperty:value];
	}
    else if([key isEqualToString:@MAW_EDIT_BOX_INPUT_MODE])
    {
        returnedValue = [self setInputModeProperty:value];
    }
    else if([key isEqualToString:@MAW_EDIT_BOX_INPUT_FLAG])
    {
        returnedValue = [self setInputFlagProperty:value];
    }
	else if([key isEqualToString:@MAW_LABEL_FONT_COLOR])
    {
		returnedValue = [self setFontColorProperty:value];
	}
    else if ([key isEqualToString:@MAW_EDIT_BOX_MAX_LENGTH])
    {
        returnedValue = [self setMaxLengthProperty:value];
    }
	else if([key isEqualToString:@MAW_EDIT_BOX_MODE])
    {
        returnedValue = [self setModeProperty:value];
    }
	else if([key isEqualToString:@MAW_EDIT_BOX_AUTOCORRECTION_TYPE])
    {
        returnedValue = [self setAutoCorrectionTypeProperty:value];
	}
	else if([key isEqualToString:@MAW_EDIT_BOX_TEXT_HORIZONTAL_ALIGNMENT])
    {
        returnedValue = [self setTextHorizontalAlignmentProperty:value];
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
        return [[self textProperty] retain];
	}
    else if ([key isEqualToString:@MAW_EDIT_BOX_MAX_LENGTH])
    {
        return [[NSString alloc] initWithFormat:@"%d", _maxTextLength];
    }
    else if ([key isEqualToString:@MAW_EDIT_BOX_AUTOCORRECTION_TYPE])
    {
        return [[self autoCorrectionTypeProperty] retain];
    }
    else if ([key isEqualToString:@MAW_EDIT_BOX_MODE])
    {
        return [[self modeProperty] retain];
    }
	else if([key isEqualToString:@MAW_LABEL_TEXT_HORIZONTAL_ALIGNMENT])
    {
        return [[self textHorizontalAlignmentProperty] retain];
    }

	return [super getPropertyWithKey:key];
}

/**
 * Getter for view object.
 * Will return nil if widget's mode is not MAW_EDIT_BOX_MODE_SINGLE_LINE.
 */
- (UITextField*) textField
{
    UITextField* textField = nil;
    if (_mode == MAW_EDIT_BOX_MODE_SINGLE_LINE)
    {
        textField = (UITextField*) self.view;
    }
    return textField;
}

/**
 * Getter for view object.
 * Will return nil if widget's mode is not MAW_EDIT_BOX_MODE_MULTI_LINE.
 */
- (UITextView*) textView
{
    UITextView* textView = nil;
    if (_mode == MAW_EDIT_BOX_MODE_MULTI_LINE)
    {
        textView = (UITextView*) self.view;
    }
    return textView;
}

#pragma mark UITextFieldDelegate

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

#pragma mark UITextViewDelegate

/**
 * Delegate for editing began action for this widget.
 * @param textField The text view for which an editing session began.
 */
- (void)textViewDidBeginEditing:(UITextField *)textField
{
    [super sendEvent:MAW_EVENT_EDIT_BOX_EDITING_DID_BEGIN];
    if (_isPlaceholderShown)
    {
        [self hidePlaceholder];
    }
}

/**
 * Delegate for editing stopped action for this widget.
 * @param textField The text view for which editing ended.
 */
- (void)textViewDidEndEditing:(UITextField *)textField
{
    [super sendEvent:MAW_EVENT_EDIT_BOX_EDITING_DID_END];

    // Check if the placeholder needs to be set.
    if (self.textView.text.length == 0)
    {
        [self showPlaceholder];
    }
    else
    {
        _isPlaceholderShown = NO;
    }
}

/**
 * Called when the text or attributes in the specified text view were changed by the user.
 * This method is not called in response to programmatically initiated changes.
 * @param textView The text view containing the changes.
 */
- (void)textViewDidChange:(UITextView *)textView
{
    [super sendEvent:MAW_EVENT_EDIT_BOX_TEXT_CHANGED];
}

/**
 * Dealloc method.
 */
-(void) dealloc
{
    [_placeholder release];
    [_textColor release];
    [super dealloc];
}

@end

/**
 * Implementation for hidden methods/functions.
 */
@implementation EditBoxWidget(hidden)

/**
 * Set the edit box text.
 * Setter for MAW_EDIT_BOX_TEXT.
 * @param value Text to set.
 * @return MAW_RES_OK.
 */
- (int)setTextProperty:(NSString*)value
{
    if (_mode == MAW_EDIT_BOX_MODE_SINGLE_LINE)
    {
        [self textField].text = value;
    }
    else if (_mode == MAW_EDIT_BOX_MODE_MULTI_LINE)
    {
        [self textView].text = value;
    }

    return MAW_RES_OK;
}

/**
 * Get the edit box text.
 * Getter for MAW_EDIT_BOX_TEXT.
 * @return The edit box text.
 */
- (NSString*)textProperty
{
    NSString* text = @"";
    if (_mode == MAW_EDIT_BOX_MODE_SINGLE_LINE)
    {
        text = self.textField.text;
    }
    else if (_mode == MAW_EDIT_BOX_MODE_MULTI_LINE)
    {
        text = _isPlaceholderShown ? @"" : self.textView.text;
    }

    return text;
}

/**
 * Set the edit box placeholder.
 * Setter for MAW_EDIT_BOX_PLACEHOLDER.
 * @param value Placeholder to set.
 * @return MAW_RES_OK.
 */
- (int)setPlaceholderProperty:(NSString*)value
{
    self.placeholder = value;
    if (_mode == MAW_EDIT_BOX_MODE_SINGLE_LINE)
    {
        self.textField.placeholder = self.placeholder;
    }
    else if (_mode == MAW_EDIT_BOX_MODE_MULTI_LINE)
    {
        if (_isPlaceholderShown)
        {
            self.textView.text = self.placeholder;
        }
    }
    return MAW_RES_OK;
}

/**
 * Show/hide the keyboard.
 * Setter for MAW_EDIT_BOX_SHOW_KEYBOARD.
 * @param value "true" or "false" strings.
 * @return MAW_RES_OK if the value param is as expected, otherwise MAW_RES_INVALID_PROPERTY_VALUE.
 */
- (int)setShowKeyboardProperty:(NSString*)value
{
    if ([value isEqualToString:kWidgetTrueValue])
    {
        if (_mode == MAW_EDIT_BOX_MODE_SINGLE_LINE)
        {
            [self.textField becomeFirstResponder];
        }
        else if (_mode == MAW_EDIT_BOX_MODE_MULTI_LINE)
        {
            [self.textView becomeFirstResponder];
        }
    }
    else if ([value isEqualToString:kWidgetFalseValue])
    {
        if (_mode == MAW_EDIT_BOX_MODE_SINGLE_LINE)
        {
            [self.textField resignFirstResponder];
        }
        else if (_mode == MAW_EDIT_BOX_MODE_MULTI_LINE)
        {
            [self.textView resignFirstResponder];
            // Show the placeholder, if needed.
            if ([self.textView.text length] == 0)
            {
                [self showPlaceholder];
            }
        }
    }
    else
    {
        return MAW_RES_INVALID_PROPERTY_VALUE;
    }

    return MAW_RES_OK;
}

/**
 * Set the edit mode to password or text.
 * Setter for MAW_EDIT_BOX_EDIT_MODE.
 * @param value "password" or "text" strings.
 * @return MAW_RES_OK if the value param is as expected, otherwise MAW_RES_INVALID_PROPERTY_VALUE.
 */
- (int)setEditModeProperty:(NSString*)value
{
    BOOL passwordMode;
    if ([value isEqualToString:@"password"])
    {
        passwordMode= YES;
    }
    else if ([value isEqualToString:@"text"])
    {
        passwordMode = NO;
    }
    else
    {
        return MAW_RES_INVALID_PROPERTY_VALUE;
    }

    if (_mode == MAW_EDIT_BOX_MODE_SINGLE_LINE)
    {
        self.textField.secureTextEntry = passwordMode;
    }
    else if (_mode == MAW_EDIT_BOX_MODE_MULTI_LINE)
    {
        self.textView.secureTextEntry = passwordMode;
    }

    return MAW_RES_OK;
}

/**
 * Set the input mode.
 * Those flags are mutual exclusive, so the previous value is always ignored.
 * Setter for MAW_EDIT_BOX_INPUT_MODE.
 * @param value One of the values MAW_EDIT_BOX_TYPE_ANY, MAW_EDIT_BOX_TYPE_EMAILADDR,
 * MAW_EDIT_BOX_TYPE_NUMERIC, MAW_EDIT_BOX_TYPE_PHONENUMBER, MAW_EDIT_BOX_TYPE_URL,
 * MAW_EDIT_BOX_TYPE_DECIMAL, MAW_EDIT_BOX_TYPE_SINGLELINE.
 * @return MAW_RES_OK if the value param is as expected, otherwise MAW_RES_INVALID_PROPERTY_VALUE.
 */
- (int)setInputModeProperty:(NSString*)value
{
    if (![self isInputTypeValid:value])
    {
        return MAW_RES_INVALID_PROPERTY_VALUE;
    }

    UIKeyboardType keyboardType = [self keyboardTypeFromString:value];
    if (_mode == MAW_EDIT_BOX_MODE_SINGLE_LINE)
    {
        self.textField.keyboardType = keyboardType;
    }
    else if (_mode == MAW_EDIT_BOX_MODE_MULTI_LINE)
    {
        self.textView.keyboardType = keyboardType;
    }

    return MAW_RES_OK;
}

/**
 * Set the input flag.
 * Those flags are mutual exclusive, so the previous value is always ignored.
 * Setter for MAW_EDIT_BOX_INPUT_TYPE.
 * @param value One of the values: MAW_EDIT_BOX_FLAG_NONE,
 * MAW_EDIT_BOX_FLAG_INITIAL_CAPS_WORD, MAW_EDIT_BOX_FLAG_INITIAL_CAPS_SENTENCE,
 * MAW_EDIT_BOX_FLAG_INITIAL_CAPS_ALL_CHARACTERS,
 * @return MAW_RES_OK if the value param is as expected, otherwise MAW_RES_INVALID_PROPERTY_VALUE.
 */
- (int)setInputFlagProperty:(NSString*)value
{
    if (![value canParseNumber])
    {
        return MAW_RES_INVALID_PROPERTY_VALUE;
    }

    int inputFlagInt = [value intValue];
    UITextAutocapitalizationType autoCapitalizationType;
    BOOL isInputTypeParamValid = YES;
    BOOL isPasswordModeRequired = NO;

    switch (inputFlagInt)
    {
        case MAW_EDIT_BOX_FLAG_PASSWORD:
            isPasswordModeRequired = YES;
            break;
        case MAW_EDIT_BOX_FLAG_INITIAL_CAPS_WORD:
            autoCapitalizationType = UITextAutocapitalizationTypeWords;
            break;
        case MAW_EDIT_BOX_FLAG_INITIAL_CAPS_ALL_CHARACTERS:
            autoCapitalizationType = UITextAutocapitalizationTypeAllCharacters;
            break;
        case MAW_EDIT_BOX_FLAG_INITIAL_CAPS_SENTENCE:
            autoCapitalizationType = UITextAutocapitalizationTypeSentences;
            break;
        default:
            isInputTypeParamValid = NO;
    }

    if (!isInputTypeParamValid)
    {
        return MAW_RES_INVALID_PROPERTY_VALUE;
    }

    if (_mode == MAW_EDIT_BOX_MODE_SINGLE_LINE)
    {
        if (isPasswordModeRequired)
        {
            self.textField.secureTextEntry = YES;
        }
        else
        {
            self.textField.autocapitalizationType = autoCapitalizationType;
        }
    }
    else if (_mode == MAW_EDIT_BOX_MODE_MULTI_LINE)
    {
        if (isPasswordModeRequired)
        {
            self.textView.secureTextEntry = YES;
        }
        else
        {
            self.textView.autocapitalizationType = autoCapitalizationType;
        }
    }
    return MAW_RES_OK;
}

/**
 * Set the text font color.
 * Setter for MAW_EDIT_BOX_FONT_COLOR.
 * @param value A hexadecimal value 0xRRGGBB, where R, G and B are the red, green and blue
 * components respectively.
 * @return MAW_RES_OK if the value param is as expected, otherwise MAW_RES_INVALID_PROPERTY_VALUE.
 */
- (int)setFontColorProperty:(NSString*)value
{
    UIColor* color = [UIColor colorWithHexString:value];
    if (!color)
    {
        return MAW_RES_INVALID_PROPERTY_VALUE;
    }

    if (_mode == MAW_EDIT_BOX_MODE_SINGLE_LINE)
    {
        self.textField.textColor = color;
    }
    else if (_mode == MAW_EDIT_BOX_MODE_MULTI_LINE)
    {
        self.textColor = color;
        if (!_isPlaceholderShown)
        {
            self.textView.textColor = self.textColor;
        }
    }

    return MAW_RES_OK;
}

/**
 * Set the maximum input lenght of the edit box.
 * Setter for MAW_EDIT_BOX_MAX_LENGTH.
 * @param value A positive integer.
 * @return MAW_RES_OK if the value param is as expected, otherwise MAW_RES_INVALID_PROPERTY_VALUE.
 */
- (int)setMaxLengthProperty:(NSString*)value
{
    if (![value canParseNumber])
    {
        return MAW_RES_INVALID_PROPERTY_VALUE;
    }

    int maxTextLength = [value intValue];
    if (maxTextLength < 0)
    {
        return MAW_RES_INVALID_PROPERTY_VALUE;
    }

    _maxTextLength = maxTextLength;
    return MAW_RES_OK;
}

/**
 * Get the maximum input lenght of the edit box.
 * Getter for MAW_EDIT_BOX_MAX_LENGTH.
 * @return The maximum input lenght of the edit box.
 */
- (NSString*)maxLengthProperty
{
    return [NSString stringWithFormat:@"%d", _maxTextLength];
}

/**
 * Set the edit box mode.
 * Setter for MAW_EDIT_BOX_MODE.
 * @param value MAW_EDIT_BOX_MODE_SINGLE_LINE or MAW_EDIT_BOX_MULTI_LINE.
 * @return MAW_RES_OK if the value param is as expected, otherwise MAW_RES_INVALID_PROPERTY_VALUE.
 */
- (int)setModeProperty:(NSString*)value
{
    if (![value canParseNumber])
    {
        return MAW_RES_INVALID_PROPERTY_VALUE;
    }

    int mode = [value intValue];
    if (mode == MAW_EDIT_BOX_MODE_SINGLE_LINE)
    {
        [self createTextField];
    }
    else if (mode == MAW_EDIT_BOX_MODE_MULTI_LINE)
    {
        [self createTextView];
    }
    else
    {
        return MAW_RES_INVALID_PROPERTY_VALUE;
    }

    return MAW_RES_OK;
}

/**
 * Get the edit box mode.
 * Getter for MAW_EDIT_BOX_MODE.
 * @return MAW_EDIT_BOX_MODE_SINGLE_LINE or MAW_EDIT_BOX_MULTI_LINE.
 */
- (NSString*)modeProperty
{
    return [NSString stringWithFormat:@"%d", _mode];
}

/**
 * Set the auto correction type.
 * Setter for MAW_EDIT_BOX_AUTO_CORRECTION_TYPE.
 * @param value One of the following constants: MAW_EDIT_BOX_AUTOCORRECTION_TYPE_DEAFULT,
 * MAW_EDIT_BOX_AUTOCORRECTION_TYPE_ENABLE, MAW_EDIT_BOX_AUTOCORRECTION_TYPE_DISABLE.
 * @return MAW_RES_OK if the value param is as expected, otherwise MAW_RES_INVALID_PROPERTY_VALUE.
 */
- (int)setAutoCorrectionTypeProperty:(NSString*)value
{
    if (![value canParseNumber])
    {
        return MAW_RES_INVALID_PROPERTY_VALUE;
    }

    int autoCorrectionTypeInt = [value intValue];
    UITextAutocorrectionType autoCorrectionType;
    BOOL isAutoCorrectionTypeParamValid = YES;

    switch (autoCorrectionTypeInt) {
        case MAW_EDIT_BOX_AUTOCORRECTION_TYPE_DEFAULT:
            autoCorrectionType = UITextAutocorrectionTypeDefault;
            break;
        case MAW_EDIT_BOX_AUTOCORRECTION_TYPE_ENABLE:
            autoCorrectionType = UITextAutocorrectionTypeYes;
            break;
        case MAW_EDIT_BOX_AUTOCORRECTION_TYPE_DISABLE:
            autoCorrectionType = UITextAutocorrectionTypeNo;
            break;
        default:
            isAutoCorrectionTypeParamValid = NO;
            break;
    }

    if (!isAutoCorrectionTypeParamValid)
    {
        return MAW_RES_INVALID_PROPERTY_VALUE;
    }

    if (_mode == MAW_EDIT_BOX_MODE_SINGLE_LINE)
    {
        self.textField.autocorrectionType = autoCorrectionType;
    }
    else if (_mode == MAW_EDIT_BOX_MODE_MULTI_LINE)
    {
        self.textView.autocorrectionType = autoCorrectionType;
    }
    return MAW_RES_OK;
}

/**
 * Get the auto correction type.
 * Getter for MAW_EDIT_BOX_AUTO_CORRECTION_TYPE.
 * @return One of the following constants: MAW_EDIT_BOX_AUTOCORRECTION_TYPE_DEAFULT,
 * MAW_EDIT_BOX_AUTOCORRECTION_TYPE_ENABLE, MAW_EDIT_BOX_AUTOCORRECTION_TYPE_DISABLE.
 */
- (NSString*)autoCorrectionTypeProperty
{
    UITextAutocorrectionType autoCorrectionType;
    if (_mode == MAW_EDIT_BOX_MODE_SINGLE_LINE)
    {
        autoCorrectionType = self.textField.autocorrectionType;
    }
    else if (_mode == MAW_EDIT_BOX_MODE_MULTI_LINE)
    {
        autoCorrectionType = self.textView.autocorrectionType;
    }

    int autoCorrectionTypeInt;
    switch (autoCorrectionType)
    {
        case UITextAutocorrectionTypeYes:
            autoCorrectionTypeInt = MAW_EDIT_BOX_AUTOCORRECTION_TYPE_ENABLE;
            break;
        case UITextAutocorrectionTypeNo:
            autoCorrectionTypeInt = MAW_EDIT_BOX_AUTOCORRECTION_TYPE_DISABLE;
            break;
        case UITextAutocorrectionTypeDefault:
        default:
            autoCorrectionTypeInt = MAW_EDIT_BOX_AUTOCORRECTION_TYPE_DEFAULT;
            break;
    }

    return [NSString stringWithFormat:@"%d", autoCorrectionTypeInt];
}

/**
 * Set the text horizontal alignment.
 * Setter for MAW_EDIT_BOX_TEXT_HORIZONTAL_ALIGNMENT.
 * @param value One of the following constants: MAW_ALIGNMENT_LEFT, MAW_ALIGNMENT_CENTER
 * or MAW_ALIGNMENT_RIGHT.
 * @return MAW_RES_OK if the value param is as expected, otherwise MAW_RES_INVALID_PROPERTY_VALUE.
 */
- (int)setTextHorizontalAlignmentProperty:(NSString*)value
{
    UITextAlignment textAlignment;
    if ([value isEqualToString:@MAW_ALIGNMENT_LEFT])
    {
        textAlignment = UITextAlignmentLeft;
    }
    else if ([value isEqualToString:@MAW_ALIGNMENT_CENTER])
    {
        textAlignment = UITextAlignmentCenter;
    }
    else if ([value isEqualToString:@MAW_ALIGNMENT_RIGHT])
    {
        textAlignment = UITextAlignmentRight;
    }
    else
    {
        return MAW_RES_INVALID_PROPERTY_VALUE;
    }

    if (_mode == MAW_EDIT_BOX_MODE_SINGLE_LINE)
    {
        self.textField.textAlignment = textAlignment;
    }
    else if (_mode == MAW_EDIT_BOX_MODE_MULTI_LINE)
    {
        self.textView.textAlignment = textAlignment;
    }
    return MAW_RES_OK;
}

/**
 * Get the text horizontal alignment.
 * Getter for MAW_EDIT_BOX_TEXT_HORIZONTAL_ALIGNMENT.
 * @return One of the following constants: MAW_ALIGNMENT_LEFT, MAW_ALIGNMENT_CENTER
 * or MAW_ALIGNMENT_RIGHT.
 */
- (NSString*)textHorizontalAlignmentProperty
{
    UITextAlignment textAlignment;
    if (_mode == MAW_EDIT_BOX_MODE_SINGLE_LINE)
    {
        textAlignment = self.textField.textAlignment;
    }
    else if (_mode == MAW_EDIT_BOX_MODE_MULTI_LINE)
    {
        textAlignment = self.textView.textAlignment;
    }

    NSString* textAlignmentString;
    switch (textAlignment)
    {
        case UITextAlignmentCenter:
            textAlignmentString = @MAW_ALIGNMENT_CENTER;
            break;
        case UITextAlignmentRight:
            textAlignmentString = @MAW_ALIGNMENT_RIGHT;
            break;
        case UITextAlignmentLeft:
        default:
            textAlignmentString = @MAW_ALIGNMENT_LEFT;
            break;
    }
    return textAlignmentString;
}

/**
 * Show the placeholder text inside the text view.
 * This method should be called when the text view is empty.
 */
-(void) showPlaceholder
{
    _isPlaceholderShown = YES;
    self.textView.textColor = [UIColor lightGrayColor];
    self.textView.text = _placeholder;
}

/**
 * Hide the placeholder from the text view.
 * This method should be called when the user wants to write on the text view.
 */
-(void) hidePlaceholder
{
    self.textView.text = @"";
    self.textView.textColor = self.textColor;
    _isPlaceholderShown = NO;
}

/**
 * Get the keyboard type from string.
 * @param value One of the values MAW_EDIT_BOX_TYPE_ANY, MAW_EDIT_BOX_TYPE_EMAILADDR,
 * MAW_EDIT_BOX_TYPE_NUMERIC, MAW_EDIT_BOX_TYPE_PHONENUMBER, MAW_EDIT_BOX_TYPE_URL,
 * MAW_EDIT_BOX_TYPE_DECIMAL.
 * @return The keyboard type assocciated. If the value param is not one of the above values,
 * UIKeyboardTypeDefault will be returned.
 */
- (UIKeyboardType)keyboardTypeFromString:(NSString*)value
{
    int type = [value intValue];
    UIKeyboardType keyboardType;
    switch (type)
    {
        case MAW_EDIT_BOX_TYPE_EMAILADDR:
            keyboardType = UIKeyboardTypeEmailAddress;
            break;
        case MAW_EDIT_BOX_TYPE_NUMERIC:
            keyboardType = UIKeyboardTypeNumbersAndPunctuation;
            break;
        case MAW_EDIT_BOX_TYPE_PHONENUMBER:
            keyboardType = UIKeyboardTypePhonePad;
            break;
        case MAW_EDIT_BOX_TYPE_URL:
            keyboardType = UIKeyboardTypeURL;
            break;
        case MAW_EDIT_BOX_TYPE_DECIMAL:
            keyboardType = UIKeyboardTypeDecimalPad;
            break;
        case MAW_EDIT_BOX_TYPE_ANY:
        default:
            keyboardType = UIKeyboardTypeDefault;
            break;
    }
    return keyboardType;
}

/**
 * Check if an given input type is valid.
 * @param value One of the values MAW_EDIT_BOX_TYPE_ANY, MAW_EDIT_BOX_TYPE_EMAILADDR,
 * MAW_EDIT_BOX_TYPE_NUMERIC, MAW_EDIT_BOX_TYPE_PHONENUMBER, MAW_EDIT_BOX_TYPE_URL,
 * MAW_EDIT_BOX_TYPE_DECIMAL.
 * @return YES if the value param is one of the above values, NO otherwise.
 */
- (BOOL)isInputTypeValid:(NSString*)value
{
    if (![value canParseNumber])
    {
        return NO;
    }

    int type = [value intValue];
    BOOL isValid = NO;
    switch (type)
    {
        case MAW_EDIT_BOX_TYPE_EMAILADDR:
        case MAW_EDIT_BOX_TYPE_NUMERIC:
        case MAW_EDIT_BOX_TYPE_PHONENUMBER:
        case MAW_EDIT_BOX_TYPE_URL:
        case MAW_EDIT_BOX_TYPE_DECIMAL:
        case MAW_EDIT_BOX_TYPE_ANY:
            isValid = YES;
            break;
        default:
            isValid = NO;
            break;
    }
    return isValid;
}

/**
 * Get the input type constant from a native UIKeyboardType enun.
 * @param keyboardType The given enun.
 * @return The associated constant.
 */
- (NSString*)getInputType:(UIKeyboardType) keyboardType
{
    int type;
    switch (keyboardType)
    {
        case UIKeyboardTypeEmailAddress:
            type = MAW_EDIT_BOX_TYPE_EMAILADDR;
            break;
        case UIKeyboardTypeNumbersAndPunctuation:
            type = MAW_EDIT_BOX_TYPE_NUMERIC;
            break;
        case UIKeyboardTypePhonePad:
            type = MAW_EDIT_BOX_TYPE_PHONENUMBER;
            break;
        case UIKeyboardTypeURL:
            type = MAW_EDIT_BOX_TYPE_URL;
            break;
        case UIKeyboardTypeDecimalPad:
            type = MAW_EDIT_BOX_TYPE_DECIMAL;
            break;
        case UIKeyboardTypeDefault:
        default:
            type = MAW_EDIT_BOX_TYPE_ANY;
            break;
    }
    return [NSString stringWithFormat:@"%d", type];
}

/**
 * Set text field/view delegate to nil.
 */
- (void)removeDelegate
{
    if (_mode == MAW_EDIT_BOX_MODE_SINGLE_LINE)
    {
        self.textField.delegate = nil;
        [self.textField removeTarget:self action:@selector(textChanged)
                    forControlEvents:UIControlEventEditingChanged];
    }
    else if (_mode == MAW_EDIT_BOX_MODE_MULTI_LINE)
    {
        self.textView.delegate = nil;
    }
}

/**
 * Create an UITextField object and assign it to view.
 * Also set its delegate and properties.
 * Changes edit box mode to MAW_EDIT_BOX_SINGLE_LINE.
 */
- (void)createTextField
{
    [self removeDelegate];
    self.placeholder = nil;
    self.textColor = nil;
    [self.view removeFromSuperview];
    CGRect rect;
    if (self.view)
    {
        rect = self.view.frame;
    }
    else
    {
        rect = CGRectMake(0, 0, DEFAULT_WIDTH, DEFAULT_HEIGHT);
    }

    UITextField* textField = [[UITextField alloc] initWithFrame:rect];
    textField.delegate = self;
    textField.borderStyle = UITextBorderStyleRoundedRect;
    self.view.contentMode = UIViewContentModeRedraw;
    [textField setOpaque:NO];

    self.autoSizeWidth = WidgetAutoSizeFixed;
    self.autoSizeHeight = WidgetAutoSizeFixed;
    [textField addTarget:self action:@selector(textChanged) forControlEvents:UIControlEventEditingChanged];

    self.view = textField;
    [textField release];
    _mode = MAW_EDIT_BOX_MODE_SINGLE_LINE;
    _isPlaceholderShown = NO;
    IWidget* parent = self.parent;
    if (parent)
    {
        NSUInteger childIndex = [parent indexOfChild:self];
        if (childIndex != NSNotFound)
        {
            [parent removeChild:self];
            [parent insertChild:self atIndex:[NSNumber numberWithInt:childIndex]];
        }
    }
}

/**
 * Create an UITextView object and assign it to view.
 * Also set its delegate and properties.
 * Changes edit box mode to MAW_EDIT_BOX_MULTI_LINE.
 */
- (void)createTextView
{
    [self removeDelegate];
    self.placeholder = nil;
    self.textColor = nil;
    [self.view removeFromSuperview];
    CGRect rect;
    if (self.view)
    {
        rect = self.view.frame;
    }
    else
    {
        rect = CGRectMake(0, 0, DEFAULT_WIDTH, DEFAULT_HEIGHT);
    }

    UITextView* textView = [[UITextView alloc] initWithFrame:rect];
    textView.delegate = self;

    //The rounded corner part, where you specify your view's corner radius:
    textView.layer.cornerRadius = TEXT_VIEW_CORNER_RADIUS;
    textView.clipsToBounds = YES;
    [textView.layer setBorderColor:[[[UIColor grayColor] colorWithAlphaComponent:TEXT_VIEW_BORDER_COLOR_ALPHA] CGColor]];
    [textView.layer setBorderWidth:TEXT_VIEW_BORDER_WIDTH];

    [textView setUserInteractionEnabled:YES];
    textView.contentMode = UIViewContentModeRedraw;
    textView.autoresizesSubviews = NO;
    [textView setOpaque:NO];

    self.autoSizeWidth = WidgetAutoSizeFixed;
    self.autoSizeHeight = WidgetAutoSizeFixed;

    self.view = textView;
    [textView release];
    _mode = MAW_EDIT_BOX_MODE_MULTI_LINE;
    [self showPlaceholder];
    IWidget* parent = self.parent;
    if (parent)
    {
        NSUInteger childIndex = [parent indexOfChild:self];
        if (childIndex != NSNotFound)
        {
            [parent removeChild:self];
            [parent insertChild:self atIndex:[NSNumber numberWithInt:childIndex]];
        }
    }
}

@end