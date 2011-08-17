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
 * Checks if the field contains values.
 * If not returns MA_PIM_ERR_NONE.
 */
#define CheckFieldValues(value) if (0 == value) \
                                {\
                                return MA_PIM_ERR_NONE;\
                                }
/**
 * Checks the error code. If the error code if different then 
 * MA_PIM_ERR_NONE then it returns the error code.
 */
#define CheckErrorCode(errorCode) if (MA_PIM_ERR_NONE != errorCode)\
                                {\
                                return errorCode;\
                                }

#import "PimContactItem.h"
#include "PimSyscall.h"

@implementation PimContactItem

/**
 * Init function.
 * @param record Address Book record.
 */
-(id) initWithRecord:(ABRecordRef) record
{
    mRecord = record;
    mItemStatus = kImportedItem;
    [self importDataFromRecord];
    
    return [super init];
}

/**
 * Init function.
 */
-(id) init
{
    mItemStatus = kNewItem;
    mRecord = ABPersonCreate();
    
    return [super init];
}

/**
 * Writes all the fields into record.
 * Deletes all item's fields.
 * @return One of the MA_PIM_ERR constants.
 */
-(int) close
{
    NSArray* fieldsArray = [mFieldsDictionary allValues];
    int countFields = [fieldsArray count];
         
    for (int i = 0; i < countFields; i++) 
    {
        [self writeField:[fieldsArray objectAtIndex:i]];
    }
    
    [mFieldsDictionary removeAllObjects];
//    [mFieldsDictionary release]; bogdan - error!!!!!
    
    return MA_PIM_ERR_NONE;
}

/**
 * Write a given field into record.
 * @param field The specified field.
 * @return One of MA_PIM_ERR constants.
 */
-(int) writeField:(PimFieldItem*) itemField
{
    CheckFieldValues([itemField count]);
    int fieldConstant = [itemField getFieldConstant];
    int returnValue;
    switch (fieldConstant) 
    {
        case MA_PIM_FIELD_CONTACT_ADDR:
            returnValue = [self writeAddresField:itemField];
            break;
        case MA_PIM_FIELD_CONTACT_BIRTHDAY:
            returnValue = [self writeSingleFieldValue:itemField propertyID:kABPersonBirthdayProperty];
            break;
        case MA_PIM_FIELD_CONTACT_EMAIL:
            returnValue = [self writeEmailField:itemField];
            break;
        case MA_PIM_FIELD_CONTACT_NAME:
            returnValue = [self writeContactNameField:itemField];
            break;
        case MA_PIM_FIELD_CONTACT_NICKNAME:
            returnValue = [self writeSingleFieldValue:itemField propertyID:kABPersonNicknameProperty];
            break;
        case MA_PIM_FIELD_CONTACT_NOTE:
            returnValue = [self writeSingleFieldValue:itemField propertyID:kABPersonNoteProperty];
            break;
        case MA_PIM_FIELD_CONTACT_ORG:
            returnValue = [self writeSingleFieldValue:itemField propertyID:kABPersonOrganizationProperty];
            break;
        case MA_PIM_FIELD_CONTACT_PHOTO:
            returnValue = [self writePhotoField:itemField];
            break;
        case MA_PIM_FIELD_CONTACT_PHOTO_URL:
             returnValue = [self writePhotoURLField:itemField];
            break;
        case MA_PIM_FIELD_CONTACT_REVISION:
            returnValue = [self writeSingleFieldValue:itemField propertyID:kABPersonModificationDateProperty];
            break;
        case MA_PIM_FIELD_CONTACT_TEL:
            returnValue = [self writePhoneField:itemField];
            break;
        case MA_PIM_FIELD_CONTACT_TITLE:
            returnValue = [self writeSingleFieldValue:itemField propertyID:kABPersonJobTitleProperty];
            break;
        case MA_PIM_FIELD_CONTACT_URL:
            returnValue = [self writeURLField:itemField];
            break;
        case MA_PIM_FIELD_CONTACT_ORG_INFO:
            returnValue = [self writeOrgInfoField:itemField];
            break;
        case MA_PIM_FIELD_CONTACT_IM:
            returnValue = [self writeIMField:itemField];
            break;
        case MA_PIM_FIELD_CONTACT_RELATION:
            returnValue = [self writeRelatedNameField:itemField];
            break;    
        default:
            break;
    }

    return returnValue;
}

/**
 * Writes the address field into record.
 * @param itemField The specified field.
 * @return One of the MA_PIM_ERR constants.
 */
-(int) writeAddresField:(PimFieldItem*) itemField
{
    int returnValue = MA_PIM_ERR_NONE;
    bool didAdd;
    
    // Add the address to the multivalue.
    ABMultiValueIdentifier identifier;
    ABMutableMultiValueRef multiValue = 
        ABMultiValueCreateMutable(kABDictionaryPropertyType);
    
    // Set up keys and values for the dictionary.
    CFStringRef keys[5] = 
    {
        kABPersonAddressStreetKey,
        kABPersonAddressCityKey,
        kABPersonAddressStateKey,
        kABPersonAddressZIPKey,
        kABPersonAddressCountryKey
    };
    
    int noFields = [itemField count];
    for (int i = 0; i < noFields; i++)
    {
        NSMutableArray* array = [itemField getValue:i];
        NSString* street = [array objectAtIndex:MA_PIM_CONTACT_ADDR_STREET];
        NSString* city = [array objectAtIndex:MA_PIM_CONTACT_ADDR_LOCALITY];
        NSString* state = [array objectAtIndex:MA_PIM_CONTACT_ADDR_REGION];
        NSString* zip = [array objectAtIndex:MA_PIM_CONTACT_ADDR_POSTALCODE];
        NSString* country = [array objectAtIndex:MA_PIM_CONTACT_ADDR_COUNTRY];
        
        CFStringRef addressValues[5];
        addressValues[0] = (CFStringRef)street;
        addressValues[1] = (CFStringRef)city;
        addressValues[2] = (CFStringRef)state;
        addressValues[3] = (CFStringRef)zip;
        addressValues[4] = (CFStringRef)country;
        
        CFStringRef label = (CFStringRef) [[itemField getItem:i] getStringLabel];
        CFDictionaryRef aDict = CFDictionaryCreate(
            kCFAllocatorDefault,
            (void *)keys,
            (void *)addressValues,
            5,
            &kCFCopyStringDictionaryKeyCallBacks,
            &kCFTypeDictionaryValueCallBacks);
        
        didAdd = ABMultiValueAddValueAndLabel(multiValue, aDict, label, &identifier);
        CFRelease(aDict);
        
        if (!didAdd) 
        {
            returnValue = MA_PIM_ERR_OPERATION_NOT_PERMITTED;
            break;
        }
    }
    
    // Check if the values were added.
    if (MA_PIM_ERR_NONE == returnValue)
    {
        returnValue = [self setDataToRecord:multiValue propertyID:kABPersonAddressProperty checkLength:false];
    }
    
    CFRelease(multiValue);
    return returnValue;
}

/**
 * Writes the contact name field into record.
 * @param itemField The specified field.
 * @return One of the MA_PIM_ERR constants.
 */
-(int) writeContactNameField:(PimFieldItem*) itemField
{
    int returnValue = MA_PIM_ERR_NONE;
    
    NSMutableArray* array = [itemField getValue:0];
    NSString* firstName = [array objectAtIndex:MA_PIM_CONTACT_NAME_GIVEN];
    NSString* lastName = [array objectAtIndex:MA_PIM_CONTACT_NAME_FAMILY];
    NSString* middleName = [array objectAtIndex:MA_PIM_CONTACT_NAME_OTHER];
    NSString* prefix = [array objectAtIndex:MA_PIM_CONTACT_NAME_PREFIX];
    NSString* suffix = [array objectAtIndex:MA_PIM_CONTACT_NAME_SUFFIX];
    NSString* phoneticLastName = [array objectAtIndex:MA_PIM_CONTACT_NAME_PHONETIC_FAMILY];
    NSString* phoneticFirstName = [array objectAtIndex:MA_PIM_CONTACT_NAME_PHONETIC_GIVEN];
//    NSString* phoneticMiddleName = [array objectAtIndex:MA_PIM_CONTACT_NAME_PHONETIC_OTHER];
    
    returnValue = [self setDataToRecord:firstName propertyID:kABPersonFirstNameProperty checkLength:true];
    CheckErrorCode(returnValue);
    returnValue = [self setDataToRecord:lastName propertyID:kABPersonLastNameProperty checkLength:true];
    CheckErrorCode(returnValue);
    returnValue = [self setDataToRecord:middleName propertyID:kABPersonMiddleNameProperty checkLength:true];
    CheckErrorCode(returnValue);
    returnValue = [self setDataToRecord:prefix propertyID:kABPersonPrefixProperty checkLength:true];
    CheckErrorCode(returnValue);
    returnValue = [self setDataToRecord:suffix propertyID:kABPersonSuffixProperty checkLength:true];
    CheckErrorCode(returnValue);
    returnValue = [self setDataToRecord:phoneticFirstName propertyID:kABPersonFirstNamePhoneticProperty checkLength:true];
    CheckErrorCode(returnValue);
    returnValue = [self setDataToRecord:phoneticLastName propertyID:kABPersonLastNamePhoneticProperty checkLength:true];
    CheckErrorCode(returnValue);
//    returnValue = [self setDataToRecord:phoneticMiddleName propertyID:kABPersonMiddleNamePhoneticProperty checkLength:true];
//    CheckErrorCode(returnValue);
    
    return MA_PIM_ERR_NONE;
}

/**
 * Writes the email field into record.
 * @param itemField The specified field.
 * @return One of the MA_PIM_ERR constants.
 */
-(int) writeEmailField:(PimFieldItem*) itemField
{
    int returnValue = MA_PIM_ERR_NONE;
    
    // Add the email addresses to the multivalue.
    ABMultiValueIdentifier identifier;
    ABMutableMultiValueRef multiValue =
        ABMultiValueCreateMutable(kABDictionaryPropertyType);
    
    int noFields = [itemField count];
    for (int i = 0; i < noFields; i++) 
    {
        NSMutableArray* array = [itemField getValue:i];
        NSString* email = [array objectAtIndex:0];
        
        CFStringRef label = (CFStringRef) [[itemField getItem:i] getStringLabel];
        bool didAdd = ABMultiValueAddValueAndLabel(
            multiValue, 
            (CFStringRef) email, 
            label, 
            &identifier);
        
        if (!didAdd) 
        {
            returnValue = MA_PIM_ERR_OPERATION_NOT_PERMITTED;
            break;
        }
    }
    
    // Check if the values were added.
    if (MA_PIM_ERR_NONE == returnValue)
    {
        returnValue = [self setDataToRecord:multiValue propertyID:kABPersonEmailProperty checkLength:false];
    }
    
    CFRelease(multiValue);
    return returnValue;
}

/**
 * Writes the photo field into record.
 * @param itemField The specified field.
 * @return One of the MA_PIM_ERR constants.
 */
-(int) writePhotoField:(PimFieldItem*) itemField
{
    NSMutableArray* array = [itemField getValue:0];
    NSData* data = [array objectAtIndex:0];
    bool didSet = ABPersonSetImageData (mRecord, (CFDataRef) data, nil);
    
    if (!didSet) 
    {
        return MA_PIM_ERR_OPERATION_NOT_PERMITTED;
    }
    
    return MA_PIM_ERR_NONE;
}

/**
 * Writes the photo url field into record.
 * @param itemField The specified field.
 * @return One of the MA_PIM_ERR constants.
 */
-(int) writePhotoURLField:(PimFieldItem*) itemField
{
    NSString* photoURL = [[itemField getValue:0] objectAtIndex:0];
    NSURL *url = [NSURL URLWithString:photoURL];
    NSData *data = [NSData dataWithContentsOfURL:url];
    
    bool didSet = ABPersonSetImageData(mRecord, (CFDataRef) data, nil);
    if (!didSet) 
    {
        return MA_PIM_ERR_OPERATION_NOT_PERMITTED;
    }
    return MA_PIM_ERR_NONE;
}

/**
 * Writes the phone field into record.
 * @param itemField The specified field.
 * @return One of the MA_PIM_ERR constants.
 */
-(int) writePhoneField:(PimFieldItem*) itemField
{
    int returnValue = MA_PIM_ERR_NONE;
    
    // Add the phone numbers to the multivalue.
    ABMultiValueIdentifier identifier;
    ABMutableMultiValueRef multiValue = 
        ABMultiValueCreateMutable(kABDictionaryPropertyType);
    
    int noFields = [itemField count];
    for (int i = 0; i < noFields; i++) 
    {
        NSMutableArray* array = [itemField getValue:i];
        NSString* phone = [array objectAtIndex:0];
        CFStringRef label = (CFStringRef) [[itemField getItem:i] getStringLabel];
        bool didAdd = ABMultiValueAddValueAndLabel(
            multiValue, 
            (CFStringRef) phone, 
            label, 
            &identifier);
        
        if (!didAdd) 
        {
            returnValue = MA_PIM_ERR_OPERATION_NOT_PERMITTED;
            break;
        }
        
    }

    // Check if the values were added.
    if (MA_PIM_ERR_NONE == returnValue)
    {
        returnValue = [self setDataToRecord:multiValue propertyID:kABPersonPhoneProperty checkLength:false];
    }
    CFRelease(multiValue);
    return returnValue;
}

/**
 * Writes the URL field into record.
 * @param itemField The specified field.
 * @return One of the MA_PIM_ERR constants.
 */
-(int) writeURLField:(PimFieldItem*) itemField
{
    int returnValue = MA_PIM_ERR_NONE;
    
    // Add the URLs to the multivalue.
    ABMultiValueIdentifier identifier;
    ABMutableMultiValueRef multiValue =
        ABMultiValueCreateMutable(kABDictionaryPropertyType);
    
    int noFields = [itemField count];
    for (int i = 0; i < noFields; i++) 
    {
        NSMutableArray* array = [itemField getValue:i];
        NSString* url = [array objectAtIndex:0];
        CFStringRef label = (CFStringRef) [[itemField getItem:i] getStringLabel];
        bool didAdd = ABMultiValueAddValueAndLabel(
            multiValue, 
            (CFStringRef) url, 
            label, 
            &identifier);
        
        if (!didAdd) 
        {
            returnValue = MA_PIM_ERR_OPERATION_NOT_PERMITTED;
            break;
        }
    }

    // Check if the values were added.
    if (MA_PIM_ERR_NONE == returnValue)
    {
        returnValue = [self setDataToRecord:multiValue propertyID:kABPersonURLProperty checkLength:false];
    }
    CFRelease(multiValue);
    return returnValue;
}

/**
 * Writes the supplemental info organization field into record.
 * @param itemField The specified field.
 * @return One of the MA_PIM_ERR constants.
 */
-(int) writeOrgInfoField:(PimFieldItem*) itemField
{
    NSString* department = [[itemField getValue:0] objectAtIndex:MA_PIM_CONTACT_ORGANIZATION_DEPARTMENT];
    return [self setDataToRecord:department propertyID:kABPersonDepartmentProperty checkLength:true];    
}

/**
 * Writes the anniversary field into record.
 * @param itemField The specified field.
 * @return One of MA_PIM_ERR constants.
 */
-(int) writeAnniversaryField:(PimFieldItem*) itemField
{
    int returnValue = MA_PIM_ERR_NONE;
    
    // Add the address to the multivalue.
    ABMultiValueIdentifier identifier;
    ABMutableMultiValueRef multiValue = 
        ABMultiValueCreateMutable(kABDictionaryPropertyType);
    
    int noFields = [itemField count];
    for (int i = 0; i < noFields; i++)
    {
        NSMutableArray* array = [itemField getValue:i];
        NSDate* anniversary = [array objectAtIndex:0];
        
        CFStringRef label = (CFStringRef) [[itemField getItem:i] getStringLabel];
        bool didAdd = ABMultiValueAddValueAndLabel(
            multiValue, 
            (CFStringRef) anniversary, 
            label, 
            &identifier);
        
        if (!didAdd) 
        {
            returnValue = MA_PIM_ERR_OPERATION_NOT_PERMITTED;
            break;
        }
    }

    // Check if the values were added.
    if (MA_PIM_ERR_NONE == returnValue)
    {
        returnValue = [self setDataToRecord:multiValue propertyID:kABPersonDateProperty checkLength:false];
    }
    CFRelease(multiValue);
    return returnValue;
}

/**
 * Writes the instant message field into record.
 * @param itemField The specified field.
 * @return One of MA_PIM_ERR constants.
 */
-(int) writeIMField:(PimFieldItem*) itemField
{
    int returnValue = MA_PIM_ERR_NONE;

    // Add the im values to the multivalue.
    ABMultiValueIdentifier identifier;
    ABMutableMultiValueRef multiValue = 
        ABMultiValueCreateMutable(kABDictionaryPropertyType);
    
    // Set up keys and values for the dictionary.
    CFStringRef keys[2];
    keys[0] = kABPersonInstantMessageServiceKey;
    keys[1] = kABPersonInstantMessageUsernameKey;
    
    int noFields = [itemField count];
    for (int i = 0; i < noFields; i++)
    {
        NSMutableArray* array = [itemField getValue:i];
        const CFStringRef protocol = [self getIMProtocol:[array objectAtIndex:0]];
        if (nil == protocol)
        {
            continue;
        }
        
        NSString* username = [array objectAtIndex:1];
        
        CFStringRef imValues[2];
        imValues[0] = protocol;
        imValues[1] = (CFStringRef)username;
        
        CFStringRef label = (CFStringRef) [[itemField getItem:i] getStringLabel];
        CFDictionaryRef aDict = CFDictionaryCreate(
                                                   kCFAllocatorDefault,
                                                   (void *)keys,
                                                   (void *)imValues,
                                                   2,
                                                   &kCFCopyStringDictionaryKeyCallBacks,
                                                   &kCFTypeDictionaryValueCallBacks);
        
        bool didAdd = ABMultiValueAddValueAndLabel(
                                                   multiValue, 
                                                   aDict, 
                                                   label, 
                                                   &identifier);
        CFRelease(aDict);
        
        if (!didAdd) 
        {
            returnValue = MA_PIM_ERR_OPERATION_NOT_PERMITTED;
            break;
        }
    }

    // Check if the values were added.
    if (MA_PIM_ERR_NONE == returnValue)
    {
        returnValue = [self setDataToRecord:multiValue propertyID:kABPersonInstantMessageProperty checkLength:false];    
    }
    CFRelease(multiValue);
    return returnValue;
}

/**
 * Writes the related name field into record.
 * @param itemField The specified field.
 * @return One of MA_PIM_ERR constants.
 */
-(int) writeRelatedNameField:(PimFieldItem*) itemField;
{
    int returnValue = MA_PIM_ERR_NONE;
    ABMutableMultiValueRef multi = 
        ABMultiValueCreateMutable(kABMultiStringPropertyType);
    
    int noFields = [itemField count];
    for (int i = 0; i < noFields; i++)
    {
        NSMutableArray* array = [itemField getValue:i];
        const CFStringRef value = (CFStringRef)[array objectAtIndex:0];
        const CFStringRef label = (CFStringRef) [[itemField getItem:i] getStringLabel];
        bool didAdd = ABMultiValueAddValueAndLabel(multi, value, label, NULL);
        if (!didAdd) 
        {
            returnValue = MA_PIM_ERR_OPERATION_NOT_PERMITTED;
            break;
        }        
    }
    
    if ( MA_PIM_ERR_NONE == returnValue) 
    {
        returnValue = [self setDataToRecord:multi propertyID:kABPersonRelatedNamesProperty checkLength:false];
    }
    CFRelease(multi);
    return returnValue;
}

/**
 * Writes a single field value into record.
 * @param itemField The given field.
 * @param property The property of whose value is being set.
 * @return One of MA_PIM_ERR constants.
 */
-(int) writeSingleFieldValue:(PimFieldItem*) itemField
                  propertyID:(ABPropertyID) property
{
    NSArray* valueArray = [itemField getValue:0];
    bool checkValueLength = false;
    if (MA_PIM_TYPE_STRING == [itemField getFieldType])
    {
        checkValueLength = true;
    }
    CFStringRef value = (CFStringRef)[valueArray objectAtIndex:0];
    return [self setDataToRecord:value propertyID:property checkLength:checkValueLength];
}

/**
 * Sets data to record.
 * @param value The specified value.
 * @param property The property of whose value is being set.
 * @param verify If true the length of the value is checked.
 *               If the length is zero the value is not written.
 * @return One of MA_PIM_ERR constants.
 */
-(int) setDataToRecord:(CFTypeRef) value
            propertyID:(ABPropertyID) property
           checkLength:(bool) verify
{
    CFErrorRef error;
    bool didSet;
    int returnValue = MA_PIM_ERR_NONE;
    
    // Check for a empty value.
    if (verify && 0 == CFStringGetLength(value))
    {
        return MA_PIM_ERR_NONE;
    }
    
    didSet = ABRecordSetValue(mRecord, property, value, &error);  
    if (!didSet)
    {
        returnValue = MA_PIM_ERR_OPERATION_NOT_PERMITTED;
        NSLog(@"PimContactItem--setDataToRecord--field NOT set for value:%@ reason:%@",
              (NSString*) value, [(NSError *)error localizedDescription]);
    }
    
    return returnValue;
}

/**
 * Imports data from record and writes it into pim fields.
 */
-(void) importDataFromRecord
{
    NSLog(@"PimContactItem--importDataFromRecord--in");
    NSMutableArray* array;
    NSString* key; 
    PimFieldItem* itemField;
    PimUtils* utils = [PimUtils sharedInstance];
    ABMutableMultiValueRef multi;
    
    NSString* firstName = (NSString*) ABRecordCopyValue(mRecord, kABPersonFirstNameProperty);
    NSString* lastName  = (NSString*) ABRecordCopyValue(mRecord, kABPersonLastNameProperty);
    NSString* middleName = (NSString*) ABRecordCopyValue(mRecord, kABPersonMiddleNameProperty);
    NSString* prefix  = (NSString*) ABRecordCopyValue(mRecord, kABPersonPrefixProperty);
    NSString* suffix = (NSString*) ABRecordCopyValue(mRecord, kABPersonSuffixProperty);
    NSString* firstNamePhonetic = (NSString*) ABRecordCopyValue(mRecord, kABPersonFirstNamePhoneticProperty);
    NSString* lastNamePhonetic  = (NSString*) ABRecordCopyValue(mRecord, kABPersonLastNamePhoneticProperty);
    NSString* middleNamePhonetic = (NSString*) ABRecordCopyValue(mRecord, kABPersonMiddleNamePhoneticProperty);
    
    // Add address field.
    multi = ABRecordCopyValue(mRecord, kABPersonAddressProperty);
    CFStringRef addressLabel;
    itemField = [[PimFieldItem alloc] initWithFieldConstant:MA_PIM_FIELD_CONTACT_ADDR];
    key = [[NSString alloc] initWithFormat:@"%d", MA_PIM_FIELD_CONTACT_ADDR];
    
    for (CFIndex i = 0; i < ABMultiValueGetCount(multi); i++) {
        array = [[NSMutableArray alloc] init];
        addressLabel = ABMultiValueCopyLabelAtIndex(multi, i);
        
        CFDictionaryRef aDict = ABMultiValueCopyValueAtIndex(multi, i);
        
        NSString* street = (NSString*) CFDictionaryGetValue(aDict, kABPersonAddressStreetKey);
        NSString* city = (NSString*)CFDictionaryGetValue(aDict, kABPersonAddressCityKey);
        NSString* state = (NSString*)CFDictionaryGetValue(aDict, kABPersonAddressStateKey);
        NSString* zip = (NSString*)CFDictionaryGetValue(aDict, kABPersonAddressZIPKey);
        NSString* country = (NSString*)CFDictionaryGetValue(aDict, kABPersonAddressCountryKey);
        
        NSLog(@"%@ %@ %@ %@ %@ %@",(NSString*)addressLabel, street, city, state, zip, country);
        [utils addStringToArray:array string:@""];    // Add POBOX field.
        [utils addStringToArray:array string:@""];    // Add EXTRA field.
        [utils addStringToArray:array string:street]; // Add STREET field.
        [utils addStringToArray:array string:city];   // Add LOCALITY field.
        [utils addStringToArray:array string:state];  // Add REGION field.
        [utils addStringToArray:array string:zip];    // Add POSTALCODE field.
        [utils addStringToArray:array string:country];// Add COUNTRY field.
        [utils addStringToArray:array string:@""];    // Add COUNTRY_CODE field.
        [utils addStringToArray:array string:@""];    // Add NEIGHBORHOOD field.
        
        [itemField addValue:array withStringAttribute:(NSString*)addressLabel];
        CFRelease(aDict);
    }
    
    [mFieldsDictionary setObject:itemField forKey:key];
    CFRelease(multi);
    
    // Add birthday field.
    NSDate* birthday = (NSDate*) ABRecordCopyValue(mRecord, kABPersonBirthdayProperty);
    itemField = [[PimFieldItem alloc] initWithFieldConstant:MA_PIM_FIELD_CONTACT_BIRTHDAY];
    key = [[NSString alloc] initWithFormat:@"%d", MA_PIM_FIELD_CONTACT_BIRTHDAY];
    array = [[NSMutableArray alloc] init];
    [array addObject:birthday];
    [itemField addValue:array withAttribute:0];
    [mFieldsDictionary setObject:itemField forKey:key];
    
    // Add email field.
    multi = ABRecordCopyValue(mRecord, kABPersonEmailProperty);
    itemField = [[PimFieldItem alloc] initWithFieldConstant:MA_PIM_FIELD_CONTACT_EMAIL];
    key = [[NSString alloc] initWithFormat:@"%d", MA_PIM_FIELD_CONTACT_EMAIL];
    
    for (CFIndex i = 0; i < ABMultiValueGetCount(multi); i++) {
        array = [[NSMutableArray alloc] init];
        NSString* emailLabel = (NSString*)ABMultiValueCopyLabelAtIndex(multi, i);
        NSString* email = (NSString*)ABMultiValueCopyValueAtIndex(multi, i);
        NSLog(@"%@ %@ ",emailLabel, email);
       
        [utils addStringToArray:array string:email];
        [itemField addValue:array withStringAttribute:emailLabel];
    }
    
    [mFieldsDictionary setObject:itemField forKey:key];
    CFRelease(multi);
    
    // Add contact name field.
    itemField = [[PimFieldItem alloc] initWithFieldConstant:MA_PIM_FIELD_CONTACT_NAME];
    key = [[NSString alloc] initWithFormat:@"%d", MA_PIM_FIELD_CONTACT_NAME];
    array = [[NSMutableArray alloc] init];
    [utils addStringToArray:array string:lastName];
    [utils addStringToArray:array string:firstName];
    [utils addStringToArray:array string:middleName];
    [utils addStringToArray:array string:prefix];
    [utils addStringToArray:array string:suffix];
    [utils addStringToArray:array string:lastNamePhonetic];
    [utils addStringToArray:array string:firstNamePhonetic];
    [utils addStringToArray:array string:middleNamePhonetic];
    [itemField addValue:array withAttribute:0];
    [mFieldsDictionary setObject:itemField forKey:key];
    NSLog(@"%@ %@ %@ %@ %@ %@ %@ %@",firstName, lastName, middleName, prefix, suffix,lastNamePhonetic, firstNamePhonetic,middleNamePhonetic);
    
    // Add nickname field.
    NSString* nickname  = (NSString*) ABRecordCopyValue(mRecord, kABPersonNicknameProperty);
    itemField = [[PimFieldItem alloc] initWithFieldConstant:MA_PIM_FIELD_CONTACT_NICKNAME];
    key = [[NSString alloc] initWithFormat:@"%d", MA_PIM_FIELD_CONTACT_NICKNAME];
    array = [[NSMutableArray alloc] init];
    [array addObject:nickname];
    [itemField addValue:array withAttribute:0];
    [mFieldsDictionary setObject:itemField forKey:key];
    NSLog(@"nickname: %@", nickname);
    
    // Add note field.
    NSString* note = (NSString*) ABRecordCopyValue(mRecord, kABPersonNoteProperty);
    itemField = [[PimFieldItem alloc] initWithFieldConstant:MA_PIM_FIELD_CONTACT_NOTE];
    key = [[NSString alloc] initWithFormat:@"%d", MA_PIM_FIELD_CONTACT_NOTE];
    array = [[NSMutableArray alloc] init];
    [array addObject:note];
    [itemField addValue:array withAttribute:0];
    [mFieldsDictionary setObject:itemField forKey:key];
    NSLog(@"note: %@", note);
    
    // Add organization field.
    NSString* organization  = (NSString*) ABRecordCopyValue(mRecord, kABPersonOrganizationProperty);
    itemField = [[PimFieldItem alloc] initWithFieldConstant:MA_PIM_FIELD_CONTACT_ORG];
    key = [[NSString alloc] initWithFormat:@"%d", MA_PIM_FIELD_CONTACT_ORG];
    array = [[NSMutableArray alloc] init];
    [array addObject:organization];
    [itemField addValue:array withAttribute:0];
    [mFieldsDictionary setObject:itemField forKey:key];
    NSLog(@"organization: %@", organization);
    
    
    // Add phone field.
    multi = ABRecordCopyValue(mRecord, kABPersonPhoneProperty);
    itemField = [[PimFieldItem alloc] initWithFieldConstant:MA_PIM_FIELD_CONTACT_TEL];
    key = [[NSString alloc] initWithFormat:@"%d", MA_PIM_FIELD_CONTACT_TEL];
    
    for (CFIndex i = 0; i < ABMultiValueGetCount(multi); i++) {
        array = [[NSMutableArray alloc] init];
        NSString* phoneLabel = (NSString*)ABMultiValueCopyLabelAtIndex(multi, i);
        NSString* phone = (NSString*)ABMultiValueCopyValueAtIndex(multi, i);
        NSLog(@"%@ %@ ",phoneLabel, phone);
        
        [utils addStringToArray:array string:phone];
        [itemField addValue:array withStringAttribute:phoneLabel];
    }
    
    [mFieldsDictionary setObject:itemField forKey:key];
    CFRelease(multi);
    
    // Add job title field.
    NSString* jobTitle = (NSString*) ABRecordCopyValue(mRecord, kABPersonJobTitleProperty);
    itemField = [[PimFieldItem alloc] initWithFieldConstant:MA_PIM_FIELD_CONTACT_TITLE];
    key = [[NSString alloc] initWithFormat:@"%d", MA_PIM_FIELD_CONTACT_TITLE];
    array = [[NSMutableArray alloc] init];
    [array addObject:jobTitle];
    [itemField addValue:array withAttribute:0];
    [mFieldsDictionary setObject:itemField forKey:key];
    NSLog(@"jobTitle: %@", jobTitle);
    
    // Add url field.
    multi = ABRecordCopyValue(mRecord, kABPersonURLProperty);
    itemField = [[PimFieldItem alloc] initWithFieldConstant:MA_PIM_FIELD_CONTACT_URL];
    key = [[NSString alloc] initWithFormat:@"%d", MA_PIM_FIELD_CONTACT_URL];
    
    for (CFIndex i = 0; i < ABMultiValueGetCount(multi); i++) {
        array = [[NSMutableArray alloc] init];
        NSString* urlLabel = (NSString*)ABMultiValueCopyLabelAtIndex(multi, i);
        NSString* url = (NSString*)ABMultiValueCopyValueAtIndex(multi, i);
        NSLog(@"%@ %@ ",urlLabel, url);
        
        [utils addStringToArray:array string:url];
        [itemField addValue:array withStringAttribute:urlLabel];
    }
    
    [mFieldsDictionary setObject:itemField forKey:key];
    CFRelease(multi);
    
    // Add organization info field.
    NSString* department  = (NSString*) ABRecordCopyValue(mRecord, kABPersonDepartmentProperty);
    itemField = [[PimFieldItem alloc] initWithFieldConstant:MA_PIM_FIELD_CONTACT_ORG_INFO];
    key = [[NSString alloc] initWithFormat:@"%d", MA_PIM_FIELD_CONTACT_ORG_INFO];
    array = [[NSMutableArray alloc] init];
    [array addObject:department];
    [array addObject:@""];
    [array addObject:@""];
    [array addObject:@""];
    [array addObject:@""];

    [itemField addValue:array withAttribute:0];
    [mFieldsDictionary setObject:itemField forKey:key];
    NSLog(@"department: %@", department);
    
    for (CFIndex i = 0; i < ABMultiValueGetCount(multi); i++) {
        array = [[NSMutableArray alloc] init];
        NSString* anniversaryLabel = (NSString*)ABMultiValueCopyLabelAtIndex(multi, i);
        NSString* anniversary = (NSString*)ABMultiValueCopyValueAtIndex(multi, i);
        NSLog(@"%@ %@ ",anniversaryLabel, anniversary);
        
        [utils addStringToArray:array string:anniversary];
        [itemField addValue:array withStringAttribute:anniversaryLabel];
    }
}

/**
 * Get the item's record.
 * @return The item's record.
 */
-(ABRecordRef) getRecord
{
    return mRecord;
}

/**
 * Gets the instant message protocol associeted with a string constant.
 * @param value Must be one of the MA_PIM_CONTACT_IM constants.
 * @return One of the IM protocol(see ABPerson reference - iOS Developer Library),
 *         or nil if the value param is not supported on iphone platform.
 */
-(const CFStringRef) getIMProtocol:(NSString*) value
{
    CFStringRef returnValue = nil;
    if ([value isEqualToString: @MA_PIM_CONTACT_IM_PROTOCOL_AIM])
    {
        returnValue = kABPersonInstantMessageServiceAIM;
    } 
    else if ([value isEqualToString: @MA_PIM_CONTACT_IM_PROTOCOL_MSN])
    {
        returnValue = kABPersonInstantMessageServiceMSN;
    }
    else if ([value isEqualToString: @MA_PIM_CONTACT_IM_PROTOCOL_YAHOO])
    {
        returnValue = kABPersonInstantMessageServiceYahoo;
    }
    else if ([value isEqualToString: @MA_PIM_CONTACT_IM_PROTOCOL_ICQ])
    {
        returnValue = kABPersonInstantMessageServiceICQ;
    }
    else if ([value isEqualToString: @MA_PIM_CONTACT_IM_PROTOCOL_JABBER])
    {
        returnValue = kABPersonInstantMessageServiceJabber;
    }
    
    return returnValue;
}

/**
 * Release all the objects.
 */
- (void) dealloc 
{
    CFRelease(mRecord);
    [super dealloc];
}

@end
