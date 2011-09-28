package com.mosync.pim;

import static com.mosync.internal.android.MoSyncHelpers.DebugPrint;

import static com.mosync.internal.generated.IX_PIM.MA_PIM_ATTR_PHONE_HOME;
import static com.mosync.internal.generated.IX_PIM.MA_PIM_ATTR_PHONE_MOBILE;
import static com.mosync.internal.generated.IX_PIM.MA_PIM_ATTR_PHONE_HOME_FAX;
import static com.mosync.internal.generated.IX_PIM.MA_PIM_ATTR_PHONE_WORK_FAX;
import static com.mosync.internal.generated.IX_PIM.MA_PIM_ATTR_PHONE_PAGER;
import static com.mosync.internal.generated.IX_PIM.MA_PIM_ATTR_PHONE_IPHONE;
import static com.mosync.internal.generated.IX_PIM.MA_PIM_ATTR_PHONE_WORK;
import static com.mosync.internal.generated.IX_PIM.MA_PIM_ATTR_PHONE_CALLBACK;
import static com.mosync.internal.generated.IX_PIM.MA_PIM_ATTR_PHONE_CAR;
import static com.mosync.internal.generated.IX_PIM.MA_PIM_ATTR_PHONE_COMPANY_MAIN;
import static com.mosync.internal.generated.IX_PIM.MA_PIM_ATTR_PHONE_ISDN;
import static com.mosync.internal.generated.IX_PIM.MA_PIM_ATTR_PHONE_OTHER_FAX;
import static com.mosync.internal.generated.IX_PIM.MA_PIM_ATTR_PHONE_RADIO;
import static com.mosync.internal.generated.IX_PIM.MA_PIM_ATTR_PHONE_TELEX;
import static com.mosync.internal.generated.IX_PIM.MA_PIM_ATTR_PHONE_TTY_TDD;
import static com.mosync.internal.generated.IX_PIM.MA_PIM_ATTR_PHONE_WORK_MOBILE;
import static com.mosync.internal.generated.IX_PIM.MA_PIM_ATTR_PHONE_WORK_PAGER;
import static com.mosync.internal.generated.IX_PIM.MA_PIM_ATTR_PHONE_ASSISTANT;
import static com.mosync.internal.generated.IX_PIM.MA_PIM_ATTR_PHONE_MMS;
import static com.mosync.internal.generated.IX_PIM.MA_PIM_ATTR_PHONE_OTHER;
import static com.mosync.internal.generated.IX_PIM.MA_PIM_ATTR_PHONE_CUSTOM;
import static com.mosync.internal.generated.IX_PIM.MA_PIM_ATTRPREFERRED;
import static com.mosync.internal.generated.IX_PIM.MA_PIM_ERR_FIELD_EMPTY;
import static com.mosync.internal.generated.IX_PIM.MA_PIM_ERR_INDEX_INVALID;

import static com.mosync.internal.generated.IX_PIM.MA_PIM_ERR_NONE;
import static com.mosync.internal.generated.IX_PIM.MA_PIM_ERR_ATTRIBUTE_COMBO_UNSUPPORTED;
import static com.mosync.internal.generated.IX_PIM.MA_PIM_FIELD_CONTACT_TEL;
import static com.mosync.internal.generated.IX_PIM.MA_PIM_TYPE_STRING;

import android.provider.ContactsContract.CommonDataKinds.Phone;

public class PIMFieldPhone extends PIMField {

	/**
	 * Constructor
	 */
	public PIMFieldPhone() {
		mType = MA_PIM_FIELD_CONTACT_TEL;
		mStrType = Phone.CONTENT_ITEM_TYPE;
		mDataType = MA_PIM_TYPE_STRING;

		mNames = new String[] { Phone._ID, Phone.NUMBER, Phone.TYPE,
				Phone.LABEL, Phone.IS_PRIMARY };
	}

	void createMaps() {
		// attributes
		mAttributes.put(MA_PIM_ATTR_PHONE_HOME, Phone.TYPE_HOME);
		mAttributes.put(MA_PIM_ATTR_PHONE_MOBILE, Phone.TYPE_MOBILE);
		mAttributes.put(MA_PIM_ATTR_PHONE_HOME_FAX, Phone.TYPE_FAX_HOME);
		mAttributes.put(MA_PIM_ATTR_PHONE_WORK_FAX, Phone.TYPE_FAX_WORK);
		mAttributes.put(MA_PIM_ATTR_PHONE_PAGER, Phone.TYPE_PAGER);
		mAttributes.put(MA_PIM_ATTR_PHONE_IPHONE, Phone.TYPE_CUSTOM);
		mAttributes.put(MA_PIM_ATTR_PHONE_WORK, Phone.TYPE_WORK);
		mAttributes.put(MA_PIM_ATTR_PHONE_CALLBACK, Phone.TYPE_CALLBACK);
		mAttributes.put(MA_PIM_ATTR_PHONE_CAR, Phone.TYPE_CAR);
		mAttributes
				.put(MA_PIM_ATTR_PHONE_COMPANY_MAIN, Phone.TYPE_COMPANY_MAIN);
		mAttributes.put(MA_PIM_ATTR_PHONE_ISDN, Phone.TYPE_ISDN);
		mAttributes.put(MA_PIM_ATTR_PHONE_OTHER_FAX, Phone.TYPE_OTHER_FAX);
		mAttributes.put(MA_PIM_ATTR_PHONE_RADIO, Phone.TYPE_RADIO);
		mAttributes.put(MA_PIM_ATTR_PHONE_TELEX, Phone.TYPE_TELEX);
		mAttributes.put(MA_PIM_ATTR_PHONE_TTY_TDD, Phone.TYPE_TTY_TDD);
		mAttributes.put(MA_PIM_ATTR_PHONE_WORK_MOBILE, Phone.TYPE_WORK_MOBILE);
		mAttributes.put(MA_PIM_ATTR_PHONE_WORK_PAGER, Phone.TYPE_WORK_PAGER);
		mAttributes.put(MA_PIM_ATTR_PHONE_ASSISTANT, Phone.TYPE_ASSISTANT);
		mAttributes.put(MA_PIM_ATTR_PHONE_MMS, Phone.TYPE_MMS);
		mAttributes.put(MA_PIM_ATTR_PHONE_OTHER, Phone.TYPE_OTHER);
		mAttributes.put(MA_PIM_ATTR_PHONE_CUSTOM, Phone.TYPE_CUSTOM);
	}

	int checkForPreferredAttribute(int index) {
		if (Integer.parseInt(getColumnValue(index, Phone.IS_PRIMARY)) != 0)
			return MA_PIM_ATTRPREFERRED;
		return 0;
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

		int attr = getAndroidAttribute(index);

		if (attr < 0) {
			return 0;
		}

		int ret = 0;
		if ((attr == Phone.TYPE_CUSTOM)
				&& (getColumnValue(index, Phone.LABEL).equals("iPhone"))) {
			ret |= MA_PIM_ATTR_PHONE_IPHONE;
		} else {
			ret = ((Integer) PIMUtil.getKeyFromValue(mAttributes, attr))
					.intValue();
		}

		ret |= checkForPreferredAttribute(index);

		return ret;
	}

	/**
	 * Gets the field attribute.
	 */
	int getAndroidAttribute(int index) {
		String attribute = null;
		if ((attribute = getColumnValue(index, Phone.TYPE)) == null) {
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
		return getColumnValue(index, Phone.LABEL).toCharArray();
	}

	/**
	 * Gets the field's custom label.
	 * @param index
	 * @return
	 */
	void setLabel(int index, String label) {
		setColumnValue(index, Phone.LABEL, label);
	}

	/**
	 * Checks to see if the given field has a custom label.
	 * @param index
	 */
	boolean hasCustomLabel(int index) {
		return ((Integer.parseInt(getColumnValue(index, Phone.TYPE)) == Phone.TYPE_CUSTOM) ? true
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
			setColumnValue(index, Phone.IS_PRIMARY, Integer.toString(1));
		}
		attribute &= 0xFFFF;

		int ret = 0;
		if (mAttributes.containsKey(attribute)) {
			ret = mAttributes.get(attribute);
		} else {
			return MA_PIM_ERR_ATTRIBUTE_COMBO_UNSUPPORTED;
		}

		if (attribute == MA_PIM_ATTR_PHONE_IPHONE) {
			setColumnValue(index, Phone.TYPE,
					Integer.toString(Phone.TYPE_CUSTOM));
			setColumnValue(index, Phone.LABEL, "iPhone");
		} else {
			setColumnValue(index, Phone.TYPE, Integer.toString(ret));
		}

		return MA_PIM_ERR_NONE;
	}

	/**
	 * Print field values.
	 */
	void print() {
		DebugPrint("***********PHONES**********");
		DebugPrint("COUNT = " + mValues.size());
		for (int i = 0; i < mValues.size(); i++) {
			String[] val = mValues.get(i);
			DebugPrint("###Phone " + i);
			DebugPrint(mNames[1] + ": " + val[1]);
		}
		DebugPrint("***************************");
	}
}
