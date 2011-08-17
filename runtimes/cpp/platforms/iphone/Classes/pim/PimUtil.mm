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
    NSString* homeLabel = [NSString stringWithString:(NSString*)kABHomeLabel];
    NSString* workLabel = [NSString stringWithString:(NSString*)kABWorkLabel];
    NSString* otherLabel = [NSString stringWithString:(NSString*)kABOtherLabel];
    mNoAttributes = [[NSMutableDictionary alloc] init];
    
    mAddressAttributes = [[NSMutableDictionary alloc] init];
    [mAddressAttributes setValue:homeLabel
                          forKey:[NSString stringWithFormat:@"%d", MA_PIM_ATTR_ADDR_HOME]];
    [mAddressAttributes setValue:workLabel
                          forKey:[NSString stringWithFormat:@"%d", MA_PIM_ATTR_ADDR_WORK]];
    [mAddressAttributes setValue:otherLabel
                          forKey:[NSString stringWithFormat:@"%d", MA_PIM_ATTR_ADDR_OTHER]];
    [mAddressAttributes setValue:[NSString string]
                          forKey:[NSString stringWithFormat:@"%d", MA_PIM_ATTR_ADDR_CUSTOM]];

    mEmailAttributes = [[NSMutableDictionary alloc] init];
    [mEmailAttributes setValue:homeLabel
                        forKey:[NSString stringWithFormat:@"%d", MA_PIM_ATTR_EMAIL_HOME]];
    [mEmailAttributes setValue:workLabel
                        forKey:[NSString stringWithFormat:@"%d", MA_PIM_ATTR_EMAIL_WORK]];
    [mEmailAttributes setValue:[NSString stringWithString:@"mobile"]
                        forKey:[NSString stringWithFormat:@"%d", MA_PIM_ATTR_EMAIL_MOBILE]];
    [mEmailAttributes setValue:otherLabel
                        forKey:[NSString stringWithFormat:@"%d", MA_PIM_ATTR_EMAIL_OTHER]];
    [mEmailAttributes setValue:[NSString string]
                        forKey:[NSString stringWithFormat:@"%d", MA_PIM_ATTR_EMAIL_CUSTOM]];

    mPhoneAttributes = [[NSMutableDictionary alloc] init];
    [mPhoneAttributes setValue:homeLabel
                        forKey:[NSString stringWithFormat:@"%d", MA_PIM_ATTR_PHONE_HOME]];
    [mPhoneAttributes setValue:workLabel
                        forKey:[NSString stringWithFormat:@"%d", MA_PIM_ATTR_PHONE_WORK]];
    [mPhoneAttributes setValue:homeLabel
                        forKey:[NSString stringWithFormat:@"%d", MA_PIM_ATTR_PHONE_OTHER]];
    [mPhoneAttributes setValue:[NSString string]
                        forKey:[NSString stringWithFormat:@"%d", MA_PIM_ATTR_PHONE_CUSTOM]];
    [mPhoneAttributes setValue:[NSString stringWithString:(NSString*)kABPersonPhoneMobileLabel]
                        forKey:[NSString stringWithFormat:@"%d", MA_PIM_ATTR_PHONE_MOBILE]];
    [mPhoneAttributes setValue:[NSString stringWithString:(NSString*)kABPersonPhoneHomeFAXLabel]
                        forKey:[NSString stringWithFormat:@"%d", MA_PIM_ATTR_PHONE_HOME_FAX]];
    [mPhoneAttributes setValue:[NSString stringWithString:(NSString*)kABPersonPhoneWorkFAXLabel]
                        forKey:[NSString stringWithFormat:@"%d", MA_PIM_ATTR_PHONE_WORK_FAX]];
    [mPhoneAttributes setValue:[NSString stringWithString:(NSString*)kABPersonPhonePagerLabel]
                        forKey:[NSString stringWithFormat:@"%d", MA_PIM_ATTR_PHONE_PAGER]];
    [mPhoneAttributes setValue:[NSString stringWithString:(NSString*)kABPersonPhoneIPhoneLabel]
                        forKey:[NSString stringWithFormat:@"%d", MA_PIM_ATTR_PHONE_IPHONE]];
    [mPhoneAttributes setValue:[NSString stringWithString:@"callback"]
                        forKey:[NSString stringWithFormat:@"%d", MA_PIM_ATTR_PHONE_CALLBACK]];
    [mPhoneAttributes setValue:[NSString stringWithString:@"company main"]
                        forKey:[NSString stringWithFormat:@"%d", MA_PIM_ATTR_PHONE_COMPANY_MAIN]];
    [mPhoneAttributes setValue:[NSString stringWithString:@"ISDN"]
                        forKey:[NSString stringWithFormat:@"%d", MA_PIM_ATTR_PHONE_ISDN]];
    [mPhoneAttributes setValue:[NSString stringWithString:@"radio"]
                        forKey:[NSString stringWithFormat:@"%d", MA_PIM_ATTR_PHONE_RADIO]];
    [mPhoneAttributes setValue:[NSString stringWithString:@"other fax"]
                        forKey:[NSString stringWithFormat:@"%d", MA_PIM_ATTR_PHONE_OTHER_FAX]];
    [mPhoneAttributes setValue:[NSString stringWithString:@"telex"]
                        forKey:[NSString stringWithFormat:@"%d", MA_PIM_ATTR_PHONE_TELEX]];
    [mPhoneAttributes setValue:[NSString stringWithString:@"TTY TDD"]
                        forKey:[NSString stringWithFormat:@"%d", MA_PIM_ATTR_PHONE_TTY_TDD]];
    [mPhoneAttributes setValue:[NSString stringWithString:@"work mobile"]
                        forKey:[NSString stringWithFormat:@"%d", MA_PIM_ATTR_PHONE_WORK_MOBILE]];
    [mPhoneAttributes setValue:[NSString stringWithString:@"work pager"]
                        forKey:[NSString stringWithFormat:@"%d", MA_PIM_ATTR_PHONE_WORK_PAGER]];
    [mPhoneAttributes setValue:[NSString stringWithString:@"assistent"]
                        forKey:[NSString stringWithFormat:@"%d", MA_PIM_ATTR_PHONE_ASSISTANT]];
    [mPhoneAttributes setValue:[NSString stringWithString:@"MMS"]
                        forKey:[NSString stringWithFormat:@"%d", MA_PIM_ATTR_PHONE_MMS]];
    
    mWebSiteAttributes = [[NSMutableDictionary alloc] init];
    [mWebSiteAttributes setValue:homeLabel
                        forKey:[NSString stringWithFormat:@"%d", MA_PIM_ATTR_WEBSITE_HOME]];
    [mWebSiteAttributes setValue:workLabel
                          forKey:[NSString stringWithFormat:@"%d", MA_PIM_ATTR_WEBSITE_WORK]];
    [mWebSiteAttributes setValue:otherLabel
                          forKey:[NSString stringWithFormat:@"%d", MA_PIM_ATTR_WEBSITE_OTHER]];
    [mWebSiteAttributes setValue:[NSString stringWithString:@"blog"]
                          forKey:[NSString stringWithFormat:@"%d", MA_PIM_ATTR_WEBSITE_BLOG]];
    [mWebSiteAttributes setValue:[NSString stringWithString:@"profile"]
                          forKey:[NSString stringWithFormat:@"%d", MA_PIM_ATTR_WEBSITE_PROFILE]];
    [mWebSiteAttributes setValue:[NSString stringWithString:@"FTP"]
                          forKey:[NSString stringWithFormat:@"%d", MA_PIM_ATTR_WEBSITE_FTP]];
    [mWebSiteAttributes setValue:[NSString string]
                          forKey:[NSString stringWithFormat:@"%d", MA_PIM_ATTR_WEBSITE_CUSTOM]];
    
    mIMAttributes = [[NSMutableDictionary alloc] init];
    [mIMAttributes setValue:homeLabel
                          forKey:[NSString stringWithFormat:@"%d", MA_PIM_ATTR_ADDR_HOME]];
    [mIMAttributes setValue:workLabel
                          forKey:[NSString stringWithFormat:@"%d", MA_PIM_ATTR_ADDR_WORK]];
    [mIMAttributes setValue:otherLabel
                          forKey:[NSString stringWithFormat:@"%d", MA_PIM_ATTR_ADDR_OTHER]];
    [mIMAttributes setValue:[NSString string]
                          forKey:[NSString stringWithFormat:@"%d", MA_PIM_ATTR_ADDR_CUSTOM]];
    
    mRelationAttributes = [[NSMutableDictionary alloc] init];
    [mRelationAttributes setValue:(NSString*)kABPersonMotherLabel
                     forKey:[NSString stringWithFormat:@"%d", MA_PIM_ATTR_RELATION_MOTHER]];
    [mRelationAttributes setValue:(NSString*)kABPersonFatherLabel
                           forKey:[NSString stringWithFormat:@"%d", MA_PIM_ATTR_RELATION_FATHER]];
    [mRelationAttributes setValue:(NSString*)kABPersonParentLabel
                           forKey:[NSString stringWithFormat:@"%d", MA_PIM_ATTR_RELATION_PARENT]];
    [mRelationAttributes setValue:(NSString*)kABPersonSisterLabel
                           forKey:[NSString stringWithFormat:@"%d", MA_PIM_ATTR_RELATION_SISTER]];
    [mRelationAttributes setValue:(NSString*)kABPersonBrotherLabel
                           forKey:[NSString stringWithFormat:@"%d", MA_PIM_ATTR_RELATION_BROTHER]];
    [mRelationAttributes setValue:(NSString*)kABPersonChildLabel
                           forKey:[NSString stringWithFormat:@"%d", MA_PIM_ATTR_RELATION_CHILD]];
    [mRelationAttributes setValue:(NSString*)kABPersonFriendLabel
                           forKey:[NSString stringWithFormat:@"%d", MA_PIM_ATTR_RELATION_FRIEND]];
    [mRelationAttributes setValue:(NSString*)kABPersonSpouseLabel
                           forKey:[NSString stringWithFormat:@"%d", MA_PIM_ATTR_RELATION_SPOUSE]];
    [mRelationAttributes setValue:(NSString*)kABPersonPartnerLabel
                           forKey:[NSString stringWithFormat:@"%d", MA_PIM_ATTR_RELATION_PARTNER]];
    [mRelationAttributes setValue:(NSString*)kABPersonManagerLabel
                           forKey:[NSString stringWithFormat:@"%d", MA_PIM_ATTR_RELATION_MANAGER]];
    [mRelationAttributes setValue:(NSString*)kABPersonAssistantLabel
                           forKey:[NSString stringWithFormat:@"%d", MA_PIM_ATTR_RELATION_ASSISTANT]];
    [mRelationAttributes setValue:[NSString stringWithString:@"domectic partner"]
                           forKey:[NSString stringWithFormat:@"%d", MA_PIM_ATTR_RELATION_DOMESTIC_PARTNER]];
    [mRelationAttributes setValue:[NSString stringWithString:@"reffered by"]
                           forKey:[NSString stringWithFormat:@"%d", MA_PIM_ATTR_RELATION_REFERRED_BY]];
    [mRelationAttributes setValue:[NSString stringWithString:@"relative"]
                           forKey:[NSString stringWithFormat:@"%d", MA_PIM_ATTR_RELATION_RELATIVE]];
    [mRelationAttributes setValue:[NSString string]
                           forKey:[NSString stringWithFormat:@"%d", MA_PIM_ATTR_RELATION_CUSTOM]];
    return [super init];
}

/**
 * Release all the objects.
 */
- (void) dealloc 
{
    [mAddressAttributes release];
    [mEmailAttributes release];
    [mPhoneAttributes release];
    [mWebSiteAttributes release];
    [mIMAttributes release];
    [mRelationAttributes release];
    [mNoAttributes release];
    
    [super dealloc];
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
     setIsSingleValue:(bool*) singleValue
{
    int returnValue = MA_PIM_ERR_NONE;
    *type = MA_PIM_ERR_FIELD_UNSUPPORTED;
    *singleValue = true;
    
    switch (fieldID) 
    {
        case MA_PIM_FIELD_CONTACT_ADDR:
            *type = MA_PIM_TYPE_STRING_ARRAY;
            *singleValue = false;
            break;
        case MA_PIM_FIELD_CONTACT_BIRTHDAY:
            *type = MA_PIM_TYPE_DATE;
            *singleValue = false;
            break;
        case MA_PIM_FIELD_CONTACT_CLASS:
            returnValue = MA_PIM_ERR_FIELD_UNSUPPORTED;
            break;
        case MA_PIM_FIELD_CONTACT_EMAIL:
            *type = MA_PIM_TYPE_STRING;
            *singleValue = false;
            break;
        case MA_PIM_FIELD_CONTACT_FORMATTED_ADDR:
            returnValue = MA_PIM_ERR_FIELD_UNSUPPORTED;
            break;
        case MA_PIM_FIELD_CONTACT_FORMATTED_NAME:
            returnValue = MA_PIM_ERR_FIELD_UNSUPPORTED;
            break;
        case MA_PIM_FIELD_CONTACT_NAME:
            *type = MA_PIM_TYPE_STRING_ARRAY;
            *singleValue = false;
            break;
        case MA_PIM_FIELD_CONTACT_NICKNAME:
            *type = MA_PIM_TYPE_STRING;
            break;
        case MA_PIM_FIELD_CONTACT_NOTE:
            *type = MA_PIM_TYPE_STRING;
            break;
        case MA_PIM_FIELD_CONTACT_ORG:
            *type = MA_PIM_TYPE_STRING;
            break;
        case MA_PIM_FIELD_CONTACT_PHOTO:
            type = MA_PIM_TYPE_BINARY;
            break;
        case MA_PIM_FIELD_CONTACT_PHOTO_URL:
            *type = MA_PIM_TYPE_STRING;
            break;
        case MA_PIM_FIELD_CONTACT_REVISION:
            *type = MA_PIM_TYPE_DATE;
            break;
        case MA_PIM_FIELD_CONTACT_TEL:
            *type = MA_PIM_TYPE_STRING;
            *singleValue = false;
            break;
        case MA_PIM_FIELD_CONTACT_TITLE:
            *type = MA_PIM_TYPE_STRING;
            break;
        case MA_PIM_FIELD_CONTACT_URL:
            *type = MA_PIM_TYPE_STRING;
            *singleValue = false;
            break;
        case MA_PIM_FIELD_CONTACT_ORG_INFO:
            *type = MA_PIM_TYPE_STRING_ARRAY;
            break;
        case MA_PIM_FIELD_CONTACT_IM:
            *type = MA_PIM_TYPE_STRING_ARRAY;
            *singleValue = false;
            break;
        case MA_PIM_FIELD_CONTACT_RELATION:
            *type = MA_PIM_TYPE_STRING;
            *singleValue = false;
            break;
        default:
            returnValue = MA_PIM_ERR_FIELD_UNSUPPORTED;
    }
    
    return returnValue;
}

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
-(NSMutableDictionary*) getAttributesForFieldId:(const int) fieldID
{
    NSMutableDictionary* returnedDictionary = mNoAttributes;
    switch (fieldID) 
    {
        case MA_PIM_FIELD_CONTACT_ADDR:
            returnedDictionary = mAddressAttributes;
            break;
        case MA_PIM_FIELD_CONTACT_EMAIL:
            returnedDictionary = mEmailAttributes;
            break;
        case MA_PIM_FIELD_CONTACT_TEL:
            returnedDictionary = mPhoneAttributes;
            break;
        case MA_PIM_FIELD_CONTACT_URL:
            returnedDictionary = mWebSiteAttributes;
            break;
        case MA_PIM_FIELD_CONTACT_IM:
            returnedDictionary = mIMAttributes;
            break;
        case MA_PIM_FIELD_CONTACT_RELATION:
            returnedDictionary = mRelationAttributes;    
            break;
    }
    
    return returnedDictionary;
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
