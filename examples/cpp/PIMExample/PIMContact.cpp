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
 * @file PIMContact.cpp
 * @author Bogdan Iusco
 *
 * PIMContact class shows how to:
 * - read, modify and delete field values.
 * - set field value attributes and labels(custom attributes).
 * - remove contact item from the list.
 * - count field values.
 */

// Buffer size used for reading and writing field values.
#define BUF_SIZE 512

#include <mavsprintf.h>
#include <MAUtil/util.h>
#include <conprint.h>

#include "PIMContact.h"
#include "Util.h"
#include "MAHeaders.h"

/**
 * Value for MA_PIM_FIELD_CONTACT_NAME field.
 */
static const wchar_t* sContactName[] =
{
	L"O'Conner", // MA_PIM_CONTACT_NAME_FAMILY
	L"Brian", // MA_PIM_CONTACT_NAME_GIVEN
	L"Robert", // MA_PIM_CONTACT_NAME_OTHER
	L"Mr", // MA_PIM_CONTACT_NAME_PREFIX
	L"Jr", // MA_PIM_CONTACT_NAME_SUFFIX
	L"", // MA_PIM_CONTACT_NAME_PHONETIC_FAMILY
	L"", // MA_PIM_CONTACT_NAME_PHONETIC_GIVEN
	L"", // MA_PIM_CONTACT_NAME_PHONETIC_OTHER
	L"", // MA_PIM_CONTACT_NAME_DISPLAY
};

/**
 * Values for MA_PIM_FIELD_CONTACT_ADDR field.
 */
static const wchar_t* sAddressWork[] =
{
	L"", // MA_PIM_CONTACT_ADDR_POBOX
	L"", // MA_PIM_CONTACT_ADDR_EXTRA
	L"Saltmatargatan 8A", // MA_PIM_CONTACT_ADDR_STREET
	L"Stockholm", // MA_PIM_CONTACT_ADDR_LOCALITY
	L"", // MA_PIM_CONTACT_ADDR_REGION
	L"113 59", // MA_PIM_CONTACT_ADDR_POSTALCODE
	L"Sweden", // MA_PIM_CONTACT_ADDR_COUNTRY
	L"", // MA_PIM_CONTACT_ADDR_NEIGHBORHOOD
};
static const wchar_t* sAddressHome[] =
{
	L"", // MA_PIM_CONTACT_ADDR_POBOX
	L"", // MA_PIM_CONTACT_ADDR_EXTRA
	L"21 Decembrie 70", // MA_PIM_CONTACT_ADDR_STREET
	L"Cluj-Napoca", // MA_PIM_CONTACT_ADDR_LOCALITY
	L"Cluj", // MA_PIM_CONTACT_ADDR_REGION
	L"400123", // MA_PIM_CONTACT_ADDR_POSTALCODE
	L"Romania", // MA_PIM_CONTACT_ADDR_COUNTRY
	L"Marasti", // MA_PIM_CONTACT_ADDR_NEIGHBORHOOD
};

static const wchar_t* sAddressModified[] =
{
	L"", // MA_PIM_CONTACT_ADDR_POBOX
	L"", // MA_PIM_CONTACT_ADDR_EXTRA
	L"Dorobantilor 14", // MA_PIM_CONTACT_ADDR_STREET
	L"Cluj-Napoca", // MA_PIM_CONTACT_ADDR_LOCALITY
	L"Cluj", // MA_PIM_CONTACT_ADDR_REGION
	L"400117", // MA_PIM_CONTACT_ADDR_POSTALCODE
	L"Romania", // MA_PIM_CONTACT_ADDR_COUNTRY
	L"Marasti", // MA_PIM_CONTACT_ADDR_NEIGHBORHOOD
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
	L"Dev-SDK", // MA_PIM_CONTACT_ORG_INFO_DEPARTMENT
	L"", // MA_PIM_CONTACT_ORG_INFO_JOB_DESCRIPTION
	L"", // MA_PIM_CONTACT_ORG_INFO_SYMBOL
	L"", // MA_PIM_CONTACT_ORG_INFO_PHONETIC_NAME
	L"", // MA_PIM_CONTACT_ORG_INFO_OFFICE_LOCATION
	L"", // MA_PIM_CONTACT_ORG_INFO_PHONETIC_NAME_STYLE
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
 * - MA_PIM_CONTACT_NAME_DISPLAY
 */
static const int COUNT_NAME_INDICES = 9;

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
 * Print all contact's field values.
 */
void PIMContact::printContact()
{
	mArgs.bufSize = BUF_SIZE;

    printf(sFieldSeparator);

	printContactName();
	waitForClick();

	printAddress();
	waitForClick();

	printBirthday();
	waitForClick();

	printEmail();
	waitForClick();

	printFormattedAddress();
	waitForClick();

	printNickname();
	waitForClick();

	printNote();
	waitForClick();

	printOrg();
	waitForClick();

	printPhoto();
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
 * All fields must be empty.
 */
void PIMContact::addDataToContact()
{
	printf("\n==============Add data to contact==============\n");
	printf(sFieldSeparator);

	addContactName();
	waitForClick();

	addAddress();
	waitForClick();

	addBirthday();
	waitForClick();

	addEmail();
	waitForClick();

	addNickname();
	waitForClick();

	addNote();
	waitForClick();

	addOrg();
	waitForClick();

	// If you want to set a photo URL call addPhotoURL method instead of
	// addPhoto method.
	addPhoto();
	waitForClick();

	addPhone();
	waitForClick();

	addTitle();
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
 * Modify the first value of the address field.
 * Set a custom label for that value.
 */
void PIMContact::modifyAddressField()
{
	printf("==============Modify address field=============\n\n");
	mArgs.field = MA_PIM_FIELD_CONTACT_ADDR;

	// Print new value on the screen.
	for (int i = 0; i < COUNT_ADDRESS_INDICES; i++)
	{
		MAUtil::String addressValueIndex = getAddressIndexString(i);
		const wchar* addressValue = sAddressModified[i];
		if (*addressValue != 0)
		{
			printf("%s %S", addressValueIndex.c_str(), addressValue);
		}
	}
	printf("\n");

	// Write the address into the buffer.
	mArgs.bufSize = writeWCharArraysToBuf(
	mArgs.buf,
	sAddressModified,
	COUNT_ADDRESS_INDICES);

	// Set the value for the address field at position 0.
	// Use MA_PIM_ATTR_ADDR_CUSTOM so we can set the label later.
	checkResultCode(maPimItemSetValue(&mArgs, 0, MA_PIM_ATTR_ADDR_CUSTOM));

	// Set custom attribute(label) for the above address.
	printf("\n Set label for the this address.");
	printf("Label: %S", sAddressLabel);

	// Write label value into buffer.
	mArgs.bufSize = copyWCharArray(mArgs.buf, sAddressLabel);

	// Set label value for address field at position 0.
	checkResultCode(maPimItemSetLabel(&mArgs, 0));
	waitForClick();
}

/**
 * Remove a specified field value.
 * @param fieldID One of the MA_PIM_FIELD_CONTACT constants.
 * @param index Value's index in the given field.
 */
void PIMContact::removeFieldValue(const int fieldID, const int index)
{
	checkResultCode(maPimItemRemoveValue(
		mItemHandle,
		fieldID,
		index));
	waitForClick();
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
 * Wait for a pointer released event.
 */
void PIMContact::waitForClick()
{
	printf("\nTap the screen to continue......\n");
	printf(sFieldSeparator);
	MAEvent event;

	while (true)
	{
		// Wait for a event.
		maWait(-1);

		// Get the event.
		maGetEvent(&event);

		// Check if the event is pointer released.
		if (EVENT_TYPE_POINTER_RELEASED == event.type)
		{
			break;
		}
		else if (EVENT_TYPE_CLOSE == event.type)
		{
			// Close the app.
			maExit(0);
		}
	}
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
	MAUtil::String attributeString = "";

	// Check if the attribute contains a primary value combined with an
	// attribute.
	if (attribute >> 16)
	{
		attributeString = "primary ";

		// Remove primary value from attribute.
		attribute = (attribute & 0xFFFF);
	}

	// Print the string associated with the attribute.
	attributeString += pointerToFunc(attribute);
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
	printf("Contact name field:\n\n");
	mArgs.field = MA_PIM_FIELD_CONTACT_NAME;

	// Get value from name field at position 0.
	checkResultCode(maPimItemGetValue(&mArgs, 0));

	// Print data on the screen.
	for (int i = 0; i < COUNT_NAME_INDICES; i++)
	{
		MAUtil::String contactNameType = getContactNameIndexString(i);
		const wchar* contactNameValue = getWCharArrayFromBuf(mArgs.buf, mArgs.bufSize, i);
		if (contactNameValue && *contactNameValue != 0)
		{
			printf("%s %S", contactNameType.c_str(), contactNameValue);
		}
	}
}

/**
 * Print the address field values.
 */
void PIMContact::printAddress()
{
	printf("Address field:\n\n");
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
			const wchar* addressValue = getWCharArrayFromBuf(mArgs.buf, mArgs.bufSize, j);
			if (addressValue && *addressValue != 0)
			{
				printf("%s %S", addressValueIndex.c_str(), addressValue);
				printf("\n");
			}
		}
	}
}

/**
 * Print birthday field value.
 */
void PIMContact::printBirthday()
{
	printf("Birthday field:\n\n");
	mArgs.field = MA_PIM_FIELD_CONTACT_BIRTHDAY;

	checkIfFieldIsSupported(mItemHandle, MA_PIM_FIELD_CONTACT_BIRTHDAY);

	// Get value from birthday field at position 0.
	checkResultCode(maPimItemGetValue(&mArgs, 0));

	// Print value on screen(the value it's in seconds - UNIX time).
	int birthday = *(int*) mArgs.buf;
	printf("Birthday: %d (in seconds)", birthday);
}

/**
 * Print email field values.
 */
void PIMContact::printEmail()
{
	printf("Email field:\n\n");
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
	printf("Formatted address field:\n\n");
	mArgs.field = MA_PIM_FIELD_CONTACT_FORMATTED_ADDR;

	checkIfFieldIsSupported(mItemHandle, MA_PIM_FIELD_CONTACT_FORMATTED_ADDR);

	// Get the value from formatted address at position 0.
	checkResultCode(maPimItemGetValue(&mArgs, 0));
	printf("Formatted address: %S", (wchar*) mArgs.buf);
}

/**
 * Print nickname field value.
 */
void PIMContact::printNickname()
{
	printf("Nickname field:");
	mArgs.field = MA_PIM_FIELD_CONTACT_NICKNAME;

	checkIfFieldIsSupported(mItemHandle, MA_PIM_FIELD_CONTACT_NICKNAME);

	// Get value from nickname field at position 0.
	checkResultCode(maPimItemGetValue(&mArgs, 0));
	printf("Nickname: %S", (wchar*) mArgs.buf);
}

/**
 * Print note field value.
 */
void PIMContact::printNote()
{
	printf("Note field:\n\n");
	mArgs.field = MA_PIM_FIELD_CONTACT_NOTE;

	checkIfFieldIsSupported(mItemHandle, MA_PIM_FIELD_CONTACT_NOTE);

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
	printf("Photo field:\n\n");
	mArgs.field = MA_PIM_FIELD_CONTACT_PHOTO;

	checkIfFieldIsSupported(mItemHandle, MA_PIM_FIELD_CONTACT_PHOTO);

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
	printf("Public key field:\n\n");
	mArgs.field = MA_PIM_FIELD_CONTACT_PUBLIC_KEY;

	checkIfFieldIsSupported(mItemHandle, MA_PIM_FIELD_CONTACT_PUBLIC_KEY);

	checkResultCode(maPimItemGetValue(&mArgs, 0));
}

/**
 * Print public key field value.
 */
void PIMContact::printPublicKeyString()
{
	printf("Public key string field:\n\n");
	mArgs.field = MA_PIM_FIELD_CONTACT_PUBLIC_KEY_STRING;

	checkIfFieldIsSupported(mItemHandle, MA_PIM_FIELD_CONTACT_PUBLIC_KEY_STRING);

	checkResultCode(maPimItemGetValue(&mArgs, 0));
	printf("Public key string: %S", (wchar*) mArgs.buf);
}

/**
 * Print revision field value.
 */
void PIMContact::printRevision()
{
	printf("Revision field:\n\n");
	mArgs.field = MA_PIM_FIELD_CONTACT_REVISION;

	checkIfFieldIsSupported(mItemHandle, MA_PIM_FIELD_CONTACT_REVISION);

	// Get value from revision field at position 0;
	// Returned value is a date in seconds(UNIX time).
	checkResultCode(maPimItemGetValue(&mArgs, 0));

	// Print date on the screen.
	int revisionDate = *(int*) mArgs.buf;
	printf("Revision date: %d (in seconds)", revisionDate);
}

/**
 * Print phone field values.
 */
void PIMContact::printPhone()
{
	printf("Phone field:\n\n");
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
	printf("Title field:\n\n");
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
	printf("UID field:\n\n");
	mArgs.field = MA_PIM_FIELD_CONTACT_UID;

	checkIfFieldIsSupported(mItemHandle, MA_PIM_FIELD_CONTACT_UID);

	checkResultCode(maPimItemGetValue(&mArgs, 0));
	printf("UID: %S", (wchar*) mArgs.buf);
}

/**
 * Print URL field values.
 */
void PIMContact::printURL()
{
	printf("URL field:\n\n");
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
	printf("Instant message field:\n\n");
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
			mArgs.buf, mArgs.bufSize,
			MA_PIM_CONTACT_IM_PROTOCOL);
		const wchar* usernameValue = getWCharArrayFromBuf(
			mArgs.buf, mArgs.bufSize,
			MA_PIM_CONTACT_IM_USERNAME);
		printf("Username: %S \nProtocol: %S \n\n",
			usernameValue,
			protocolValue);
	}
}

void PIMContact::printRelation()
{
	printf("Relation field:\n\n");
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
	printf("Organization info field:\n\n");
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
			const wchar* orgInfoValue = getWCharArrayFromBuf(mArgs.buf, mArgs.bufSize, j);
			if (*orgInfoValue != 0)
			{
				printf("%s %S\n\n", orgInfoValueIndex.c_str(), orgInfoValue);
			}
		}
	}
}

/**
 * Add value to contact name field.
 */
void PIMContact::addContactName()
{
	printf("Add value to contact name field. \n\n");
	mArgs.field = MA_PIM_FIELD_CONTACT_NAME;

	// Write value to buffer.
	mArgs.bufSize = writeWCharArraysToBuf(
		mArgs.buf,
		sContactName,
		COUNT_NAME_INDICES);

	// Print value on the screen.
	for (int i = 0; i < COUNT_NAME_INDICES; i++)
	{
		MAUtil::String contactNameType = getContactNameIndexString(i);
		const wchar* contactNameValue = sContactName[i];
		if (*contactNameValue != 0)
		{
			printf("%s %S", contactNameType.c_str(), contactNameValue);
		}
	}

	// Add value to field.
	checkResultCode(maPimItemAddValue(&mArgs, MA_PIM_ATTRPREFERRED));
}

/**
 * Add values to address field.
 */
void PIMContact::addAddress()
{
	printf("Add values to address field\n\n");
	mArgs.field = MA_PIM_FIELD_CONTACT_ADDR;

	// Print value for home address.
	for (int i = 0; i < COUNT_ADDRESS_INDICES; i++)
	{
		MAUtil::String addressValueIndex = getAddressIndexString(i);
		const wchar* addressValue = sAddressHome[i];
		if (*addressValue != 0)
		{
			printf("%s %S", addressValueIndex.c_str(), addressValue);
		}
	}
	printf("\n");

	// Write value to buffer.
	mArgs.bufSize = writeWCharArraysToBuf(
		mArgs.buf,
		sAddressHome,
		COUNT_ADDRESS_INDICES);

	// Add value to address field.
	checkResultCode(maPimItemAddValue(&mArgs, MA_PIM_ATTR_ADDR_HOME));

	// Print value for work address.
	for (int i = 0; i < COUNT_ADDRESS_INDICES; i++)
	{
		MAUtil::String addressValueIndex = getAddressIndexString(i);
		const wchar* addressValue = sAddressWork[i];
		if (*addressValue != 0)
		{
			printf("%s %S", addressValueIndex.c_str(), addressValue);
		}
	}

	// Write value to buffer.
	mArgs.bufSize = writeWCharArraysToBuf(
		mArgs.buf,
		sAddressWork,
		COUNT_ADDRESS_INDICES);

	// Add value to address field.
	checkResultCode(maPimItemAddValue(&mArgs, MA_PIM_ATTR_ADDR_WORK));
}

/**
 * Add value to birthday field.
 */
void PIMContact::addBirthday()
{
	printf("Add value to birthday field.\n\n");
	mArgs.field = MA_PIM_FIELD_CONTACT_BIRTHDAY;
	int birthday = 962928000;
	mArgs.bufSize = sizeof(int);

	// Print value on the screen.
	// The date is in seconds(UNIX time).
	printf("Birthday: %d (in seconds)", birthday);
	*(int*) mArgs.buf = birthday;

	// Add value to birthday field.
	checkResultCode(maPimItemAddValue(&mArgs, MA_PIM_ATTRPREFERRED));
}

/**
 * Add values to email field.
 */
void PIMContact::addEmail()
{
	printf("Add values to email field.\n\n");
	MAUtil::String attribute;
	mArgs.field = MA_PIM_FIELD_CONTACT_EMAIL;

	// Print home email value and attribute for on the screen.
	attribute = getEmailAttributeString(MA_PIM_ATTR_EMAIL_HOME);
	printf("Attribute: %s", attribute.c_str());
	printf("Email: %S", sEmailHome);

	// Write home email value to buffer.
	mArgs.bufSize = copyWCharArray(mArgs.buf, sEmailHome);

	// Add value to the email field.
	checkResultCode(maPimItemAddValue(&mArgs, MA_PIM_ATTR_EMAIL_HOME));
	printf("\n");

	// Print work email value and attribute for on the screen.
	attribute = getEmailAttributeString(MA_PIM_ATTR_EMAIL_WORK);
	printf("Attribute: %s", attribute.c_str());
	printf("Email: %S", sEmailWork);

	// Write work email value to buffer.
	mArgs.bufSize = copyWCharArray(mArgs.buf, sEmailWork);

	// Add value to the email field.
	checkResultCode(maPimItemAddValue(&mArgs, MA_PIM_ATTR_EMAIL_WORK));
}

/**
 * Add value to nickname field.
 */
void PIMContact::addNickname()
{
	printf("Add value to nickname field.\n\n");
	mArgs.field = MA_PIM_FIELD_CONTACT_NICKNAME;

	// Print value on the screen.
	printf("Nickname: %S", sNickname);

	// Write nickname value to buffer.
	mArgs.bufSize = copyWCharArray(mArgs.buf, sNickname);

	// Add value to nickname field.
	checkResultCode(maPimItemAddValue(&mArgs, MA_PIM_ATTRPREFERRED));
}

/**
 * Add value to note field.
 */
void PIMContact::addNote()
{
	printf("Add value to note field.\n\n");
	mArgs.field = MA_PIM_FIELD_CONTACT_NOTE;

	// Print value on the screen.
	printf("Note: %S", sNote);

	// Write note value to buffer.
	mArgs.bufSize = copyWCharArray(mArgs.buf, sNote);

	// Add value to note field.
	checkResultCode(maPimItemAddValue(&mArgs, MA_PIM_ATTRPREFERRED));
}

/**
 * Add value to organization field.
 */
void PIMContact::addOrg()
{
	printf("Add value to organization field.\n\n");
	mArgs.field = MA_PIM_FIELD_CONTACT_ORG;

	// Print value on the screen.
	printf("Organization: %S", sOrg);

	// Write organization value to buffer.
	mArgs.bufSize = copyWCharArray(mArgs.buf, sOrg);

	// Add value to organization field.
	checkResultCode(maPimItemAddValue(&mArgs, MA_PIM_ATTRPREFERRED | MA_PIM_ATTR_ORG_WORK));
}

/**
 * Add value to photo field.
 */
void PIMContact::addPhoto()
{
	printf("Add value to photo field.\n\n");
	mArgs.field = MA_PIM_FIELD_CONTACT_PHOTO;
	mArgs.bufSize = 4;

	// Print value to screen.
	printf("Data handle = %d", RES_IMAGE);

	// Write the data handle of the image to buffer.
	*(int*) mArgs.buf = RES_IMAGE;

	// Add value to photo field.
	checkResultCode(maPimItemAddValue(&mArgs, MA_PIM_ATTRPREFERRED));
}

/**
 * Add value to URL field.
 */
void PIMContact::addPhotoURL()
{
	printf("Add value to photo URL field.\n\n");
	mArgs.field = MA_PIM_FIELD_CONTACT_PHOTO_URL;

	// Write value on the screen.
	printf("Photo URL: %S", sPhotoURL);

	// Write photo URL value to buffer.
	mArgs.bufSize = copyWCharArray(mArgs.buf, sPhotoURL);

	// Add value to photo URL field.
	checkResultCode(maPimItemAddValue(&mArgs, MA_PIM_ATTRPREFERRED));
}

/**
 * Add values to phone field.
 */
void PIMContact::addPhone()
{
	printf("Add values to phone field field.\n\n");
	mArgs.field = MA_PIM_FIELD_CONTACT_TEL;
	MAUtil::String attribute;

	// Print mobile number and attribute on the screen.
	attribute = getPhoneAttributeString(MA_PIM_ATTR_PHONE_MOBILE);
	printf("Attribute: %s", attribute.c_str());
	printf("Mobile number: %S", sPhoneMobile);

	// Write mobile number value to buffer.
	mArgs.bufSize = copyWCharArray(mArgs.buf, sPhoneMobile);

	// Add value to phone field.
	checkResultCode(maPimItemAddValue(&mArgs, MA_PIM_ATTR_PHONE_MOBILE));
	printf("\n");

	// Print iPhone number and attribute on the screen.
	attribute = getPhoneAttributeString(MA_PIM_ATTR_PHONE_IPHONE);
	printf("Attribute: %s", attribute.c_str());
	printf("iPhone number: %S", sPhoneIPhone);

	// Write iPhone number value to buffer.
	mArgs.bufSize = copyWCharArray(mArgs.buf, sPhoneIPhone);

	// Add value to the phone field.
	checkResultCode(maPimItemAddValue(&mArgs, MA_PIM_ATTR_PHONE_IPHONE));
	printf("\n");

	// Print home number and attribute on the screen.
	attribute = getPhoneAttributeString(MA_PIM_ATTR_PHONE_HOME);
	printf("Attribute: %s", attribute.c_str());
	printf("Home number: %S", sPhoneHome);

	// Write home number value to buffer.
	mArgs.bufSize = copyWCharArray(mArgs.buf, sPhoneHome);

	// Add value to the phone field.
	checkResultCode(maPimItemAddValue(&mArgs, MA_PIM_ATTR_PHONE_HOME));
}

/**
 * Add value to title field.
 */
void PIMContact::addTitle()
{
	printf("Add value to title field.\n\n");
	mArgs.field = MA_PIM_FIELD_CONTACT_TITLE;

	// Print value on the screen.
	printf("Public key string: %S", sTitle);

	// Write title value to buffer.
	mArgs.bufSize = copyWCharArray(mArgs.buf, sTitle);

	// Add value to title field.
	checkResultCode(maPimItemAddValue(&mArgs, MA_PIM_ATTRPREFERRED | MA_PIM_ATTR_TITLE_WORK));
}

/**
 * Add values to URL field.
 */
void PIMContact::addURL()
{
	MAUtil::String attribute;
	printf("Add values to URL field.\n\n");
	mArgs.field = MA_PIM_FIELD_CONTACT_URL;

	// Print home URL value and attribute.
	attribute = getWebsiteAttributeString(MA_PIM_ATTR_WEBSITE_HOME);
	printf("Attribute: %s", attribute.c_str());
	printf("URL1: %S", sURLHome);

	// Write home URL value to buffer.
	mArgs.bufSize = copyWCharArray(mArgs.buf, sURLHome);

	// Add value to URL field.
	checkResultCode(maPimItemAddValue(&mArgs, MA_PIM_ATTR_WEBSITE_HOME));
	printf("\n");

	// Print work URL value and attribute.
	attribute = getWebsiteAttributeString(MA_PIM_ATTR_WEBSITE_WORK);
	printf("Attribute: %s", attribute.c_str());
	printf("URL2: %S", sURLWork);

	// Write work URL value to buffer.
	mArgs.bufSize = copyWCharArray(mArgs.buf, sURLWork);

	// Add value to URL field.
	checkResultCode(maPimItemAddValue(&mArgs, MA_PIM_ATTR_WEBSITE_WORK));
}

/**
 * Add value to instant message field.
 */
void PIMContact::addIM()
{
	MAUtil::String attribute;
	printf("Add values to instant message field.\n\n");
	mArgs.field = MA_PIM_FIELD_CONTACT_IM;

	// Print home IM value and attribute.
	attribute = getIMAttributeString(MA_PIM_ATTR_IM_HOME);
	printf("Attribute: %s", attribute.c_str());
	const wchar* imUsernameHome = sIMHome[0];
	const wchar* imProtocolHome = sIMHome[1];
	printf("Username: %S", imUsernameHome);
	printf("Protocol: %S", imProtocolHome);

	// Write home IM value to buffer.
	mArgs.bufSize = writeWCharArraysToBuf(mArgs.buf, sIMHome, COUNT_IM_INDICES);

	// Add value to im field.
	checkResultCode(maPimItemAddValue(&mArgs, MA_PIM_ATTR_IM_HOME));
	printf("\n");

	// Print other IM value and attribute.
	attribute = getIMAttributeString(MA_PIM_ATTR_IM_OTHER);
	printf("Attribute: %s", attribute.c_str());
	const wchar* imUsernameOther = sIMOther[0];
	const wchar* imProtocolOther = sIMOther[1];
	printf("Username: %S", imUsernameOther);
	printf("Protocol: %S", imProtocolOther);

	// Write other IM value to buffer.
	mArgs.bufSize = writeWCharArraysToBuf(
		mArgs.buf,
		sIMOther,
		COUNT_IM_INDICES);

	// Add value to im field.
	checkResultCode(maPimItemAddValue(&mArgs, MA_PIM_ATTR_IM_OTHER));
}

/**
 * Add values to relation field.
 */
void PIMContact::addRelation()
{
	printf("Add values to relation field.\n\n");
	mArgs.field = MA_PIM_FIELD_CONTACT_RELATION;
	MAUtil::String attribute;

	// Print brother relation value and attribute.
	attribute = getRelationAttributeString(MA_PIM_ATTR_RELATION_BROTHER);
	printf("Attribute: %s", attribute.c_str());
	printf("Relation1: %S", sRelationBrother);

	// Write brother value to buffer.
	mArgs.bufSize = copyWCharArray(mArgs.buf, sRelationBrother);

	// Add value to relation field.
	checkResultCode(maPimItemAddValue(&mArgs, MA_PIM_ATTR_RELATION_BROTHER));
	printf("\n");

	// Print manager relation value and attribute.
	attribute = getRelationAttributeString(MA_PIM_ATTR_RELATION_MANAGER);
	printf("Attribute: %s", attribute.c_str());
	printf("Relation2: %S", sRelationManager);

	// Write manager value to buffer.
	mArgs.bufSize = copyWCharArray(mArgs.buf, sRelationManager);

	// Add value to relation field.
	checkResultCode(maPimItemAddValue(&mArgs, MA_PIM_ATTR_RELATION_MANAGER));
}

/**
 * Add value to organization field.
 */
void PIMContact::addOrgInfo()
{
	printf("Add value to organization info field.\n\n");
	mArgs.field = MA_PIM_FIELD_CONTACT_ORG_INFO;

	// Print organization value on the screen.
	for (int i = 0; i < COUNT_ORG_INFO_INDICES; i++)
	{
		MAUtil::String orgInfoValueIndex = getOrgInfoIndexString(i);
		const wchar* orgInfoValue = sOrgInfo[i];
		if (*orgInfoValue != 0)
		{
			printf("%s %S", orgInfoValueIndex.c_str(), orgInfoValue);
		}
	}

	// Write organization info value to buffer.
	mArgs.bufSize = writeWCharArraysToBuf(
		mArgs.buf,
		sOrgInfo,
		COUNT_ORG_INFO_INDICES);

	// Add value to organization info field.
	checkResultCode(maPimItemAddValue(&mArgs, MA_PIM_ATTRPREFERRED | MA_PIM_ATTR_ORG_INFO_WORK));
}
