package com.mosync.pim;

import static com.mosync.internal.android.MoSyncHelpers.DebugPrint;

import com.mosync.pim.PIMField.State;

import static com.mosync.internal.generated.IX_PIM.MA_PIM_ATTR_ORG_INFO_WORK;
import static com.mosync.internal.generated.IX_PIM.MA_PIM_ATTR_ORG_INFO_OTHER;
import static com.mosync.internal.generated.IX_PIM.MA_PIM_ATTR_ORG_INFO_CUSTOM;
import static com.mosync.internal.generated.IX_PIM.MA_PIM_ATTRPREFERRED;

import static com.mosync.internal.generated.IX_PIM.MA_PIM_ERR_NONE;
import static com.mosync.internal.generated.IX_PIM.MA_PIM_ERR_ATTRIBUTE_COMBO_UNSUPPORTED;
import static com.mosync.internal.generated.IX_PIM.MA_PIM_FIELD_CONTACT_ORG_INFO;
import static com.mosync.internal.generated.IX_PIM.MA_PIM_TYPE_STRING_ARRAY;

import android.content.ContentResolver;
import android.database.Cursor;
import android.provider.ContactsContract.Data;
import android.provider.ContactsContract.CommonDataKinds.Organization;

public class PIMFieldOrganizationInfo extends PIMField {

	/**
	 * Constructor
	 */
	public PIMFieldOrganizationInfo() {
		mType = MA_PIM_FIELD_CONTACT_ORG_INFO;
		mStrType = Organization.CONTENT_ITEM_TYPE;
		mDataType = MA_PIM_TYPE_STRING_ARRAY;

		mNames = new String[] { Organization._ID, Organization.DEPARTMENT,
				Organization.JOB_DESCRIPTION, Organization.SYMBOL,
				Organization.PHONETIC_NAME, Organization.OFFICE_LOCATION,
				Organization.TYPE, Organization.LABEL, Organization.IS_PRIMARY };
	}

	void createMaps() {
		// attributes
		mAttributes.put(MA_PIM_ATTR_ORG_INFO_WORK, Organization.TYPE_WORK);
		mAttributes.put(MA_PIM_ATTR_ORG_INFO_OTHER, Organization.TYPE_OTHER);
		mAttributes.put(MA_PIM_ATTR_ORG_INFO_CUSTOM, Organization.TYPE_CUSTOM);
	}

	/**
	 * Read field
	 * @param cr
	 * @param contactId
	 */
	void read(ContentResolver cr, String contactId) {
		DebugPrint("PIMField.read(" + cr + ", " + contactId + ")");
		Cursor cursor = cr.query(Data.CONTENT_URI, mNames, Data.CONTACT_ID
				+ "=?" + " AND " + Data.MIMETYPE + "=?",
				new String[] { String.valueOf(contactId), mStrType }, null);

		// if (cursor == null)
		// return;

		while (cursor.moveToNext()) {
			String[] val = new String[mNames.length];
			for (int i = 0; i < mNames.length; i++) {
				val[i] = new String("");
				if (!mNames[i].equals(DUMMY)) {
					int index = cursor.getColumnIndex(mNames[i]);
					if (index >= 0) {
						val[i] = cursor.getString(index);
					}
				}
			}

			boolean canAdd = false;
			for (int i = 1; i < 6; i++) {
				if (val[i] != null) {
					canAdd = true;
					break;
				}
			}
			if (canAdd) {
				mValues.add(val);
				mStates.add(State.NONE);
			}
		}

		cursor.close();
		cursor = null;

		preProcessData();

		print();
	}

	int checkForPreferredAttribute(int index) {
		if (Integer.parseInt(getColumnValue(index, Organization.IS_PRIMARY)) != 0)
			return MA_PIM_ATTRPREFERRED;
		return 0;
	}

	/**
	 * Gets the field attribute.
	 */
	int getAndroidAttribute(int index) {
		String attribute = null;
		if ((attribute = getColumnValue(index, Organization.TYPE)) == null) {
			return -1;
		}
		return Integer.parseInt(attribute);
	}

	/**
	 * Gets the field's custom label.
	 * @param index
	 * @return
	 */
	char[] getLabel(int index) {
		return getColumnValue(index, Organization.LABEL).toCharArray();
	}

	/**
	 * Gets the field's custom label.
	 * @param index
	 * @return
	 */
	void setLabel(int index, String label) {
		setColumnValue(index, Organization.LABEL, label);
	}

	/**
	 * Checks to see if the given field has a custom label.
	 * @param index
	 */
	boolean hasCustomLabel(int index) {
		return ((Integer.parseInt(getColumnValue(index, Organization.TYPE)) == Organization.TYPE_CUSTOM) ? true
				: false);
	}

	char[] getData(int index) {
		String[] val = getSpecificData(index);
		DebugPrint("DATA SIZE = " + getDataSize(val));
		char[] buffer = new char[getDataSize(val)];
		PIMUtil.writeStringArray(val, buffer);
		return buffer;
	}

	String[] getSpecificData(int index) {
		String[] val = mValues.get(index);
		String[] ret = new String[val.length - 4];
		for (int i = 0; i < val.length - 4; i++) {
			ret[i] = val[i + 1];
		}
		return ret;
	}

	int getDataSize(String[] val) {
		int size = 4;
		for (int i = 0; i < val.length; i++) {
			if (val[i] != null) {
				size += val[i].length();
			}
			size += 1;
		}
		return size;
	}

	void setData(int index, char[] buffer) {
		String[] val = PIMUtil.readStringArray(buffer);
		setSpecificData(val, index);
	}

	void setSpecificData(String[] data, int index) {
		String[] val = mValues.get(index);
		for (int i = 0; i < data.length; i++) {
			val[i + 1] = data[i];
		}
		mValues.set(index, val);
	}

	int setAttribute(int index, int attribute) {
		if ((attribute | MA_PIM_ATTRPREFERRED) != 0) {
			setColumnValue(index, Organization.IS_PRIMARY, Integer.toString(1));
		}
		attribute &= 0xFFFF;

		int ret = 0;
		if (mAttributes.containsKey(attribute)) {
			ret = mAttributes.get(attribute);
		} else {
			return MA_PIM_ERR_ATTRIBUTE_COMBO_UNSUPPORTED;
		}

		setColumnValue(index, Organization.TYPE, Integer.toString(ret));

		return MA_PIM_ERR_NONE;
	}

	/**
	 * Print field values.
	 */
	void print() {
		DebugPrint("*****ORGANIZATION INFO*****");
		DebugPrint("COUNT = " + mValues.size());
		for (int i = 0; i < mValues.size(); i++) {
			String[] val = mValues.get(i);
			DebugPrint("###Organization info " + i);
			for (int j = 0; j < val.length; j++) {
				DebugPrint(mNames[j] + ": " + val[j]);
			}
		}
		DebugPrint("***************************");
	}
}
