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

#include "Util.h"

int writeString(void* buf, const wchar_t* srca)
{
    wchar_t* dst = (wchar_t*) buf;

    wchar_t* src = (wchar_t*) srca;

    while (true)
    {
        *dst = *src;
        dst++;
        if (*src == 0)
            break;
        src++;
    }

    char* buffer = (char*) buf;
    return (char*) dst - buffer;
}

void printStringArray(void* buf)
{
    int num = *(int*) buf;
    char* buffer = (char*) buf;
    const wchar* ptr = (wchar*) (buffer + 4);
    //	printf("number of Strings = %d", num);

    const wchar* t = ptr;
    for (int i = 0; i < num; i++)
    {
        printf("string %d = %S", i, ptr);
        ptr += wcslen(ptr) + 1;
    }

}
const wchar* getStringFromArray(void* buf, const int stringIndex)
{
    int num = *(int*) buf;
    char* buffer = (char*) buf;
    const wchar* ptr = (wchar*) (buffer + 4);

    const wchar* t = ptr;
    for (int i = 0; i < num; i++)
    {
        if (stringIndex == i)
        {
            break;
        }
        ptr += wcslen(ptr) + 1;
    }

    return ptr;
}

/**
 * Print the result code.
 * If resultCode is different then MA_PIM_ERR_NONE prints
 * the error.
 * @param The given result code.
 */
void printResultCode(const int resultCode)
{
    if (0 <= resultCode)
    {
        return;
    }
    MAUtil::String error;
    switch (resultCode)
    {
    case MA_PIM_ERR_UNAVAILABLE:
        error = "MA_PIM_ERR_UNAVAILABLE";
        break;
    case MA_PIM_ERR_NATIVE_TYPE_MISMATCH:
        error = "MA_PIM_ERR_NATIVE_TYPE_MISMATCH";
        break;
    case MA_PIM_ERR_FIELD_UNSUPPORTED:
        error = "MA_PIM_ERR_FIELD_UNSUPPORTED";
        break;
    case MA_PIM_ERR_READ_ONLY:
        error = "MA_PIM_ERR_READ_ONLY";
        break;
    case MA_PIM_ERR_WRITE_ONLY:
        error = "MA_PIM_ERR_WRITE_ONLY";
        break;
    case MA_PIM_ERR_COMBO_UNSUPPORTED:
        error = "MA_PIM_ERR_COMBO_UNSUPPORTED";
        break;
    case MA_PIM_ERR_FIELD_COUNT_MAX:
        error = "MA_PIM_ERR_FIELD_COUNT_MAX";
        break;
    case MA_PIM_ERR_NO_LABEL:
        error = "MA_PIM_ERR_NO_LABEL";
        break;
    case MA_PIM_ERR_CUSTOM_ATTRIBUTE:
        error = "MA_PIM_ERR_CUSTOM_ATTRIBUTE";
        break;
    case MA_PIM_ERR_INVALID_INDEX:
        error = "MA_PIM_ERR_INVALID_INDEX";
        break;

    case MA_PIM_ERR_UNAVAILABLE_LIST:
        error = "MA_PIM_ERR_UNAVAILABLE_LIST";
        break;
    case MA_PIM_ERR_LIST_NOT_OPENED:
        error = "MA_PIM_ERR_LIST_NOT_OPENED";
        break;
    case MA_PIM_ERR_OPERATION_NOT_PERMITTED:
        error = "MA_PIM_ERR_OPERATION_NOT_PERMITTED";
        break;
    case MA_PIM_ERR_INVALID_HANDLE:
        error = "MA_PIM_ERR_INVALID_HANDLE";
        break;
    }
    error = "error: " + error;
    printf(error.c_str());
}

/**
 * Return the string associated with a contact name field index.
 * @param index One of the MA_PIM_CONTACT_NAME constants.
 */
MAUtil::String getContactNameFieldString(const int index)
{
    MAUtil::String text;
    switch (index)
    {
    case MA_PIM_CONTACT_NAME_FAMILY:
        text = "Family name:";
        break;
    case MA_PIM_CONTACT_NAME_GIVEN:
        text = "Given name:";
        break;
    case MA_PIM_CONTACT_NAME_OTHER:
        text = "Other:";
        break;
    case MA_PIM_CONTACT_NAME_PREFIX:
        text = "Prefix:";
        break;
    case MA_PIM_CONTACT_NAME_SUFFIX:
        text = "Suffix:";
        break;
    case MA_PIM_CONTACT_NAME_PHONETIC_FAMILY:
        text = "Phonetic family:";
        break;
    case MA_PIM_CONTACT_NAME_PHONETIC_GIVEN:
        text = "Phonetic given:";
        break;
    case MA_PIM_CONTACT_NAME_PHONETIC_OTHER:
        text = "Phonetic other:";
        break;
    }

    return text;
}
/**
 * Return the string associated with a address field index.
 * @param index One of the MA_PIM_CONTACT_ADDR constants.
 */
MAUtil::String getAddressFieldString(const int index)
{
    MAUtil::String text;
    switch (index)
    {
    case MA_PIM_CONTACT_ADDR_POBOX:
        text = "POBOX:";
        break;
    case MA_PIM_CONTACT_ADDR_EXTRA:
        text = "Extra:";
        break;
    case MA_PIM_CONTACT_ADDR_STREET:
        text = "Street:";
        break;
    case MA_PIM_CONTACT_ADDR_LOCALITY:
        text = "Locality:";
        break;
    case MA_PIM_CONTACT_ADDR_REGION:
        text = "Region:";
        break;
    case MA_PIM_CONTACT_ADDR_POSTALCODE:
        text = "Postal code:";
        break;
    case MA_PIM_CONTACT_ADDR_COUNTRY:
        text = "Country:";
        break;
    case MA_PIM_CONTACT_ADDR_COUNTRY_CODE:
        text = "Country code:";
        break;
    case MA_PIM_CONTACT_ADDR_NEIGHBORHOOD:
        text = "Neighborhood:";
        break;
    }

    return text;
}

/**
 * Return the string associated with a class field index.
 * @param index One of the MA_PIM_CONTACT_CLASS constants.
 */
MAUtil::String getClassFieldString(const int index)
{
    MAUtil::String text;
    switch (index)
    {
    case MA_PIM_CONTACT_CLASS_CONFIDENTIAL:
        text = "Confidential";
        break;
    case MA_PIM_CONTACT_CLASS_PRIVATE:
        text = "Private";
        break;
    case MA_PIM_CONTACT_CLASS_PUBLIC:
        text = "Public";
        break;
    }

    return text;
}

/**
 * Return the string associated with a org info field index.
 * @param index One of the MA_PIM_CONTACT_ORG_INFO constants.
 */
MAUtil::String getOrgInfoFieldString(const int index)
{
    MAUtil::String text;

    switch (index)
    {
    case MA_PIM_CONTACT_ORG_INFO_DEPARTMENT:
        text = "Department:";
        break;
    case MA_PIM_CONTACT_ORG_INFO_JOB_DESCRIPTION:
        text = "Job description:";
        break;
    case MA_PIM_CONTACT_ORG_INFO_SYMBOL:
        text = "Symbol:";
        break;
    case MA_PIM_CONTACT_ORG_INFO_PHONETIC_NAME:
        text = "Phonetic name:";
        break;
    case MA_PIM_CONTACT_ORG_INFO_OFFICE_LOCATION:
        text = "Office location";
        break;
    case MA_PIM_CONTACT_ORG_INFO_PHONETIC_NAME_STYLE:
        text = "Phonetic name style:";
        break;
    }
    return text;
}

/**
 * Return the preferred string attribute.
 * @param MA_PIM_ATTR_PREFERRED.
 */
MAUtil::String getPrefferedAttributeString(const int attribute)
{
    MAUtil::String text = "Attribute is not preferred!";
    if (MA_PIM_ATTR_PREFERRED == attribute)
    {
        text = "preferred";
    }

    return text;
}

/**
 * Return the string associated with a specified address attribute.
 * @param index One of the MA_PIM_ATTR_ADDR constants.
 */
MAUtil::String getAddressAttributeString(const int attribute)
{
    MAUtil::String text;

    switch (attribute)
    {
    case MA_PIM_ATTR_ADDR_HOME:
        text = "home";
        break;
    case MA_PIM_ATTR_ADDR_WORK:
        text = "work";
        break;
    case MA_PIM_ATTR_ADDR_OTHER:
        text = "Other";
        break;
    case MA_PIM_ATTR_ADDR_CUSTOM:
        text = "custom";
        break;
    }

    return text;
}

/**
 * Return the string associated with a specified email attribute.
 * @param index One of the MA_PIM_ATTR_EMAIL constants.
 */
MAUtil::String getEmailAttributeString(const int attribute)
{
    MAUtil::String text;

    switch (attribute)
    {
    case MA_PIM_ATTR_EMAIL_HOME:
        text = "home";
        break;
    case MA_PIM_ATTR_EMAIL_WORK:
        text = "work";
        break;
    case MA_PIM_ATTR_EMAIL_MOBILE:
        text = "mobile";
        break;
    case MA_PIM_ATTR_EMAIL_OTHER:
        text = "other";
        break;
    case MA_PIM_ATTR_EMAIL_CUSTOM:
        text = "custom";
        break;
    }

    return text;
}

/**
 * Return the string associated with a specified formatted address attribute.
 * @param index One of the MA_PIM_ATTR_FORMATTED_ADDR constants.
 */
MAUtil::String getFormattedAddressAttributeString(const int attribute)
{
    MAUtil::String text;

    switch (attribute)
    {
    case MA_PIM_ATTR_FORMATTED_ADDR_HOME:
        text = "home";
        break;
    case MA_PIM_ATTR_FORMATTED_ADDR_WORK:
        text = "work";
        break;
    case MA_PIM_ATTR_FORMATTED_ADDR_OTHER:
        text = "other";
        break;
    case MA_PIM_ATTR_FORMATTED_ADDR_CUSTOM:
        text = "custom";
        break;
    }

    return text;
}

/**
 * Return the string associated with a specified org attribute.
 * @param index One of the MA_PIM_ATTR_ORG_ constants.
 */
MAUtil::String getOrgAttributeString(const int attribute)
{
    MAUtil::String text;

    switch (attribute)
    {
    case MA_PIM_ATTR_ORG_WORK:
        text = "work";
        break;
    case MA_PIM_ATTR_ORG_OTHER:
        text = "other";
        break;
    case MA_PIM_ATTR_ORG_CUSTOM:
        text = "custom";
        break;
    }

    return text;
}

/**
 * Return the string associated with a specified phone attribute.
 * @param index One of the MA_PIM_ATTR_PHONE constants.
 */
MAUtil::String getPhoneAttributeString(const int attribute)
{
    MAUtil::String text;

    switch (attribute)
    {
    case MA_PIM_ATTR_PHONE_HOME:
        text = "home";
        break;
    case MA_PIM_ATTR_PHONE_MOBILE:
        text = "mobile";
        break;
    case MA_PIM_ATTR_PHONE_HOME_FAX:
        text = "home fax";
        break;
    case MA_PIM_ATTR_PHONE_WORK_FAX:
        text = "work fax";
        break;
    case MA_PIM_ATTR_PHONE_PAGER:
        text = "pager";
        break;
    case MA_PIM_ATTR_PHONE_IPHONE:
        text = "iphone";
        break;
    case MA_PIM_ATTR_PHONE_WORK:
        text = "work";
        break;
    case MA_PIM_ATTR_PHONE_CALLBACK:
        text = "callback";
        break;
    case MA_PIM_ATTR_PHONE_CAR:
        text = "car";
        break;
    case MA_PIM_ATTR_PHONE_COMPANY_MAIN:
        text = "company main";
        break;
    case MA_PIM_ATTR_PHONE_ISDN:
        text = "ISDN";
        break;
    case MA_PIM_ATTR_PHONE_OTHER_FAX:
        text = "other fax";
        break;
    case MA_PIM_ATTR_PHONE_RADIO:
        text = "radio";
        break;
    case MA_PIM_ATTR_PHONE_TELEX:
        text = "telex";
        break;
    case MA_PIM_ATTR_PHONE_TTY_TDD:
        text = "TTY TDD";
        break;
    case MA_PIM_ATTR_PHONE_WORK_MOBILE:
        text = "work mobile";
        break;
    case MA_PIM_ATTR_PHONE_WORK_PAGER:
        text = "work pager";
        break;
    case MA_PIM_ATTR_PHONE_ASSISTANT:
        text = "assistant";
        break;
    case MA_PIM_ATTR_PHONE_MMS:
        text = "MMS";
        break;
    case MA_PIM_ATTR_PHONE_OTHER:
        text = "other";
        break;
    case MA_PIM_ATTR_PHONE_CUSTOM:
        text = "custom";
        break;
    }

    return text;
}

/**
 * Return the string associated with a specified title attribute.
 * @param index One of the MA_PIM_ATTR_TITLE constants.
 */
MAUtil::String getTitleAttributeString(const int attribute)
{
    MAUtil::String text;

    switch (attribute)
    {
    case MA_PIM_ATTR_TITLE_WORK:
        text = "work";
        break;
    case MA_PIM_ATTR_TITLE_OTHER:
        text = "other";
        break;
    case MA_PIM_ATTR_TITLE_CUSTOM:
        text = "custom";
        break;
    }

    return text;
}

/**
 * Return the string associated with a specified website attribute.
 * @param index One of the MA_PIM_ATTR_WEBSITE constants.
 */
MAUtil::String getWebsiteAttributeString(const int attribute)
{
    MAUtil::String text;

    switch (attribute)
    {
    case MA_PIM_ATTR_WEBSITE_HOMEPAGE:
        text = "homepage";
        break;
    case MA_PIM_ATTR_WEBSITE_BLOG:
        text = "blog";
        break;
    case MA_PIM_ATTR_WEBSITE_PROFILE:
        text = "profile";
        break;
    case MA_PIM_ATTR_WEBSITE_HOME:
        text = "home";
        break;
    case MA_PIM_ATTR_WEBSITE_WORK:
        text = "work";
        break;
    case MA_PIM_ATTR_WEBSITE_FTP:
        text = "FTP";
        break;
    case MA_PIM_ATTR_WEBSITE_OTHER:
        text = "other";
        break;
    case MA_PIM_ATTR_WEBSITE_CUSTOM:
        text = "custom";
        break;
    }

    return text;
}

/**
 * Return the string associated with a specified IM attribute.
 * @param index One of the MA_PIM_ATTR_IM_ constants.
 */
MAUtil::String getIMAttributeString(const int attribute)
{
    MAUtil::String text;

    switch (attribute)
    {
    case MA_PIM_ATTR_IM_HOME:
        text = "home";
        break;
    case MA_PIM_ATTR_IM_WORK:
        text = "work";
        break;
    case MA_PIM_ATTR_IM_OTHER:
        text = "other";
        break;
    case MA_PIM_ATTR_IM_CUSTOM:
         text = "custom";
         break;
    }

    return text;
}

/**
 * Return the string associated with a specified relation attribute.
 * @param index One of the MA_PIM_ATTR_RELATION constants.
 */
MAUtil::String getRelationAttributeString(const int attribute)
{
    MAUtil::String text;

    switch (attribute)
    {
    case MA_PIM_ATTR_RELATION_MOTHER:
        text = "mother";
        break;
    case MA_PIM_ATTR_RELATION_FATHER:
        text = "father";
        break;
    case MA_PIM_ATTR_RELATION_PARENT:
        text = "parent";
        break;
    case MA_PIM_ATTR_RELATION_SISTER:
         text = "sister";
         break;
    case MA_PIM_ATTR_RELATION_BROTHER:
        text = "brother";
        break;
    case MA_PIM_ATTR_RELATION_CHILD:
        text = "child";
        break;
    case MA_PIM_ATTR_RELATION_FRIEND:
        text = "friend";
        break;
    case MA_PIM_ATTR_RELATION_SPOUSE:
         text = "spouse";
         break;
    case MA_PIM_ATTR_RELATION_PARTNER:
        text = "partner";
        break;
    case MA_PIM_ATTR_RELATION_MANAGER:
        text = "manager";
        break;
    case MA_PIM_ATTR_RELATION_ASSISTANT:
        text = "assistant";
        break;
    case MA_PIM_ATTR_RELATION_DOMESTIC_PARTNER:
         text = "domestic partner";
         break;
    case MA_PIM_ATTR_RELATION_REFERRED_BY:
        text = "refered by";
        break;
    case MA_PIM_ATTR_RELATION_RELATIVE:
        text = "relative";
        break;
    case MA_PIM_ATTR_RELATION_CUSTOM:
        text = "custom";
        break;
    }

    return text;
}

/**
 * Return the string associated with a specified org info attribute.
 * @param index One of the MA_PIM_ATTR_ORG_INFO constants.
 */
MAUtil::String getOrgInfoAttributeString(const int attribute)
{
    MAUtil::String text;

    switch (attribute)
    {
    case MA_PIM_ATTR_ORG_INFO_WORK:
        text = "work";
        break;
    case MA_PIM_ATTR_ORG_INFO_OTHER:
        text = "other";
        break;
    case MA_PIM_ATTR_ORG_INFO_CUSTOM:
         text = "custom";
         break;
    }

    return text;
}
