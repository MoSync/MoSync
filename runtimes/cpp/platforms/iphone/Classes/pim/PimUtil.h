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

	NSMutableDictionary* mAddressAttributes;
	NSMutableDictionary* mEmailAttributes;
	NSMutableDictionary* mPhoneAttributes;
	NSMutableDictionary* mWebSiteAttributes;
	NSMutableDictionary* mIMAttributes;
	NSMutableDictionary* mRelationAttributes;
	NSMutableDictionary* mNoAttributes;
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
 * Gets an int value from a given address.
 * @param address The specified address.
 * @return An array containing an NSNumber type object.
 */
-(NSMutableArray*) getIntValue:(void*) address;

/**
 * Writes an int value to a given address.
 * @param value The given int value.
 * @param address The specified address.
 * @param size The maximum size(in bytes) that can be written at the given address.
 * @return The size(in bytes) of an int. If the size of the int is greater than
 *         the maximum size(the size parameter) the date was not written.
 */
-(int) writeIntValue:(const int) value
           atAddress:(void*) address
             maxSize:(const int) size;

/**
 * Gets the image data from a data handle.
 * @param handle The given data handle.
 * @return The image data if the handle is valid, or nil otherwise.
 * The ownership of the result is passed to the caller.
 */
-(NSData*) getImageDataFromHandle:(const int) handle;

/**
 * Creates a data handle for a NSData type object.
 * @param data The given data object.
 * @return The data handle.
 */
-(const int) createDataHandle:(NSData*) data;

/**
 * Adds an given string to a given array. If string param is nil
 * an empty string will be added.
 * @param array The given array.
 * @param value The given value.
 */
-(void) addStringToArray:(NSMutableArray*) array
                  string:(NSString*) value;

/**
 * Gets field structure(the type, if it can contain only one value and the allowed
 * attributes for this field) for a specified field ID.
 * @param fieldId The given field ID(one of MA_PIM_FIELD_CONTACT constants).
 * @param type Will contain the field type (one of MA_PIM_TYPE constants).
 * @param singleValue Will be set to true if the field can contain only one value.
 * @return MA_PIM_ERR_NONE if the field is supported, or
 *         MA_PIM_ERR_FIELD_UNSUPPORTED in case the field is not supported.
 */
-(int) fieldStructure:(const int) fieldID
               setType:(int*) type
     setIsSingleValue:(bool*) singleValue;

/**
 * Gets the allowed attributes for an given field ID.
 * @param fieldID The given field ID.
 * @return An dictionary that contains:
 *         - keys An string array with all the attributes(e.g. MA_PIM_ATTR_ADDR_HOME).
 *         - values An string array with all the strings associedted with attributes.
 *         Do not release the returned object.
 *         If the field is not supported or it does not allow attributes an empty
 *         dictionary will be returned.
 */
-(NSMutableDictionary*) getAttributesForFieldId:(const int) fieldID;

/**
 * Gets the custom attribute for a specified field ID.
 * @param fieldID the given field ID.
 * @return The custom attribute, or 0 if it does not have one.
 */
-(int) getCustomAttributeForFieldID:(const int) fieldID;

/**
 * Checks if a given field supports attributes.
 * @param fieldID The given field ID. One of the MA_PIM_FIELD_CONTACT contants.
 * @return True if field supports attributes, false otherwise.
 */
-(bool) fieldSupportsAttribute:(const int) fieldID;

/**
 * Gets the absolute memory address for a specified address from MoSync memory pool.
 * @param address The given address from memory pool.
 * param size The size of the address in bytes.
 * @return The absolute memory address.
 */
-(void*) getValidatedMemRange:(const int) address
                     withSize:(const int) size;
@end
