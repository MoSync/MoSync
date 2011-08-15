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

#define methodNotImplemented() @throw [NSException exceptionWithName:NSInvalidArgumentException reason:[NSString stringWithFormat:   @"%@ method must be overriden in a subclass", NSStringFromSelector(_cmd)] userInfo:nil]


#import <Foundation/Foundation.h>

@interface PimUtils: NSObject {
    
    /**
     * The current handle value. 
     */
    int mHandle;
}

/**
 * Returns an instance to the shared singleton. 
 * @return The shared generator object.
 */
+(PimUtils *) sharedInstance;

/**
 * Releases the class instance.
 */
+(void) deleteInstance;


-(int) getNextHandle;

/**
 * Get an array of string from a given address.
 * @param address The specified address.
 *                The address must have the following structure:
 *                     - the first element must be a 4-byte int that specifies
 *                       the number of strings that can be read.
 *                     - first null terminated string(UTF-16 encoding).
 *                     - second null terminated string(UTF-16 encoding). 
 *                     - etc 
 * @return An array containing the strings. 
 */
-(NSMutableArray*) getStringArray:(void*) address;

/**
 * Writes an array of strings at a given address.
 * @param array The given string array.
 * @param address The specified address.
 *                The address will have the following structure:
 *                     - the first element must be a 4-byte int that specifies
 *                       the number of strings that can be read.
 *                     - first null terminated string(UTF-16 encoding).
 *                     - second null terminated string(UTF-16 encoding). 
 *                     - etc
 * @param size  The maximum size(in bytes) that can be written at the given address.
 * @return The size(in bytes) of the strings. If the size of the strings is greater than
 *         the maximum size(the size parameter) then the strings were not written. 
 */
-(int) writeStringArray:(NSMutableArray*) array
               atAddress:(void*) address
                maxSize:(const int) size;

/**
 * Get a string from a given address.
 * @param address The specified address.
 *                The address must contain a null terminated string(UTF-16 encoding).
 * @return An array containing the string. 
 */
-(NSMutableArray*) getString:(void*) address;

/**
 * Writes a string to a given address.
 * If the size of the string is bigger(in bytes) than the size parameter, the string
 * will not be written.
 * @param value The given string.
 * @param address The specified address.
 *                The address will contain a null terminated string(UTF-16 encoding).
 * @return The string's size(in bytes). 
 */
-(int) writeString:(NSString*) value
         atAddress:(void*) address
           maxSize:(const int) size;

/**
 * Gets a date from a given address.
 * @param address The specified address.
 *                The address must contain a 4-byte int representing the number of 
 *                seconds elapsed since January 1 1970(Unix time).
 * @return An array containing the date. 
 */
-(NSMutableArray*) getDate:(void*) address;

/**
 * Writes a date to a given address.
 * @param date The given date.
 * @param address The specified address.
 *                The address will contain a 4-byte int representing the number of 
 *                seconds elapsed since January 1 1970(Unix time).
 * @param size The maximum size(in bytes) that can be written at the given address.
 * @return The size(in bytes) of the date. If the size of the date is greater than
 *         the maximum size(the size parameter) the date was not written. 
 */
-(int) writeDate:(NSDate*) date
        atAddress:(void*) address
         maxSize:(int) size;

/**
 * Gets the bytes from a given address.
 * @param address The specified address.
 * @param size The size that can be read from the memory address.
 * @return An array containing the pixels. 
 */
-(NSMutableArray*) getBytes:(void*) address
                       size:(const int) size;

/**
 * Adds an given string to a given array. If string param is nil
 * an empty string will be added.
 * @param array The given array.
 * @param value The given value.
 */
-(void) addStringToArray:(NSMutableArray*) array
                  string:(NSString*) value;

/**
 * Gets field type for a specified field ID.
 * @param The given field ID(one of MA_PIM_FIELD_CONTACT constants).
 * @return The field type (one of MA_PIM_TYPE constants), or 
 *         MA_PIM_ERR_UNAVAILABLE_FIELD in case the field is not supported.
 */
-(int) getFieldType:(const int) fieldID;

/**
 * Gets the string associated with a given attribute.
 * @param attribute Must be one of MA_PIM_ATTR constants.
 * @return The string associated with a given attribute. 
 */
-(NSString*) getStringFromAttribute:(const int) attribute;

/**
 * Gets the attribute associated with a specified string.
 * @param string The given string.
 * @return One of MA_PIM_ATTR constants. 
 */
-(int) getAttributeFromString:(CFStringRef) string;

/**
 * Gets the absolute memory address for a specified address from MoSync memory pool.
 * @param address The given address from memory pool.
 * param size The size of the address in bytes.
 * @return The absolute memory address.
 */
-(void*) getValidatedMemRange:(const int) address
                     withSize:(const int) size;
@end
