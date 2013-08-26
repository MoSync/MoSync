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

package com.mosync.pim;

import static com.mosync.internal.android.MoSyncHelpers.DebugPrint;
import static com.mosync.internal.generated.IX_PIM.MA_PIM_ERR_NONE;

import static com.mosync.internal.generated.IX_PIM.MA_PIM_FIELD_CONTACT_UID;
import static com.mosync.internal.generated.IX_PIM.MA_PIM_TYPE_STRING;

import android.content.ContentResolver;

public class PIMFieldUID extends PIMStringField {

	/**
	 * Constructor
	 */
	public PIMFieldUID() {
		MAX_SIZE = 1;

		mType = MA_PIM_FIELD_CONTACT_UID;
		mDataType = MA_PIM_TYPE_STRING;

		mPermission = Permission.READ_ONLY;
	}

	void createMaps() {
	}

	/**
	 * Read address field.
	 */
	void read(ContentResolver cr, String contactId) {
		DebugPrint("PIMFieldUID.read(" + cr + ", " + contactId + ")");
		mValues.add(new String[] { contactId });
		mStates.add(State.NONE);

		print();
	}

	int getAttributes(int index) {
		return 0;
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

	String getSpecificData(int index) {
		String[] val = mValues.get(index);
		return val[0];
	}

	void setSpecificData(String data, int index) {
		mValues.set(index, new String[] { data });
	}

	/**
	 * Print field values.
	 */
	void print() {
		String[] val = null;
		DebugPrint("*************UID***********");
		if ((mValues == null) || (mValues.size() == 0)
				|| ((val = mValues.get(0)) == null)) {
			DebugPrint("NOT AVAILABLE");
		} else {
			DebugPrint("UID: " + val[0]);
		}
		DebugPrint("***************************");
	}
}
