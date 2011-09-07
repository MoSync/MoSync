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

/**
 * @file PimItem.m
 * Abstract class for pim items.
 */

#import "PimItem.h"
#import "PimSyscall.h"
#import "PimFieldItem.h"

@implementation PimItem

/**
 * Init function.
 */
-(id) init
{
    mFieldsDictionary = [[NSMutableDictionary alloc] init];
    return [super init];
}

/**
 * Gets the number of fields.
 * @return The number of fields for this item.
 */
-(int) count
{
   return [mFieldsDictionary count];
}

/**
 * Gets field's id for a given field index.
 * A field id is one of the MA_PIM_FIELD constants.
 * @param fieldIndex Field's index.
 * @return One of the MA_PIM_FIELD constants, or one of the MA_PIM_ERR
 *         constants in case of error.
 */
-(int) getFieldID:(const int) fieldIndex
{
    int fieldsCount = [mFieldsDictionary count];
    if (0 > fieldIndex || fieldIndex >= fieldsCount)
    {
        return MA_PIM_ERR_INDEX_INVALID;
    }
    else
    {
        NSArray* allValuesFromDictionary = [mFieldsDictionary allValues];
        PimFieldItem* fieldItem = [allValuesFromDictionary objectAtIndex:fieldIndex];
        return [fieldItem getFieldConstant];
    }
}

/**
 * Gets the number of values for a given field ID.
 * Does not check if field is supported.
 * @param field The specified field ID(one of the MA_PIM_FIELD constants).
 * @return The number of values for the given field, or 0 if the field ID
 *         has no values/does not exist.
 */
-(int) fieldCount:(const int) fieldID
{
    int returnValue;
    NSString* key = [[NSString alloc] initWithFormat:@"%d",fieldID];
    PimFieldItem* itemField = [mFieldsDictionary objectForKey:key];
    [key release];

    if (nil == itemField)
    {
        // Field is empty.
        returnValue = 0;
    }
    else
    {
        returnValue = [itemField count];
    }

    return returnValue;
}

/**
 * Gets the attribute for a given field value.
 * Does not check if field is supported.
 * @param fieldID The specified field ID(one of the MA_PIM_FIELD constants).
 * @param index The index of the given field value.
 * @return One of MA_PIM_ATTR constants, or MA_PIM_ERR in case of error.
 */
-(int) getAttribute:(const int) fieldID
         indexValue:(const int) index
{
    int returnValue = MA_PIM_ERR_NONE;
    NSString* key = [[NSString alloc] initWithFormat:@"%d",fieldID];
    PimFieldItem* itemField = [mFieldsDictionary objectForKey:key];
    [key release];

    if (nil == itemField)
    {
        returnValue = MA_PIM_ERR_FIELD_INVALID;
    } else {
        returnValue = [itemField getAttribute:index];
    }

    return returnValue;
}

/**
 * Sets a custom label for a given field value.
 * Does not check if field is supported or valid.
 * @param args Common arguments.
 *             The item's handle is stored in args.item.
 *             The field's ID is stored in args.field.
 *             The custom label's value is stored in args.buf.
 *             The size of the value(in bytes) is stored in args.bufSize.
 * @param index Field's value index.
 * @return One of the MA_PIM_ERR constants.
 */
-(int) setLabel:(const MA_PIM_ARGS*) args
     indexValue:(const int) index
{
    int returnValue = MA_PIM_ERR_NONE;
    PimUtils* utils = [PimUtils sharedInstance];
    void* address = [utils getValidatedMemRange:(int)args->buf withSize: args->bufSize];

    // Create string with UTF-16 encoding.
    NSString* customLabel = [[NSString alloc] initWithBytes:address
                                                     length:args->bufSize
                                                   encoding:NSUTF16LittleEndianStringEncoding];

    int field = args->field;
    NSString* key = [[NSString alloc] initWithFormat:@"%d",field];
    PimFieldItem* itemField = [mFieldsDictionary objectForKey:key];
    [key release];

    if (nil == itemField)
    {
        returnValue = MA_PIM_ERR_FIELD_EMPTY;
    }
    else
    {
        returnValue = [itemField setLabel:customLabel atIndex:index];
    }

    return returnValue;
}

/**
 * Gets a custom label for a given field value.
 * Does not check if field is supported.
 * @param args Common arguments.
 *             The item's handle is stored in args.item.
 *             The field's ID is stored in args.field.
 *             The custom label's value will be stored in args.buf.
 *             The maximum size of the value(in bytes) is stored in args.bufSize.
 * @param index Field's value index.
 * @return The number of bytes occupied by the value. If the number is greater than
 *         args.bufSize the value was not written into args.buf.
 *         In case of error the function returns one of the MA_PIM_ERR constants.
 *         If the field's value does not have a custom label, the function will
 *         return MA_PIM_ERR_NO_LABEL.
 */
-(int) getLabel:(const MA_PIM_ARGS*) args
     indexValue:(const int) index
{
    int returnValue = MA_PIM_ERR_NONE;
    NSMutableString* customLabel = [[NSMutableString alloc] init];
    PimUtils* utils = [PimUtils sharedInstance];
    void* address = [utils getValidatedMemRange:(int)args->buf withSize: args->bufSize];
    int field = args->field;

    NSString* key = [[NSString alloc] initWithFormat:@"%d",field];
    PimFieldItem* itemField = [mFieldsDictionary objectForKey:key];
    [key release];

    if (nil == itemField)
    {
        return MA_PIM_ERR_FIELD_EMPTY;
    }

    returnValue = [itemField getLabel:customLabel indexValue:index];

    // If no error occurred write custom label's value.
    if (MA_PIM_ERR_NONE == returnValue)
    {
        returnValue = [[PimUtils sharedInstance] writeString:customLabel
                                                   atAddress:address
                                                     maxSize:args->bufSize];
    }

    [customLabel release];

    return returnValue;
}

/**
 * Gets a field's value at a given index.
 * Does not check if field is supported or write-only.
 * @param args Common arguments.
 *             The item's handle is stored in args.item.
 *             The field's ID is stored in args.field.
 *             The field's value will be stored in args.buf.
 *             The maximum size of the value(in bytes) is stored in args.bufSize.
 * @param index Field's value index.
 * @return The number of bytes occupied by the value. If the number is greater than
 *         args.bufSize the value was not written into args.buf.
 *         In case of error the function returns one of the MA_PIM_ERR constants.
 */
-(int) getValue:(const MA_PIM_ARGS*) args
     indexValue:(const int) index
{

    int returnValue;
    int fieldConstant = args->field;
    PimUtils* utils = [PimUtils sharedInstance];
    void* address = [utils getValidatedMemRange:(int)args->buf withSize:args->bufSize];
    NSString* key = [[NSString alloc] initWithFormat:@"%d", fieldConstant];
    PimFieldItem* fieldItem = [mFieldsDictionary objectForKey:key];
    [key release];

    if (nil == fieldItem ||
        0 > index  ||
        index > ([fieldItem count] - 1))
    {
        return MA_PIM_ERR_INDEX_INVALID;
    }

    int fieldType = [fieldItem getFieldType];
    NSMutableArray* valuesArray = [fieldItem getValue:index];

    switch (fieldType)
    {
        case MA_PIM_TYPE_DATE:
            returnValue = [utils writeDate:[valuesArray objectAtIndex:0]
                                 atAddress:address];
            break;
        case MA_PIM_TYPE_INT:
            returnValue = [utils writeIntValue:[valuesArray objectAtIndex:0] atAddress:address];
            break;
        case MA_PIM_TYPE_STRING:
            returnValue = [utils writeString:[valuesArray objectAtIndex:0]
                                   atAddress:address
                                     maxSize:args->bufSize];
            break;
        case MA_PIM_TYPE_STRING_ARRAY:
            returnValue = [utils writeStringArray:valuesArray
                                        atAddress:address
                                          maxSize:args->bufSize];
            break;
        default:
            break;
    }

    return returnValue;
}

/**
 * Sets a field's value and attribute at a given index.
 * Does not check if field is supported or write-only.
 * @param args Common arguments.
 *             The item's handle is stored in args.item.
 *             The field's ID is stored in args.field.
 *             The field's value is stored in args.buf.
 *             The size of the value(in bytes) is stored in args.bufSize.
 * @param index Field's value index.
 * @param attribute Field's value attribute.
 * @return One of the MA_PIM_ERR constants.
 */
-(int) setValue:(const MA_PIM_ARGS*) args
     indexValue:(const int) index
 valueAttribute:(const int) attribute
{
    int returnValue;
    int field = args->field;
    NSMutableArray* valuesArray = nil;
    PimUtils* utils = [PimUtils sharedInstance];
    void* address = [utils getValidatedMemRange:(int)args->buf
                                       withSize:args->bufSize];

    // Check if the field exists.
    NSString* key = [[NSString alloc] initWithFormat:@"%d", field];
    PimFieldItem* itemField;
    itemField = [mFieldsDictionary objectForKey:key];
    [key release];

    if(nil == itemField)
    {
        return MA_PIM_ERR_FIELD_EMPTY;
    }

    // Check if there is a value at the given index.
    valuesArray = [itemField getValue:index];
    if (nil == valuesArray)
    {
        return MA_PIM_ERR_INDEX_INVALID;
    }

    // Get the new value.
    int fieldType = [itemField getFieldType];
    switch (fieldType)
    {
        case MA_PIM_TYPE_DATE:
            valuesArray = [utils getDate:address];
            break;
        case MA_PIM_TYPE_STRING:
            valuesArray = [utils getString:address withSize:args->bufSize];
            break;
        case MA_PIM_TYPE_INT:
            valuesArray = [utils getIntValue:address];
            break;
        case MA_PIM_TYPE_STRING_ARRAY:
            valuesArray = [utils getStringArray:address forFieldID:field withSize:args->bufSize];
            break;
        default:
            valuesArray = nil;
            break;
    }

    // Check if values were read from the buffer.
    if (!valuesArray)
    {
        return MA_PIM_ERR_BUFFER_INVALID;
    }

    returnValue = [itemField setValue:valuesArray atIndex:index
                        withAttribute:attribute];

    // Modify item's status if it's imported.
    if (MA_PIM_ERR_NONE == returnValue &&
        mItemStatus == kImportedItem)
    {
        mItemStatus = kModifiedItem;
    }

    return returnValue;
}

/**
 * Adds a value to a specified field.
 * Does not check if the field is supported or read-only.
 * @param args Common arguments.
 *             The item's handle is stored in args.item.
 *             The field's ID is stored in args.field.
 *             The field's value is stored in args.buf.
 *             The size of the value(in bytes) is stored in args.bufSize.
 * @param attribute Field's value attribute.
 * @return  New value's index in field, or one of the MA_PIM_ERR constants
 *          in case of error.
 */
-(int) addValue:(const MA_PIM_ARGS*) args
  withAttribute:(const int) attribute
{
    NSLog(@"PimItem--addValue--");
    int field = args->field;
    NSMutableArray* valuesArray;
    PimUtils* utils = [PimUtils sharedInstance];
    void* address = [utils getValidatedMemRange:(int)args->buf
                                       withSize:args->bufSize];

    // Check if the field exists.
    NSString* key = [[NSString alloc] initWithFormat:@"%d", field];
    PimFieldItem* itemField;
    itemField = [mFieldsDictionary objectForKey:key];
    if (nil == itemField)
    {
        // If the field does not exist create a new one.
        itemField = [[PimFieldItem alloc] initWithFieldID:field];

        // Check if the field was created.
        if (nil == itemField)
        {
            [key release];
            return MA_PIM_ERR_FIELD_UNSUPPORTED;
        }
        [mFieldsDictionary setObject:itemField forKey:key];
    }
    [key release];

    int fieldType = [itemField getFieldType];
    switch (fieldType)
    {
        case MA_PIM_TYPE_DATE:
            valuesArray = [utils getDate:address];
            break;
        case MA_PIM_TYPE_INT:
            valuesArray = [utils getIntValue:address];
            break;
        case MA_PIM_TYPE_STRING:
            valuesArray = [utils getString:address withSize:args->bufSize];
            break;
        case MA_PIM_TYPE_STRING_ARRAY:
            valuesArray = [utils getStringArray:address forFieldID:field withSize:args->bufSize];
            break;
        default:
            valuesArray = nil;
            break;
    }

    // Check if values were read from the buffer.
    if (!valuesArray)
    {
        return MA_PIM_ERR_BUFFER_INVALID;
    }

    int returnValue = [itemField addValue:valuesArray withAttribute:attribute];

    // Modify item's status if it's imported.
    if (MA_PIM_ERR_NONE == returnValue &&
        mItemStatus == kImportedItem)
    {
        mItemStatus = kModifiedItem;
    }

    return returnValue;
}

/**
 * Removes a value from a field.
 * Does not check if the field is valid or supported.
 * @param field One of the MA_PIM_FIELD constants.
 * @param index Field's value index.
 * @return One of MA_PIM_ERR constants.
 */
-(int) removeValue:(const int) field
           atIndex:(const int) index
{
    int returnValue = MA_PIM_ERR_NONE;
    NSString* key = [[NSString alloc] initWithFormat:@"%d",field];
    PimFieldItem* itemField = [mFieldsDictionary objectForKey:key];
    [key release];

    if (nil == itemField)
    {
        returnValue = MA_PIM_ERR_FIELD_EMPTY;
    } else
    {
        returnValue = [itemField removeValue:index];
    }

    return returnValue;
}

/**
 * Writes all the fields into record.
 * Deletes all item's fields.
 * @return One of the MA_PIM_ERR constants.
 */
-(int) close
{
    methodNotImplemented();
}

/**
 * Gets the status of the item.
 * @return The status.
 */
-(PimItemStatus) getStatus
{
    return mItemStatus;
}

/**
 * Release all the objects.
 */
- (void) dealloc
{
    [mFieldsDictionary release];
    [super dealloc];
}

@end
