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
 * Writes the related name field into record.
 * @param itemField The specified field.
 * @return One of MA_PIM_ERR constants.
 */
-(int) writeRelatedNameField:(PimFieldItem*) itemField;

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
 * Gets the instant message protocol associeted with a string constant.
 * @param value Must be one of the MA_PIM_CONTACT_IM constants.
 * @return One of the IM protocol(see ABPerson reference - iOS Developer Library),
 *         or nil if the value param is not supported on iphone platform.
 */
-(const CFStringRef) getIMProtocol:(NSString*) value;

@end
