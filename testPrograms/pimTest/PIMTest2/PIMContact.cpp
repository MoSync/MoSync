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
 * @file PIMContact.cpp
 *
 * PIMContact class shows how to:
 * - read field values.
 */

// Buffer size used for reading and writing field values.
#define BUF_SIZE 512

#include <mavsprintf.h>
#include <MAUtil/util.h>
#include <conprint.h>

#include "PIMContact.h"
#include "Util.h"

/**
 * Constructor.
 * @param pimItemHandle Handle to a pim contact item.
 */
PIMContact::PIMContact(MAHandle pimItemHandle)
{
    mItemHandle = pimItemHandle;
    mArgs.item = mItemHandle;

    mBuffer = new char[BUF_SIZE];
    mArgs.buf = mBuffer;
    mArgs.bufSize = BUF_SIZE;
}

/**
 * Destructor.
 */
PIMContact::~PIMContact()
{
    delete[] mBuffer;
}

/**
 * Print contact's field values.
 * @param fieldID One of the MA_PIM_FIELD_CONTACT constants.
 */
void PIMContact::printContactField(const int fieldID)
{
    switch(fieldID)
    {
    case MA_PIM_FIELD_CONTACT_ADDR:
        printAddress();
        break;
    case MA_PIM_FIELD_CONTACT_BIRTHDAY:
        printBirthday();
        break;
    case MA_PIM_FIELD_CONTACT_EMAIL:
        printEmail();
        break;
    case MA_PIM_FIELD_CONTACT_FORMATTED_ADDR:
        printFormattedAddress();
        break;
    case MA_PIM_FIELD_CONTACT_NAME:
        printContactName();
        break;
    case MA_PIM_FIELD_CONTACT_NICKNAME:
        printNickname();
        break;
    case MA_PIM_FIELD_CONTACT_NOTE:
        printNote();
        break;
    case MA_PIM_FIELD_CONTACT_ORG:
        printOrg();
        break;
    case MA_PIM_FIELD_CONTACT_PHOTO:
        printPhoto();
        break;
    case MA_PIM_FIELD_CONTACT_PUBLIC_KEY:
        printPublicKey();
        break;
    case MA_PIM_FIELD_CONTACT_PUBLIC_KEY_STRING:
        printPublicKeyString();
        break;
    case MA_PIM_FIELD_CONTACT_REVISION:
        printRevision();
        break;
    case MA_PIM_FIELD_CONTACT_TEL:
        printPhone();
        break;
    case MA_PIM_FIELD_CONTACT_TITLE:
        printTitle();
        break;
    case MA_PIM_FIELD_CONTACT_UID:
        printUID();
        break;
    case MA_PIM_FIELD_CONTACT_URL:
        printURL();
        break;
    case MA_PIM_FIELD_CONTACT_IM:
        printIM();
        break;
    case MA_PIM_FIELD_CONTACT_RELATION:
        printRelation();
        break;
    case MA_PIM_FIELD_CONTACT_ORG_INFO:
        printOrgInfo();
        break;
    default:
        printf("field unsupported");
    }
}

/**
 * Get contact handle.
 * @return The contact's handle.
 */
MAHandle PIMContact::getHandle() const
{
    return mItemHandle;
}

/**
 * Print the attribute for a given index field value.
 * If the attribute is custom will print the label also.
 * @param field One of the MA_PIM_FIELD constants.
 * @param index The field's value index.
 * Must be >= 0 and < maPimItemFieldCount().
 * @param pointerToFunc A pointer to a function that returns the string
 * associated with the attribute.
 */
void PIMContact::printAttribute(const int field, const int index,
    MAUtil::String(*pointerToFunc)(const int))
{
    // Get the attribute.
    int attribute = maPimItemGetAttributes(mItemHandle, field, index);

    // Print the string associated with the attribute.
    MAUtil::String attributeString = pointerToFunc(attribute);
    printf("Attribute: %s", attributeString.c_str());

    // Check if the attribute is custom.
    MAUtil::String customAttributeValue = sCustomAttributeValue;
    if (customAttributeValue == attributeString)
    {
        // Custom attribute. Get label value.
        MA_PIM_ARGS labelArgs;
        labelArgs.item = mItemHandle;
        labelArgs.field = field;

        char buf[BUF_SIZE];
        labelArgs.buf = buf;
        labelArgs.bufSize = BUF_SIZE;
        int resultCode = maPimItemGetLabel(&labelArgs, index);
        checkResultCode(resultCode);
        printf("Label: %S", (wchar*) buf);
    }
}

/**
 * Print the contact name field value.
 */
void PIMContact::printContactName()
{
    mArgs.field = MA_PIM_FIELD_CONTACT_NAME;

    // Get value from name field at position 0.
    checkResultCode(maPimItemGetValue(&mArgs, 0));

    // Print data on the screen.
    for (int i = 0; i < COUNT_NAME_INDICES; i++)
    {
        MAUtil::String contactNameType = getContactNameIndexString(i);
        const wchar* contactNameValue = getWCharArrayFromBuf(mArgs.buf, i);
        printf("%s %S", contactNameType.c_str(), contactNameValue);
    }
}

/**
 * Print the address field values.
 */
void PIMContact::printAddress()
{
    mArgs.field = MA_PIM_FIELD_CONTACT_ADDR;

    // Get the number of values.
    int countValues = maPimItemFieldCount(
        mItemHandle,
        MA_PIM_FIELD_CONTACT_ADDR);
    checkResultCode(countValues);

    for (int i = 0; i < countValues; i++)
    {
        // Get the value at position i.
        checkResultCode(maPimItemGetValue(&mArgs, i));

        // Print the attribute for this value.
        printAttribute(MA_PIM_FIELD_CONTACT_ADDR, i, getAddressAttributeString);

        // Print data on the screen.
        for (int j = 0; j < COUNT_ADDRESS_INDICES; j++)
        {
            MAUtil::String addressValueIndex = getAddressIndexString(j);
            const wchar* addressValue = getWCharArrayFromBuf(mArgs.buf, j);
            printf("%s %S", addressValueIndex.c_str(), addressValue);
        }

        printf("\n");
    }
}

/**
 * Print birthday field value.
 */
void PIMContact::printBirthday()
{
    mArgs.field = MA_PIM_FIELD_CONTACT_BIRTHDAY;

    // Get value from birthday field at position 0.
    checkResultCode(maPimItemGetValue(&mArgs, 0));

    // Print value on screen(the value it's in milliseconds - UNIX time).
    int birthday = *(int*) mArgs.buf;
    printf("Birthday: %d (in milliseconds)", birthday);
}

/**
 * Print email field values.
 */
void PIMContact::printEmail()
{
    mArgs.field = MA_PIM_FIELD_CONTACT_EMAIL;

    // Get the number of value.
    int countValues = maPimItemFieldCount(mItemHandle,
        MA_PIM_FIELD_CONTACT_EMAIL);
    checkResultCode(countValues);

    for (int i = 0; i < countValues; i++)
    {
        // Get the value from email field at position i.
        checkResultCode(maPimItemGetValue(&mArgs, i));

        // Print the value's attribute.
        printAttribute(MA_PIM_FIELD_CONTACT_EMAIL, i, getEmailAttributeString);

        // Print email value on the screen.
        printf("Email: %S \n\n", (wchar*) mArgs.buf);
    }
}

/**
 * Print formatted address field value.
 */
void PIMContact::printFormattedAddress()
{
    mArgs.field = MA_PIM_FIELD_CONTACT_FORMATTED_ADDR;

    // Get the value from formatted address at position 0.
    checkResultCode(maPimItemGetValue(&mArgs, 0));
    printf("Formatted address: %S", (wchar*) mArgs.buf);
}

/**
 * Print nickname field value.
 */
void PIMContact::printNickname()
{
    mArgs.field = MA_PIM_FIELD_CONTACT_NICKNAME;

    // Get value from nickname field at position 0.
    checkResultCode(maPimItemGetValue(&mArgs, 0));
    printf("Nickname: %S", (wchar*) mArgs.buf);
}

/**
 * Print note field value.
 */
void PIMContact::printNote()
{
    mArgs.field = MA_PIM_FIELD_CONTACT_NOTE;

    // Get value from note field at position 0.
    checkResultCode(maPimItemGetValue(&mArgs, 0));
    printf("Note: %S", (wchar*) mArgs.buf);
}

/**
 * Print organization field value.
 */
void PIMContact::printOrg()
{
    printf("Organization field:\n\n");
    mArgs.field = MA_PIM_FIELD_CONTACT_ORG;

    // Get the number of values.
    int countValues =
        maPimItemFieldCount(mItemHandle, MA_PIM_FIELD_CONTACT_ORG);
    checkResultCode(countValues);

    for (int i = 0; i < countValues; i++)
    {
        // Get value form org field at position i.
        checkResultCode(maPimItemGetValue(&mArgs, i));
        printf("Organization: %S \n\n", (wchar*) mArgs.buf);
    }
}

/**
 * Print photo field value.
 */
void PIMContact::printPhoto()
{
    mArgs.field = MA_PIM_FIELD_CONTACT_PHOTO;

    // Get value from photo field at position 0.
    // A data handle to the image will be returned.
    checkResultCode(maPimItemGetValue(&mArgs, 0));

    int handle = *(int*) mArgs.buf;
    printf("Photo data handle: %d", handle);
}

/**
 * Print key field value.
 */
void PIMContact::printPublicKey()
{
    mArgs.field = MA_PIM_FIELD_CONTACT_PUBLIC_KEY;
    checkResultCode(maPimItemGetValue(&mArgs, 0));
}

/**
 * Print public key field value.
 */
void PIMContact::printPublicKeyString()
{
    mArgs.field = MA_PIM_FIELD_CONTACT_PUBLIC_KEY_STRING;
    checkResultCode(maPimItemGetValue(&mArgs, 0));
    printf("Public key string: %S", (wchar*) mArgs.buf);
}

/**
 * Print revision field value.
 */
void PIMContact::printRevision()
{
    mArgs.field = MA_PIM_FIELD_CONTACT_REVISION;

    // Get value from revision field at position 0;
    // Returned value is a date in milliseconds(UNIX time).
    checkResultCode(maPimItemGetValue(&mArgs, 0));

    // Print date on the screen.
    int revisionDate = *(int*) mArgs.buf;
    printf("Revision date: %d (in milliseconds)", revisionDate);
}

/**
 * Print phone field values.
 */
void PIMContact::printPhone()
{
    mArgs.field = MA_PIM_FIELD_CONTACT_TEL;

    // Get the number of values.
    int countValues =
        maPimItemFieldCount(mItemHandle, MA_PIM_FIELD_CONTACT_TEL);
    checkResultCode(countValues);

    for (int i = 0; i < countValues; i++)
    {
        // Get value from tel field at position i.
        checkResultCode(maPimItemGetValue(&mArgs, i));

        // Print value's attribute.
        printAttribute(MA_PIM_FIELD_CONTACT_TEL, i, getPhoneAttributeString);

        printf("Phone: %S\n\n", (wchar*) mArgs.buf);
    }
}

/**
 * Print title field value.
 */
void PIMContact::printTitle()
{
    mArgs.field = MA_PIM_FIELD_CONTACT_TITLE;

    int countValues = maPimItemFieldCount(mItemHandle,
        MA_PIM_FIELD_CONTACT_TITLE);
    checkResultCode(countValues);

    for (int i = 0; i < countValues; i++)
    {
        checkResultCode(maPimItemGetValue(&mArgs, i));
        printAttribute(MA_PIM_FIELD_CONTACT_TITLE, i, getTitleAttributeString);
        printf("Title: %S \n \n", (wchar*) mArgs.buf);
    }
}

/**
 * Print UID field value.
 */
void PIMContact::printUID()
{
    mArgs.field = MA_PIM_FIELD_CONTACT_UID;
    checkResultCode(maPimItemGetValue(&mArgs, 0));
    printf("UID: %S", (wchar*) mArgs.buf);
}

/**
 * Print URL field values.
 */
void PIMContact::printURL()
{
    mArgs.field = MA_PIM_FIELD_CONTACT_URL;

    // Get the number of values.
    int countValues =
        maPimItemFieldCount(mItemHandle, MA_PIM_FIELD_CONTACT_URL);
    checkResultCode(countValues);

    for (int i = 0; i < countValues; i++)
    {
        // Get the value from URL field at position 0.
        checkResultCode(maPimItemGetValue(&mArgs, i));

        // Print value's attribute.
        printAttribute(MA_PIM_FIELD_CONTACT_URL, i, getWebsiteAttributeString);

        // Print value on the screen.
        printf("URL: %S\n\n", (wchar*) mArgs.buf);
    }
}

/**
 * Print instant message field values.
 */
void PIMContact::printIM()
{
    mArgs.field = MA_PIM_FIELD_CONTACT_IM;

    // Get the number of values.
    int countValues = maPimItemFieldCount(mItemHandle, MA_PIM_FIELD_CONTACT_IM);
    checkResultCode(countValues);

    for (int i = 0; i < countValues; i++)
    {
        // Get value from im field at position i.
        checkResultCode(maPimItemGetValue(&mArgs, i));

        // Print value's attribute.
        printAttribute(MA_PIM_FIELD_CONTACT_IM, i, getIMAttributeString);

        // Print value on the screen.
        const wchar* protocolValue = getWCharArrayFromBuf(
            mArgs.buf,
            MA_PIM_CONTACT_IM_PROTOCOL);
        const wchar* usernameValue = getWCharArrayFromBuf(
            mArgs.buf,
            MA_PIM_CONTACT_IM_USERNAME);
        printf("Username: %S \nProtocol: %S \n\n",
            usernameValue,
            protocolValue);
    }
}

void PIMContact::printRelation()
{
    mArgs.field = MA_PIM_FIELD_CONTACT_RELATION;

    // Get the number of values.
    int countValues = maPimItemFieldCount(mItemHandle,
        MA_PIM_FIELD_CONTACT_RELATION);
    checkResultCode(countValues);

    for (int i = 0; i < countValues; i++)
    {
        // Get value from relation field at position i.
        checkResultCode(maPimItemGetValue(&mArgs, i));

        // Print value's attribute.
        printAttribute(MA_PIM_FIELD_CONTACT_RELATION, i,
            getRelationAttributeString);

        // Print value on the screen.
        printf("Relation: %S\n\n", (wchar*) mArgs.buf);
    }
}

/**
 * Print organization info field value.
 */
void PIMContact::printOrgInfo()
{
    mArgs.field = MA_PIM_FIELD_CONTACT_ORG_INFO;

    // Get the number of values.
    int countValues = maPimItemFieldCount(mItemHandle,
        MA_PIM_FIELD_CONTACT_ORG_INFO);
    checkResultCode(countValues);

    for (int i = 0; i < countValues; i++)
    {
        // Get value from org info field at position i.
        checkResultCode(maPimItemGetValue(&mArgs, i));

        // Print value's attribute.
        printAttribute(MA_PIM_FIELD_CONTACT_RELATION, i,
            getOrgInfoAttributeString);

        // Print value.
        for (int j = 0; j < COUNT_ORG_INFO_INDICES; j++)
        {
            MAUtil::String orgInfoValueIndex = getOrgInfoIndexString(j);
            const wchar* orgInfoValue = getWCharArrayFromBuf(mArgs.buf, j);
            printf("%s %S\n\n", orgInfoValueIndex.c_str(), orgInfoValue);
        }
    }
}
