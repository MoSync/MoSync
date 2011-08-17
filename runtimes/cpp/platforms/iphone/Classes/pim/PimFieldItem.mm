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


#import "PimFieldItem.h"
#import "helpers/cpp_defs.h"
#import "helpers/cpp_ix_pim.h"
#import "PimSyscall.h"
#import "PimUtil.h"

@implementation PimFieldItem

/**
 * Init function.
 * @param fieldID Must be one of MA_PIM_FIELD_CONTACT constants.
 * If the fieldID param is invalid or not supported on this platform, 
 * the functions return nil.
 */
-(id) initWithFieldID:(int) fieldID
{
    mFieldConstant = fieldID;
    int result = [[PimUtils sharedInstance] fieldStructure:mFieldConstant 
                                                   setType:&mFieldType
                                          setIsSingleValue:&mIsSingleFieldValue];
    if (result == MA_PIM_ERR_FIELD_UNSUPPORTED)
    {
        return nil;
    }
    
    mFieldValuesArray = [[NSMutableArray alloc] init];
    return [super init];
}

/**
 * Get field constant.
 * @return One of MA_PIM_FIELD_CONTACT constants.
 */
-(int) getFieldConstant
{
    return mFieldConstant;
}

/**
 * Get field's type.
 * @return One of MA_PIM_TYPE constants.
 */
-(int) getFieldType
{
    return mFieldType;
}

/**
 * Get the number of values inside this field.
 * @return The number of values inside this field.
 */
-(int) count
{
    return [mFieldValuesArray count];
}

/**
 * Get the attribute of a specified value.
 * @param index The specified index value.
 * @return One of MA_PIM_ATTR constants, or MA_PIM_ERR_INVALID_INDEX
 *         if index param is not valid.
 */
-(int) getAttribute:(int) index
{
    int fieldValuesCount = [mFieldValuesArray count];
    if (0 > index || index <= fieldValuesCount) {
        return MA_PIM_ERR_INVALID_INDEX;
    }
    
    return [[mFieldValuesArray objectAtIndex:index] getAttribute];
}

/**
 * Set a custom label for a value in this field.
 * @param customLabel Label's value.
 * @param index Value's index.
 * @return One of MA_PIM_ERR constants.
 */
-(int) setLabel:(NSString*) customLabel
        atIndex:(int) index
{
    int fieldValuesCount = [mFieldValuesArray count];
    if (0 > index || index <= fieldValuesCount) {
        return MA_PIM_ERR_INVALID_INDEX;
    }
    
    return [[mFieldValuesArray objectAtIndex:index] setCustomLabel:customLabel];
}

/**
 * Gets a custom label value for a given field's value.
 * @param value Will contain the custom label value.
 * @param index Value's index.
 * @return One of MA_PIM_ERR constants.
 */
-(int) getLabel:(NSString*) value
     indexValue:(const int) index
{
    int fieldValuesCount = [mFieldValuesArray count];
    if (0 > index || index >= fieldValuesCount) {
        return MA_PIM_ERR_INVALID_INDEX;
    }
    
    // Check if the attribute is set to MA_PIM_ATTR_CUSTOM.
    PimFieldItemValue* itemValue = [mFieldValuesArray objectAtIndex:index];
//    int attribute = [itemValue getAttribute];
//    if (MA_PIM_ATTR_CUSTOM != attribute) 
//    {
//        return MA_PIM_ERR_NO_LABEL;
//    }
    
    [value copy:[itemValue getCustomLabel]];
    return MA_PIM_ERR_NONE; 
}

/**
 * Gets the values at a specified index.
 * @param index The specified index.
 * @return An array containing the values, or nil if the index
 *         does not exist.
 */
-(NSMutableArray*) getValue:(const int) index
{
    int fieldValuesCount = [mFieldValuesArray count];
    if (0 > index || index >= fieldValuesCount) {
        return nil;
    }
    
    return [[mFieldValuesArray objectAtIndex:index] getValue]; 
}

/**
 * Set the value and attribute at a specified index.
 * @param value The value.
 * @param index The specified index.
 * @param attribute The specified attribute.
 * @return One of MA_PIM_ERR constants.
 */
-(int) setValue:(NSMutableArray*) value
        atIndex:(int) index
  withAttribute:(int) attribute
{
    int fieldValuesCount = [mFieldValuesArray count];
    if (0 > index || index <= fieldValuesCount) {
        return MA_PIM_ERR_INVALID_INDEX;
    }
    
    PimFieldItemValue* fieldValue = [mFieldValuesArray objectAtIndex:index];
    [fieldValue setValue:value];
    
    return MA_PIM_ERR_NONE;
}

/**
 * A value and attribute.
 * @param value The value.
 * @param attribute The specified attribute.
 * @return The new value's index, or one of MA_PIM_ERR constants in
 *         case of error.
 */
-(int) addValue:(NSMutableArray*) value
  withAttribute:(const int) attribute
{
    int returnValue = MA_PIM_ERR_NONE;
    // Check if the field can contain more then one value.
    int countFieldValues = [mFieldValuesArray count];
    if (mIsSingleFieldValue &&
        1 == countFieldValues) 
    {
        returnValue = MA_PIM_ERR_FIELD_COUNT_MAX;
    }
    else
    {
        // Check if the attribute is allowed.
        bool attributeAllowed = [self isAttributeValid:attribute];
        if (!attributeAllowed)
        {
            returnValue = MA_PIM_ERR_COMBO_UNSUPPORTED;
        }
        else 
        {
            PimFieldItemValue* fieldValue = [[PimFieldItemValue alloc] init];
            [fieldValue setValue:value];
            [fieldValue setAttribute:attribute];
            [mFieldValuesArray addObject:fieldValue];
            returnValue = [mFieldValuesArray count] - 1;
        }
    }
    
    return returnValue;
}

///**
// * Add a value and attribute.
// * @param value The value.
// * @param attribute The specified string attribute.
// * @return The new value's index, or one of MA_PIM_ERR constants in
// *         case of error.
// */
//-(int) addValue:(NSMutableArray*) value
//withStringAttribute:(NSString*) attribute
//{
////    int intAttibute;
////    if ([attribute isEqualToString:@"asst"]) {
////        intAttibute = MA_PIM_ATTR_ASST;
////    } else if ([attribute isEqualToString:@"auto"]) {
////        intAttibute = MA_PIM_ATTR_AUTO;
////    } else if ([attribute isEqualToString:@"fax"]) {
////        intAttibute = MA_PIM_ATTR_FAX;
////    } else if ([attribute isEqualToString:@"home"]) {
////        intAttibute = MA_PIM_ATTR_HOME;
////    } else if ([attribute isEqualToString:@"mobile"]) {
////        intAttibute = MA_PIM_ATTR_MOBILE;
////    } else if ([attribute isEqualToString:@"other"]) {
////        intAttibute = MA_PIM_ATTR_OTHER;
////    } else if ([attribute isEqualToString:@"pager"]) {
////        intAttibute = MA_PIM_ATTR_AUTO;
////    } else if ([attribute isEqualToString:@"preferred"]) {
////        intAttibute = MA_PIM_ATTR_PREFERRED;
////    } else if ([attribute isEqualToString:@"work"]) {
////        intAttibute = MA_PIM_ATTR_WORK;
////    } else if ([attribute isEqualToString:@"sms"]) {
////        intAttibute = MA_PIM_ATTR_SMS;
////    } else if ([attribute isEqualToString:@"iphone"]) {
////        intAttibute = MA_PIM_ATTR_IPHONE;
////    } else {
////        intAttibute = MA_PIM_ATTR_CUSTOM;
////    }
////    
////    [self addValue:value withAttribute:intAttibute];
////    if (MA_PIM_ATTR_CUSTOM == intAttibute) {
////        [self setLabel:attribute atIndex:([mFieldValuesArray count] - 1)];
////    } // bogdan
//    
//    return 0;
//}

/**
 * Remove the value at a specified index.
 * @param index The specified index.
 * @return One of MA_PIM_ERR constants.
 */
-(int) removeValue:(int) index
{
    int fieldValuesCount = [mFieldValuesArray count];
    if (0 > index || index <= fieldValuesCount) {
        return MA_PIM_ERR_INVALID_INDEX;
    }
    
    [mFieldValuesArray removeObjectAtIndex:index];
    
    return MA_PIM_ERR_NONE;
}

/**
 * Gets and item at a given index.
 * @param index The speicified index.
 * @return The item.
 */
-(PimFieldItemValue*) getItem:(int) index
{
    int fieldValuesCount = [mFieldValuesArray count];
    if (0 > index || index <= fieldValuesCount) {
        return [mFieldValuesArray objectAtIndex:index];;
    }
    
    return nil;
}

/**
 * Checks if an specified attribute is valid.
 * @param attribute The given attribute.
 * @return True if the attribute is valid, false otherwise.
 */
-(bool) isAttributeValid:(const int) attribute
{
    NSString* stringAttribute = [[NSString alloc] initWithFormat:@"%d",attribute];
    NSMutableDictionary* allowedAttributes =
        [[PimUtils sharedInstance] getAttributesForFieldId:mFieldConstant];

    // If the field does not have attributes and if the attribute is preffered.
    if (0 == [[allowedAttributes allKeys] count] &&
        MA_PIM_ATTR_PREFERRED == attribute)
    {
        return true;
    }

    NSString* value = [allowedAttributes valueForKey:stringAttribute];

    if (nil == value)
    {
        return false;
    }

    return true;
}

/**
 * Release all the objects.
 */
- (void) dealloc
{
    [mFieldValuesArray release];
    
    [super dealloc];
}

@end

