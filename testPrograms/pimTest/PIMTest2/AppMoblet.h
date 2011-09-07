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
     * Test maPimItemCount, maPimItemGetField, maPimItemFieldCount
     * maPimItemGetValue, maPimItemGetAttributes and maPimItemGetLabel syscall.
     * @param item Handle to a pim item.
     */
    virtual void testSyscalls(MAHandle item);

    /**
     * Test maPimItemCount, maPimItemGetField, maPimItemFieldCount
     * maPimItemGetValue, maPimItemGetAttributes and maPimItemGetLabel syscall
     * with random values.
     * @param item Handle to a pim item.
     */
    virtual void testSyscallsWithRandomValues(MAHandle item);

private:
    /**
     * Handle to a pim list.
     */
    MAHandle mContactsListHandle;
};

#endif /* APPMOBLET_H_ */
