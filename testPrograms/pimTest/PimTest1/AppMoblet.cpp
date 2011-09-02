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

/**
 * Constructor.
 */
AppMoblet::AppMoblet() :
    Moblet()
{
    this->testListSyscalls();
    this->testMaPimFieldType();
}

/**
 * Destructor.
 */
AppMoblet::~AppMoblet()
{

}

/**
 * Test maPimListOpen, maPimListClose and maPimListNext syscalls.
 */
void AppMoblet::testListSyscalls()
{
    printf("\n============maPimListOpen syscall==============");
    // Open PIM contacts list.
    MAHandle firstContactsList = this->openPIMList(MA_PIM_CONTACTS);

    // Open again contacts list.
    MAHandle secondContactsList = this->openPIMList(MA_PIM_CONTACTS);

    // Open events list.
    MAHandle eventsList = this->openPIMList(MA_PIM_EVENTS);

    // Open a random list.
    MAHandle randomList = this->openPIMList(21);
    waitForClick();

    // Close the lists.
    printf("\n============maPimListClose syscall=============");
    this->closePIMList(firstContactsList);
    this->closePIMList(firstContactsList);
    this->closePIMList(secondContactsList);
    this->closePIMList(eventsList);
    this->closePIMList(randomList);
    waitForClick();

    // Test maPimListNext syscall.
    printf("\n============maPimListNext syscall==============");
    MAHandle contactsListHandle = maPimListOpen(MA_PIM_CONTACTS);
    printf("contacts list handle = %d", contactsListHandle);
    while(true)
    {
        MAHandle contactItemHandle = maPimListNext(contactsListHandle);
        if (0 < contactItemHandle)
        {
            printf("contact item handle = %d", contactItemHandle);
        }
        else if (0 == contactItemHandle)
        {
            printf("reached at the end of the list");
            break;
        }
        else
        {
            printResultCode(contactItemHandle);
            break;
        }
    }

    printf("\ntest maPimListNext with a invalid handle.");
    printResultCode(maPimListNext(4));

    this->closePIMList(contactsListHandle);
    waitForClick();
}

/**
 * Test maPimFieldType syscall.
 */
void AppMoblet::testMaPimFieldType()
{
    printf("\n============maPimFieldType syscall==============");
    // Open PIM contacts list.
    MAHandle list = this->openPIMList(MA_PIM_CONTACTS);

    // Test syscall with invalid list handle
    printf("Test syscall with invalid list handle");
    printResultCode(maPimFieldType(5, 10));

    // Test syscall with valid list handle, invalid field.
    printf("Test syscall with valid list handle, invalid field.");
    printResultCode(maPimFieldType(list, 10));

    // Test syscall with valid list handle, unsupported field.
    printf("Test syscall with valid list handle, unsupported field.");
    printResultCode(maPimFieldType(list, MA_PIM_FIELD_CONTACT_CLASS));

    // Test syscall with valid list handle and valid field.
    printf("Test syscall with valid list handle and valid field.");
    printResultCode(maPimFieldType(list, MA_PIM_FIELD_CONTACT_NAME));
}

/**
 * Open PIM list.
 * @param listType One of the next contants:
 * - MA_PIM_CONTACTS
 * - MA_PIM_EVENTS
 * - MA_PIM_TODOS
 * @return A handle to the list, or one of the MA_PIM_ERR constants otherwise.
 */
MAHandle AppMoblet::openPIMList(const int listType)
{
    printf("Open list type = %d", listType);

    // Get a handle to the list of contacts.
    MAHandle handle = maPimListOpen(listType);
    printResultCode(handle);

    printf("\n");
    return handle;
}

/**
 * Close a PIM list.
 * @param list A handle to the given list.
 */
void AppMoblet::closePIMList(MAHandle list)
{
    printf("Close list handle = %d", list);
    printResultCode(maPimListClose(list));
    printf("\n");
}
