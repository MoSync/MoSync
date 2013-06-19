package com.mosync.pim;

import static com.mosync.internal.android.MoSyncHelpers.DebugPrint;

import com.mosync.api.Pointer;

import static com.mosync.internal.generated.IX_PIM.MA_PIM_ATTRPREFERRED;

import static com.mosync.internal.generated.IX_PIM.MA_PIM_ERR_FIELD_EMPTY;
import static com.mosync.internal.generated.IX_PIM.MA_PIM_ERR_INDEX_INVALID;
import static com.mosync.internal.generated.IX_PIM.MA_PIM_ERR_NONE;
import static com.mosync.internal.generated.IX_PIM.MA_PIM_ERR_ATTRIBUTE_COMBO_UNSUPPORTED;
import static com.mosync.internal.generated.IX_PIM.MA_PIM_FIELD_CONTACT_NOTE;
import static com.mosync.internal.generated.IX_PIM.MA_PIM_TYPE_STRING;

import android.provider.ContactsContract.CommonDataKinds.Note;

public class PIMFieldNote extends PIMStringField {

	/**
	 * Constructor
	 */
	public PIMFieldNote() {
		mType = MA_PIM_FIELD_CONTACT_NOTE;
		mStrType = Note.CONTENT_ITEM_TYPE;
		mDataType = MA_PIM_TYPE_STRING;

		mNames = new String[] { Note._ID, Note.NOTE, Note.IS_PRIMARY };
	}

	void createMaps() {
	}

	/**
	 * Get field's attributes.
	 */
	int getAttributes(int index) {
		if (isEmpty()) {
			return MA_PIM_ERR_FIELD_EMPTY;
		}
		if ((index < 0) || (index >= length())) {
			return MA_PIM_ERR_INDEX_INVALID;
		}

		int ret = checkForPreferredAttribute(index);

		return ret;
	}

	int checkForPreferredAttribute(int index) {
		if (Integer.parseInt(getColumnValue(index, Note.IS_PRIMARY)) != 0)
			return MA_PIM_ATTRPREFERRED;
		return 0;
	}

	int getAndroidAttribute(int index) {
		return 0;
	}

	/**
	 * Gets the field's custom label.
	 * @param index
	 * @return
	 */
	char[] getLabel(int index) {
		return null;
	}

	/**
	 * Gets the field's custom label.
	 * @param index
	 * @return
	 */
	void setLabel(int index, String label) {
	}

	/**
	 * Checks to see if the given field has a custom label.
	 * @param index
	 */
	boolean hasCustomLabel(int index) {
		return false;
	}

	int setAttribute(int index, int attribute) {
		if ((attribute | MA_PIM_ATTRPREFERRED) != 0) {
			setColumnValue(index, Note.IS_PRIMARY, Integer.toString(1));
		}
		attribute &= 0xFFFF;

		if (attribute != 0) {
			return MA_PIM_ERR_ATTRIBUTE_COMBO_UNSUPPORTED;
		}

		return MA_PIM_ERR_NONE;
	}

	/**
	 * Print field values.
	 */
	void print() {
		DebugPrint("***********NOTE************");
		DebugPrint("COUNT = " + mValues.size());
		for (int i = 0; i < mValues.size(); i++) {
			String[] val = mValues.get(i);
			DebugPrint("###Note " + i);
			DebugPrint(mNames[1] + ": " + val[1]);
		}
		DebugPrint("***************************");
	}
}
