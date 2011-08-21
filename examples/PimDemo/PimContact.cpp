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
 * Check the result of a syscall.
 * If any error occurred the error is printed on the screen.
 */
#define checkResultCode(resultCode) \
    if (0 > resultCode) \
    {\
    printResultCode(resultCode);\
    return;\
    }

#include <mavsprintf.h>
#include <MAUtil/util.h>
#include <conprint.h>

#include "PimContact.h"
#include "Util.h"
#include "MAHeaders.h"

/**
 * Constructor.
 * @param pimItemHandle Handle to a pim contact item.
 */
PimContact::PimContact(MAHandle pimItemHandle)
{
    mItemHandle = pimItemHandle;
    mArgs.item = mItemHandle;

    char buf[512];
    mArgs.buf = buf;
    mArgs.bufSize = 512;
}

/**
 * Destructor.
 */
PimContact::~PimContact()
{

}

/**
 * Print the contact's fields.
 */
void PimContact::printContact()
{
    printContactNameField();
    waitForClick();
    printAddress();
    waitForClick();
    printBirthday();
    waitForClick();
    printClass();
    waitForClick();
    printEmail();
    waitForClick();
    printFormatedAddress();
    waitForClick();
    printFormatedName();
    waitForClick();
    printNickname();
    waitForClick();
    printNote();
    waitForClick();
    printOrg();
    waitForClick();
    printPhoto();
    waitForClick();
    printPhotoURL();
    waitForClick();
    printPublicKey();
    waitForClick();
    printPublicKeyString();
    waitForClick();
    printRevision();
    waitForClick();
    printPhone();
    waitForClick();
    printTitle();
    waitForClick();
    printUID();
    waitForClick();
    printURL();
    waitForClick();
    printIM();
    waitForClick();
    printRelation();
    waitForClick();
    printOrgInfo();
    waitForClick();
    printf("===============================================");
}

/**
 * Add data to the contact.
 * The contact's fields must be empty.
 */
void PimContact::addDataToContact()
{
    addContactName();
    waitForClick();
    addAddress();
    waitForClick();
    addBirthday();
    waitForClick();
    addClass();
    waitForClick();
    addEmail();
    waitForClick();
    addFormatedAddress();
    waitForClick();
    addFormatedName();
    waitForClick();
    addNickname();
    waitForClick();
    addNote();
    waitForClick();
    addOrg();
    waitForClick();
    addPhoto();
    waitForClick();
//    addPhotoURL();
//    waitForClick();
    addPublicKey();
    waitForClick();
    addPublicKeyString();
    waitForClick();
    addRevision();
    waitForClick();
    addPhone();
    waitForClick();
    addTitle();
    waitForClick();
    addUID();
    waitForClick();
    addURL();
    waitForClick();
    addIM();
    waitForClick();
    addRelation();
    waitForClick();
    addOrgInfo();
    waitForClick();
}

/**
 * Wait for a pointer released event.
 */
void PimContact::waitForClick()
{
    printf("Tap the screen to continue......\n");
    MAEvent event;
    while (true)
    {
        maWait(-1);
        maGetEvent(&event);
        if (EVENT_TYPE_POINTER_RELEASED == event.type)
        {
            break;
        }
    }
    printf(sFieldSeparator);
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
void PimContact::printAttribute(const int field, const int index,
    MAUtil::String(*pointerToFunc)(const int))
{
    int attribute = maPimItemGetAttributes(mItemHandle, field, index);
    MAUtil::String attributeString = pointerToFunc(attribute);
    printf("Attribute: %s", attributeString.c_str());
    if (MA_PIM_ATTR_ADDR_CUSTOM == attribute)
    {
        MA_PIM_ARGS attributeArgs;
        attributeArgs.item = mItemHandle;
        attributeArgs.field = field;

        char buf[512];
        attributeArgs.buf = buf;
        attributeArgs.bufSize = 512;
        int resultCode = maPimItemGetLabel(&attributeArgs, index);
        checkResultCode(resultCode);
        printf("Label: %S", (wchar*) buf);
    }
}

/**
 * Print the contact name field values.
 */
void PimContact::printContactNameField()
{
    printf("Contact name field:");
    mArgs.field = MA_PIM_FIELD_CONTACT_NAME;
    checkResultCode(maPimItemGetValue(&mArgs, 0));
    printAttribute(MA_PIM_FIELD_CONTACT_NAME, 0, getPrefferedAttributeString);
    for (int i = 0; i < 8; i++)
    {
        MAUtil::String contactNameType = getContactNameIndexString(i);
        const wchar* contactNameValue = getWCharArrayFromBuf(mArgs.buf, i);
        printf("%s %S", contactNameType.c_str(), contactNameValue);
    }
}

/**
 * Print the address field values.
 */
void PimContact::printAddress()
{
    printf("MA_PIM_FIELD_CONTACT_ADDR field:");
    mArgs.field = MA_PIM_FIELD_CONTACT_ADDR;

    int countValues = maPimItemFieldCount(mItemHandle,
        MA_PIM_FIELD_CONTACT_ADDR);
    checkResultCode(countValues);

    for (int i = 0; i < countValues; i++)
    {

        checkResultCode(maPimItemGetValue(&mArgs, i));
        printAttribute(MA_PIM_FIELD_CONTACT_ADDR, i, getAddressAttributeString);

        for (int j = 0; j < 9; j++)
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
void PimContact::printBirthday()
{
    printf("MA_PIM_FIELD_CONTACT_BIRTHDAY field:");
    mArgs.field = MA_PIM_FIELD_CONTACT_BIRTHDAY;
    checkResultCode(maPimItemGetValue(&mArgs, 0));
    printAttribute(MA_PIM_FIELD_CONTACT_BIRTHDAY, 0,
        getPrefferedAttributeString);
    int birthday = *(int*) mArgs.buf;
    printf("Birthday: %d", birthday);
}

/**
 * Print class field value.
 */
void PimContact::printClass()
{
    printf("MA_PIM_FIELD_CONTACT_CLASS field:");
    mArgs.field = MA_PIM_FIELD_CONTACT_CLASS;
    checkResultCode(maPimItemGetValue(&mArgs, 0));
    printAttribute(MA_PIM_FIELD_CONTACT_CLASS, 0, getPrefferedAttributeString);
    int classFieldValue = *(int*) mArgs.buf;
    MAUtil::String classFieldString = getClassValueString(classFieldValue);
    printf("Class: %s", classFieldString.c_str());
}

/**
 * Print email field values.
 */
void PimContact::printEmail()
{
    printf("MA_PIM_FIELD_CONTACT_EMAIL field:");
    mArgs.field = MA_PIM_FIELD_CONTACT_EMAIL;

    int countValues = maPimItemFieldCount(mItemHandle,
        MA_PIM_FIELD_CONTACT_EMAIL);
    checkResultCode(countValues);

    for (int i = 0; i < countValues; i++)
    {
        checkResultCode(maPimItemGetValue(&mArgs, i));
        printAttribute(MA_PIM_FIELD_CONTACT_EMAIL, i, getEmailAttributeString);
        printf("Email: %S \n \n", (wchar*) mArgs.buf);
    }
}

/**
 * Print formatted address field value.
 */
void PimContact::printFormatedAddress()
{
    printf("MA_PIM_FIELD_CONTACT_FORMATTED_ADDR field:");
    mArgs.field = MA_PIM_FIELD_CONTACT_FORMATTED_ADDR;
    checkResultCode(maPimItemGetValue(&mArgs, 0));
    printAttribute(MA_PIM_FIELD_CONTACT_FORMATTED_ADDR, 0,
        getPrefferedAttributeString);
    printf("Formatted address: %S", (wchar*) mArgs.buf);
}

/**
 * Print formatted name field value.
 */
void PimContact::printFormatedName()
{
    printf("MA_PIM_FIELD_CONTACT_FORMATTED_NAME field:");
    mArgs.field = MA_PIM_FIELD_CONTACT_FORMATTED_NAME;
    checkResultCode(maPimItemGetValue(&mArgs, 0));
    printAttribute(MA_PIM_FIELD_CONTACT_FORMATTED_NAME, 0,
        getPrefferedAttributeString);
    printf("Formatted name: %S", (wchar*) mArgs.buf);
}

/**
 * Print nickname field value.
 */
void PimContact::printNickname()
{
    printf("MA_PIM_FIELD_CONTACT_NICKNAME field:");
    mArgs.field = MA_PIM_FIELD_CONTACT_NICKNAME;

    int countValues = maPimItemFieldCount(mItemHandle,
        MA_PIM_FIELD_CONTACT_NICKNAME);
    checkResultCode(countValues);

    for (int i = 0; i < countValues; i++)
    {
        checkResultCode(maPimItemGetValue(&mArgs, i));
        printAttribute(MA_PIM_FIELD_CONTACT_NICKNAME, i,
            getPrefferedAttributeString);
        printf("Nickname: %S \n \n", (wchar*) mArgs.buf);
    }
}

/**
 * Print note field value.
 */
void PimContact::printNote()
{
    printf("MA_PIM_FIELD_CONTACT_NOTE field:");
    mArgs.field = MA_PIM_FIELD_CONTACT_NOTE;

    int countValues = maPimItemFieldCount(mItemHandle,
        MA_PIM_FIELD_CONTACT_NOTE);
    checkResultCode(countValues);

    for (int i = 0; i < countValues; i++)
    {
        checkResultCode(maPimItemGetValue(&mArgs, i));
        printAttribute(MA_PIM_FIELD_CONTACT_NOTE, i,
            getPrefferedAttributeString);
        printf("Note: %S \n \n", (wchar*) mArgs.buf);
    }
}

/**
 * Print organization field value.
 */
void PimContact::printOrg()
{
    printf("MA_PIM_FIELD_CONTACT_ORG field:");
    mArgs.field = MA_PIM_FIELD_CONTACT_ORG;

    int countValues =
        maPimItemFieldCount(mItemHandle, MA_PIM_FIELD_CONTACT_ORG);
    checkResultCode(countValues);

    for (int i = 0; i < countValues; i++)
    {
        checkResultCode(maPimItemGetValue(&mArgs, i));
        printAttribute(MA_PIM_FIELD_CONTACT_ORG, i, getPrefferedAttributeString);
        printf("Organization: %S \n \n", (wchar*) mArgs.buf);
    }
}

/**
 * Print photo field value.
 */
void PimContact::printPhoto()
{
    printf("MA_PIM_FIELD_CONTACT_PHOTO field:");
    mArgs.field = MA_PIM_FIELD_CONTACT_PHOTO;
    checkResultCode(maPimItemGetValue(&mArgs, 0));
    printAttribute(MA_PIM_FIELD_CONTACT_PHOTO, 0, getPrefferedAttributeString);
    int handle = *(int*) mArgs.buf;
    printf("Photo data handle: %d", handle);
}

/**
 * Print photo URL field value.
 */
void PimContact::printPhotoURL()
{
    printf("MA_PIM_FIELD_CONTACT_PHOTO_URL field:");
    mArgs.field = MA_PIM_FIELD_CONTACT_PHOTO_URL;
    checkResultCode(maPimItemGetValue(&mArgs, 0));
    printAttribute(MA_PIM_FIELD_CONTACT_PHOTO_URL, 0,
        getPrefferedAttributeString);
    printf("Photo URL: %S", (wchar*) mArgs.buf);
}

/**
 * Print key field value.
 */
void PimContact::printPublicKey()
{
    printf("MA_PIM_FIELD_CONTACT_PUBLIC_KEY field:");
    mArgs.field = MA_PIM_FIELD_CONTACT_PUBLIC_KEY;
    checkResultCode(maPimItemGetValue(&mArgs, 0));
    printAttribute(MA_PIM_FIELD_CONTACT_PUBLIC_KEY, 0,
        getPrefferedAttributeString);
    printf("Public key"); // TODO print the binary value?
}

/**
 * Print public key field value.
 */
void PimContact::printPublicKeyString()
{
    printf("MA_PIM_FIELD_CONTACT_PUBLIC_KEY_STRING field:");
    mArgs.field = MA_PIM_FIELD_CONTACT_PUBLIC_KEY_STRING;
    checkResultCode(maPimItemGetValue(&mArgs, 0));
    printAttribute(MA_PIM_FIELD_CONTACT_PUBLIC_KEY_STRING, 0,
        getPrefferedAttributeString);
    printf("Public key string: %S", (wchar*) mArgs.buf);
}

/**
 * Print revision field value.
 */
void PimContact::printRevision()
{
    printf("MA_PIM_FIELD_CONTACT_REVISION field:");
    mArgs.field = MA_PIM_FIELD_CONTACT_REVISION;
    checkResultCode(maPimItemGetValue(&mArgs, 0));
    printAttribute(MA_PIM_FIELD_CONTACT_REVISION, 0,
        getPrefferedAttributeString);
    int revisionDate = *(int*) mArgs.buf;
    printf("Revision date: %d", revisionDate);
}

/**
 * Print phone field values.
 */
void PimContact::printPhone()
{
    printf("MA_PIM_FIELD_CONTACT_TEL field:");
    mArgs.field = MA_PIM_FIELD_CONTACT_TEL;

    int countValues =
        maPimItemFieldCount(mItemHandle, MA_PIM_FIELD_CONTACT_TEL);
    checkResultCode(countValues);

    for (int i = 0; i < countValues; i++)
    {
        checkResultCode(maPimItemGetValue(&mArgs, i));
        printAttribute(MA_PIM_FIELD_CONTACT_TEL, i, getPhoneAttributeString);
        printf("Phone: %S", (wchar*) mArgs.buf);
        printf("\n");
    }
}

/**
 * Print title field value.
 */
void PimContact::printTitle()
{
    printf("MA_PIM_FIELD_CONTACT_TITLE field:");
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
void PimContact::printUID()
{
    printf("MA_PIM_FIELD_CONTACT_UID field:");
    mArgs.field = MA_PIM_FIELD_CONTACT_UID;
    checkResultCode(maPimItemGetValue(&mArgs, 0));
    printAttribute(MA_PIM_FIELD_CONTACT_UID, 0, getPrefferedAttributeString);
    printf("UID: %S", (wchar*) mArgs.buf);
}

/**
 * Print URL field values.
 */
void PimContact::printURL()
{
    printf("MA_PIM_FIELD_CONTACT_URL field:");
    mArgs.field = MA_PIM_FIELD_CONTACT_URL;

    int countValues =
        maPimItemFieldCount(mItemHandle, MA_PIM_FIELD_CONTACT_URL);
    checkResultCode(countValues);

    for (int i = 0; i < countValues; i++)
    {
        checkResultCode(maPimItemGetValue(&mArgs, i));
        printAttribute(MA_PIM_FIELD_CONTACT_URL, i, getWebsiteAttributeString);
        printf("URL: %S", (wchar*) mArgs.buf);
        printf("\n");
    }
}

/**
 * Print instant message field values.
 */
void PimContact::printIM()
{
    printf("MA_PIM_FIELD_CONTACT_IM field:");
    mArgs.field = MA_PIM_FIELD_CONTACT_IM;

    int countValues = maPimItemFieldCount(mItemHandle, MA_PIM_FIELD_CONTACT_IM);
    checkResultCode(countValues);

    for (int i = 0; i < countValues; i++)
    {
        checkResultCode(maPimItemGetValue(&mArgs, i));
        printAttribute(MA_PIM_FIELD_CONTACT_IM, i, getIMAttributeString);
        const wchar* protocolValue = getWCharArrayFromBuf(mArgs.buf, 1);
        const wchar* usernameValue = getWCharArrayFromBuf(mArgs.buf, 0);
        printf("username: %S \n protocol: %S \n", usernameValue, protocolValue);
        printf("\n");
    }
}

void PimContact::printRelation()
{
    printf("MA_PIM_FIELD_CONTACT_RELATION field:");
    mArgs.field = MA_PIM_FIELD_CONTACT_RELATION;

    int countValues = maPimItemFieldCount(mItemHandle,
        MA_PIM_FIELD_CONTACT_RELATION);
    checkResultCode(countValues);

    for (int i = 0; i < countValues; i++)
    {
        checkResultCode(maPimItemGetValue(&mArgs, i));
        printAttribute(MA_PIM_FIELD_CONTACT_RELATION, i,
            getRelationAttributeString);
        printf("Relation: %S", (wchar*) mArgs.buf);
        printf(sFieldSeparator);
    }
}

/**
 * Print nickname field value.
 */
void PimContact::printOrgInfo()
{
    printf("MA_PIM_FIELD_CONTACT_ORG_INFO field:");
    mArgs.field = MA_PIM_FIELD_CONTACT_ORG_INFO;

    int countValues = maPimItemFieldCount(mItemHandle,
        MA_PIM_FIELD_CONTACT_ORG_INFO);
    checkResultCode(countValues);

    for (int i = 0; i < countValues; i++)
    {
        checkResultCode(maPimItemGetValue(&mArgs, i));
        printAttribute(MA_PIM_FIELD_CONTACT_RELATION, i,
            getOrgInfoAttributeString);
        for (int j = 0; j < 6; j++)
        {
            MAUtil::String orgInfoValueIndex = getOrgInfoIndexString(j);
            const wchar* orgInfoValue = getWCharArrayFromBuf(mArgs.buf, j);
            printf("%s %S", orgInfoValueIndex.c_str(), orgInfoValue);
        }
        printf("\n");
    }
}

/**
 * Add value to contact name field.
 */
void PimContact::addContactName()
{
    printf(" add MA_PIM_FIELD_CONTACT_NAME field:");
    mArgs.field = MA_PIM_FIELD_CONTACT_NAME;
    mArgs.bufSize = writeWCharArraysToBuf(mArgs.buf, sContactName, 8);
    for (int i = 0; i < 8; i++)
    {
        MAUtil::String contactNameType = getContactNameIndexString(i);
        const wchar* contactNameValue = sContactName[i];
        printf("%s %S", contactNameType.c_str(), contactNameValue);
    }
    checkResultCode(maPimItemAddValue(&mArgs, MA_PIM_ATTR_PREFERRED));
}

/**
 * Add values to address field.
 */
void PimContact::addAddress()
{
    printf(" add MA_PIM_FIELD_CONTACT_ADDR field:");
    mArgs.field = MA_PIM_FIELD_CONTACT_ADDR;

    // Write home address.
    for (int i = 0; i < 9; i++)
    {
        MAUtil::String addressValueIndex = getAddressIndexString(i);
        const wchar* addressValue = sAddressHome[i];
        printf("%s %S", addressValueIndex.c_str(), addressValue);
    }
    printf("\n");

    mArgs.bufSize = writeWCharArraysToBuf(mArgs.buf, sAddressHome, 8);
    checkResultCode(maPimItemAddValue(&mArgs, MA_PIM_ATTR_ADDR_HOME));

    // Write work address.
    for (int i = 0; i < 8; i++)
    {
        MAUtil::String addressValueIndex = getAddressIndexString(i);
        const wchar* addressValue = sAddressWork[i];
        printf("%s %S", addressValueIndex.c_str(), addressValue);
    }
    mArgs.bufSize = writeWCharArraysToBuf(mArgs.buf, sAddressWork, 8);
    checkResultCode(maPimItemAddValue(&mArgs, MA_PIM_ATTR_ADDR_WORK));
}

/**
 * Add value to birthday field.
 */
void PimContact::addBirthday()
{
    int birthday = 962928000;
    printf(" add MA_PIM_FIELD_CONTACT_BIRTHDAY field:");
    MAUtil::String attribute = getPrefferedAttributeString(
        MA_PIM_ATTR_PREFERRED);
    printf("Attribute: %s", attribute.c_str());
    printf("Birthday: %d", birthday);
    *(int*) mArgs.buf = birthday;
    mArgs.bufSize = 4;
    mArgs.field = MA_PIM_FIELD_CONTACT_BIRTHDAY;
    checkResultCode(maPimItemAddValue(&mArgs, MA_PIM_ATTR_PREFERRED));
}

/**
 * Add value to class field.
 */
void PimContact::addClass()
{
    printf(" add MA_PIM_FIELD_CONTACT_CLASS field:");
    MAUtil::String attribute = getPrefferedAttributeString(
        MA_PIM_ATTR_PREFERRED);
    printf("Attribute: %s", attribute.c_str());
    *(int*) mArgs.buf = MA_PIM_CONTACT_CLASS_PRIVATE;
    mArgs.bufSize = 4;
    mArgs.field = MA_PIM_FIELD_CONTACT_CLASS;
    checkResultCode(maPimItemAddValue(&mArgs, MA_PIM_ATTR_PREFERRED));
}

/**
 * Add values to email field.
 */
void PimContact::addEmail()
{
    MAUtil::String attribute;
    printf(" add MA_PIM_FIELD_CONTACT_EMAIL field:");
    mArgs.field = MA_PIM_FIELD_CONTACT_EMAIL;

    // Write home email.
    mArgs.bufSize = copyWCharArray(mArgs.buf, sEmailHome);
    attribute = getEmailAttributeString(MA_PIM_ATTR_EMAIL_WORK);
    printf("Attribute: %s", attribute.c_str());
    printf("Email: %S", sEmailHome);
    checkResultCode(maPimItemAddValue(&mArgs, MA_PIM_ATTR_EMAIL_WORK));
    printf("\n");

    // Write work email.
    mArgs.bufSize = copyWCharArray(mArgs.buf, sEmailWork);
    attribute = getEmailAttributeString(MA_PIM_ATTR_EMAIL_HOME);
    printf("Attribute: %s", attribute.c_str());
    printf("Email: %S", sEmailWork);
    checkResultCode(maPimItemAddValue(&mArgs, MA_PIM_ATTR_EMAIL_HOME));
}

/**
 * Add value to formatted address field.
 */
void PimContact::addFormatedAddress()
{
    MAUtil::String attribute;
    printf(" add MA_PIM_FIELD_CONTACT_FORMATTED_ADDR field:");
    mArgs.field = MA_PIM_FIELD_CONTACT_FORMATTED_ADDR;
    mArgs.bufSize = copyWCharArray(mArgs.buf, sFormattedAddress);
    attribute = getPrefferedAttributeString(MA_PIM_ATTR_PREFERRED);
    printf("Attribute: %s", attribute.c_str());
    printf("Formatted address: %S", sFormattedAddress);
    checkResultCode(maPimItemAddValue(&mArgs, MA_PIM_ATTR_PREFERRED));
}

/**
 * Add value to formatted name field.
 */
void PimContact::addFormatedName()
{
    MAUtil::String attribute;
    printf(" add MA_PIM_FIELD_CONTACT_FORMATTED_NAME field:");
    mArgs.field = MA_PIM_FIELD_CONTACT_FORMATTED_NAME;
    mArgs.bufSize = copyWCharArray(mArgs.buf, sFormattedName);
    attribute = getPrefferedAttributeString(MA_PIM_ATTR_PREFERRED);
    printf("Attribute: %s", attribute.c_str());
    printf("Formatted address: %S", sFormattedName);
    checkResultCode(maPimItemAddValue(&mArgs, MA_PIM_ATTR_PREFERRED));
}
/**
 * Add value to nickname field.
 */
void PimContact::addNickname()
{
    MAUtil::String attribute;
    printf(" add MA_PIM_FIELD_CONTACT_NICKNAME field:");
    mArgs.field = MA_PIM_FIELD_CONTACT_NICKNAME;
    mArgs.bufSize = copyWCharArray(mArgs.buf, sNickname);
    attribute = getPrefferedAttributeString(MA_PIM_ATTR_PREFERRED);
    printf("Attribute: %s", attribute.c_str());
    printf("Nickname: %S", sNickname);
    checkResultCode(maPimItemAddValue(&mArgs, MA_PIM_ATTR_PREFERRED));
}

/**
 * Add value to note field.
 */
void PimContact::addNote()
{
    MAUtil::String attribute;
    printf(" add MA_PIM_FIELD_CONTACT_NOTE field:");
    mArgs.field = MA_PIM_FIELD_CONTACT_NOTE;
    mArgs.bufSize = copyWCharArray(mArgs.buf, sNote);
    attribute = getPrefferedAttributeString(MA_PIM_ATTR_PREFERRED);
    printf("Attribute: %s", attribute.c_str());
    printf("Note: %S", sNote);
    checkResultCode(maPimItemAddValue(&mArgs, MA_PIM_ATTR_PREFERRED));
}

/**
 * Add value to organization field.
 */
void PimContact::addOrg()
{
    MAUtil::String attribute;
    printf(" add MA_PIM_FIELD_CONTACT_ORG field:");
    mArgs.field = MA_PIM_FIELD_CONTACT_ORG;
    mArgs.bufSize = copyWCharArray(mArgs.buf, sOrg);
    attribute = getPrefferedAttributeString(MA_PIM_ATTR_PREFERRED);
    printf("Attribute: %s", attribute.c_str());
    printf("Org: %S", sOrg);
    checkResultCode(maPimItemAddValue(&mArgs, MA_PIM_ATTR_PREFERRED));
}

/**
 * Add value to photo field.
 */
void PimContact::addPhoto()
{
    printf(" add MA_PIM_FIELD_CONTACT_PHOTO field:");
    MAUtil::String attribute = getPrefferedAttributeString(
        MA_PIM_ATTR_PREFERRED);
    printf("Attribute: %s", attribute.c_str());
    printf("Data handle = %d", RES_IMAGE);
    *(int*) mArgs.buf = RES_IMAGE;
    mArgs.bufSize = 4;
    mArgs.field = MA_PIM_FIELD_CONTACT_PHOTO;
    checkResultCode(maPimItemAddValue(&mArgs, MA_PIM_ATTR_PREFERRED));
}

/**
 * Add value to URL field.
 */
void PimContact::addPhotoURL()
{
    MAUtil::String attribute;
    printf(" add MA_PIM_FIELD_CONTACT_PHOTO_URL field:");
    mArgs.field = MA_PIM_FIELD_CONTACT_PHOTO_URL;
    mArgs.bufSize = copyWCharArray(mArgs.buf, sPhotoURL);
    attribute = getPrefferedAttributeString(MA_PIM_ATTR_PREFERRED);
    printf("Attribute: %s", attribute.c_str());
    printf("Photo URL: %S", sPhotoURL);
    checkResultCode(maPimItemAddValue(&mArgs, MA_PIM_ATTR_PREFERRED));
}

/**
 * Add value to public key field.
 */
void PimContact::addPublicKey()
{
    MAUtil::String attribute;
    printf(" add MA_PIM_FIELD_CONTACT_PUBLIC_KEY field:");
    mArgs.field = MA_PIM_FIELD_CONTACT_PUBLIC_KEY;
    mArgs.bufSize = copyWCharArray(mArgs.buf, sPublicKey);// TODO write binary value.
    attribute = getPrefferedAttributeString(MA_PIM_ATTR_PREFERRED);
    printf("Attribute: %s", attribute.c_str());
    printf("Public key: %S", sPublicKey);
    checkResultCode(maPimItemAddValue(&mArgs, MA_PIM_ATTR_PREFERRED));
}

/**
 * Add value to public key string field.
 */
void PimContact::addPublicKeyString()
{
    MAUtil::String attribute;
    printf(" add MA_PIM_FIELD_CONTACT_PUBLIC_KEY_STRING field:");
    mArgs.field = MA_PIM_FIELD_CONTACT_PUBLIC_KEY_STRING;
    mArgs.bufSize = copyWCharArray(mArgs.buf, sPublicKeyString);
    attribute = getPrefferedAttributeString(MA_PIM_ATTR_PREFERRED);
    printf("Attribute: %s", attribute.c_str());
    printf("Public key string: %S", sPublicKeyString);
    checkResultCode(maPimItemAddValue(&mArgs, MA_PIM_ATTR_PREFERRED));
}

/**
 * Add value to revision field.
 */
void PimContact::addRevision()
{
    printf(" add MA_PIM_FIELD_CONTACT_REVISION field:");
    int revision = 962928000;
    MAUtil::String attribute = getPrefferedAttributeString(
        MA_PIM_ATTR_PREFERRED);
    printf("Attribute: %s", attribute.c_str());
    printf("Revision: %d", revision);
    *(int*) mArgs.buf = revision;
    mArgs.bufSize = 4;
    mArgs.field = MA_PIM_FIELD_CONTACT_REVISION;
    checkResultCode(maPimItemAddValue(&mArgs, MA_PIM_ATTR_PREFERRED));
}

/**
 * Add values to phone field.
 */
void PimContact::addPhone()
{
    MAUtil::String attribute;
    printf(" add MA_PIM_FIELD_CONTACT_TEL field:");

    // Write mobile number.
    mArgs.field = MA_PIM_FIELD_CONTACT_TEL;
    mArgs.bufSize = copyWCharArray(mArgs.buf, sPhoneMobile);
    attribute = getPhoneAttributeString(MA_PIM_ATTR_PHONE_MOBILE);
    printf("Attribute: %s", attribute.c_str());
    printf("Mobile number: %S", sPhoneMobile);
    checkResultCode(maPimItemAddValue(&mArgs, MA_PIM_ATTR_PHONE_MOBILE));
    printf("\n");

    // Write iPhone number.
    mArgs.bufSize = copyWCharArray(mArgs.buf, sPhoneIPhone);
    attribute = getPhoneAttributeString(MA_PIM_ATTR_PHONE_IPHONE);
    printf("Attribute: %s", attribute.c_str());
    printf("iPhone number: %S", sPhoneIPhone);
    checkResultCode(maPimItemAddValue(&mArgs, MA_PIM_ATTR_PHONE_IPHONE));
    printf("\n");

    // Write home number.
    mArgs.bufSize = copyWCharArray(mArgs.buf, sPhoneHome);
    attribute = getPhoneAttributeString(MA_PIM_ATTR_PHONE_HOME);
    printf("Attribute: %s", attribute.c_str());
    printf("Home number: %S", sPhoneHome);
    checkResultCode(maPimItemAddValue(&mArgs, MA_PIM_ATTR_PHONE_HOME));
}

/**
 * Add value to title field.
 */
void PimContact::addTitle()
{
    MAUtil::String attribute;
    printf(" add MA_PIM_FIELD_CONTACT_TITLE field:");
    mArgs.field = MA_PIM_FIELD_CONTACT_TITLE;
    mArgs.bufSize = copyWCharArray(mArgs.buf, sTitle);
    attribute = getPrefferedAttributeString(MA_PIM_ATTR_PREFERRED);
    printf("Attribute: %s", attribute.c_str());
    printf("Public key string: %S", sTitle);
    checkResultCode(maPimItemAddValue(&mArgs, MA_PIM_ATTR_PREFERRED));
}

/**
 * Add value to UID field.
 */
void PimContact::addUID()
{
    MAUtil::String attribute;
    printf(" add MA_PIM_FIELD_CONTACT_UID field:");
    mArgs.field = MA_PIM_FIELD_CONTACT_UID;
    mArgs.bufSize = copyWCharArray(mArgs.buf, sUID);
    attribute = getPrefferedAttributeString(MA_PIM_ATTR_PREFERRED);
    printf("Attribute: %s", attribute.c_str());
    printf("Public key string: %S", sUID);
    checkResultCode(maPimItemAddValue(&mArgs, MA_PIM_ATTR_PREFERRED));
}

/**
 * Add values to URL field.
 */
void PimContact::addURL()
{
    MAUtil::String attribute;
    printf(" add MA_PIM_FIELD_CONTACT_URL field:");
    mArgs.field = MA_PIM_FIELD_CONTACT_URL;

    // Write home URL.
    mArgs.bufSize = copyWCharArray(mArgs.buf, sURLHome);
    attribute = getWebsiteAttributeString(MA_PIM_ATTR_WEBSITE_HOME);
    printf("Attribute: %s", attribute.c_str());
    printf("URL1: %S", sURLHome);
    checkResultCode(maPimItemAddValue(&mArgs, MA_PIM_ATTR_WEBSITE_HOME));
    printf("\n");

    // Write work URL.
    mArgs.bufSize = copyWCharArray(mArgs.buf, sURLWork);
    attribute = getWebsiteAttributeString(MA_PIM_ATTR_WEBSITE_WORK);
    printf("Attribute: %s", attribute.c_str());
    printf("URL2: %S", sURLWork);
    checkResultCode(maPimItemAddValue(&mArgs, MA_PIM_ATTR_WEBSITE_WORK));
}

/**
 * Add value to instant message field.
 */
void PimContact::addIM()
{
    MAUtil::String attribute;
    printf(" add MA_PIM_FIELD_CONTACT_IM field:");
    mArgs.field = MA_PIM_FIELD_CONTACT_IM;

    // Write home IM.
    mArgs.bufSize = writeWCharArraysToBuf(mArgs.buf, sIMHome, 2);
    attribute = getIMAttributeString(MA_PIM_ATTR_IM_HOME);
    printf("Attribute: %s", attribute.c_str());
    const wchar* imUsernameHome = sIMHome[0];
    const wchar* imProtocolHome = sIMHome[1];
    printf("Username: %S", imUsernameHome);
    printf("Protocol: %S", imProtocolHome);
    checkResultCode(maPimItemAddValue(&mArgs, MA_PIM_ATTR_IM_HOME));

    // Write other IM.
    mArgs.bufSize = writeWCharArraysToBuf(mArgs.buf, sIMOther, 2);
    attribute = getIMAttributeString(MA_PIM_ATTR_IM_OTHER);
    printf("Attribute: %s", attribute.c_str());
    const wchar* imUsernameOther = sIMOther[0];
    const wchar* imProtocolOther = sIMOther[1];
    printf("Username: %S", imUsernameOther);
    printf("Protocol: %S", imProtocolOther);
    checkResultCode(maPimItemAddValue(&mArgs, MA_PIM_ATTR_IM_OTHER));
}

/**
 * Add values to relation field.
 */
void PimContact::addRelation()
{
    MAUtil::String attribute;
    printf(" add MA_PIM_FIELD_CONTACT_RELATION field:");
    mArgs.field = MA_PIM_FIELD_CONTACT_RELATION;

    // Write brother relation.
    mArgs.bufSize = copyWCharArray(mArgs.buf, sRelationBrother);
    attribute = getRelationAttributeString(MA_PIM_ATTR_RELATION_BROTHER);
    printf("Attribute: %s", attribute.c_str());
    printf("Relation1: %S", sRelationBrother);
    checkResultCode(maPimItemAddValue(&mArgs, MA_PIM_ATTR_RELATION_BROTHER));
    printf("\n");

    // Write manager relation.
    mArgs.bufSize = copyWCharArray(mArgs.buf, sRelationManager);
    attribute = getRelationAttributeString(MA_PIM_ATTR_RELATION_MANAGER);
    printf("Attribute: %s", attribute.c_str());
    printf("Relation2: %S", sRelationManager);
    checkResultCode(maPimItemAddValue(&mArgs, MA_PIM_ATTR_RELATION_MANAGER));
}

/**
 * Add value to organization field.
 */
void PimContact::addOrgInfo()
{
    MAUtil::String attribute;
    printf(" add MA_PIM_FIELD_CONTACT_ORG_INFO field:");
    mArgs.field = MA_PIM_FIELD_CONTACT_ORG_INFO;

    mArgs.bufSize = writeWCharArraysToBuf(mArgs.buf, sOrgInfo, 6);
    attribute = getPrefferedAttributeString(MA_PIM_ATTR_PREFERRED);
    printf("Attribute: %s", attribute.c_str());
    for (int i = 0; i < 6; i++)
    {
        MAUtil::String orgInfoValueIndex = getOrgInfoIndexString(i);
        const wchar* orgInfoValue = sOrgInfo[i];
        printf("%s %S \n", orgInfoValueIndex.c_str(), orgInfoValue);
    }
    checkResultCode(maPimItemAddValue(&mArgs, MA_PIM_ATTR_PREFERRED));
}
