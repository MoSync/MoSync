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

/**
 * @file Util.h
 * @author Bogdan Iusco
 *
 * This file contains:
 * - values for a new PIM contact.
 * - functions for reading/writing value from/to a buffer.
 * - functions for getting the string associated with a attribute.
 * - functions for getting the string associated with a field index.
 * - method for printing the string associated with a error code.
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

/**
 * Check the a given field is supported on current platform and
 * if it has values.
 */
#define checkIfFieldIsSupported(contactHandle, fieldID) \
	int result = maPimItemFieldCount(contactHandle, fieldID); \
	if (0 >= result) \
	{\
		printResultCode(result);\
		return;\
	}

#include <conprint.h>
#include <wchar.h>
#include <ma.h>
#include <maassert.h>
#include <mawstring.h>
#include <IX_PIM.h>
#include <MAUtil/String.h>

// Common value for custom attributes.
static const MAUtil::String sCustomAttributeValue = "custom";

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
 * NULL otherwise.
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

#endif /* UTIL_H_ */
