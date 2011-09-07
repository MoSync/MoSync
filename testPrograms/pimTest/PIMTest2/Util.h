/*
 Copyright (C) 2011 MoSync AB

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License,
 version 2, as published by the Free Software Foundation.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 MA 02110-1301, USA.
 */

#ifndef UTIL_H_
#define UTIL_H_

/**
 * Check the result of a syscall.
 * If any error occurred the error is printed on the screen.
 */
#define checkResultCode(resultCode) \
    if (0 > resultCode) \
    {\
        printResultCode(resultCode);\
        return;\
    }

#include <conprint.h>
#include <wchar.h>
#include <ma.h>
#include <maassert.h>
#include <mawstring.h>
#include <IX_PIM.h>
#include <MAUtil/String.h>

/**
 * Value for MA_PIM_FIELD_CONTACT_NAME field.
 */
static const wchar_t* sContactName[] =
{
    L"O'Conner",   // MA_PIM_CONTACT_NAME_FAMILY
    L"Brian",      // MA_PIM_CONTACT_NAME_GIVEN
    L"Robert",     // MA_PIM_CONTACT_NAME_OTHER
    L"Mr",         // MA_PIM_CONTACT_NAME_PREFIX
    L"Jr",         // MA_PIM_CONTACT_NAME_SUFFIX
    L"",           // MA_PIM_CONTACT_NAME_PHONETIC_FAMILY
    L"",           // MA_PIM_CONTACT_NAME_PHONETIC_GIVEN
    L""            // MA_PIM_CONTACT_NAME_PHONETIC_OTHER
};

/**
 * Values for MA_PIM_FIELD_CONTACT_ADDR field.
 */
static const wchar_t* sAddressWork[] =
{
    L"",                  // MA_PIM_CONTACT_ADDR_POBOX
    L"",                  // MA_PIM_CONTACT_ADDR_EXTRA
    L"Saltmatargatan 8A", // MA_PIM_CONTACT_ADDR_STREET
    L"Stockholm",         // MA_PIM_CONTACT_ADDR_LOCALITY
    L"",                  // MA_PIM_CONTACT_ADDR_REGION
    L"113 59",            // MA_PIM_CONTACT_ADDR_POSTALCODE
    L"Sweden",            // MA_PIM_CONTACT_ADDR_COUNTRY
    L""                   // MA_PIM_CONTACT_ADDR_NEIGHBORHOOD
};
static const wchar_t* sAddressHome[] =
{
    L"",                 // MA_PIM_CONTACT_ADDR_POBOX
    L"",                 // MA_PIM_CONTACT_ADDR_EXTRA
    L"21 Decembrie 70",  // MA_PIM_CONTACT_ADDR_STREET
    L"Cluj-Napoca",      // MA_PIM_CONTACT_ADDR_LOCALITY
    L"Cluj",             // MA_PIM_CONTACT_ADDR_REGION
    L"400123",           // MA_PIM_CONTACT_ADDR_POSTALCODE
    L"Romania",          // MA_PIM_CONTACT_ADDR_COUNTRY
    L"Marasti"           // MA_PIM_CONTACT_ADDR_NEIGHBORHOOD
};

static const wchar_t* sAddressModified[] =
{
    L"",                 // MA_PIM_CONTACT_ADDR_POBOX
    L"",                 // MA_PIM_CONTACT_ADDR_EXTRA
    L"Dorobantilor 14",  // MA_PIM_CONTACT_ADDR_STREET
    L"Cluj-Napoca",      // MA_PIM_CONTACT_ADDR_LOCALITY
    L"Cluj",             // MA_PIM_CONTACT_ADDR_REGION
    L"400117",           // MA_PIM_CONTACT_ADDR_POSTALCODE
    L"Romania",          // MA_PIM_CONTACT_ADDR_COUNTRY
    L"Marasti"           // MA_PIM_CONTACT_ADDR_NEIGHBORHOOD
};

/**
 * Value for address label.
 */
static const wchar_t* sAddressLabel = L"New work address";

/**
 * Value for MA_PIM_FIELD_CONTACT_ORG_INFO field.
 */
static const wchar_t* sOrgInfo[] =
{
    L"Dev-SDK",      // MA_PIM_CONTACT_ORG_INFO_DEPARTMENT
    L"",              // MA_PIM_CONTACT_ORG_INFO_JOB_DESCRIPTION
    L"",              // MA_PIM_CONTACT_ORG_INFO_SYMBOL
    L"",              // MA_PIM_CONTACT_ORG_INFO_PHONETIC_NAME
    L"",              // MA_PIM_CONTACT_ORG_INFO_OFFICE_LOCATION
    L""               // MA_PIM_CONTACT_ORG_INFO_PHONETIC_NAME_STYLE
};

/**
 * Values for MA_PIM_FIELD_CONTACT_EMAIL field.
 */
static const wchar_t* sEmailWork = L"brian@mosync.com";
static const wchar_t* sEmailHome = L"brian13@yahoo.com";

/**
 * Value for MA_PIM_FIELD_CONTACT_PHOTO_URL field.
 */
static const wchar_t* sPhotoURL =
    L"http://www.mosync.com/files/press/images/mosync_logo_monochrome_vertical_thumb.png";

/**
 * Values for MA_PIM_FIELD_CONTACT_TEL field.
 */
static const wchar_t* sPhoneMobile = L"+46767345769";
static const wchar_t* sPhoneIPhone = L"+40742123456";
static const wchar_t* sPhoneHome = L"+40364768453";

/**
 * Value for MA_PIM_FIELD_CONTACT_NICKNAME field.
 */
static const wchar_t* sNickname = L"Spike";

/**
 * Value for MA_PIM_FIELD_CONTACT_NOTE field.
 */
static const wchar_t* sNote = L"Nissan Skyline R34";

/**
 * Value for MA_PIM_FIELD_CONTACT_ORG field.
 */
static const wchar_t* sOrg = L"MoSync";

/**
 * Value for MA_PIM_FIELD_CONTACT_TITLE field.
 */
static const wchar_t* sTitle = L"Developer";

/**
 * Values for MA_PIM_FIELD_CONTACT_URL field.
 */
static const wchar_t* sURLHome = L"www.brian.com";
static const wchar_t* sURLWork = L"www.mosync.com";

/**
 * Values for MA_PIM_FIELD_CONTACT_IM field.
 */
static const wchar_t* sIMHome[] =
{
    L"brian13",
    L"Yahoo"
};
static const wchar_t* sIMOther[] =
{
    L"brian",
    L"AIM"
};

/**
 * Value for MA_PIM_FIELD_CONTACT_RELATION field.
 */
static const wchar_t* sRelationBrother = L"Chester";
static const wchar_t* sRelationManager = L"Alex";

// Used for separating fields on the screen.
static const char* sFieldSeparator =
    "______________________________________________\n\n";

// Common value for custom attributes.
static const char* sCustomAttributeValue = "custom";

/**
 * Indices for MA_PIM_FIELD_CONTACT_ADDR field:
 * - MA_PIM_CONTACT_ADDR_POBOX
 * - MA_PIM_CONTACT_ADDR_EXTRA
 * - MA_PIM_CONTACT_ADDR_STREET
 * - MA_PIM_CONTACT_ADDR_LOCALITY
 * - MA_PIM_CONTACT_ADDR_REGION
 * - MA_PIM_CONTACT_ADDR_POSTALCODE
 * - MA_PIM_CONTACT_ADDR_COUNTRY
 * - MA_PIM_CONTACT_ADDR_NEIGHBORHOOD
 */
static const int COUNT_ADDRESS_INDICES = 8;

/**
 * Indices for MA_PIM_FIELD_CONTACT_NAME field:
 * - MA_PIM_CONTACT_NAME_FAMILY
 * - MA_PIM_CONTACT_NAME_GIVEN
 * - MA_PIM_CONTACT_NAME_OTHER
 * - MA_PIM_CONTACT_NAME_PREFIX
 * - MA_PIM_CONTACT_NAME_SUFFIX
 * - MA_PIM_CONTACT_NAME_PHONETIC_FAMILY
 * - MA_PIM_CONTACT_NAME_PHONETIC_GIVEN
 * - MA_PIM_CONTACT_NAME_PHONETIC_OTHER
 */
static const int COUNT_NAME_INDICES = 8;

/**
 * Indices for MA_PIM_FIELD_CONTACT_ORG_INFO field:
 * - MA_PIM_CONTACT_ORG_INFO_DEPARTMENT
 * - MA_PIM_CONTACT_ORG_INFO_JOB_DESCRIPTION
 * - MA_PIM_CONTACT_ORG_INFO_SYMBOL
 * - MA_PIM_CONTACT_ORG_INFO_PHONETIC_NAME
 * - MA_PIM_CONTACT_ORG_INFO_OFFICE_LOCATION
 * - MA_PIM_CONTACT_ORG_INFO_PHONETIC_NAME_STYLE
 */
static const int COUNT_ORG_INFO_INDICES = 6;

/**
 * Indices for MA_PIM_FIELD_CONTACT_IM field:
 * - MA_PIM_CONTACT_IM_USERNAME
 * - MA_PIM_CONTACT_IM_PROTOCOL
 */
static const int COUNT_IM_INDICES = 2;

/**
 * Wait for a pointer released event.
 */
void waitForClick();

/**
 * Copy wchar array.
 * @param destination Pointer to the destination array where the content will
 * be copied
 * @param source Pointer to the source of data to be copied.
 * @return The number of bytes copied.
 */
int copyWCharArray(void* destination, const wchar_t* source);

/**
 * Get a wchar array from a specified buffer.
 * @param buffer Contains n wchar-arrays, where n is the int value that can
 * be read from buffer address.
 * @param arrayIndex The index of the array.
 * @return A pointer to the wchar array if the arrayIndex is valid, or
 * a pointer to the latest wchar array from buffer otherwise.
 */
const wchar* getWCharArrayFromBuf(void* buffer, const int arrayIndex);

/**
 * Write n wchar-arrays to buffer.
 * @param buffer The given buffer.
 * The first value in buffer will be the number of wchar-arrays(n argument).
 * @param src Contains the n wchar-arrays that will be written into buffer.
 * @return The number of written bytes.
 */
int writeWCharArraysToBuf(
    void* buffer,
    const wchar_t** src,
    const int n);

/**
 * Print the result code.
 * If resultCode is different then MA_PIM_ERR_NONE prints
 * the error.
 * @param The given result code.
 */
void printResultCode(const int resultCode);

/**
 * Get the string associated with a contact name field index.
 * @param index One of the MA_PIM_CONTACT_NAME constants.
 * @return The contact name index string.
 */
MAUtil::String getContactNameIndexString(const int index);

/**
 * Get the string associated with a address field index.
 * @param index One of the MA_PIM_CONTACT_ADDR constants.
 * @return The address index string.
 */
MAUtil::String getAddressIndexString(const int index);

/**
 * Get the string associated with a org info field index.
 * @param index One of the MA_PIM_CONTACT_ORG_INFO constants.
 * @return The org info index string.
 */
MAUtil::String getOrgInfoIndexString(const int index);

/**
 * Return the string associated with a specified address attribute.
 * @param index One of the MA_PIM_ATTR_ADDR constants.
 */
MAUtil::String getAddressAttributeString(const int attribute);

/**
 * Return the string associated with a specified email attribute.
 * @param index One of the MA_PIM_ATTR_EMAIL constants.
 */
MAUtil::String getEmailAttributeString(const int attribute);

/**
 * Return the string associated with a specified formatted address attribute.
 * @param index One of the MA_PIM_ATTR_FORMATTED_ADDR constants.
 */
MAUtil::String getFormattedAddressAttributeString(const int attribute);

/**
 * Return the string associated with a specified org attribute.
 * @param index One of the MA_PIM_ATTR_ORG_ constants.
 */
MAUtil::String getOrgAttributeString(const int attribute);

/**
 * Return the string associated with a specified phone attribute.
 * @param index One of the MA_PIM_ATTR_PHONE constants.
 */
MAUtil::String getPhoneAttributeString(const int attribute);

/**
 * Return the string associated with a specified title attribute.
 * @param index One of the MA_PIM_ATTR_TITLE constants.
 */
MAUtil::String getTitleAttributeString(const int attribute);

/**
 * Return the string associated with a specified website attribute.
 * @param index One of the MA_PIM_ATTR_WEBSITE constants.
 */
MAUtil::String getWebsiteAttributeString(const int attribute);

/**
 * Return the string associated with a specified IM attribute.
 * @param index One of the MA_PIM_ATTR_IM_ constants.
 */
MAUtil::String getIMAttributeString(const int attribute);

/**
 * Return the string associated with a specified relation attribute.
 * @param index One of the MA_PIM_ATTR_RELATION constants.
 */
MAUtil::String getRelationAttributeString(const int attribute);

/**
 * Return the string associated with a specified org info attribute.
 * @param index One of the MA_PIM_ATTR_ORG_INFO constants.
 */
MAUtil::String getOrgInfoAttributeString(const int attribute);

/**
 * Return the string associated with a specified field id.
 * @param index One of the MA_PIM_FIELD_CONTACT constants.
 */
MAUtil::String getFieldString(const int fieldID);

#endif /* UTIL_H_ */
