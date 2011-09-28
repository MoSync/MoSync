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
#import "PimError.h"
#import "MoSyncPanic.h"

@implementation PimContactItem

/**
 * Init function.
 * @param record Address Book record.
 */
-(id) initWithRecord:(ABRecordRef) record
{
    [super init];
    mRecord = record;
    mItemStatus = kImportedItem;
    [self importDataFromRecord];
    return self;
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
    bool writeDataIntoRecord;
    if (kNewItem == mItemStatus)
    {
        writeDataIntoRecord = true;
    }
    else if (kImportedItem == mItemStatus)
    {
        writeDataIntoRecord = false;
    }
    else if (kModifiedItem == mItemStatus)
    {
        writeDataIntoRecord = true;
    }

    if (writeDataIntoRecord)
    {
        NSArray* fieldsArray = [mFieldsDictionary allValues];
        int countFields = [fieldsArray count];

        for (int i = 0; i < countFields; i++)
        {
            [self writeField:[fieldsArray objectAtIndex:i]];
        }

        [mFieldsDictionary removeAllObjects];
    }

    return MA_PIM_ERR_NONE;
}

/**
 * Gets the number of values for a given field ID.
 * Checks if field is valid and supported.
 * @param field The specified field ID(one of the MA_PIM_FIELD constants).
 * @return The number of values for the given field, or 0 if the field ID
 *         has no values/does not exist.
 */
-(int) fieldCount:(const int) fieldID
{
    if (![self isFieldSupported:fieldID])
    {
        return MA_PIM_ERR_FIELD_UNSUPPORTED;
    }
    if (![self isFieldValid:fieldID])
    {
        return [[MoSyncPanic getInstance] error:MA_PIM_ERR_FIELD_INVALID
                                  withPanicCode:PANIC_FIELD_INVALID
                                  withPanicText:@PANIC_FIELD_INVALID_TEXT];
    }

    return [super fieldCount:fieldID];
}

/**
 * Gets the attribute for a given field value.
 * Checks if field is valid and supported.
 * @param fieldID The specified field ID(one of the MA_PIM_FIELD constants).
 * @param index The index of the given field value.
 * @return One of MA_PIM_ATTR constants, or MA_PIM_ERR in case of error.
 */
-(int) getAttribute:(const int) fieldID
         indexValue:(const int) index
{
    if (![self isFieldSupported:fieldID])
    {
        return MA_PIM_ERR_FIELD_UNSUPPORTED;
    }
    if (![self isFieldValid:fieldID])
    {
        return [[MoSyncPanic getInstance] error:MA_PIM_ERR_FIELD_INVALID
                                  withPanicCode:PANIC_FIELD_INVALID
                                  withPanicText:@PANIC_FIELD_INVALID_TEXT];
    }

    return [super getAttribute:fieldID indexValue:index];
}

/**
 * Sets a custom label for a given field value.
 * Checks if field is valid and supported.
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
    if (![self isFieldSupported:args->field])
    {
        return MA_PIM_ERR_FIELD_UNSUPPORTED;
    }
    if (![self isFieldValid:args->field])
    {
        return [[MoSyncPanic getInstance] error:MA_PIM_ERR_FIELD_INVALID
                                  withPanicCode:PANIC_FIELD_INVALID
                                  withPanicText:@PANIC_FIELD_INVALID_TEXT];
    }

    return [super setLabel:args indexValue:index];
}

/**
 * Gets a custom label for a given field value.
 * Checks if field is valid and supported.
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
    if (![self isFieldSupported:args->field])
    {
        return MA_PIM_ERR_FIELD_UNSUPPORTED;
    }
    if (![self isFieldValid:args->field])
    {
        return [[MoSyncPanic getInstance] error:MA_PIM_ERR_FIELD_INVALID
                                  withPanicCode:PANIC_FIELD_INVALID
                                  withPanicText:@PANIC_FIELD_INVALID_TEXT];
    }

    return [super getLabel:args indexValue:index];
}

/**
 * Gets a field's value at a given index.
 * Checks if field is supported or write-only.
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
    int fieldID = args->field;
    if (![self isFieldSupported:fieldID])
    {
        return MA_PIM_ERR_FIELD_UNSUPPORTED;
    }
    if (![self isFieldValid:args->field])
    {
        return [[MoSyncPanic getInstance] error:MA_PIM_ERR_FIELD_INVALID
                                  withPanicCode:PANIC_FIELD_INVALID
                                  withPanicText:@PANIC_FIELD_INVALID_TEXT];
    }
    if ([self isFieldWriteOnly:fieldID])
    {
        return MA_PIM_ERR_FIELD_WRITE_ONLY;
    }

    NSString* key = [[NSString alloc] initWithFormat:@"%d", fieldID];
    PimFieldItem* fieldItem = [mFieldsDictionary objectForKey:key];
    [key release];
    if (nil == fieldItem)
    {
        return [[MoSyncPanic getInstance] error:MA_PIM_ERR_FIELD_EMPTY
                                  withPanicCode:PANIC_FIELD_EMPTY
                                  withPanicText:@PANIC_FIELD_EMPTY_TEXT];
    }

    return [super getValue:args indexValue:index];
}

/**
 * Adds a value to a specified field.
 * Checks if the field is supported or read-only.
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
    int fieldID = args->field;
    if (![self isFieldSupported:fieldID])
    {
        return MA_PIM_ERR_FIELD_UNSUPPORTED;
    }
    if (![self isFieldValid:args->field])
    {
        return [[MoSyncPanic getInstance] error:MA_PIM_ERR_FIELD_INVALID
                                  withPanicCode:PANIC_FIELD_INVALID
                                  withPanicText:@PANIC_FIELD_INVALID_TEXT];
    }
    if ([self isFieldReadOnly:fieldID])
    {
        return MA_PIM_ERR_FIELD_READ_ONLY;
    }

    // Remove primary mask from attribute.
    int newAttribute = (attribute & 0xFFFF);
    return [super addValue:args withAttribute:newAttribute];
}

/**
 * Sets a field's value and attribute at a given index.
 * Checks if the field is supported or read-only.
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
    int fieldID = args->field;
    if (![self isFieldSupported:fieldID])
    {
        return MA_PIM_ERR_FIELD_UNSUPPORTED;
    }
    if (![self isFieldValid:args->field])
    {
        return [[MoSyncPanic getInstance] error:MA_PIM_ERR_FIELD_INVALID
                                  withPanicCode:PANIC_FIELD_INVALID
                                  withPanicText:@PANIC_FIELD_INVALID_TEXT];
    }
    if ([self isFieldReadOnly:fieldID])
    {
        return MA_PIM_ERR_FIELD_READ_ONLY;
    }

    // Remove primary mask from attribute.
    int newAttribute = (attribute & 0xFFFF);
    return [super setValue:args indexValue:index valueAttribute:newAttribute];
}

/**
 * Removes a value from a field.
 * Checks if the field is valid or supported.
 * @param field One of the MA_PIM_FIELD constants.
 * @param index Field's value index.
 * @return One of MA_PIM_ERR constants.
 */
-(int) removeValue:(const int) field
           atIndex:(const int) index
{
    if (![self isFieldSupported:field])
    {
        return MA_PIM_ERR_FIELD_UNSUPPORTED;
    }
    if (![self isFieldValid:field])
    {
        return [[MoSyncPanic getInstance] error:MA_PIM_ERR_FIELD_INVALID
                                  withPanicCode:PANIC_FIELD_INVALID
                                  withPanicText:@PANIC_FIELD_INVALID_TEXT];
    }

    return [super removeValue:field atIndex:index];
}

/**
 * Write a given field into record.
 * @param field The specified field.
 */
-(void) writeField:(PimFieldItem*) itemField
{
    int fieldConstant = [itemField getFieldConstant];
    switch (fieldConstant)
    {
        case MA_PIM_FIELD_CONTACT_ADDR:
            [self writeAddresField:itemField];
            break;
        case MA_PIM_FIELD_CONTACT_BIRTHDAY:
            [self writeSingleFieldValue:itemField propertyID:kABPersonBirthdayProperty];
            break;
        case MA_PIM_FIELD_CONTACT_EMAIL:
            [self writeEmailField:itemField];
            break;
        case MA_PIM_FIELD_CONTACT_NAME:
            [self writeContactNameField:itemField];
            break;
        case MA_PIM_FIELD_CONTACT_NICKNAME:
            [self writeSingleFieldValue:itemField propertyID:kABPersonNicknameProperty];
            break;
        case MA_PIM_FIELD_CONTACT_NOTE:
            [self writeSingleFieldValue:itemField propertyID:kABPersonNoteProperty];
            break;
        case MA_PIM_FIELD_CONTACT_ORG:
            [self writeSingleFieldValue:itemField propertyID:kABPersonOrganizationProperty];
            break;
        case MA_PIM_FIELD_CONTACT_PHOTO:
            [self writePhotoField:itemField];
            break;
        case MA_PIM_FIELD_CONTACT_PHOTO_URL:
            [self writePhotoURLField:itemField];
            break;
        case MA_PIM_FIELD_CONTACT_REVISION:
            [self writeSingleFieldValue:itemField propertyID:kABPersonModificationDateProperty];
            break;
        case MA_PIM_FIELD_CONTACT_TEL:
            [self writePhoneField:itemField];
            break;
        case MA_PIM_FIELD_CONTACT_TITLE:
            [self writeSingleFieldValue:itemField propertyID:kABPersonJobTitleProperty];
            break;
        case MA_PIM_FIELD_CONTACT_URL:
            [self writeURLField:itemField];
            break;
        case MA_PIM_FIELD_CONTACT_ORG_INFO:
            [self writeOrgInfoField:itemField];
            break;
        case MA_PIM_FIELD_CONTACT_IM:
            [self writeIMField:itemField];
            break;
        case MA_PIM_FIELD_CONTACT_RELATION:
            [self writeRelationField:itemField];
            break;
    }
}

/**
 * Writes the address field into record.
 * @param itemField The specified field.
 */
-(void) writeAddresField:(PimFieldItem*) itemField
{
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

        CFStringRef label = (CFStringRef) [[itemField getItem:i] getLabel];
        CFDictionaryRef aDict = CFDictionaryCreate(
            kCFAllocatorDefault,
            (void *)keys,
            (void *)addressValues,
            5,
            &kCFCopyStringDictionaryKeyCallBacks,
            &kCFTypeDictionaryValueCallBacks);

        ABMultiValueAddValueAndLabel(multiValue, aDict, label, &identifier);
        CFRelease(aDict);
    }

    [self setDataToRecord:multiValue propertyID:kABPersonAddressProperty checkLength:false];

    CFRelease(multiValue);
}

/**
 * Writes the contact name field into record.
 * @param itemField The specified field.
 */
-(void) writeContactNameField:(PimFieldItem*) itemField
{
    NSMutableArray* array = [itemField getValue:0];
    NSString* firstName = [array objectAtIndex:MA_PIM_CONTACT_NAME_GIVEN];
    NSString* lastName = [array objectAtIndex:MA_PIM_CONTACT_NAME_FAMILY];
    NSString* middleName = [array objectAtIndex:MA_PIM_CONTACT_NAME_OTHER];
    NSString* prefix = [array objectAtIndex:MA_PIM_CONTACT_NAME_PREFIX];
    NSString* suffix = [array objectAtIndex:MA_PIM_CONTACT_NAME_SUFFIX];
    NSString* phoneticLastName = [array objectAtIndex:MA_PIM_CONTACT_NAME_PHONETIC_FAMILY];
    NSString* phoneticFirstName = [array objectAtIndex:MA_PIM_CONTACT_NAME_PHONETIC_GIVEN];
    NSString* phoneticMiddleName = [array objectAtIndex:MA_PIM_CONTACT_NAME_PHONETIC_OTHER];

    [self setDataToRecord:firstName propertyID:kABPersonFirstNameProperty checkLength:true];
    [self setDataToRecord:lastName propertyID:kABPersonLastNameProperty checkLength:true];
    [self setDataToRecord:middleName propertyID:kABPersonMiddleNameProperty checkLength:true];
    [self setDataToRecord:prefix propertyID:kABPersonPrefixProperty checkLength:true];
    [self setDataToRecord:suffix propertyID:kABPersonSuffixProperty checkLength:true];
    [self setDataToRecord:phoneticFirstName propertyID:kABPersonFirstNamePhoneticProperty checkLength:true];
    [self setDataToRecord:phoneticLastName propertyID:kABPersonLastNamePhoneticProperty checkLength:true];
    [self setDataToRecord:phoneticMiddleName propertyID:kABPersonMiddleNamePhoneticProperty checkLength:true];
}

/**
 * Writes the email field into record.
 * @param itemField The specified field.
 */
-(void) writeEmailField:(PimFieldItem*) itemField
{
    // Add the email addresses to the multivalue.
    ABMultiValueIdentifier identifier;
    ABMutableMultiValueRef multiValue =
        ABMultiValueCreateMutable(kABDictionaryPropertyType);

    int noFields = [itemField count];
    for (int i = 0; i < noFields; i++)
    {
        NSMutableArray* array = [itemField getValue:i];
        NSString* email = [array objectAtIndex:0];
        CFStringRef label = (CFStringRef) [[itemField getItem:i] getLabel];
        ABMultiValueAddValueAndLabel(multiValue, (CFStringRef) email, label, &identifier);
    }

    [self setDataToRecord:multiValue propertyID:kABPersonEmailProperty checkLength:false];
    CFRelease(multiValue);
}

/**
 * Writes the photo field into record.
 * @param itemField The specified field.
 */
-(void) writePhotoField:(PimFieldItem*) itemField
{
    NSMutableArray* array = [itemField getValue:0];
    NSNumber* handle = [array objectAtIndex:0];
    NSData* data = [[PimUtils sharedInstance] getImageDataFromHandle:[handle intValue]];
    ABPersonSetImageData (mRecord, (CFDataRef) data, nil);
    [data release];
}

/**
 * Writes the photo url field into record.
 * @param itemField The specified field.
 */
-(void) writePhotoURLField:(PimFieldItem*) itemField
{
    NSString* photoURL = [[itemField getValue:0] objectAtIndex:0];
    NSURL *url = [NSURL URLWithString:photoURL];
    NSData *data = [NSData dataWithContentsOfURL:url];

    ABPersonSetImageData(mRecord, (CFDataRef) data, nil);
    [url release];
    [data release];
}

/**
 * Writes the phone field into record.
 * @param itemField The specified field.
 */
-(void) writePhoneField:(PimFieldItem*) itemField
{
    // Add the phone numbers to the multivalue.
    ABMultiValueIdentifier identifier;
    ABMutableMultiValueRef multiValue =
        ABMultiValueCreateMutable(kABDictionaryPropertyType);

    int noFields = [itemField count];
    for (int i = 0; i < noFields; i++)
    {
        NSMutableArray* array = [itemField getValue:i];
        NSString* phone = [array objectAtIndex:0];
        CFStringRef label = (CFStringRef) [[itemField getItem:i] getLabel];
        ABMultiValueAddValueAndLabel(multiValue, (CFStringRef) phone, label, &identifier);
    }

    [self setDataToRecord:multiValue propertyID:kABPersonPhoneProperty checkLength:false];
    CFRelease(multiValue);
}

/**
 * Writes the URL field into record.
 * @param itemField The specified field.
 */
-(void) writeURLField:(PimFieldItem*) itemField
{
    // Add the URLs to the multivalue.
    ABMultiValueIdentifier identifier;
    ABMutableMultiValueRef multiValue =
        ABMultiValueCreateMutable(kABDictionaryPropertyType);

    int noFields = [itemField count];
    for (int i = 0; i < noFields; i++)
    {
        NSMutableArray* array = [itemField getValue:i];
        NSString* url = [array objectAtIndex:0];
        CFStringRef label = (CFStringRef) [[itemField getItem:i] getLabel];
        ABMultiValueAddValueAndLabel(multiValue, (CFStringRef) url, label, &identifier);
    }

    [self setDataToRecord:multiValue propertyID:kABPersonURLProperty checkLength:false];
    CFRelease(multiValue);
}

/**
 * Writes the supplemental info organization field into record.
 * @param itemField The specified field.
 */
-(void) writeOrgInfoField:(PimFieldItem*) itemField
{
    NSString* department = [[itemField getValue:0] objectAtIndex:MA_PIM_CONTACT_ORG_INFO_DEPARTMENT];
    [self setDataToRecord:department propertyID:kABPersonDepartmentProperty checkLength:true];
}

/**
 * Writes the anniversary field into record.
 * @param itemField The specified field.
 */
-(void) writeAnniversaryField:(PimFieldItem*) itemField
{
    // Add the anniversary to the multivalue.
    ABMultiValueIdentifier identifier;
    ABMutableMultiValueRef multiValue =
        ABMultiValueCreateMutable(kABDictionaryPropertyType);

    int noFields = [itemField count];
    for (int i = 0; i < noFields; i++)
    {
        NSMutableArray* array = [itemField getValue:i];
        NSDate* anniversary = [array objectAtIndex:0];
        CFStringRef label = (CFStringRef) [[itemField getItem:i] getLabel];
        ABMultiValueAddValueAndLabel(multiValue, (CFStringRef) anniversary, label, &identifier);
    }

    [self setDataToRecord:multiValue propertyID:kABPersonDateProperty checkLength:false];
    CFRelease(multiValue);
}

/**
 * Writes the instant message field into record.
 * @param itemField The specified field.
 */
-(void) writeIMField:(PimFieldItem*) itemField
{
    // Add the im values to the multivalue.
    ABMultiValueIdentifier identifier;
    ABMutableMultiValueRef multiValue =
        ABMultiValueCreateMutable(kABDictionaryPropertyType);

    // Set up keys and values for the dictionary.
    CFStringRef keys[2];
    keys[0] =  kABPersonInstantMessageUsernameKey;
    keys[1] = kABPersonInstantMessageServiceKey;

    int noFields = [itemField count];
    for (int i = 0; i < noFields; i++)
    {
        NSMutableArray* array = [itemField getValue:i];
        const CFStringRef protocol = [self getIMProtocol:[array objectAtIndex:MA_PIM_CONTACT_IM_PROTOCOL]];
        if (nil == protocol)
        {
            continue;
        }

        NSString* username = [array objectAtIndex:MA_PIM_CONTACT_IM_USERNAME];

        CFStringRef imValues[2];
        imValues[0] = (CFStringRef)username;
        imValues[1] = protocol;

        CFStringRef label = (CFStringRef) [[itemField getItem:i] getLabel];
        CFDictionaryRef aDict = CFDictionaryCreate(
                                                   kCFAllocatorDefault,
                                                   (void *)keys,
                                                   (void *)imValues,
                                                   2,
                                                   &kCFCopyStringDictionaryKeyCallBacks,
                                                   &kCFTypeDictionaryValueCallBacks);

        ABMultiValueAddValueAndLabel(multiValue, aDict, label, &identifier);
        CFRelease(aDict);
    }

    [self setDataToRecord:multiValue propertyID:kABPersonInstantMessageProperty checkLength:false];
    CFRelease(multiValue);
}

/**
 * Writes the relation field into record.
 * @param itemField The specified field.
 */
-(void) writeRelationField:(PimFieldItem*) itemField
{
    ABMutableMultiValueRef multi =
        ABMultiValueCreateMutable(kABMultiStringPropertyType);

    int noFields = [itemField count];
    for (int i = 0; i < noFields; i++)
    {
        NSMutableArray* array = [itemField getValue:i];
        const CFStringRef value = (CFStringRef)[array objectAtIndex:0];
        const CFStringRef label = (CFStringRef) [[itemField getItem:i] getLabel];
        ABMultiValueAddValueAndLabel(multi, value, label, NULL);
    }

    [self setDataToRecord:multi propertyID:kABPersonRelatedNamesProperty checkLength:false];
    CFRelease(multi);
}

/**
 * Writes a single field value into record.
 * @param itemField The given field.
 * @param property The property of whose value is being set.
 */
-(void) writeSingleFieldValue:(PimFieldItem*) itemField
                  propertyID:(ABPropertyID) property
{
    NSArray* valueArray = [itemField getValue:0];
    bool checkValueLength = false;
    if (MA_PIM_TYPE_STRING == [itemField getFieldType])
    {
        checkValueLength = true;
    }
    CFStringRef value = (CFStringRef)[valueArray objectAtIndex:0];
    [self setDataToRecord:value propertyID:property checkLength:checkValueLength];
}

/**
 * Sets data to record.
 * @param value The specified value.
 * @param property The property of whose value is being set.
 * @param verify If true the length of the value is checked.
 *               If the length is zero the value is not written.
 */
-(void) setDataToRecord:(CFTypeRef) value
            propertyID:(ABPropertyID) property
           checkLength:(bool) verify
{
    // Check for a empty value.
    if (verify && 0 == CFStringGetLength(value))
    {
        return;
    }
    ABRecordSetValue(mRecord, property, value, nil);
}

/**
 * Reads the contact name field from the record.
 */
-(void) readContactNameField
{
    NSMutableArray* array;
    NSString* key; 
    PimFieldItem* itemField = [[PimFieldItem alloc] initWithFieldID:MA_PIM_FIELD_CONTACT_NAME];
    PimUtils* utils = [PimUtils sharedInstance];

    NSString* firstName = (NSString*) ABRecordCopyValue(mRecord, kABPersonFirstNameProperty);
    NSString* lastName  = (NSString*) ABRecordCopyValue(mRecord, kABPersonLastNameProperty);
    NSString* middleName = (NSString*) ABRecordCopyValue(mRecord, kABPersonMiddleNameProperty);
    NSString* prefix  = (NSString*) ABRecordCopyValue(mRecord, kABPersonPrefixProperty);
    NSString* suffix = (NSString*) ABRecordCopyValue(mRecord, kABPersonSuffixProperty);
    NSString* firstNamePhonetic = (NSString*) ABRecordCopyValue(mRecord, kABPersonFirstNamePhoneticProperty);
    NSString* lastNamePhonetic  = (NSString*) ABRecordCopyValue(mRecord, kABPersonLastNamePhoneticProperty);
    NSString* middleNamePhonetic = (NSString*) ABRecordCopyValue(mRecord, kABPersonMiddleNamePhoneticProperty);

    key = [[NSString alloc] initWithFormat:@"%d", MA_PIM_FIELD_CONTACT_NAME];
    array = [[NSMutableArray alloc] init];
    [utils addStringToArray:array string:lastName];          // MA_PIM_CONTACT_NAME_FAMILY
    [utils addStringToArray:array string:firstName];         // MA_PIM_CONTACT_NAME_GIVEN
    [utils addStringToArray:array string:@""];               // MA_PIM_CONTACT_NAME_OTHER
    [utils addStringToArray:array string:middleName];        // MA_PIM_CONTACT_NAME_PREFIX
    [utils addStringToArray:array string:prefix];            // MA_PIM_CONTACT_NAME_SUFFIX
    [utils addStringToArray:array string:suffix];            // MA_PIM_CONTACT_NAME_MIDDLE
    [utils addStringToArray:array string:lastNamePhonetic];  // MA_PIM_CONTACT_NAME_PHONETIC_FAMILY
    [utils addStringToArray:array string:firstNamePhonetic]; // MA_PIM_CONTACT_NAME_PHONETIC_GIVEN
    [utils addStringToArray:array string:middleNamePhonetic];// MA_PIM_CONTACT_NAME_PHONETIC_OTHER
    [itemField addValue:array withAttribute:MA_PIM_ATTR_PREFERRED];
    [mFieldsDictionary setObject:itemField forKey:key];
}

/**
 * Reads the address field from the record.
 */
-(void) readAddressField
{
    NSMutableArray* array;
    NSString* key;
    PimFieldItem* itemField = [[PimFieldItem alloc] initWithFieldID:MA_PIM_FIELD_CONTACT_ADDR];
    PimUtils* utils = [PimUtils sharedInstance];
    ABMutableMultiValueRef multi;

    multi = ABRecordCopyValue(mRecord, kABPersonAddressProperty);
    CFStringRef addressLabel;
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

        [utils addStringToArray:array string:@""];    // Add POBOX field.
        [utils addStringToArray:array string:@""];    // Add EXTRA field.
        [utils addStringToArray:array string:street]; // Add STREET field.
        [utils addStringToArray:array string:city];   // Add LOCALITY field.
        [utils addStringToArray:array string:state];  // Add REGION field.
        [utils addStringToArray:array string:zip];    // Add POSTALCODE field.
        [utils addStringToArray:array string:country];// Add COUNTRY field.
        [utils addStringToArray:array string:@""];    // Add NEIGHBORHOOD field.

        [itemField addValue:array withLabel:(NSString*)addressLabel];
        CFRelease(aDict);
    }

    // Check if any values were added to field.
    if (0 == [itemField count])
    {
        [itemField release];
    }
    else
    {
        [mFieldsDictionary setObject:itemField forKey:key];
    }

    CFRelease(multi);
}

/**
 * Reads the birthday field from the record.
 */
-(void) readBirthdayField
{
    NSMutableArray* array;
    NSString* key;
    PimFieldItem* itemField = [[PimFieldItem alloc] initWithFieldID:MA_PIM_FIELD_CONTACT_BIRTHDAY];

    NSDate* birthday = (NSDate*) ABRecordCopyValue(mRecord, kABPersonBirthdayProperty);
    if (nil != birthday)
    {
        key = [[NSString alloc] initWithFormat:@"%d", MA_PIM_FIELD_CONTACT_BIRTHDAY];
        array = [[NSMutableArray alloc] init];
        [array addObject:birthday];
        [itemField addValue:array withAttribute:MA_PIM_ATTR_PREFERRED];
        [mFieldsDictionary setObject:itemField forKey:key];
    }
}

/**
 * Reads the email field from the record.
 */
-(void) readEmailField
{
    NSMutableArray* array;
    NSString* key;
    PimFieldItem* itemField = [[PimFieldItem alloc] initWithFieldID:MA_PIM_FIELD_CONTACT_EMAIL];;
    PimUtils* utils = [PimUtils sharedInstance];
    ABMutableMultiValueRef multi;
    multi = ABRecordCopyValue(mRecord, kABPersonEmailProperty);
    key = [[NSString alloc] initWithFormat:@"%d", MA_PIM_FIELD_CONTACT_EMAIL];

    for (CFIndex i = 0; i < ABMultiValueGetCount(multi); i++) {
        array = [[NSMutableArray alloc] init];
        NSString* emailLabel = (NSString*)ABMultiValueCopyLabelAtIndex(multi, i);
        NSString* email = (NSString*)ABMultiValueCopyValueAtIndex(multi, i);

        [utils addStringToArray:array string:email];
        [itemField addValue:array withLabel:emailLabel];
    }

    // Check if any values were added to field.
    if (0 == [itemField count])
    {
        [itemField release];
    }
    else
    {
        [mFieldsDictionary setObject:itemField forKey:key];
    }

    CFRelease(multi);
}

/**
 * Reads the nickname field from the record.
 */
-(void) readNicknameField
{
    NSMutableArray* array;
    NSString* key;
    PimFieldItem* itemField = [[PimFieldItem alloc] initWithFieldID:MA_PIM_FIELD_CONTACT_NICKNAME];

    NSString* nickname  = (NSString*) ABRecordCopyValue(mRecord, kABPersonNicknameProperty);
    if (nil != nickname)
    {
        key = [[NSString alloc] initWithFormat:@"%d", MA_PIM_FIELD_CONTACT_NICKNAME];
        array = [[NSMutableArray alloc] init];
        [array addObject:nickname];
        [itemField addValue:array withAttribute:MA_PIM_ATTR_PREFERRED];
        [mFieldsDictionary setObject:itemField forKey:key];
    }
}

/**
 * Reads the note field from the record.
 */
-(void) readNoteField
{
    NSMutableArray* array;
    NSString* key;
    PimFieldItem* itemField = [[PimFieldItem alloc] initWithFieldID:MA_PIM_FIELD_CONTACT_NOTE];

    NSString* note = (NSString*) ABRecordCopyValue(mRecord, kABPersonNoteProperty);
    if (nil != note)
    {
        key = [[NSString alloc] initWithFormat:@"%d", MA_PIM_FIELD_CONTACT_NOTE];
        array = [[NSMutableArray alloc] init];
        [array addObject:note];
        [itemField addValue:array withAttribute:MA_PIM_ATTR_PREFERRED];
        [mFieldsDictionary setObject:itemField forKey:key];
    }
}

/**
 * Reads the organization field from the record.
 */
-(void) readOrganizationField
{
    NSMutableArray* array;
    NSString* key;
    PimFieldItem* itemField = [[PimFieldItem alloc] initWithFieldID:MA_PIM_FIELD_CONTACT_ORG];

    NSString* organization  = (NSString*) ABRecordCopyValue(mRecord, kABPersonOrganizationProperty);
    if (nil != organization)
    {
        key = [[NSString alloc] initWithFormat:@"%d", MA_PIM_FIELD_CONTACT_ORG];
        array = [[NSMutableArray alloc] init];
        [array addObject:organization];
        [itemField addValue:array withAttribute:MA_PIM_ATTR_PREFERRED];
        [mFieldsDictionary setObject:itemField forKey:key];
    }
}

/**
 * Reads the phone field from the record.
 */
-(void) readPhoneField
{
    NSMutableArray* array;
    NSString* key;
    PimFieldItem* itemField = [[PimFieldItem alloc] initWithFieldID:MA_PIM_FIELD_CONTACT_TEL];
    PimUtils* utils = [PimUtils sharedInstance];
    ABMutableMultiValueRef multi;

    multi = ABRecordCopyValue(mRecord, kABPersonPhoneProperty);
    key = [[NSString alloc] initWithFormat:@"%d", MA_PIM_FIELD_CONTACT_TEL];

    for (CFIndex i = 0; i < ABMultiValueGetCount(multi); i++) {
        array = [[NSMutableArray alloc] init];
        NSString* phoneLabel = (NSString*)ABMultiValueCopyLabelAtIndex(multi, i);
        NSString* phone = (NSString*)ABMultiValueCopyValueAtIndex(multi, i);

        [utils addStringToArray:array string:phone];
        [itemField addValue:array withLabel:phoneLabel];
    }

    // Check if any values were added to field.
    if (0 == [itemField count])
    {
        [itemField release];
    }
    else
    {
        [mFieldsDictionary setObject:itemField forKey:key];
    }

    CFRelease(multi);
}

/**
 * Reads the job title field from the record.
 */
-(void) readTitleField
{
    NSMutableArray* array;
    NSString* key;
    PimFieldItem* itemField =[[PimFieldItem alloc] initWithFieldID:MA_PIM_FIELD_CONTACT_TITLE];

    NSString* jobTitle = (NSString*) ABRecordCopyValue(mRecord, kABPersonJobTitleProperty);
    if (nil != jobTitle)
    {
        key = [[NSString alloc] initWithFormat:@"%d", MA_PIM_FIELD_CONTACT_TITLE];
        array = [[NSMutableArray alloc] init];
        [array addObject:jobTitle];
        [itemField addValue:array withAttribute:MA_PIM_ATTR_PREFERRED];
        [mFieldsDictionary setObject:itemField forKey:key];
    }
}

/**
 * Reads the URL field from the record.
 */
-(void) readURLField
{
    NSMutableArray* array;
    NSString* key;
    PimFieldItem* itemField = [[PimFieldItem alloc] initWithFieldID:MA_PIM_FIELD_CONTACT_URL];;
    PimUtils* utils = [PimUtils sharedInstance];
    ABMutableMultiValueRef multi;

    multi = ABRecordCopyValue(mRecord, kABPersonURLProperty);
    key = [[NSString alloc] initWithFormat:@"%d", MA_PIM_FIELD_CONTACT_URL];

    for (CFIndex i = 0; i < ABMultiValueGetCount(multi); i++)
    {
        array = [[NSMutableArray alloc] init];
        NSString* urlLabel = (NSString*)ABMultiValueCopyLabelAtIndex(multi, i);
        NSString* url = (NSString*)ABMultiValueCopyValueAtIndex(multi, i);

        [utils addStringToArray:array string:url];
        [itemField addValue:array withLabel:urlLabel];
    }

    // Check if any values were added to field.
    if (0 == [itemField count])
    {
        [itemField release];
    }
    else
    {
        [mFieldsDictionary setObject:itemField forKey:key];
    }

    CFRelease(multi);
}

/**
 * Reads the organization info field from the record.
 */
-(void) readOrgInfoField
{
    NSMutableArray* array;
    NSString* key;
    PimFieldItem* itemField = [[PimFieldItem alloc] initWithFieldID:MA_PIM_FIELD_CONTACT_ORG_INFO];;

    NSString* department  = (NSString*) ABRecordCopyValue(mRecord, kABPersonDepartmentProperty);
    if (nil != department)
    {
        key = [[NSString alloc] initWithFormat:@"%d", MA_PIM_FIELD_CONTACT_ORG_INFO];
        array = [[NSMutableArray alloc] init];
        [array addObject:department];
        [array addObject:@""];
        [array addObject:@""];
        [array addObject:@""];
        [array addObject:@""];

        [itemField addValue:array withAttribute:MA_PIM_ATTR_PREFERRED];
        [mFieldsDictionary setObject:itemField forKey:key];
    }
}

/**
 * Reads the revision field from the record.
 */
-(void) readRevisionField;
{
    NSMutableArray* array;
    NSString* key;
    PimFieldItem* itemField = [[PimFieldItem alloc] initWithFieldID:MA_PIM_FIELD_CONTACT_REVISION];
    NSDate* revision = (NSDate*) ABRecordCopyValue(mRecord, kABPersonModificationDateProperty);
    if (nil != revision)
    {
        key = [[NSString alloc] initWithFormat:@"%d", MA_PIM_FIELD_CONTACT_REVISION];
        array = [[NSMutableArray alloc] init];
        [array addObject:revision];
        [itemField addValue:array withAttribute:MA_PIM_ATTR_PREFERRED];
        [mFieldsDictionary setObject:itemField forKey:key];
    }
}

/**
 * Reads the UID field from the record.
 */
-(void) readUIDField
{
    NSMutableArray* array;
    NSString* key;
    PimFieldItem* itemField = [[PimFieldItem alloc] initWithFieldID:MA_PIM_FIELD_CONTACT_UID];
    int recordID = ABRecordGetRecordID(mRecord);
    NSString* uid = [[NSString alloc] initWithFormat:@"%d", recordID];
    if (nil != uid)
    {
        key = [[NSString alloc] initWithFormat:@"%d", MA_PIM_FIELD_CONTACT_UID];
        array = [[NSMutableArray alloc] init];
        [array addObject:uid];
        [itemField addValue:array withAttribute:MA_PIM_ATTR_PREFERRED];
        [mFieldsDictionary setObject:itemField forKey:key];
    }
}

/**
 * Reads the instant message field from the record.
 */
-(void) readIMField
{
    NSMutableArray* array;
    NSString* key;
    PimFieldItem* itemField = [[PimFieldItem alloc] initWithFieldID:MA_PIM_FIELD_CONTACT_IM];
    PimUtils* utils = [PimUtils sharedInstance];
    ABMutableMultiValueRef multi;

    multi = ABRecordCopyValue(mRecord, kABPersonInstantMessageProperty);
    CFStringRef imLabel;
    key = [[NSString alloc] initWithFormat:@"%d", MA_PIM_FIELD_CONTACT_IM];

    for (CFIndex i = 0; i < ABMultiValueGetCount(multi); i++)
    {
        array = [[NSMutableArray alloc] init];
        imLabel = ABMultiValueCopyLabelAtIndex(multi, i);

        CFDictionaryRef aDict = ABMultiValueCopyValueAtIndex(multi, i);
        NSString* protocol = (NSString*) CFDictionaryGetValue(aDict, kABPersonInstantMessageServiceKey);
        NSString* username = (NSString*)CFDictionaryGetValue(aDict, kABPersonInstantMessageUsernameKey);

        [utils addStringToArray:array string:username];    // Add MA_PIM_CONTACT_IM_USERNAME.
        [utils addStringToArray:array string:protocol];    // Add MA_PIM_CONTACT_IM_PROTOCOL.

        [itemField addValue:array withLabel:(NSString*)imLabel];
        CFRelease(aDict);
    }

    // Check if any values were added to field.
    if (0 == [itemField count])
    {
        [itemField release];
    }
    else
    {
        [mFieldsDictionary setObject:itemField forKey:key];
    }

    CFRelease(multi);
}

/**
 * Reads the relation field from the record.
 */
-(void) readRelationField
{
    NSMutableArray* array;
    NSString* key;
    PimFieldItem* itemField = [[PimFieldItem alloc] initWithFieldID:MA_PIM_FIELD_CONTACT_RELATION];
    PimUtils* utils = [PimUtils sharedInstance];
    ABMutableMultiValueRef multi;

    multi = ABRecordCopyValue(mRecord, kABPersonRelatedNamesProperty);
    key = [[NSString alloc] initWithFormat:@"%d", MA_PIM_FIELD_CONTACT_RELATION];

    for (CFIndex i = 0; i < ABMultiValueGetCount(multi); i++) {
        array = [[NSMutableArray alloc] init];
        NSString* relationLabel = (NSString*)ABMultiValueCopyLabelAtIndex(multi, i);
        NSString* relationValue = (NSString*)ABMultiValueCopyValueAtIndex(multi, i);

        [utils addStringToArray:array string:relationValue];
        [itemField addValue:array withLabel:relationLabel];
        [relationValue release];
    }

    // Check if any values were added to field.
    if (0 == [itemField count])
    {
        [itemField release];
    }
    else
    {
        [mFieldsDictionary setObject:itemField forKey:key];
    }

    CFRelease(multi);
}

/**
 * Reads the photo field from the record.
 */
-(void) readPhotoField
{
    if (!ABPersonHasImageData(mRecord))
    {
        return;
    }

    NSMutableArray* array;
    NSString* key;
    PimFieldItem* itemField = [[PimFieldItem alloc] initWithFieldID:MA_PIM_FIELD_CONTACT_PHOTO];
    PimUtils* utils = [PimUtils sharedInstance];
    NSData* imagaData = (NSData*)ABPersonCopyImageData(mRecord);
    int handle = [utils createDataHandle:imagaData];
    NSNumber* number = [NSNumber numberWithInt:handle];
    key = [[NSString alloc] initWithFormat:@"%d", MA_PIM_FIELD_CONTACT_PHOTO];
    array = [[NSMutableArray alloc] init];
    [array addObject:number];
    [itemField addValue:array withAttribute:MA_PIM_ATTR_PREFERRED];
    [mFieldsDictionary setObject:itemField forKey:key];
    [imagaData release];
    [number release];
}

/**
 * Imports data from record and writes it into pim fields.
 */
-(void) importDataFromRecord
{
    [self readContactNameField];
    [self readAddressField];
    [self readBirthdayField];
    [self readEmailField];
    [self readNicknameField];
    [self readNoteField];
    [self readOrganizationField];
    [self readPhoneField];
    [self readTitleField];
    [self readURLField];
    [self readOrgInfoField];
    [self readRevisionField];
    [self readUIDField];
    [self readIMField];
    [self readRelationField];
    [self readPhotoField];
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
 * Checks if the field is supported on iphone platform.
 * @param fieldID One of the MA_PIM_FIELD_CONTACT constants.
 * @return True is the field is supported, false otherwise.
 */
-(bool) isFieldSupported:(const int) fieldID
{
    bool isSupported;
    switch (fieldID)
    {
        case MA_PIM_FIELD_CONTACT_CLASS:
        case MA_PIM_FIELD_CONTACT_FORMATTED_ADDR:
        case MA_PIM_FIELD_CONTACT_FORMATTED_NAME:
        case MA_PIM_FIELD_CONTACT_PUBLIC_KEY:
        case MA_PIM_FIELD_CONTACT_PUBLIC_KEY_STRING:
            isSupported = false;
            break;
        default:
            isSupported = true;
    }
    return isSupported;
}

/**
 * Checks if the field is valid.
 * @param fieldID One of the MA_PIM_FIELD_CONTACT constants.
 * @return True is the field is valid, false otherwise.
 */
-(bool) isFieldValid:(const int) fieldID
{
    if (fieldID < MA_PIM_FIELD_CONTACT_ADDR ||
        fieldID > MA_PIM_FIELD_CONTACT_ORG_INFO)
    {
        return false;
    }
    return true;
}

/**
 * Checks if the user can read data from a specified field.
 * @param fieldID One of the MA_PIM_FIELD_CONTACT constants.
 * @return True is the field is write only, false otherwise.
 */
-(bool) isFieldWriteOnly:(const int) fieldID
{
    if (MA_PIM_FIELD_CONTACT_PHOTO_URL == fieldID)
    {
        return true;
    }

    return false;
}

/**
 * Checks if the user can write data to a specified field.
 * @param fieldID One of the MA_PIM_FIELD_CONTACT constants.
 * @return True is the field is read only, false otherwise.
 */
-(bool) isFieldReadOnly:(const int) fieldID
{
    if (MA_PIM_FIELD_CONTACT_REVISION == fieldID ||
        MA_PIM_FIELD_CONTACT_UID == fieldID)
    {
        return true;
    }

    return false;
}

/**
 * Release all the objects.
 */
- (void) dealloc
{
    [super dealloc];
}

@end
