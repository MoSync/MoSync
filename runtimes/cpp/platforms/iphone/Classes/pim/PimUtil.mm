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

#import "PimUtil.h"
#include "PimSyscall.h"
#include "config_platform.h"
#import "helpers/cpp_defs.h"
#import "helpers/cpp_ix_pim.h"

#import <AddressBook/AddressBook.h>

static PimUtils *sharedInstance = nil;

@implementation PimUtils


/**
 * Returns an instance to the shared singleton. 
 * @return The shared generator object.
 */
+(PimUtils*) sharedInstance
{

   if (nil == sharedInstance) {
          sharedInstance = [[PimUtils alloc] init];
   }
      
   return sharedInstance;
}

/**
 * Releases the class instance.
 */
+(void) deleteInstance
{
    [sharedInstance release];
}

-(int) getNextHandle
{
    return mHandle++;
}

- (id)init
{
    mHandle = 0;
    return [super init];
}

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
{

    NSMutableArray* array = [[NSMutableArray alloc] init];
    int noStrings = *(int*) address;
//    NSLog(@"PimUtil--getArray--noStrings = %d", noStrings);
    
    UInt16* src = (UInt16*) ((int)address + sizeof(int));
    for (int i = 0; i < noStrings; i++) {
        UInt16* stringSrc = src;
        // Calculate the length of the string.
        int stringLength = 0;
        bool condition = true;
        while (condition) {
            
            if (0 == *src)
            {
                src++;
                condition = false;
            } 
            else 
            {
                src++;
                stringLength += sizeof(UInt16);    
            }
        }
        // Create string with UTF-16 encoding.
        NSString* string = [[NSString alloc] initWithBytes:stringSrc length:stringLength 
                                                  encoding:NSUTF16LittleEndianStringEncoding];
        [array addObject:string];
        NSLog(@"PimUtil--getArray--string %d = %@", i, string);
    }
    
    return array;
}

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
                maxSize:(const int) size
{
    int noStrings = [array count];
    *(int*) address = noStrings;
//    NSLog(@"PimUtil--writeStringArray--noStrings = %d", noStrings);
   
    UInt16* dst = (UInt16*) ((int)address + sizeof(int));
    
    for (int i = 0; i < noStrings; i++) {
        NSString* currentString = [array objectAtIndex:i];
        [self writeString:currentString atAddress:(void*) dst maxSize:512];
        dst += [currentString length] + 1; 
    }
    
    return 1;
}

/**
 * Get a string from a given address.
 * @param address The specified address.
 *                The address must contain a null terminated string(UTF-16 encoding).
 * @return An array containing the string. 
 */
-(NSMutableArray*) getString:(void*) address
{
    NSMutableArray* array = [[NSMutableArray alloc] init];
    UInt16* src = (UInt16*) (address);
    
    // Calculate the length of the string.
    int stringLength = 0;
    bool condition = true;
    while (condition) {
            
        if (0 == *src)
        {
            src++;
            condition = false;
        } 
        else 
        {
           src++;
           stringLength += sizeof(UInt16);    
        }
    }
    // Create string with UTF-16 encoding.
    NSString* string = [[NSString alloc] initWithBytes:address length:stringLength 
                                                  encoding:NSUTF16LittleEndianStringEncoding];
    [array addObject:string];
//    NSLog(@"PimUtil--getString--string = %@", string);
    
    return array;
}

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
           maxSize:(const int) size
{

//    NSLog(@"PimUtil--writeString--strings = %@", value);
    
    // Check the size of the string.
    int stringSize = ([value length] + 1) * sizeof(UInt16);
    if(stringSize > size)
    {
        return stringSize;
    }
    
    // Write string at the given address.
    UInt16* dst = (UInt16*)address;
    for (int charIndex = 0; charIndex < [value length]; charIndex++) {
        UniChar currentChar = [value characterAtIndex:charIndex];
        *dst = currentChar;
        dst++;
    }
        
    *dst = 0;
    dst++;
    
    return stringSize;
}

/**
 * Gets a date from a given address.
 * @param address The specified address.
 *                The address must contain a 4-byte int representing the number of 
 *                seconds elapsed since January 1 1970(Unix time).
 * @return An array containing the date. 
 */
-(NSMutableArray*) getDate:(void*) address;

{
    int seconds = *(int*) address; 
    NSData* date = [[NSDate alloc] initWithTimeIntervalSince1970:seconds];
    NSMutableArray* array = [[NSMutableArray alloc] init];
    [array addObject:date];
    
    return array;
}

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
         maxSize:(int) size
{
    int seconds = [date timeIntervalSince1970];
    if (sizeof(seconds) < size) 
    {
        *(int*) address = seconds; 
    }
    
    return sizeof(seconds);
}

/**
 * Gets the bytes from a given address.
 * @param address The specified address.
 * @param size The size that can be read from the memory address.
 * @return An array containing the pixels. 
 */
-(NSMutableArray*) getBytes:(void*) address
                       size:(const int) size
{
    int handle = *(int*) address;
    char *newBuf = new char[size];
	PimMaReadData(handle, newBuf,0,size);
    
    NSData* data = [NSData dataWithBytes:newBuf length:size];
    NSMutableArray* array = [[NSMutableArray alloc] init];
    [array addObject:data];
    
    return array;
}

/**
 * Adds an given string to a given array. If string param is nil
 * an empty string will be added.
 * @param array The given array.
 * @param value The given value.
 */
-(void) addStringToArray:(NSMutableArray*) array
                  string:(NSString*) value
{
    if (nil == value)
    {
        [array addObject:@""];
    }
    else
    {
        [array addObject:value];
    }
    
}


/**
 * Gets field type for a specified field ID.
 * @param The given field ID(one of MA_PIM_FIELD_CONTACT constants).
 * @return The field type (one of MA_PIM_TYPE constants), or 
 *         MA_PIM_ERR_UNAVAILABLE_FIELD in case the field is not supported.
 */
-(int) getFieldType:(const int) fieldID
{
    int returnValue;
    
    switch (fieldID) {
        case MA_PIM_FIELD_CONTACT_ADDR:
            returnValue = MA_PIM_TYPE_STRING_ARRAY;
            break;
        case MA_PIM_FIELD_CONTACT_BIRTHDAY:
            returnValue = MA_PIM_TYPE_DATE;
            break;
        case MA_PIM_FIELD_CONTACT_CLASS:
            returnValue = MA_PIM_TYPE_INT;
            break;
        case MA_PIM_FIELD_CONTACT_EMAIL:
            returnValue = MA_PIM_TYPE_STRING;
            break;
        case MA_PIM_FIELD_CONTACT_FORMATTED_ADDR:
            returnValue = MA_PIM_TYPE_STRING;
            break;
        case MA_PIM_FIELD_CONTACT_FORMATTED_NAME:
            returnValue = MA_PIM_TYPE_STRING;
            break;
        case MA_PIM_FIELD_CONTACT_NAME:
            returnValue = MA_PIM_TYPE_STRING_ARRAY;
            break;
        case MA_PIM_FIELD_CONTACT_NICKNAME:
            returnValue = MA_PIM_TYPE_STRING;
            break;
        case MA_PIM_FIELD_CONTACT_NOTE:
            returnValue = MA_PIM_TYPE_STRING;
            break;
        case MA_PIM_FIELD_CONTACT_ORG:
            returnValue = MA_PIM_TYPE_STRING;
            break;
        case MA_PIM_FIELD_CONTACT_PHOTO:
            returnValue = MA_PIM_TYPE_BINARY;
            break;
        case MA_PIM_FIELD_CONTACT_PHOTO_URL:
            returnValue = MA_PIM_TYPE_STRING;
            break;
        case MA_PIM_FIELD_CONTACT_REVISION:
            returnValue = MA_PIM_TYPE_DATE;
            break;
        case MA_PIM_FIELD_CONTACT_TEL:
            returnValue = MA_PIM_TYPE_STRING;
            break;
        case MA_PIM_FIELD_CONTACT_TITLE:
            returnValue = MA_PIM_TYPE_STRING;
            break;
        case MA_PIM_FIELD_CONTACT_URL:
            returnValue = MA_PIM_TYPE_STRING;
            break;
        case MA_PIM_FIELD_CONTACT_ORG_INFO:
            returnValue = MA_PIM_TYPE_STRING_ARRAY;
            break;
        case MA_PIM_FIELD_CONTACT_IM:
            returnValue = MA_PIM_TYPE_STRING_ARRAY;
            break;
        case MA_PIM_FIELD_CONTACT_RELATION:
            returnValue = MA_PIM_TYPE_STRING;
            break;
        default:
            returnValue = MA_PIM_ERR_INVALID_INDEX;
    }
    
    return returnValue;
}

/**
 * Gets the string associated with a given attribute.
 * @param attribute Must be one of MA_PIM_ATTR constants.
 * @return The string associated with a given attribute. 
 */
-(NSString*) getStringFromAttribute:(const int) attribute
{
    NSString* string;
    
    switch (attribute) {
        case MA_PIM_ATTR_ASST:
            string = [NSString stringWithString:@"Assistant"];
            break;
        case MA_PIM_ATTR_AUTO:
            string = [NSString stringWithString:@"Auto"];
            break;    
        case MA_PIM_ATTR_FAX:
            string = [NSString stringWithString:@"Fax"];        
            break;  
        case MA_PIM_ATTR_HOME:
            string = [NSString stringWithString:(NSString*) kABHomeLabel];
            break;  
        case MA_PIM_ATTR_MOBILE:
            string = (NSString*)kABPersonPhoneMobileLabel;
            break;  
        case MA_PIM_ATTR_OTHER:
            string = [NSString stringWithString:(NSString*)kABOtherLabel];
            break;  
        case MA_PIM_ATTR_PAGER:
            string = [NSString stringWithString:(NSString*)kABPersonPhonePagerLabel];
            break;  
        case MA_PIM_ATTR_PREFERRED:
            string = [NSString stringWithString:@"Preferred"];
            break;  
        case MA_PIM_ATTR_SMS:
            string = [NSString stringWithString:@"SMS"];
            break;  
        case MA_PIM_ATTR_WORK:
            string = [NSString stringWithString:(NSString*)kABWorkLabel];
            break;  
        case MA_PIM_ATTR_RELATION_MOTHER:
            string = [NSString stringWithString:(NSString*)kABPersonMotherLabel];
            break;
        case MA_PIM_ATTR_RELATION_FATHER:
            string = [NSString stringWithString:(NSString*)kABPersonFatherLabel];
            break;
        case MA_PIM_ATTR_RELATION_PARENT:
            string = [NSString stringWithString:(NSString*)kABPersonParentLabel];
            break;
        case MA_PIM_ATTR_RELATION_SISTER:
            string = [NSString stringWithString:(NSString*)kABPersonSisterLabel];
            break;
        case MA_PIM_ATTR_RELATION_BROTHER:
            string = [NSString stringWithString:(NSString*)kABPersonBrotherLabel];
            break;
        case MA_PIM_ATTR_RELATION_CHILD:
            string = [NSString stringWithString:(NSString*)kABPersonChildLabel];
            break;
        case MA_PIM_ATTR_RELATION_FRIEND:
            string = [NSString stringWithString:(NSString*)kABPersonFriendLabel];
            break;
        case MA_PIM_ATTR_RELATION_SPOUSE:
            string = [NSString stringWithString:(NSString*)kABPersonSpouseLabel];
            break;    
        case MA_PIM_ATTR_RELATION_PARTNER:
            string = [NSString stringWithString:(NSString*)kABPersonPartnerLabel];
            break;
        case MA_PIM_ATTR_RELATION_MANAGER:
            string = [NSString stringWithString:(NSString*)kABPersonManagerLabel];
            break;
        case MA_PIM_ATTR_RELATION_ASSISTANT:
            string = [NSString stringWithString:(NSString*)kABPersonAssistantLabel];
            break;
        default:
            string = [NSString stringWithString:@""];
    }
    
    return string;

}

/**
 * Gets the attribute associated with a specified string.
 * @param string The given string.
 * @return One of MA_PIM_ATTR constants. 
 */
-(int) getAttributeFromString:(CFStringRef) string
{
    
}

/**
 * Gets the absolute memory address for a specified address from MoSync memory pool.
 * @param address The given address from memory pool.
 * param size The size of the address in bytes.
 * @return The absolute memory address.
 */
-(void*) getValidatedMemRange:(const int) address
                     withSize:(const int) size
{
    return PimGetValidatedMemRange(address, size);
}

/**
 * Release all objects.
 */
- (void)release
{
    [super release];
}

@end
