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
 * @file AppMoblet.h
 *
 * AppMoblet class shows how to:
 * - open and close contacts list.
 * - create contact item.
 * - remove contact item from the list.
 */

#ifndef APPMOBLET_H_
#define APPMOBLET_H_

#include <MAUtil/Moblet.h>
#include <MAUtil/Vector.h>

// Forward declarations.
class PIMContact;

/**
 * Application's moblet class.
 * AppMoblet class shows how to:
 * - open and close contacts list.
 * - create contact item.
 * - remove contact item from the list.
 */
class AppMoblet: public MAUtil::Moblet
{
public:
    /**
     * Constructor.
     */
    AppMoblet();

    /**
     * Destructor.
     */
    virtual ~AppMoblet();

    /**
    *  This function is called with a coordinate when a pointer is moved.
    */
    virtual void pointerMoveEvent(MAPoint2d point);

    /**
    *  This function is called with a coordinate when a pointer is released.
    */
    virtual void pointerReleaseEvent(MAPoint2d point);

    /**
     * Called when the screen is touched.
     */
    virtual void pointerPressEvent(MAPoint2d point);

    /**
     * Start working with PIM.
     * Add, modify and remove a PIM contact.
     */
    virtual void startPIM();

    /**
     * Open PIM contacts list.
     */
    virtual void openPIMContactsList();

    /**
     * Print the next contact from the PIM contacts list.
     */
    virtual void printNextContact();

    /**
     * Create a new contact.
     * @return The new created contact, or NULL in case of error.
     * The ownership of the result is passed to the caller.
     */
    virtual PIMContact* createContact();

    /**
     * Modify the address field of a specified contact.
     * Remove the first value from the phone field.
     * @param contact The given contact item.
     */
    virtual void modifyContact(PIMContact* contact);

    /**
     * Remove a specified contact from the list.
     * It also closes the item.
     * @param contact The given contact item.
     */
    virtual void removeContact(PIMContact* contact);

    /**
     * Close the PIM contacts list.
     */
    virtual void closePIMContactsList();

    /**
     * Print show next contact / close pim list options on the screen.
     */
    virtual void printInfo();

private:
    /**
     * Handle to a pim list.
     */
    MAHandle mContactsListHandle;

    /**
     * Store the x axis values from pointer move events.
     */
    MAUtil::Vector<int> mDragValues;

    /**
     * If the flag is set to false is does not handle pointer events.
     * This flag is set to true only when the app is waiting for drag events
     * (no PIM action is in progress).
     */
    bool mHandlePointerEvent;
};

#endif /* APPMOBLET_H_ */
