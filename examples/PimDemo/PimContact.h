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

    void printContactNameField();
    void printAddress();
    void printBirthday();
    void printClass();
    void printEmail();
    void printFormatedAddress();
    void printFormatedName();
    void printNickname();
    void printNote();
    void printOrg();
    void printPhoto();
    void printPhotoURL();
    void printPublicKey();
    void printPublicKeyString();
    void printRevision();
    void printPhone();
    void printTitle();
    void printUID();
    void printURL();
    void printIM();
    void printRelation();
    void printOrgInfo();

    void addContactName();
    void addAddress();
    void addBirthday();
    void addClass();
    void addEmail();
    void addFormatedAddress();
    void addFormatedName();
    void addNickname();
    void addNote();
    void addOrg();
    void addPhoto();
    void addPhotoURL();
    void addPublicKey();
    void addPublicKeyString();
    void addRevision();
    void addPhone();
    void addTitle();
    void addUID();
    void addURL();
    void addIM();
    void addRelation();
    void addOrgInfo();
private:
    /**
     * Handle to a contact item.
     */
    MAHandle mItemHandle;

    MA_PIM_ARGS mArgs;
};

#endif /* PIMCONTACT_H_ */
