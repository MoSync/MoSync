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

#include <conprint.h>
#include <wchar.h>
#include <ma.h>
#include <maassert.h>
#include <mawstring.h>
#include <IX_PIM.h>
#include <MAUtil/String.h>

static const wchar_t* sContactName[] = { L"Iusco", L"Ioan", L"Bogdan", L"Dl", L"jr",
									     L"Iusco", L"Ioan", L"Bogdan"};

static const wchar_t* sAddress1[] = { L"", L"", L"Luceafarului", L"Cluj-Napoca", L"Cluj",
									     L"400434", L"Romania", L"RO", L"Zorilor"};
static const wchar_t* sAddress2[] = { L"", L"", L"21 Decembrie", L"Cluj-Napoca", L"Cluj",
									     L"400123", L"Romania", L"RO", L"Centru"};

static const wchar_t* sOrgInfo[] = { L"Dev-Cluj", L"", L"", L"", L"", L""};

static const wchar_t* sEmail1 = L"bogdan.iusco@mosync.com\0";
static const wchar_t* sEmail2 = L"grigaci@yahoo.com\0";

static const wchar_t* sFormattedAddress = L"formatted addr\0";
static const wchar_t* sFormattedName = L"formatted name\0";
static const wchar_t* sPhotoURL = L"http://t2.gstatic.com/images?q=tbn:ANd9GcSrJS1-i7rBQrEb8HwuonyWrIrpS_xGNWvQage1ePefwi0nL55b\0";
static const wchar_t* sPublicKey = L"\0";
static const wchar_t* sPublicKeyString = L"Public key\0";

static const wchar_t* sPhone1 = L"+40767345769\0";
static const wchar_t* sPhone2 = L"+40742123456\0";
static const wchar_t* sPhone3 = L"+40364768453\0";

static const wchar_t* sNickname = L"grigaci\0";
static const wchar_t* sNote = L"note\0";
static const wchar_t* sOrg = L"MoSync\0";
static const wchar_t* sTitle = L"developer\0";
static const wchar_t* sUID = L"contact UID\0";

static const wchar_t* sURL1 = L"www.mypage.com\0";
static const wchar_t* sURL2 = L"www.mosync.com\0";

static const wchar_t* sIM1[] = {L"myusername", L"Yahoo"};
static const wchar_t* sIM2[] = {L"myusername", L"AIM"};

static const wchar_t* sRelation1 = L"my brother";
static const wchar_t* sRelation2 = L"my manager";

int writeString(void* buf, const wchar_t* srca);

void printStringArray(void* buf);

const wchar* getStringFromArray(void* buf, const int stringIndex);

/**
 * Print the result code.
 * If resultCode is different then MA_PIM_ERR_NONE prints
 * the error.
 * @param The given result code.
 */
void printResultCode(const int resultCode);

/**
 * Return the string associated with a contact name field index.
 * @param index One of the MA_PIM_CONTACT_NAME constants.
 */
MAUtil::String getContactNameFieldString(const int index);

/**
 * Return the string associated with a address field index.
 * @param index One of the MA_PIM_CONTACT_ADDR constants.
 */
MAUtil::String getAddressFieldString(const int index);

/**
 * Return the string associated with a class field index.
 * @param index One of the MA_PIM_CONTACT_CLASS constants.
 */
MAUtil::String getClassFieldString(const int index);

/**
 * Return the string associated with a org info field index.
 * @param index One of the MA_PIM_CONTACT_ORG_INFO constants.
 */
MAUtil::String getOrgInfoFieldString(const int index);

/**
 * Return the preferred string attribute.
 * @param MA_PIM_ATTR_PREFERRED.
 */
MAUtil::String getPrefferedAttributeString(const int attribute);

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

// return the number of bytes written
template<int NUM>
int writeStringArray(void* buf, const wchar_t* srca[NUM])
{
    char* buffer = (char*) buf;
	*(int*)buffer = NUM;
	wchar_t* dst = (wchar_t*)(buffer + 4);
	for(int i=0; i<NUM; i++) {
		const wchar_t* src = srca[i];
		while(true) {
			*dst = *src;
			dst++;
			if(*src == 0)
				break;
			src++;
		}
	}
	return (char*)dst - buffer;
};




#endif /* UTIL_H_ */
