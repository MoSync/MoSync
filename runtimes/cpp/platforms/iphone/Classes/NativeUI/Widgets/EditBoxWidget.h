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

#import <Foundation/Foundation.h>
#import "IWidget.h"

@interface EditBoxWidget : IWidget <UITextFieldDelegate, UITextViewDelegate>
{
    /**
     * Edit box mode.
     * Can be MAW_EDIT_BOX_MODE_SINGLE_LINE or MAW_EDIT_BOX_MODE_MULTI_LINE.
     */
    int _mode;

    /**
     * Placeholder for text view.
     * Only for MAW_EDIT_BOX_MODE_MULTI_LINE mode.
     */
    NSString* _placeholder;

    /**
     * Text font color.
     * Used mostly for MAW_EDIT_BOX_MODE_MULTI_LINE mode.
     */
    UIColor* _textColor;

    /**
     * Flag to indicate if the placeholder is shown or not.
     * Only for MAW_EDIT_BOX_MODE_MULTI_LINE mode.
     */
    BOOL _isPlaceholderShown;

    /**
     * Maximum text length for this edit box.
     */
    int _maxTextLength;
}

/**
 * Getter for view object.
 * Will return nil if widget's mode is not MAW_EDIT_BOX_MODE_SINGLE_LINE.
 */
@property(nonatomic, readonly) UITextField* textField;

/**
 * Getter for view object.
 * Will return nil if widget's mode is not MAW_EDIT_BOX_MODE_MULTI_LINE.
 */
@property(nonatomic, readonly) UITextView* textView;

/**
 * Getter and setter for placeholder.
 */
@property(nonatomic, retain) NSString* placeholder;

/**
 * Getter and setter for font text color.
 */
@property(nonatomic, retain) UIColor* textColor;

/**
 * Set a widget property value.
 * @param key Widget's property name that should be set.
 * @param value Widget's proeprty value that should be set.
 * @return One of the following values:
 * - MAW_RES_OK if the property was set.
 * - MAW_RES_INVALID_PROPERTY_NAME if the property name was invalid.
 * - MAW_RES_INVALID_PROPERTY_VALUE if the property value was invalid.
 */
- (int)setPropertyWithKey:(NSString*)key toValue:(NSString*)value;

/**
 * Get a widget property value.
 * @param key Widget's property name.
 * @return The property value, or nil if the property name is invalid.
 * The returned value should not be autoreleased. The caller will release the returned value.
 */
- (NSString*)getPropertyWithKey:(NSString*)key;

@end
