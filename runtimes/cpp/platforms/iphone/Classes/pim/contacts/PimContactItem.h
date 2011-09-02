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

#import <Foundation/Foundation.h>
#import <AddressBook/AddressBook.h>

#import "PimUtil.h"
#import "PimFieldItem.h"
#import "PimItem.h"

#include "helpers/cpp_defs.h"
#include "helpers/cpp_ix_pim.h"

@interface PimContactItem: PimItem {

    /**
     * Stores the contact's data.
     */
    ABRecordRef mRecord;
}

/**
 * Init function.
 */
-(id) init;

/**
 * Init function with record.
 * @param record Address Book record.
 */
-(id) initWithRecord:(ABRecordRef) record;

/**
 * Imports data from record and writes it into pim fields.
 */
-(void) importDataFromRecord;

/**
 * Writes all the fields into record.
 * Deletes all item's fields.
 * @return One of the MA_PIM_ERR constants.
 */
-(int) close;

/**
 * Gets the number of values for a given field ID.
 * Checks if field is valid and supported.
 * @param field The specified field ID(one of the MA_PIM_FIELD constants).
 * @return The number of values for the given field, or 0 if the field ID
 *         has no values/does not exist.
 */
-(int) fieldCount:(const int) fieldID;

/**
 * Gets the attribute for a given field value.
 * Checks if field is valid and supported.
 * @param fieldID The specified field ID(one of the MA_PIM_FIELD constants).
 * @param index The index of the given field value.
 * @return One of MA_PIM_ATTR constants, or MA_PIM_ERR in case of error.
 */
-(int) getAttribute:(const int) fieldID
         indexValue:(const int) index;

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
     indexValue:(const int) index;

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
     indexValue:(const int) index;

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
     indexValue:(const int) index;

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
  withAttribute:(const int) attribute;

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
 valueAttribute:(const int) atttribute;

/**
 * Removes a value from a field.
 * Checks if the field is valid or supported.
 * @param field One of the MA_PIM_FIELD constants.
 * @param index Field's value index.
 * @return One of MA_PIM_ERR constants.
 */
-(int) removeValue:(const int) field
           atIndex:(const int) index;

/**
 * Get the item's record.
 * @return The item's record.
 */
-(ABRecordRef) getRecord;

/**
 * Writes a given field into record.
 * @param itemField The specified field.
 * @return One of MA_PIM_ERR constants.
 */
-(int) writeField:(PimFieldItem*) itemField;

/**
 * Writes an address field into record.
 * @param itemField The specified field.
 * @return One of the MA_PIM_ERR constants.
 */
-(int) writeAddresField:(PimFieldItem*) itemField;

/**
 * Writes the contact name field into record.
 * @param itemField The specified field.
 * @return One of the MA_PIM_ERR constants.
 */
-(int) writeContactNameField:(PimFieldItem*) itemField;

/**
 * Writes the email field into record.
 * @param itemField The specified field.
 * @return One of the MA_PIM_ERR constants.
 */
-(int) writeEmailField:(PimFieldItem*) itemField;

/**
 * Writes the photo field into record.
 * @param itemField The specified field.
 * @return One of the MA_PIM_ERR constants.
 */
-(int) writePhotoField:(PimFieldItem*) itemField;

/**
 * Writes the photo url field into record.
 * @param itemField The specified field.
 * @return One of the MA_PIM_ERR constants.
 */
-(int) writePhotoURLField:(PimFieldItem*) itemField;

/**
 * Writes the phone field into record.
 * @param itemField The specified field.
 * @return One of the MA_PIM_ERR constants.
 */
-(int) writePhoneField:(PimFieldItem*) itemField;

/**
 * Writes the URL field into record.
 * @param itemField The specified field.
 * @return One of the MA_PIM_ERR constants.
 */
-(int) writeURLField:(PimFieldItem*) itemField;

/**
 * Writes the supplemental info organization field into record.
 * @param itemField The specified field.
 * @return One of the MA_PIM_ERR constants.
 */
-(int) writeOrgInfoField:(PimFieldItem*) itemField;

/**
 * Writes the anniversary field into record.
 * @param itemField The specified field.
 * @return One of MA_PIM_ERR constants.
 */
-(int) writeAnniversaryField:(PimFieldItem*) itemField;

/**
 * Writes the instant message field into record.
 * @param itemField The specified field.
 * @return One of MA_PIM_ERR constants.
 */
-(int) writeIMField:(PimFieldItem*) itemField;

/**
 * Writes the relation field into record.
 * @param itemField The specified field.
 * @return One of MA_PIM_ERR constants.
 */
-(int) writeRelationField:(PimFieldItem*) itemField;

/**
 * Writes a single field value into record.
 * @param itemField The given field.
 * @param property The property of whose value is being set.
 * @return One of MA_PIM_ERR constants.
 */
-(int) writeSingleFieldValue:(PimFieldItem*) itemField
                  propertyID:(ABPropertyID) property;

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
           checkLength:(bool) verify;

/**
 * Reads the contact name field from the record.
 */
-(void) readContactNameField;

/**
 * Reads the address field from the record.
 */
-(void) readAddressField;

/**
 * Reads the birthday field from the record.
 */
-(void) readBirthdayField;

/**
 * Reads the email field from the record.
 */
-(void) readEmailField;

/**
 * Reads the nickname field from the record.
 */
-(void) readNicknameField;

/**
 * Reads the note field from the record.
 */
-(void) readNoteField;

/**
 * Reads the organization field from the record.
 */
-(void) readOrganizationField;

/**
 * Reads the phone field from the record.
 */
-(void) readPhoneField;

/**
 * Reads the job title field from the record.
 */
-(void) readTitleField;

/**
 * Reads the URL field from the record.
 */
-(void) readURLField;

/**
 * Reads the organization info field from the record.
 */
-(void) readOrgInfoField;

/**
 * Reads the revision field from the record.
 */
-(void) readRevisionField;

/**
 * Reads the instant message field from the record.
 */
-(void) readIMField;

/**
 * Reads the relation field from the record.
 */
-(void) readRelationField;

/**
 * Reads the photo field from the record.
 */
-(void) readPhotoField;

/**
 * Gets the instant message protocol associeted with a string constant.
 * @param value Must be one of the MA_PIM_CONTACT_IM constants.
 * @return One of the IM protocol(see ABPerson reference - iOS Developer Library),
 *         or nil if the value param is not supported on iphone platform.
 */
-(const CFStringRef) getIMProtocol:(NSString*) value;

/**
 * Checks if the field is supported on iphone platform.
 * @param fieldID One of the MA_PIM_FIELD_CONTACT constants.
 * @return True is the field is supported, false otherwise.
 */
-(bool) isFieldSupported:(const int) fieldID;

/**
 * Checks if the field is valid.
 * @param fieldID One of the MA_PIM_FIELD_CONTACT constants.
 * @return True is the field is valid, false otherwise.
 */
-(bool) isFieldValid:(const int) fieldID;

/**
 * Checks if the user can read data from a specified field.
 * @param fieldID One of the MA_PIM_FIELD_CONTACT constants.
 * @return True is the field is write only, false otherwise.
 */
-(bool) isFieldWriteOnly:(const int) fieldID;

/**
 * Checks if the user can write data to a specified field.
 * @param fieldID One of the MA_PIM_FIELD_CONTACT constants.
 * @return True is the field is read only, false otherwise.
 */
-(bool) isFieldReadOnly:(const int) fieldID;

@end
