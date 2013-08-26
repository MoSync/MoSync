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
