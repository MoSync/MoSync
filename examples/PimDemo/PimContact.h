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

#ifndef PIMCONTACT_H_
#define PIMCONTACT_H_

#include <IX_PIM.h>
#include <maapi.h>
#include <MAUtil/String.h>

class PimContact
{
public:
    /**
     * Constructor.
     * @param pimItemHandle Handle to a pim contact item.
     */
    PimContact(MAHandle pimItemHandle);

    /**
     * Destructor.
     */
    ~PimContact();

    /**
     * Print the contact's fields.
     */
    void printContact();

    /**
     * Add data to the contact.
     * The contact's fields must be empty.
     */
    void addDataToContact();

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
     * Print contact name field values.
     */
    void printContactNameField();

    /**
     * Print address field values.
     */
    void printAddress();

    /**
     * Print birthday field value.
     */
    void printBirthday();

    /**
     * Print class field value.
     */
    void printClass();

    /**
     * Print email field values.
     */
    void printEmail();

    /**
     * Print formatted address field value.
     */
    void printFormatedAddress();

    /**
     * Print formatted name field value.
     */
    void printFormatedName();

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
     * Print photo URL field value.
     */
    void printPhotoURL();

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
     * Print nickname field value.
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
     * Add value to class field.
     */
    void addClass();

    /**
     * Add values to email field.
     */
    void addEmail();

    /**
     * Add value to formatted address field.
     */
    void addFormatedAddress();

    /**
     * Add value to formatted name field.
     */
    void addFormatedName();

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
     * Add value to public key field.
     */
    void addPublicKey();

    /**
     * Add value to public key string field.
     */
    void addPublicKeyString();

    /**
     * Add value to revision field.
     */
    void addRevision();

    /**
     * Add values to phone field.
     */
    void addPhone();

    /**
     * Add value to title field.
     */
    void addTitle();

    /**
     * Add value to UID field.
     */
    void addUID();

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
