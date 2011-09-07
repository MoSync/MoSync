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
 * @file AppMoblet.cpp
 *
 * AppMoblet class shows how to:
 * - open and close contacts list.
 * - create contact item.
 * - remove contact item from the list.
 */

// Minimum number of values for drag events.
#define MIN_DRAG_VALUES 10

#include <conprint.h>
#include <IX_PIM.h>
#include <maapi.h>

#include "AppMoblet.h"
#include "Util.h"
#include "PIMContact.h"

/**
 * Constructor.
 */
AppMoblet::AppMoblet() :
    Moblet()
{
    this->startTesting();
    this->testMaPimItemCreate();
    this->testMaPimItemClose();
    this->testMaPimItemRemove();
    this->testMaPimItemSetLabel();
    this->testMaPimItemAddValue();
    this->testMaPimItemSetValue();
    this->testMaPimItemGetValue();
    this->testMaPimItemRemoveValue();
//    this->deleteAllContactsFromAddressBook();
//    this->modifyContact();
}

/**
 * Destructor.
 */
AppMoblet::~AppMoblet()
{

}

/**
 * Start the testing process.
 */
void AppMoblet::startTesting()
{
    printf("\n===========Open contacts list===========");
    MAHandle list = maPimListOpen(MA_PIM_CONTACTS);
    printResultCode(list);

    printf("\n===========Create new contact item===========");
    MAHandle newContactHandle = maPimItemCreate(list);
    printf("newContactHandle = %d", newContactHandle);
    printResultCode(newContactHandle);

    printf("\n===========Add data to contact===========");
    PIMContact* contact = new PIMContact(newContactHandle);
    contact->addDataToContact();

    printf("\n===========Close new contact===========");
    printResultCode(maPimItemClose(newContactHandle));

    printf("\n===========Close contacts list===========");
    printResultCode(maPimListClose(list));

    delete contact;
}

/**
 * Test maPimItemCreate syscall.
 */
void AppMoblet::testMaPimItemCreate()
{
    printf("\n===========Test maPimItemCreate syscall===========");

    printf("\nTest with invalid list handle");
    printResultCode(maPimItemCreate(5));

    printf("\nTest with valid list handle, but list is not open");
    printResultCode(maPimItemCreate(1));

    printf("\nTest with valid list handle and list is opened");
    MAHandle list = maPimListOpen(MA_PIM_CONTACTS);
    printResultCode(list);
    MAHandle item = maPimItemCreate(list);
    printResultCode(item);

    maPimItemRemove(list, item);
    maPimListClose(list);
}

/**
 * Test maPimItemClose syscall.
 */
void AppMoblet::testMaPimItemClose()
{
    printf("\n===========Test maPimItemClose syscall===========");

    printf("\nTest with invalid item handle");
    printResultCode(maPimItemClose(500));

    printf("\nTest with valid item handle, but list is closed");
    MAHandle list = maPimListOpen(MA_PIM_CONTACTS);
    printResultCode(list);
    MAHandle newItem = maPimItemCreate(list);
    maPimListClose(list);
    printResultCode(maPimItemClose(newItem));

    printf("\nTest with valid item handle and list is opened");
    list = maPimListOpen(MA_PIM_CONTACTS);
    printResultCode(list);
    newItem = maPimItemCreate(list);
    printResultCode(newItem);
    printResultCode(maPimItemClose(newItem));

    maPimListClose(list);
    waitForClick();
}

/**
 * Test maPimItemRemove syscall.
 */
void AppMoblet::testMaPimItemRemove()
{
    printf("\n===========Test maPimItemRemove syscall===========");

    printf("\nTest with invalid list handle");
    printResultCode(maPimItemRemove(5, 1));

    printf("\nTest with valid list handle and invalid item handle");
    MAHandle list = maPimListOpen(MA_PIM_CONTACTS);
    printResultCode(list);
    printResultCode(maPimItemRemove(list, 7));

    printf("\nTest with valid list handle and valid item handle");
    MAHandle newItem = maPimItemCreate(list);
    printResultCode(maPimItemRemove(list, newItem));

    printf("\nTest with a removed item");
    printResultCode(maPimItemRemove(list, newItem));

    maPimItemRemove(list, newItem);
    maPimListClose(list);
    waitForClick();
}

/**
 * Test maPimItemSetLabel syscall.
 */
void AppMoblet::testMaPimItemSetLabel()
{
    printf("\n===========Test maPimItemSetLabel syscall===========");
    MAHandle list = maPimListOpen(MA_PIM_CONTACTS);
    printResultCode(list);
    MAHandle item = maPimItemCreate(list);
    printResultCode(item);

    MA_PIM_ARGS args;
    char buf[1024];
    args.buf = buf;
    args.bufSize = 1024;

    printf("\nTest with invalid item handle");
    args.item = 323;
    printResultCode(maPimItemSetLabel(&args, 1));

    printf("\nTest with invalid field");
    args.item = item;
    args.field = 12333;
    printResultCode(maPimItemSetLabel(&args, 1));

    printf("\nTest with unsupported field");
    args.field = MA_PIM_FIELD_CONTACT_CLASS;
    printResultCode(maPimItemSetLabel(&args, 1));

    printf("\nTest with invalid value index and empty field");
    args.field = MA_PIM_FIELD_CONTACT_ADDR;
    printResultCode(maPimItemSetLabel(&args, 1));
    waitForClick();

    printf("\nTest with invalid value index and field is not empty");
    PIMContact* contact = new PIMContact(item);
    contact->addAddress();
    printResultCode(maPimItemSetLabel(&args, 5));
    waitForClick();

    printf("\nTest with a valid field, but value's attribute is not custom");
    printResultCode(maPimItemSetLabel(&args, 0));

    printf("\nTest with a valid field and value's attribute is custom");
    // Write value to buffer.
    args.bufSize = writeWCharArraysToBuf(args.buf, sAddressHome,
        COUNT_ADDRESS_INDICES);
    int index = maPimItemAddValue(&args, MA_PIM_ATTR_ADDR_CUSTOM);

    args.bufSize = copyWCharArray(args.buf, L"My label");
    printResultCode(maPimItemSetLabel(&args, index));

    maPimItemRemove(list, item);
    maPimListClose(list);
    waitForClick();
}

/**
 * Test maPimItemAddValue syscall.
 */
void AppMoblet::testMaPimItemAddValue()
{
    printf("\n=========Test maPimItemAddValue syscall=======");
    MAHandle list = maPimListOpen(MA_PIM_CONTACTS);
    printResultCode(list);
    MAHandle item = maPimItemCreate(list);
    printResultCode(item);

    MA_PIM_ARGS args;
    char buf[1024];
    args.buf = buf;
    args.bufSize = 1024;

    printf("\nTest syscall with invalid item handle");
    args.item = 1234;
    printResultCode(maPimItemAddValue(&args, 0));

    printf("\nTest syscall with invalid field id");
    args.item = item;
    args.field = 696;
    printResultCode(maPimItemAddValue(&args, 0));

    printf("\nTest syscall with unsupported field id");
    args.field = MA_PIM_FIELD_CONTACT_CLASS;
    printResultCode(maPimItemAddValue(&args, 0));

    printf("\nTest syscall for a read only field");
    args.field = MA_PIM_FIELD_CONTACT_REVISION;
    printResultCode(maPimItemAddValue(&args, MA_PIM_ATTR_ADDR_WORK));

    printf("\nTest syscall with invalid attribute");
    args.field = MA_PIM_FIELD_CONTACT_ADDR;
    writeWCharArraysToBuf(args.buf, sAddressHome, 8);
    printResultCode(maPimItemAddValue(&args, 9099));

    printf("\nTest syscall with attribute from another field");
    printResultCode(maPimItemAddValue(&args, MA_PIM_ATTR_RELATION_BROTHER));

    printf("\nTest syscall with invalid buffer size");
    writeWCharArraysToBuf(args.buf, sAddressHome, 8);
    args.bufSize = 10;
    printResultCode(maPimItemAddValue(&args, MA_PIM_ATTR_ADDR_HOME));

    printf("\nTest syscall with valid parameters.");
    args.bufSize = writeWCharArraysToBuf(args.buf, sAddressHome, 8);
    printResultCode(maPimItemAddValue(&args, MA_PIM_ATTR_ADDR_WORK));

    maPimItemRemove(list, item);
    maPimListClose(list);
    waitForClick();
}

/**
 * Test maPimItemSetValue syscall.
 */
void AppMoblet::testMaPimItemSetValue()
{
    printf("\n=========Test maPimItemSetValue syscall=======");
    MAHandle list = maPimListOpen(MA_PIM_CONTACTS);
    printResultCode(list);
    MAHandle item = maPimItemCreate(list);
    printResultCode(item);
    PIMContact* contact = new PIMContact(item);

    MA_PIM_ARGS args;
    char buf[1024];
    args.buf = buf;
    args.bufSize = 1024;

    printf("\nTest syscall with invalid item handle");
    args.item = 1234;
    printResultCode(maPimItemSetValue(&args, 0, 0));

    printf("\nTest syscall with invalid field id");
    args.item = item;
    args.field = 696;
    printResultCode(maPimItemSetValue(&args, 0, 0));

    printf("\nTest syscall with unsupported field id");
    args.field = MA_PIM_FIELD_CONTACT_CLASS;
    printResultCode(maPimItemSetValue(&args, 0, 0));

    printf("\nTest syscall with invalid value index - empty field");
    args.field = MA_PIM_FIELD_CONTACT_ADDR;
    printResultCode(maPimItemSetValue(&args, 0, 0));
    waitForClick();

    printf("\nTest syscall with invalid value index - field not empty");
    contact->addAddress();
    printResultCode(maPimItemSetValue(&args, 15, 0));
    waitForClick();

    printf("\nTest syscall with invalid attribute");
    writeWCharArraysToBuf(args.buf, sAddressHome, 8);
    printResultCode(maPimItemSetValue(&args, 0, 9099));

    printf("\nTest syscall with attribute from another field");
    printResultCode(maPimItemSetValue(&args, 0, MA_PIM_ATTR_RELATION_BROTHER));

    printf("\nTest syscall for a read only field");
    args.field = MA_PIM_FIELD_CONTACT_REVISION;
    printResultCode(maPimItemSetValue(&args, 0, MA_PIM_ATTR_ADDR_WORK));

    printf("\nTest syscall with invalid buffer size.");
    args.field = MA_PIM_FIELD_CONTACT_ADDR;
    writeWCharArraysToBuf(args.buf, sAddressHome, 8);
    args.bufSize = 20;
    printResultCode(maPimItemSetValue(&args, 0, MA_PIM_ATTR_ADDR_WORK));

    printf("\nTest syscall with valid parameters.");
    args.field = MA_PIM_FIELD_CONTACT_ADDR;
    args.bufSize = writeWCharArraysToBuf(args.buf, sAddressHome, 8);
    printResultCode(maPimItemSetValue(&args, 0, MA_PIM_ATTR_ADDR_WORK));

    delete contact;
    maPimItemRemove(list, item);
    maPimListClose(list);
    waitForClick();
}

/**
 * Test maPimItemGetValue syscall.
 */
void AppMoblet::testMaPimItemGetValue()
{
    printf("\n=========Test maPimItemGetValue syscall=======");
    MAHandle list = maPimListOpen(MA_PIM_CONTACTS);
    printResultCode(list);
    MAHandle item = maPimItemCreate(list);
    printResultCode(item);
    PIMContact* contact = new PIMContact(item);

    MA_PIM_ARGS args;
    char buf[1024];
    args.buf = buf;
    args.bufSize = 1024;

    printf("\nTest syscalls with invalid item handle");
    args.item = 1234;
    printResultCode(maPimItemGetValue(&args, 0));

    printf("\nTest syscalls with invalid field id");
    args.item = item;
    args.field = 696;
    printResultCode(maPimItemGetValue(&args, 0));

    printf("\nTest syscall with unsupported field id");
    args.field = MA_PIM_FIELD_CONTACT_CLASS;
    printResultCode(maPimItemGetValue(&args, 0));

    printf("\nTest syscall with invalid value index - empty field");
    args.field = MA_PIM_FIELD_CONTACT_ADDR;
    printResultCode(maPimItemGetValue(&args, 0));
    waitForClick();

    printf("\nTest syscall with invalid value index - field not empty");
    contact->addAddress();
    printResultCode(maPimItemGetValue(&args, 15));
    waitForClick();

    printf("\nTest syscall for a write only field");
    args.field = MA_PIM_FIELD_CONTACT_PHOTO_URL;
    printResultCode(maPimItemGetValue(&args, 0));
    waitForClick();

    printf("\nTest syscall with invalid buf size.");
    args.field = MA_PIM_FIELD_CONTACT_ADDR;
    args.bufSize = 20;
    printResultCode(maPimItemGetValue(&args, 0));

    printf("\nTest syscall with valid parameters.");
    args.bufSize = 1024;
    printResultCode(maPimItemGetValue(&args, 0));

    delete contact;
    maPimItemRemove(list, item);
    maPimListClose(list);
    waitForClick();
}

/**
 * Test maPimItemRemoveValue syscall.
 */
void AppMoblet::testMaPimItemRemoveValue()
{
    printf("\n=========Test maPimItemRemoveValue syscall=======");
    MAHandle list = maPimListOpen(MA_PIM_CONTACTS);
    printResultCode(list);
    MAHandle item = maPimItemCreate(list);
    printResultCode(item);
    PIMContact* contact = new PIMContact(item);

    printf("\nTest syscalls with invalid item handle");
    printResultCode(maPimItemRemoveValue(555, 0, 0));

    printf("\nTest syscalls with invalid field id");
    printResultCode(maPimItemRemoveValue(item, 19191, 0));

    printf("\nTest syscall with unsupported field id");
    printResultCode(maPimItemRemoveValue(item, MA_PIM_FIELD_CONTACT_CLASS, 0));

    printf("\nTest syscall with invalid value index - empty field");
    printResultCode(maPimItemRemoveValue(item, MA_PIM_FIELD_CONTACT_ADDR, 0));
    waitForClick();

    printf("\nTest syscall with invalid value index - field not empty");
    contact->addAddress();
    printResultCode(maPimItemRemoveValue(item, MA_PIM_FIELD_CONTACT_ADDR, 100));
    waitForClick();

    printf("\nTest syscall with valid parameters");
    printResultCode(maPimItemRemoveValue(item, MA_PIM_FIELD_CONTACT_ADDR, 1));

    delete contact;
    maPimItemRemove(list, item);
    maPimListClose(list);
    waitForClick();
}

/**
 * Test maPimItemRemove syscall.
 */
void AppMoblet::deleteAllContactsFromAddressBook()
{
    MAHandle list = maPimListOpen(MA_PIM_CONTACTS);
    while(true)
    {
        MAHandle item = maPimListNext(list);
        if (item != 0)
        {
            maPimItemRemove(list, item);
        }
        else
        {
            break;
        }
    }
    maPimListClose(list);
}

/**
 * Modify an existing contact.
 */
void AppMoblet::modifyContact()
{
    MAHandle list = maPimListOpen(MA_PIM_CONTACTS);
    MAHandle item = maPimListNext(list);
    PIMContact* contact = new PIMContact(item);
    contact->modifyAddressField();
    maPimItemClose(item);
    maPimListClose(list);
}
