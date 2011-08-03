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

#import <Foundation/Foundation.h>
#import "PimFieldItemValue.h"

/**
 * Class PimFieldItem.
 * Stores the content of a field. The field is defined by one of
 * the MA_PIM_FIELD_CONTACT constants.
 */
@interface PimFieldItem:  NSObject {
    /**
     * Type of the field. Must be one of 
     * MA_PIM_TYPE_ constants.
     */
    int mFieldType; 
    
    /**
     * Must be one of MA_PIM_FIELD_CONTACT constants.
     */
    int mFieldConstant;
    
    /**
     * Stores the values.
     */
    NSMutableArray* mFieldValuesArray;
}

/**
 * Init function.
 * @param fieldConstant Must be one of MA_PIM_FIELD_CONTACT contants.
 */
-(id) initWithFieldConstant:(int) fieldConstant;

/**
 * Get field constant.
 * @return One of MA_PIM_FIELD_CONTACT constants.
 */
-(int) getFieldConstant;

/**
 * Get field's type.
 * @return One of MA_PIM_TYPE constants.
 */
-(int) getFieldType;

/**
 * Get the number of values inside this field.
 * @return The number of values inside this field.
 */
-(int) count;

/**
 * Get the attribute of a specified value.
 * @param index The specified index value.
 * @return One of MA_PIM_ATTR constants, or MA_PIM_ERR_INVALID_INDEX
 *         if index param is not valid.
 */
-(int) getAttribute:(int) index;

/**
 * Set a custom label for a value in this field.
 * @param customLabel Label's value.
 * @param index Value's index.
 * @return One of MA_PIM_ERR constants.
 */
-(int) setLabel:(NSString*) customLabel
        atIndex:(int) index;

/**
 * Gets a custom label value for a given field's value.
 * @param value Will contain the custom label value.
 * @param index Value's index.
 * @return One of MA_PIM_ERR constants.
 */
-(int) getLabel:(NSString*) value
     indexValue:(const int) index;

/**
 * Gets the values at a specified index.
 * @param index The specified index.
 * @return An array containing the values, or nil if the index
 *         does not exist.
 */
-(NSMutableArray*) getValue:(const int) index;

/**
 * Set the value and attribute at a specified index.
 * @param value The value.
 * @param index The specified index.
 * @param attribute The specified attribute.
 * @return One of MA_PIM_ERR constants.
 */
-(int) setValue:(NSMutableArray*) value
        atIndex:(int) index
  withAttribute:(int) attribute;

/**
 * Add a value and attribute.
 * @param value The value.
 * @param attribute The specified attribute.
 * @return The new value's index, or one of MA_PIM_ERR constants in
 *         case of error.
 */
-(int) addValue:(NSMutableArray*) value
  withAttribute:(const int) attribute;

/**
 * Add a value and attribute.
 * @param value The value.
 * @param attribute The specified string attribute.
 * @return The new value's index, or one of MA_PIM_ERR constants in
 *         case of error.
 */
-(int) addValue:(NSMutableArray*) value
withStringAttribute:(NSString*) attribute;

/**
 * Remove the value at a specified index.
 * @param index The specified index.
 * @return One of MA_PIM_ERR constants.
 */
-(int) removeValue:(int) index;

/**
 * Gets and item at a given index.
 * @param index The speicified index.
 * @return The item.
 */
-(PimFieldItemValue*) getItem:(int) index;

@end
