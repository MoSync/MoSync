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
     * Test maPimListOpen, maPimListClose and maPimListNext syscalls.
     */
    virtual void testListSyscalls();

    /**
     * Test maPimFieldType syscall.
     */
    virtual void testMaPimFieldType();


    /**
     * Open PIM list.
     * @param listType One of the next constants:
     * - MA_PIM_CONTACTS
     * - MA_PIM_EVENTS
     * - MA_PIM_TODOS
     * @return A handle to the list, or one of the MA_PIM_ERR constants otherwise.
     */
    virtual MAHandle openPIMList(const int listType);

    /**
     * Close a PIM list.
     * @param list A handle to the given list.
     */
    virtual void closePIMList(MAHandle list);

private:
    /**
     * Handle to a pim list.
     */
    MAHandle mContactsListHandle;
};

#endif /* APPMOBLET_H_ */
