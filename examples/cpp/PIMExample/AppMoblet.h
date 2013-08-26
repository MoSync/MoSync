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
 * @author Bogdan Iusco
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
	* Method called when a key is pressed.
	*/
	void keyPressEvent(int keyCode, int nativeCode);

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
