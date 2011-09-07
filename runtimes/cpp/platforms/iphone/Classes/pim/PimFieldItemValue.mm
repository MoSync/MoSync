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
	mAttributeID = -1;
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
 * @return MA_PIM_ERR_NONE is the label was set, or
 *		 MA_PIM_ERR_CUSTOM_ATTRIBUTE if the attibute
 *		 is not set to custom.
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
 *		 nil if the attribute is not set to custom.
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
