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
 * @file PIMContact.h
 *
 * PIMContact class shows how to:
 * - read, modify and delete field values.
 * - set field value attributes and labels(custom attributes).
 * - remove contact item from the list.
 * - count field values.
 */

#ifndef PIMCONTACT_H_
#define PIMCONTACT_H_

#include <IX_PIM.h>
#include <maapi.h>
#include <MAUtil/String.h>

/**
 * PIMContact class shows how to:
 * - read, modify and delete field values.
 * - set field value attributes and labels(custom attributes).
 * - remove contact item from the list.
 * - count field values.
 */
class PIMContact
{
public:
    /**
     * Constructor.
     * @param pimItemHandle Handle to a pim contact item.
     */
    PIMContact(MAHandle pimItemHandle);

    /**
     * Destructor.
     */
    ~PIMContact();

    /**
     * Print all contact's field values.
     */
    void printContact();

    /**
     * Add data to the contact.
     * All fields must be empty.
     */
    void addDataToContact();

    /**
     * Modify the first value of the address field.
     * Set a custom label for that value.
     */
    virtual void modifyAddressField();

    /**
     * Remove a specified field value.
     * @param fieldID One of the MA_PIM_FIELD_CONTACT constants.
     * @param index Value's index in the given field.
     */
    virtual void removeFieldValue(const int fieldID, const int index);

    /**
     * Get contact handle.
     * @return The contact's handle.
     */
    virtual MAHandle getHandle() const;

private:
    /**
     * Wait for a pointer released event.
     */
    void waitForClick();

    /**
     * Print the attribute for a given index field value.
     * If the attribute is custom will print the label also.
     * @param field One of the MA_PIM_FIELD constants.
     * @param index The field's value index.
     * Must be >= 0 and < maPimItemFieldCount().
     * @param pointerToFunc A pointer to a function that returns the string
     * associated with the attribute.
     */
    void printAttribute(
        const int field,
        const int index,
        MAUtil::String (*pointerToFunc)(const int));

    /**
     * Print contact name field value.
     */
    void printContactName();

    /**
     * Print address field values.
     */
    void printAddress();

    /**
     * Print birthday field value.
     */
    void printBirthday();

    /**
     * Print email field values.
     */
    void printEmail();

    /**
     * Print formatted address field value.
     */
    void printFormattedAddress();

    /**
     * Print nickname field value.
     */
    void printNickname();

    /**
     * Print note field value.
     */
    void printNote();

    /**
     * Print organization field value.
     */
    void printOrg();

    /**
     * Print photo field value.
     */
    void printPhoto();

    /**
     * Print key field value.
     */
    void printPublicKey();

    /**
     * Print public key field value.
     */
    void printPublicKeyString();

    /**
     * Print revision field value.
     */
    void printRevision();

    /**
     * Print phone field values.
     */
    void printPhone();

    /**
     * Print title field value.
     */
    void printTitle();

    /**
     * Print UID field value.
     */
    void printUID();

    /**
     * Print URL field values.
     */
    void printURL();

    /**
     * Print instant message field values.
     */
    void printIM();

    /**
     * Print relation field values.
     */
    void printRelation();

    /**
     * Print organization info field value.
     */
    void printOrgInfo();

    /**
     * Add value to contact name field.
     */
    void addContactName();

    /**
     * Add values to address field.
     */
    void addAddress();

    /**
     * Add value to birthday field.
     */
    void addBirthday();

    /**
     * Add values to email field.
     */
    void addEmail();

    /**
     * Add value to formatted address field.
     */
    void addFormatedAddress();

    /**
     * Add value to nickname field.
     */
    void addNickname();

    /**
     * Add value to note field.
     */
    void addNote();

    /**
     * Add value to organization field.
     */
    void addOrg();

    /**
     * Add value to photo field.
     */
    void addPhoto();

    /**
     * Add value to URL field.
     */
    void addPhotoURL();

    /**
     * Add values to phone field.
     */
    void addPhone();

    /**
     * Add value to title field.
     */
    void addTitle();

    /**
     * Add values to URL field.
     */
    void addURL();

    /**
     * Add value to instant message field.
     */
    void addIM();

    /**
     * Add values to relation field.
     */
    void addRelation();

    /**
     * Add value to organization field.
     */
    void addOrgInfo();

private:
    /**
     * Handle to a contact item.
     */
    MAHandle mItemHandle;

    /**
     * Arguments for getting and setting field values.
     */
    MA_PIM_ARGS mArgs;
};

#endif /* PIMCONTACT_H_ */
