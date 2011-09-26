package com.mosync.pim;

import static com.mosync.internal.android.MoSyncHelpers.DebugPrint;
import android.content.ContentResolver;

import static com.mosync.internal.generated.IX_PIM.MA_PIM_ERR_NONE;

import static com.mosync.internal.generated.IX_PIM.MA_PIM_FIELD_CONTACT_FORMATTED_NAME;
import static com.mosync.internal.generated.IX_PIM.MA_PIM_TYPE_STRING;

public class PIMFieldFormattedName extends PIMField {

	/**
	 * Constructor
	 */
	public PIMFieldFormattedName() {
		mType = MA_PIM_FIELD_CONTACT_FORMATTED_NAME;
		mDataType = MA_PIM_TYPE_STRING;
	}

	void createMaps() {

	}

	void read(ContentResolver cr, String contactId) {
		print();
	}

	int checkForPreferredAttribute(int index) {
		return 0;
	}

	int getAndroidAttribute(int index) {
		return 0;
	}

	int setAttribute(int index, int attribute) {
		return MA_PIM_ERR_NONE;
	}

	char[] getLabel(int index) {
		return null;
	}

	void setLabel(int index, String label) {
	}

	boolean hasCustomLabel(int index) {
		return false;
	}

	char[] getData(int index) {
		return null;
	}

	void setData(int index, char[] buffer) {
	}

	boolean isSupported() {
		return false;
	}

	/**
	 * Print field values.
	 */
	void print() {
		DebugPrint("******FORMATTED NAMES******");
		DebugPrint("Not supported");
		DebugPrint("***************************");
	}
}
