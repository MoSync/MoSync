/* Copyright (C) 2011 Mobile Sorcery AB
 
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
    mAttribute = -1;
    mCustomLabel = [[NSString alloc] init];
    
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
-(void) setAttribute:(int) attribute
{
    mAttribute = attribute;
}

/**
 * Get field's value attribute.
 * @return One of MA_PIM_ATTR value.
 */
-(int) getAttribute
{
    return mAttribute;
}
/**
 * Set custom label value. 
 * @param customLabel The new custom label's value.
 * @return MA_PIM_ERR_NONE is the label was set, or
 *         MA_PIM_ERR_CUSTOM_ATTRIBUTE if the attibute
 *         is not set to custom.
 */
-(int) setCustomLabel:(NSString*) customLabel
{
    if (MA_PIM_ATTR_CUSTOM == mAttribute)
    {
        mCustomLabel = customLabel;
        return MA_PIM_ERR_NONE;
    }
    else
    {
        return MA_PIM_ERR_CUSTOM_ATTRIBUTE;
    }
}

/**
 * Get custom label value.
 * @return A string containing the custom label value, or
 *         an empty string if the attribute is not set to custom.
 */
-(NSString*) getCustomLabel
{
    return mCustomLabel;
}

/**
 * Get the string label.
 * @return A string associated with the attribute. 
 */ 
-(NSString*) getStringLabel
{
    return [[PimUtils sharedInstance] getStringFromAttribute:mAttribute];
}

/**
 * Release all the objects.
 */
- (void) dealloc {
    [mValuesArray release];
    [mCustomLabel release];
    
    [super dealloc];
}

@end

