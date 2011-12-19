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
#include "Syscall.h"
#import <AddressBook/AddressBook.h>
#import "PimError.h"
#import "MoSyncPanic.h"

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
    mHandle = 1;
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
                          forKey:[NSString stringWithFormat:@"%d", MA_PIM_ATTR_IM_HOME]];
    [mIMAttributes setValue:workLabel
                          forKey:[NSString stringWithFormat:@"%d", MA_PIM_ATTR_IM_WORK]];
    [mIMAttributes setValue:otherLabel
                          forKey:[NSString stringWithFormat:@"%d", MA_PIM_ATTR_IM_OTHER]];
    [mIMAttributes setValue:[NSString string]

                          forKey:[NSString stringWithFormat:@"%d", MA_PIM_ATTR_IM_CUSTOM]];

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
 * Get an array of string from a given address for a specified field.
 * @param address The specified address.
 *                The address must have the following structure:
 *                     - the first element must be a 4-byte int that specifies
 *                       the number of strings that can be read.
 *                     - first null terminated string(UTF-16 encoding).
 *                     - second null terminated string(UTF-16 encoding).
 *                     - etc
 * @param fieldID The given field.
 * @param size The size of the string array(how many bytes can be read from the given address).
 * @return An array containing the strings, or nil in case of error.
 */
-(NSMutableArray*) getStringArray:(void*) address
                       forFieldID:(const int) fieldID
                         withSize:(const int) size
{

    NSMutableArray* array = [[NSMutableArray alloc] init];
    int noStrings = *(int*) address;
    int bytesRead = sizeof(int);
    bool invalidBuffer = false;

    UInt16* src = (UInt16*) ((int)address + sizeof(int));
    for (int i = 0; ((i < noStrings) && !invalidBuffer); i++)
    {
        UInt16* stringSrc = src;

        // Calculate the length of the string.
        int stringLength = 0;
        bool condition = true;
        while (condition)
        {

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

            // Check if there are more bytes that can be read.
            bytesRead += sizeof(UInt16);
            if (bytesRead >= size)
            {
                invalidBuffer = true;
                condition = false;
            }
        }

        if (!invalidBuffer)
        {
            // Create string with UTF-16 encoding.
            NSString* string = [[NSString alloc] initWithBytes:stringSrc length:stringLength
                                                  encoding:NSUTF16LittleEndianStringEncoding];
            [array addObject:string];
            [string release];
        }
    }

    // Check if the number of strings read are valid for the given field id.
    if (!invalidBuffer)
    {
        int noIndices = [self getNumberOfIndicesForField:fieldID];
        if (noIndices == [array count])
        {
            return array;
        }
    }

    [array release];
    return nil;
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
 * @return The size(in bytes) of the strings.
 */
-(int) writeStringArray:(NSMutableArray*) array
              atAddress:(void*) address
                maxSize:(const int) size
{
    int noStrings = [array count];
    *(int*) address = noStrings;

    UInt16* dst = (UInt16*) ((int)address + sizeof(int));
    int countWrittenBytes = sizeof(int);
    for (int i = 0; i < noStrings; i++)
    {
        NSString* currentString = [array objectAtIndex:i];
        int remainingSize = size - countWrittenBytes;

        int result = [self writeString:currentString atAddress:(void*) dst maxSize:remainingSize];
        countWrittenBytes += result;
        dst += [currentString length] + 1;
    }

    return countWrittenBytes;
}

/**
 * Get a string from a given address.
 * @param address The specified address.
 *                The address must contain a null terminated string(UTF-16 encoding).
 * @param size The length of the string in bytes.
 * @return An array containing the string.
 */
-(NSMutableArray*) getString:(void*) address
                    withSize:(const int) size
{
    NSMutableArray* array = [[NSMutableArray alloc] init];

    // Create string with UTF-16 encoding.
    NSString* string = [[NSString alloc] initWithBytes:address length:size
                                              encoding:NSUTF16LittleEndianStringEncoding];
    [array addObject:string];
    [string release];

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
    // Check the size of the string.
    int stringSize = ([value length] + 1) * sizeof(UInt16);

    // Write string at the given address.
    UInt16* dst = (UInt16*)address;
    for (int charIndex = 0; charIndex < [value length]; charIndex++)
    {
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
 *                The address must contain a 4-bytes int representing the number of
 *                seconds elapsed since January 1 1970(Unix time).
 * @return An array containing the date.
 */
-(NSMutableArray*) getDate:(void*) address
{
    int seconds = *(int*) address;
    NSDate* date = [[NSDate alloc] initWithTimeIntervalSince1970:seconds];
    NSMutableArray* array = [[NSMutableArray alloc] init];
    [array addObject:date];
    [date release];
    return array;
}

/**
 * Writes a date to a given address.
 * @param date The given date.
 * @param address The specified address.
 *                The address will contain a 4-bytes int representing the number of
 *                seconds elapsed since January 1 1970(Unix time).
 * @return The size(in bytes) of the date.
 */
-(int) writeDate:(NSDate*) date
       atAddress:(void*) address
{
    int seconds = [date timeIntervalSince1970];
    *(int*) address = seconds;
    return sizeof(seconds);
}

/**
 * Gets an int value from a given address.
 * @param address The specified address.
 * @return An array containing an NSNumber type object.
 */
-(NSMutableArray*) getIntValue:(void*) address
{
    int value = *(int*) address;
    NSNumber* number = [NSNumber numberWithInt:value];
    NSMutableArray* array = [[NSMutableArray alloc] init];
    [array addObject:number];

    return array;
}

/**
 * Writes an int value to a given address.
 * @param value The given int value.
 * @param address The specified address.
 */
-(int) writeIntValue:(NSNumber*) value
           atAddress:(void*) address
{
    *(int*) address = [value intValue];;
    return sizeof(int);
}

/**
 * Gets the image data from a data handle.
 * @param handle The given data handle.
 * @return The image data if the handle is valid, or nil otherwise.
 * The ownership of the result is passed to the caller.
 */
-(NSData*) getImageDataFromHandle:(const int) handle
{
    NSData* data = nil;
    int size = maGetDataSize(handle);
    int* dst = new int[size];
    maReadData(handle, dst, 0, size);
    data = [NSData dataWithBytes:dst length:size];
    delete[] dst;

    return data;
}

/**
 * Creates a data handle for a NSData type object.
 * @param data The given data object.
 * @return The data handle.
 */
-(const int) createDataHandle:(NSData*) data
{
    MAHandle placeholder = maCreatePlaceholder();
    int size = [data length];
    const void* src = [data bytes];
    if (RES_OK == maCreateData(placeholder, size))
    {
        maWriteData(placeholder, src, 0, size);
    }

    return placeholder;
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
 * @return One of the next values:
 * - MA_PIM_ERR_NONE if the field is supported and valid
 * - MA_PIM_ERR_INVALID_FIELD if the field is invalid.
 * - MA_PIM_ERR_FIELD_UNSUPPORTED in case the field is not supported.
 */
-(int) fieldStructure:(const int) fieldID
              setType:(int*) type
     setIsSingleValue:(bool*) singleValue
{
    int returnValue = MA_PIM_ERR_NONE;
    *type = MA_PIM_ERR_FIELD_INVALID;
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
            *type = MA_PIM_TYPE_INT;
            break;
        case MA_PIM_FIELD_CONTACT_PHOTO_URL:
            *type = MA_PIM_TYPE_STRING;
            break;
        case MA_PIM_FIELD_CONTACT_PUBLIC_KEY:
            returnValue = MA_PIM_ERR_FIELD_UNSUPPORTED;
            break;
        case MA_PIM_FIELD_CONTACT_PUBLIC_KEY_STRING:
            returnValue = MA_PIM_ERR_FIELD_UNSUPPORTED;
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
        case MA_PIM_FIELD_CONTACT_UID:
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
            returnValue = MA_PIM_ERR_FIELD_INVALID;
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
 * Gets the custom attribute for a specified field ID.
 * @param fieldID the given field ID.
 * @return The custom attribute, or 0 if it does not have one.
 */
-(int) getCustomAttributeForFieldID:(const int) fieldID
{
    int attribute = 0;
    NSString* customLabel = [NSString string];
    NSMutableDictionary* allowedAttributes = [self getAttributesForFieldId:fieldID];
    NSArray* labelArray = [allowedAttributes allKeysForObject:customLabel];
    if (0 < [labelArray count])
    {
        attribute = [[labelArray objectAtIndex:0] intValue];
    }
    [customLabel release];
    [labelArray release];

    return attribute;
}

/**
 * Checks if a given field supports attributes.
 * @param fieldID The given field ID. One of the MA_PIM_FIELD_CONTACT contants.
 * @return True if field supports attributes, false otherwise.
 */
-(bool) fieldSupportsAttribute:(const int) fieldID
{
    bool returnValue;
    switch (fieldID)
    {
        case MA_PIM_FIELD_CONTACT_ADDR:
        case MA_PIM_FIELD_CONTACT_EMAIL:
        case MA_PIM_FIELD_CONTACT_TEL:
        case MA_PIM_FIELD_CONTACT_URL:
        case MA_PIM_FIELD_CONTACT_IM:
        case MA_PIM_FIELD_CONTACT_RELATION:
            returnValue = true;
            break;
        default:
            returnValue = false;
    }

    return returnValue;
}

/**
 * Get the number of indices for a given field.
 * e.g. for MA_PIM_FIELD_CONTACT_NAME it will return 8 because the field contains
 * the next indices:
 * - MA_PIM_CONTACT_NAME_FAMILY
 * - MA_PIM_CONTACT_NAME_GIVEN
 * - MA_PIM_CONTACT_NAME_OTHER
 * - MA_PIM_CONTACT_NAME_PREFIX
 * - MA_PIM_CONTACT_NAME_SUFFIX
 * - MA_PIM_CONTACT_NAME_PHONETIC_FAMILY
 * - MA_PIM_CONTACT_NAME_PHONETIC_GIVEN
 * - MA_PIM_CONTACT_NAME_PHONETIC_OTHER
 */
-(int) getNumberOfIndicesForField:(const int) fieldID
{
    int returnValue;
    switch (fieldID)
    {
        case MA_PIM_FIELD_CONTACT_ADDR:
        case MA_PIM_FIELD_CONTACT_NAME:
            returnValue = 8;
            break;
        case MA_PIM_FIELD_CONTACT_ORG_INFO:
            returnValue = 6;
            break;
        case MA_PIM_FIELD_CONTACT_IM:
            returnValue = 2;
            break;
        default:
            returnValue = 1;
            break;
    }

    return returnValue;
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
    return Base::gSyscall->GetValidatedMemRange(address, size);
}

@end
