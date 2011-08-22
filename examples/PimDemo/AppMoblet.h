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

#ifndef APPMOBLET_H_
#define APPMOBLET_H_

#include <MAUtil/Moblet.h>
#include <MAUtil/Vector.h>
#include <conprint.h>
#include <IX_PIM.h>
#include <maapi.h>

#include "PimContact.h"

/**
 * Application's moblet.
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
     * Open pim list and add a new contact.
     */
    virtual void openPimList();

    /**
     * Print the next contact from the pim list.
     */
    virtual void printNextContact();

    /**
     * Add a contact to Address Book.
     */
    virtual void addContact();

    /**
     * Close the pim list.
     */
    virtual void closePimList();

    /**
     * Print show next contact / add new contact options on the screen.
     */
    virtual void printInfo();

private:
    /**
     * Handle to a pim list.
     */
    MAHandle mContactsListHandle;

    /**
     * Handle to a new contact item.
     */
    MAHandle mNewContantHandle;

    /**
     * Store the x axis values from pointer move events.
     */
    MAUtil::Vector<int> mDragValues;

    /**
     * If the flag is set to false is does not handle pointer events.
     * This flag is set to true only when the app is waiting for drag events
     * (no pim action is in progress).
     */
    bool mHandlePointerEvent;
};
#endif /* APPMOBLET_H_ */
