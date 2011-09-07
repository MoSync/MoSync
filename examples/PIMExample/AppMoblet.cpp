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
 * @file AppMoblet.cpp
 * @author Bogdan Iusco
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
    Moblet(),
    mHandlePointerEvent(false)
{
    this->startPIM();
}

/**
 * Destructor.
 */
AppMoblet::~AppMoblet()
{

}

/**
 *  This function is called with a coordinate when a pointer is moved.
 */
void AppMoblet::pointerMoveEvent(MAPoint2d point)
{
    if (mHandlePointerEvent)
    {
        mDragValues.add(point.x);
    }
}

/**
 *  This function is called with a coordinate when a pointer is released.
 */
void AppMoblet::pointerReleaseEvent(MAPoint2d point)
{
    // Handle this event only if no PIM action is on progress.
    if (mHandlePointerEvent)
    {
        // Check if there are enough drag events for analysis.
        if (MIN_DRAG_VALUES > mDragValues.size())
        {
            return;
        }

        // Check if drag is from left to right of from right to left.
        bool dragFromLeftToRight = true;
        bool dragFromRightToLeft = true;
        int previousValue = mDragValues[0];
        for (int i = 1; (i < mDragValues.size()) && (dragFromLeftToRight
            || dragFromRightToLeft); i++)
        {
            int currentValue = mDragValues[i];
            if (previousValue > currentValue)
            {
                dragFromLeftToRight = false;
            }
            if (previousValue < currentValue)
            {
                dragFromRightToLeft = false;
            }

            previousValue = currentValue;
        }

        // If drag is from left to right print the next contact, otherwise
        // close the PIM contacts list.
        if (dragFromLeftToRight)
        {
            this->printNextContact();
        }
        else if (dragFromRightToLeft)
        {
            this->closePIMContactsList();
        }
    }
}

/**
 * Called when the screen is touched.
 */
void AppMoblet::pointerPressEvent(MAPoint2d point)
{
    if (mHandlePointerEvent)
    {
        mDragValues.clear();
    }
}

/**
 * Method called when a key is pressed.
 */
void AppMoblet::keyPressEvent(int keyCode, int nativeCode)
{
    // Close the application if the back key is pressed.
    if(MAK_BACK == keyCode)
    {
        close();
    }
}

/**
 * Start working with PIM.
 * Add, modify and remove a PIM contact.
 */
void AppMoblet::startPIM()
{
    // Open PIM contacts list.
    this->openPIMContactsList();

    // Create new contact and add data into fields.
    PIMContact* newContact = this->createContact(mContactsListHandle);
    if (NULL == newContact)
    {
        return;
    }
    newContact->addDataToContact();

    // Modify the new contact.
    this->modifyContact(newContact);

    // Remove the new contact from the list and delete the object.
    this->removeContact(newContact);
    delete newContact;

    // Print options on the screen and wait for drag events.
    this->printInfo();
}

/**
 * Open PIM contacts list.
 */
void AppMoblet::openPIMContactsList()
{
    printf("\n==============Open contacts list===============");

    // Get a handle to the list of contacts.
    mContactsListHandle = maPimListOpen(MA_PIM_CONTACTS);
    checkResultCode(mContactsListHandle);
}

/**
 * Print the next contact from the PIM contacts list.
 */
void AppMoblet::printNextContact()
{
    // Ignore all drag events handled by this moblet.
    mHandlePointerEvent = false;

    printf("\n=============Printing next contact=============\n");

    // Get a hold on a contact item.
    MAHandle pimItemHandle = maPimListNext(mContactsListHandle);

    // Check if the new contact handle is valid.
    if (0 < pimItemHandle)
    {
        // Create new contact item and print it on the screen.
        PIMContact* item = new PIMContact(pimItemHandle);
        item->printContact();
        delete item;
    }
    else if (pimItemHandle == 0)
    {
        // Reached at the end of the list.
        printf("The contacts list does not contain more items.");
    }
    else
    {
        // Print the error on the screen.
        printResultCode(pimItemHandle);
    }

    // Print options on the screen and wait for drag events.
    this->printInfo();
}

/**
 * Create a new contact.
 * @param listHandle Handle to a PIM list.
 * @return The new created contact, or NULL in case of error.
 * The ownership of the result is passed to the caller.
 */
PIMContact* AppMoblet::createContact(MAHandle listHandle)
{
    // Get a handle to the new created contact.
    printf("\n==============Create new contact==============\n");
    MAHandle newContantHandle = maPimItemCreate(listHandle);

    // Check for errors.
    if (0 > newContantHandle)
    {
        printf("Cann't create new contact.");
        printResultCode(newContantHandle);
        return NULL;
    }

    // Create and return the new contact.
    PIMContact* newContact = new PIMContact(newContantHandle);
    return newContact;
}

/**
 * Modify the address field of a specified contact.
 * Remove the first value from the phone field.
 * @param contact The given contact item.
 */
void AppMoblet::modifyContact(PIMContact* contact)
{
    contact->modifyAddressField();

    printf("\n====Remove field value from the new contact====\n");
    contact->removeFieldValue(MA_PIM_FIELD_CONTACT_TEL, 0);
}

/**
 * Remove a specified contact from the list.
 * It also closes the item.
 * @param contact The given contact item.
 */
void AppMoblet::removeContact(PIMContact* contact)
{
    printf("\n===========Remove the new contact=============\n");
    int resultCode = maPimItemRemove(mContactsListHandle, contact->getHandle());
    printResultCode(resultCode);
}

/**
 * Close the PIM contacts list.
 */
void AppMoblet::closePIMContactsList()
{
    printf("\n ==============Close the contacts list=========\n");
    printResultCode(maPimListClose(mContactsListHandle));

    // Start again working with PIM>
    this->startPIM();
}

/**
 * Print show next contact / close pim list options on the screen.
 */
void AppMoblet::printInfo()
{
    printf("\n==============================================");
    printf("Drag from left to right to show the next \n contact");
    printf("Drag from right to left to close the contacts \n list");
    printf("==============================================\n\n");

    // Allow this moblet to handle pointer events.
    mHandlePointerEvent = true;
}
