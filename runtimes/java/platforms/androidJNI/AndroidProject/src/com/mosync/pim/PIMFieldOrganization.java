package com.mosync.pim;

import static com.mosync.internal.android.MoSyncHelpers.DebugPrint;

import static com.mosync.internal.generated.IX_PIM.MA_PIM_ATTR_ORG_WORK;
import static com.mosync.internal.generated.IX_PIM.MA_PIM_ATTR_ORG_OTHER;
import static com.mosync.internal.generated.IX_PIM.MA_PIM_ATTR_ORG_CUSTOM;
import static com.mosync.internal.generated.IX_PIM.MA_PIM_ATTRPREFERRED;

import static com.mosync.internal.generated.IX_PIM.MA_PIM_ERR_NONE;
import static com.mosync.internal.generated.IX_PIM.MA_PIM_ERR_ATTRIBUTE_COMBO_UNSUPPORTED;
import static com.mosync.internal.generated.IX_PIM.MA_PIM_FIELD_CONTACT_ORG;
import static com.mosync.internal.generated.IX_PIM.MA_PIM_TYPE_STRING;

import android.content.ContentResolver;
import android.database.Cursor;
import android.provider.ContactsContract.Data;
import android.provider.ContactsContract.CommonDataKinds.Organization;

public class PIMFieldOrganization extends PIMField {

	/**
	 * Constructor
	 */
	public PIMFieldOrganization() {
		mType = MA_PIM_FIELD_CONTACT_ORG;
		mStrType = Organization.CONTENT_ITEM_TYPE;
		mDataType = MA_PIM_TYPE_STRING;

		mNames = new String[] { Organization._ID, Organization.COMPANY,
				Organization.TYPE, Organization.LABEL, Organization.IS_PRIMARY };
	}

	void createMaps() {
		// attributes
		mAttributes.put(MA_PIM_ATTR_ORG_WORK, Organization.TYPE_WORK);
		mAttributes.put(MA_PIM_ATTR_ORG_OTHER, Organization.TYPE_OTHER);
		mAttributes.put(MA_PIM_ATTR_ORG_CUSTOM, Organization.TYPE_CUSTOM);
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
			if (val[1] != null) {
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
		String val = getSpecificData(index);
		char[] buffer = new char[getDataSize(val)];
		PIMUtil.writeString(val, buffer);
		return buffer;
	}

	String getSpecificData(int index) {
		String[] val = mValues.get(index);
		return val[1];
	}

	int getDataSize(String val) {
		return val.length() + 1;
	}

	void setData(int index, char[] buffer) {
		String val = PIMUtil.readString(buffer);
		setSpecificData(val, index);
	}

	void setSpecificData(String data, int index) {
		String[] val = mValues.get(index);
		val[1] = data;
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
		DebugPrint("*******ORGANIZATIONS*******");
		DebugPrint("COUNT = " + mValues.size());
		for (int i = 0; i < mValues.size(); i++) {
			String[] val = mValues.get(i);
			DebugPrint("###Organization " + i);
			DebugPrint(mNames[1] + ": " + val[1]);
		}
		DebugPrint("***************************");
	}
}
