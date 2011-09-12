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
 * @file PimItem.h
 * Abstract class for pim items.
 */

#import <Foundation/Foundation.h>
#import "PimUtil.h"
#import "helpers/cpp_defs.h"
#import "helpers/cpp_ix_pim.h"

/**
 * The status of an pim item.
 */
typedef enum {
	/**
	* New item. The item must be added to the database(e.g. Address Book) manually.
	*/
	kNewItem,
	/**
	* The item was imported from the database(e.g. Address Book). The content
	* of the item has not been modified.
	*/
	kImportedItem,
	/**
	* The item was imported from the database(e.g. Address Book). The content
	* of the item has been modified. The item must be updated into database.
	*/
	kModifiedItem
} PimItemStatus;


/**
 * An emulated abstract class for pim items.
 * Objectiv C does not offer the possibility to create an abstract class
 * so this is an usual class, but not all methods are implemented.
 * DO NOT USE THIS CLASS TO CREATE OBJECTS. Instead use classes that are
 * derived from this class(e.g. PimContactItem).
 * This class only defines the behavior for each pim item class.
 */
@interface PimItem: NSObject {
	/**
	* Stores the fields.
	* Object type is PimItemField.
	* Object's key is one of MA_PIM_FIELD constants.
	*/
	NSMutableDictionary* mFieldsDictionary;

	/**
	* The status of the item.
	*/
	PimItemStatus mItemStatus;
}

/**
 * Init function.
 */
-(id) init;

/**
 * Gets the number of fields.
 * @return The number of fields for this item.
 */
-(int) count;

/**
 * Gets field's id for a given field index.
 * A field id is one of the MA_PIM_FIELD constants.
 * @param fieldIndex Field's index.
 * @return One of the MA_PIM_FIELD constants, or one of the MA_PIM_ERR
 *         constants in case of error.
 */
-(int) getFieldID:(const int) fieldIndex;

/**
 * Gets the number of values for a given field ID.
 * Does not check if field is supported.
 * @param field The specified field ID(one of the MA_PIM_FIELD constants).
 * @return The number of values for the given field, or 0 if the field ID
 *         has no values/does not exist.
 */
-(int) fieldCount:(const int) fieldID;

/**
 * Gets the attribute for a given field value.
 * Does not check if field is supported.
 * @param fieldID The specified field ID(one of the MA_PIM_FIELD constants).
 * @param index The index of the given field value.
 * @return One of MA_PIM_ATTR constants, or MA_PIM_ERR in case of error.
 */
-(int) getAttribute:(const int) fieldID
         indexValue:(const int) index;

/**
 * Sets a custom label for a given field value.
 * Does not check if field is supported or valid.
 * @param args Common arguments.
 *             The item's handle is stored in args.item.
 *             The field's ID is stored in args.field.
 *             The custom label's value is stored in args.buf.
 *             The size of the value(in bytes) is stored in args.bufSize.
 * @param index Field's value index.
 * @return One of the MA_PIM_ERR constants.
 */
-(int) setLabel:(const MA_PIM_ARGS*) args
     indexValue:(const int) index;

/**
 * Gets a custom label for a given field value.
 * Does not check if field is supported.
 * @param args Common arguments.
 *             The item's handle is stored in args.item.
 *             The field's ID is stored in args.field.
 *             The custom label's value will be stored in args.buf.
 *             The maximum size of the value(in bytes) is stored in args.bufSize.
 * @param index Field's value index.
 * @return The number of bytes occupied by the value. If the number is greater than
 *         args.bufSize the value was not written into args.buf.
 *         In case of error the function returns one of the MA_PIM_ERR constants.
 *         If the field's value does not have a custom label, the function will
 *         return MA_PIM_ERR_NO_LABEL.
 */
-(int) getLabel:(const MA_PIM_ARGS*) args
     indexValue:(const int) index;


/**
 * Gets a field's value at a given index.
 * Does not check if field is supported or write-only.
 * @param args Common arguments.
 *             The item's handle is stored in args.item.
 *             The field's ID is stored in args.field.
 *             The field's value will be stored in args.buf.
 *             The maximum size of the value(in bytes) is stored in args.bufSize.
 * @param index Field's value index.
 * @return The number of bytes occupied by the value. If the number is greater than
 *         args.bufSize the value was not written into args.buf.
 *         In case of error the function returns one of the MA_PIM_ERR constants.
 */
-(int) getValue:(const MA_PIM_ARGS*) args
     indexValue:(const int) index;

/**
 * Sets a field's value and attribute at a given index.
 * Does not check if field is supported or write-only.
 * @param args Common arguments.
 *             The item's handle is stored in args.item.
 *             The field's ID is stored in args.field.
 *             The field's value is stored in args.buf.
 *             The size of the value(in bytes) is stored in args.bufSize.
 * @param index Field's value index.
 * @param attribute Field's value attribute.
 * @return One of the MA_PIM_ERR constants.
 */
-(int) setValue:(const MA_PIM_ARGS*) args
     indexValue:(const int) index
 valueAttribute:(const int) attribute;

/**
 * Adds a value to a specified field.
 * Does not check if the field is supported or read-only.
 * @param args Common arguments.
 *             The item's handle is stored in args.item.
 *             The field's ID is stored in args.field.
 *             The field's value is stored in args.buf.
 *             The size of the value(in bytes) is stored in args.bufSize.
 * @param attribute Field's value attribute.
 * @return  New value's index in field, or one of the MA_PIM_ERR constants
 *          in case of error.
 */
-(int) addValue:(const MA_PIM_ARGS*) args
  withAttribute:(const int) attribute;

/**
 * Removes a value from a field.
 * Does not check if the field is valid or supported.
 * @param field One of the MA_PIM_FIELD constants.
 * @param index Field's value index.
 * @return One of MA_PIM_ERR constants.
 */
-(int) removeValue:(const int) field
           atIndex:(const int) index;

/**
 * Writes all the fields into record.
 * Deletes all item's fields.
 * @return One of the MA_PIM_ERR constants.
 */
-(int) close;

/**
 * Gets the status of the item.
 * @return The status.
 */
-(PimItemStatus) getStatus;

@end
