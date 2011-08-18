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
 * @return MA_PIM_ERR_NONE is the label was set, or
 *         MA_PIM_ERR_CUSTOM_ATTRIBUTE if the attibute
 *         is not set to custom.
 */
-(int) setLabel:(NSString*) label;

/**
 * Get the label value.
 * @return A string containing the label value, or
 *         nil if the attribute is not set to custom.
 */

-(NSString*) getLabel;

@end
