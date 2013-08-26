/* Copyright 2013 David Axmark

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

	http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
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
    int fieldType = maPimFieldType(list, MA_PIM_FIELD_CONTACT_NAME);
    printResultCode(fieldType);
    printf("Field Type = %d", fieldType);
}

/**
 * Open PIM list.
 * @param listType One of the next constants:
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
