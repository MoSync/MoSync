package com.mosync.pim;

import static com.mosync.internal.android.MoSyncHelpers.DebugPrint;

import static com.mosync.internal.generated.IX_PIM.MA_PIM_ATTR_RELATION_MOTHER;
import static com.mosync.internal.generated.IX_PIM.MA_PIM_ATTR_RELATION_FATHER;
import static com.mosync.internal.generated.IX_PIM.MA_PIM_ATTR_RELATION_PARENT;
import static com.mosync.internal.generated.IX_PIM.MA_PIM_ATTR_RELATION_SISTER;
import static com.mosync.internal.generated.IX_PIM.MA_PIM_ATTR_RELATION_BROTHER;
import static com.mosync.internal.generated.IX_PIM.MA_PIM_ATTR_RELATION_CHILD;
import static com.mosync.internal.generated.IX_PIM.MA_PIM_ATTR_RELATION_FRIEND;
import static com.mosync.internal.generated.IX_PIM.MA_PIM_ATTR_RELATION_SPOUSE;
import static com.mosync.internal.generated.IX_PIM.MA_PIM_ATTR_RELATION_PARTNER;
import static com.mosync.internal.generated.IX_PIM.MA_PIM_ATTR_RELATION_MANAGER;
import static com.mosync.internal.generated.IX_PIM.MA_PIM_ATTR_RELATION_ASSISTANT;
import static com.mosync.internal.generated.IX_PIM.MA_PIM_ATTR_RELATION_DOMESTIC_PARTNER;
import static com.mosync.internal.generated.IX_PIM.MA_PIM_ATTR_RELATION_REFERRED_BY;
import static com.mosync.internal.generated.IX_PIM.MA_PIM_ATTR_RELATION_RELATIVE;
import static com.mosync.internal.generated.IX_PIM.MA_PIM_ATTR_RELATION_CUSTOM;

import static com.mosync.internal.generated.IX_PIM.MA_PIM_ATTRPREFERRED;

import static com.mosync.internal.generated.IX_PIM.MA_PIM_ERR_NONE;
import static com.mosync.internal.generated.IX_PIM.MA_PIM_ERR_ATTRIBUTE_COMBO_UNSUPPORTED;
import static com.mosync.internal.generated.IX_PIM.MA_PIM_FIELD_CONTACT_RELATION;
import static com.mosync.internal.generated.IX_PIM.MA_PIM_TYPE_STRING;

import android.provider.ContactsContract.CommonDataKinds.Relation;

public class PIMFieldRelation extends PIMField {

	/**
	 * Constructor
	 */
	public PIMFieldRelation() {
		mType = MA_PIM_FIELD_CONTACT_RELATION;
		mStrType = Relation.CONTENT_ITEM_TYPE;
		mDataType = MA_PIM_TYPE_STRING;

		mNames = new String[] { Relation._ID, Relation.NAME, Relation.TYPE,
				Relation.LABEL, Relation.IS_PRIMARY };
	}

	void createMaps() {
		// attributes
		mAttributes.put(MA_PIM_ATTR_RELATION_MOTHER, Relation.TYPE_MOTHER);
		mAttributes.put(MA_PIM_ATTR_RELATION_FATHER, Relation.TYPE_FATHER);
		mAttributes.put(MA_PIM_ATTR_RELATION_PARENT, Relation.TYPE_PARENT);
		mAttributes.put(MA_PIM_ATTR_RELATION_SISTER, Relation.TYPE_SISTER);
		mAttributes.put(MA_PIM_ATTR_RELATION_BROTHER, Relation.TYPE_BROTHER);
		mAttributes.put(MA_PIM_ATTR_RELATION_CHILD, Relation.TYPE_CHILD);
		mAttributes.put(MA_PIM_ATTR_RELATION_FRIEND, Relation.TYPE_FRIEND);
		mAttributes.put(MA_PIM_ATTR_RELATION_SPOUSE, Relation.TYPE_SPOUSE);
		mAttributes.put(MA_PIM_ATTR_RELATION_PARTNER, Relation.TYPE_PARTNER);
		mAttributes.put(MA_PIM_ATTR_RELATION_MANAGER, Relation.TYPE_MANAGER);
		mAttributes
				.put(MA_PIM_ATTR_RELATION_ASSISTANT, Relation.TYPE_ASSISTANT);
		mAttributes.put(MA_PIM_ATTR_RELATION_DOMESTIC_PARTNER,
				Relation.TYPE_DOMESTIC_PARTNER);
		mAttributes.put(MA_PIM_ATTR_RELATION_REFERRED_BY,
				Relation.TYPE_REFERRED_BY);
		mAttributes.put(MA_PIM_ATTR_RELATION_RELATIVE, Relation.TYPE_RELATIVE);
		mAttributes.put(MA_PIM_ATTR_RELATION_CUSTOM, Relation.TYPE_CUSTOM);
	}

	int checkForPreferredAttribute(int index) {
		if (Integer.parseInt(getColumnValue(index, Relation.IS_PRIMARY)) != 0)
			return MA_PIM_ATTRPREFERRED;
		return 0;
	}

	/**
	 * Gets the field attribute.
	 */
	int getAndroidAttribute(int index) {
		String attribute = null;
		if ((attribute = getColumnValue(index, Relation.TYPE)) == null) {
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
		return getColumnValue(index, Relation.LABEL).toCharArray();
	}

	/**
	 * Gets the field's custom label.
	 * @param index
	 * @return
	 */
	void setLabel(int index, String label) {
		setColumnValue(index, Relation.LABEL, label);
	}

	/**
	 * Checks to see if the given field has a custom label.
	 * @param index
	 */
	boolean hasCustomLabel(int index) {
		return ((Integer.parseInt(getColumnValue(index, Relation.TYPE)) == Relation.TYPE_CUSTOM) ? true
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
			setColumnValue(index, Relation.IS_PRIMARY, Integer.toString(1));
		}
		attribute &= 0xFFFF;

		int ret = 0;
		if (mAttributes.containsKey(attribute)) {
			ret = mAttributes.get(attribute);
		} else {
			return MA_PIM_ERR_ATTRIBUTE_COMBO_UNSUPPORTED;
		}

		setColumnValue(index, Relation.TYPE, Integer.toString(ret));

		return MA_PIM_ERR_NONE;
	}

	/**
	 * Print field values.
	 */
	void print() {
		DebugPrint("*********RELATION**********");
		DebugPrint("COUNT = " + mValues.size());
		for (int i = 0; i < mValues.size(); i++) {
			String[] val = mValues.get(i);
			DebugPrint("###Relation " + i);
			DebugPrint(mNames[1] + ": " + val[1]);
		}
		DebugPrint("***************************");
	}
}
