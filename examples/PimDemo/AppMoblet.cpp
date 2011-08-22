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

// Minimum number of values for drag events.
#define MIN_DRAG_VALUES 10

#include "AppMoblet.h"
#include "Util.h"

/**
 * Constructor.
 */
AppMoblet::AppMoblet() :
    Moblet(), mHandlePointerEvent(true)
{
    this->openPimList();
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
    if (mHandlePointerEvent)
    {
        if (mDragValues.size() < MIN_DRAG_VALUES)
        {
            return;
        }

        bool dragFromLeftToRight = true;
        bool dragFromRightToLeft = true;
        int previousValue = mDragValues[0];
        for (int i = 0; (i < mDragValues.size()) && (dragFromLeftToRight
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
        if (dragFromLeftToRight)
        {
            this->printNextContact();
        }
        else if (dragFromRightToLeft)
        {
            this->closePimList();
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
 * Open pim list and add a new contact.
 */
void AppMoblet::openPimList()
{
    printf("==============Open Contacts List============== \n");

    // Get a handle to the list of contacts.
    mContactsListHandle = maPimListOpen(MA_PIM_CONTACTS);
    printf("Contacts list handle = %d", mContactsListHandle);

    this->addContact();
    this->printInfo();
}

/**
 * Print the next contact from the pim list.
 */
void AppMoblet::printNextContact()
{
    printf("next contact");

    // get a hold on a contact item
    MAHandle pimItemHandle = maPimListNext(mContactsListHandle);

    // Check if the new contact handle is valid.
    if (pimItemHandle > 0)
    {
        mHandlePointerEvent = false;
        PimContact* item = new PimContact(pimItemHandle);
        item->printContact();
        mHandlePointerEvent = true;
    }
    else if (pimItemHandle == 0)
    {
        printf("The contacts list does not contain more items.");
    }
    else
    {

        printResultCode(pimItemHandle);
    }

    this->printInfo();
}

/**
 * Add a contact to Address Book.
 */
void AppMoblet::addContact()
{
    printf("==============Create new contact=============\n");
    mNewContantHandle = maPimItemCreate(MA_PIM_CONTACTS);
    printf("New Contact handle = %d", mNewContantHandle);
    printf("==============Add data to contact============\n \n");

    PimContact* newContact = new PimContact(mNewContantHandle);
    newContact->addDataToContact();
}

/**
 * Close the pim list.
 */
void AppMoblet::closePimList()
{
    printf("\n ==============Close the new contact===========\n");
    printResultCode(maPimItemClose(mNewContantHandle));

    printf("\n ==============Close the contacts list=========\n");
    printResultCode(maPimListClose(mContactsListHandle));

    // Open again the pim list.
    this->openPimList();
}

/**
 * Print show next contact / add new contact options on the screen.
 */
void AppMoblet::printInfo()
{
    printf("==============================================");
    printf("Drag from left to right to show the next contact");
    printf("Drag from right to left to close the contacts list");
    printf("==============================================");
}
