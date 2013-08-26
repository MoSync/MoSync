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

#import "PimFieldItemValue.h"

#import "PimUtil.h"
#include "helpers/cpp_defs.h"
#import "helpers/cpp_ix_pim.h"

@implementation PimFieldItemValue

/**
 * Init function.
 */
-(id) init
{
    mValuesArray = [[NSMutableArray alloc] init];
    mAttributeID = 0;
    mLabel = [[NSString alloc] init];

    return [super init];
}

/**
 * Set field's value.
 * @param value The specified value.
 */
-(void) setValue:(NSMutableArray*) value
{
    [mValuesArray release];
    mValuesArray = value;
}

/**
 * Get field's value.
 * @return An array with value.
 */
-(NSMutableArray*) getValue
{
    return mValuesArray;
}

/**
 * Set field's attribute.
 * @param attribute The new field's attribute.
 */
-(void) setAttribute:(int) attributeID
{
    mAttributeID = attributeID;
}

/**
 * Get field's value attribute.
 * @return One of MA_PIM_ATTR value.
 */
-(int) getAttribute
{
    return mAttributeID;
}

/**
 * Set the label value.
 * @param label The new label's value.
 * @return One of the MA_PIM_ERR constants.
 */
-(int) setLabel:(NSString*) label
{
    if (nil != label)
    {
        [mLabel release];
        mLabel = [NSString stringWithString:label];
    }
    return MA_PIM_ERR_NONE;
}

/**
 * Get the label value.
 * @return A string containing the label value, or
 *         nil if the attribute is not set to custom.
 */

-(NSString*) getLabel
{
    return mLabel;
}

/**
 * Release all the objects.
 */
- (void) dealloc
{
    [mValuesArray release];
    [mLabel release];
    [super dealloc];
}

@end
