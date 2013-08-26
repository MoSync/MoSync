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

@interface PimFieldItemValue:  NSObject {

    /**
     * Array with field values.
     */
    NSMutableArray* mValuesArray;

    /**
     * The attribute id.
     */
    int mAttributeID;

    /**
     * The string value of the attribute id.
     * If the attribute is custom the value will be custom also.
     */
    NSString* mLabel;

}

/**
 * Init function.
 */
-(id) init;

/**
 * Set field's value.
 * @param value The specified value.
 */
-(void) setValue:(NSMutableArray*) value;

/**
 * Get field's value.
 * @return An array with values.
 */
-(NSMutableArray*) getValue;

/**
 * Set field's attribute.
 * @param attribute The new field's attribute.
 */
-(void) setAttribute:(int) attributeID;

/**
 * Get field's value attribute.
 * @return One of MA_PIM_ATTR value.
 */
-(int) getAttribute;

/**
 * Set the label value.
 * @param label The new label's value.
 * @return One of the MA_PIM_ERR constants.
 */
-(int) setLabel:(NSString*) label;

/**
 * Get the label value.
 * @return A string containing the label value, or
 *         nil if the attribute is not set to custom.
 */

-(NSString*) getLabel;

@end
