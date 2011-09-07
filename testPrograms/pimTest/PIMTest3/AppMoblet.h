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
     * Start the testing process.
     */
    virtual void startTesting();

    /**
     * Test maPimItemCreate syscall.
     */
    virtual void testMaPimItemCreate();

    /**
     * Test maPimItemClose syscall.
     */
    virtual void testMaPimItemClose();

    /**
     * Test maPimItemRemove syscall.
     */
    virtual void testMaPimItemRemove();

    /**
     * Test maPimItemSetLabel syscall.
     */
    virtual void testMaPimItemSetLabel();

    /**
     * Test maPimItemAddValue syscall.
     */
    virtual void testMaPimItemAddValue();

    /**
     * Test maPimItemSetValue syscall.
     */
    virtual void testMaPimItemSetValue();

    /**
     * Test maPimItemGetValue syscall.
     */
    virtual void testMaPimItemGetValue();

    /**
     * Test maPimItemRemoveValue syscall.
     */
    virtual void testMaPimItemRemoveValue();

    /**
     * Test maPimItemRemove syscall.
     */
    virtual void deleteAllContactsFromAddressBook();

    /**
     * Modify an existing contact.
     */
    virtual void modifyContact();

private:
    /**
     * Handle to a pim list.
     */
    MAHandle mContactsListHandle;
};

#endif /* APPMOBLET_H_ */
