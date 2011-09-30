package com.mosync.pim;

import static com.mosync.internal.android.MoSyncHelpers.DebugPrint;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.Map;

import static com.mosync.internal.generated.IX_PIM.MA_PIM_ATTR_IM_HOME;
import static com.mosync.internal.generated.IX_PIM.MA_PIM_ATTR_IM_WORK;
import static com.mosync.internal.generated.IX_PIM.MA_PIM_ATTR_IM_OTHER;
import static com.mosync.internal.generated.IX_PIM.MA_PIM_ATTR_IM_CUSTOM;
import static com.mosync.internal.generated.IX_PIM.MA_PIM_ATTRPREFERRED;

import static com.mosync.internal.generated.IX_PIM.MA_PIM_ERR_NONE;
import static com.mosync.internal.generated.IX_PIM.MA_PIM_ERR_ATTRIBUTE_COMBO_UNSUPPORTED;
import static com.mosync.internal.generated.IX_PIM.MA_PIM_FIELD_CONTACT_IM;
import static com.mosync.internal.generated.IX_PIM.MA_PIM_TYPE_STRING_ARRAY;

import static com.mosync.internal.generated.IX_PIM.MA_PIM_CONTACT_IM_PROTOCOL_AIM;
import static com.mosync.internal.generated.IX_PIM.MA_PIM_CONTACT_IM_PROTOCOL_MSN;
import static com.mosync.internal.generated.IX_PIM.MA_PIM_CONTACT_IM_PROTOCOL_YAHOO;
import static com.mosync.internal.generated.IX_PIM.MA_PIM_CONTACT_IM_PROTOCOL_SKYPE;
import static com.mosync.internal.generated.IX_PIM.MA_PIM_CONTACT_IM_PROTOCOL_QQ;
import static com.mosync.internal.generated.IX_PIM.MA_PIM_CONTACT_IM_PROTOCOL_GOOGLE_TALK;
import static com.mosync.internal.generated.IX_PIM.MA_PIM_CONTACT_IM_PROTOCOL_ICQ;
import static com.mosync.internal.generated.IX_PIM.MA_PIM_CONTACT_IM_PROTOCOL_JABBER;
import static com.mosync.internal.generated.IX_PIM.MA_PIM_CONTACT_IM_PROTOCOL_NETMEETING;

import android.provider.ContactsContract.CommonDataKinds.Im;
import android.provider.ContactsContract.CommonDataKinds.StructuredPostal;

public class PIMFieldIM extends PIMField {

	Map<Integer, String> mProtocols;

	/**
	 * Constructor
	 */
	public PIMFieldIM() {
		mType = MA_PIM_FIELD_CONTACT_IM;
		mStrType = Im.CONTENT_ITEM_TYPE;
		mDataType = MA_PIM_TYPE_STRING_ARRAY;

		mNames = new String[] { Im._ID, Im.DATA, Im.PROTOCOL,
				Im.CUSTOM_PROTOCOL, Im.TYPE, Im.LABEL, Im.IS_PRIMARY };
	}

	void createMaps() {
		// attributes
		mAttributes.put(MA_PIM_ATTR_IM_HOME, Im.TYPE_HOME);
		mAttributes.put(MA_PIM_ATTR_IM_WORK, Im.TYPE_WORK);
		mAttributes.put(MA_PIM_ATTR_IM_OTHER, Im.TYPE_OTHER);
		mAttributes.put(MA_PIM_ATTR_IM_CUSTOM, Im.TYPE_CUSTOM);

		mProtocols = new HashMap<Integer, String>();
		// Protocols
		mProtocols.put(Im.PROTOCOL_AIM, MA_PIM_CONTACT_IM_PROTOCOL_AIM);
		mProtocols.put(Im.PROTOCOL_MSN, MA_PIM_CONTACT_IM_PROTOCOL_MSN);
		mProtocols.put(Im.PROTOCOL_YAHOO, MA_PIM_CONTACT_IM_PROTOCOL_YAHOO);
		mProtocols.put(Im.PROTOCOL_SKYPE, MA_PIM_CONTACT_IM_PROTOCOL_SKYPE);
		mProtocols.put(Im.PROTOCOL_QQ, MA_PIM_CONTACT_IM_PROTOCOL_QQ);
		mProtocols.put(Im.PROTOCOL_GOOGLE_TALK,
				MA_PIM_CONTACT_IM_PROTOCOL_GOOGLE_TALK);
		mProtocols.put(Im.PROTOCOL_ICQ, MA_PIM_CONTACT_IM_PROTOCOL_ICQ);
		mProtocols.put(Im.PROTOCOL_JABBER, MA_PIM_CONTACT_IM_PROTOCOL_JABBER);
		mProtocols.put(Im.PROTOCOL_NETMEETING,
				MA_PIM_CONTACT_IM_PROTOCOL_NETMEETING);
		mProtocols.put(Im.PROTOCOL_CUSTOM, "OTHER");
	}

	void preProcessData() {
		for (int i = 0; i < mValues.size(); i++) {
			String[] val = mValues.get(i);
			val[2] = getProtocolName(val[2]);
			mValues.set(i, val);
		}
	}

	String getProtocolName(String value) {
		int val = Integer.parseInt(value);
		if (!mProtocols.containsKey(val))
			return "OTHER";
		return mProtocols.get(val);
	}

	int checkForPreferredAttribute(int index) {
		if (Integer.parseInt(getColumnValue(index, Im.IS_PRIMARY)) != 0)
			return MA_PIM_ATTRPREFERRED;
		return 0;
	}

	/**
	 * Gets the field attribute.
	 */
	int getAndroidAttribute(int index) {
		String attribute = null;
		if ((attribute = getColumnValue(index, Im.TYPE)) == null) {
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
		return getColumnValue(index, Im.LABEL).toCharArray();
	}

	/**
	 * Gets the field's custom label.
	 * @param index
	 * @return
	 */
	void setLabel(int index, String label) {
		setColumnValue(index, Im.LABEL, label);
	}

	/**
	 * Checks to see if the given field has a custom label.
	 * @param index
	 */
	boolean hasCustomLabel(int index) {
		return ((Integer.parseInt(getColumnValue(index, Im.TYPE)) == Im.TYPE_CUSTOM) ? true
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
			setColumnValue(index, Im.IS_PRIMARY, Integer.toString(1));
		}
		attribute &= 0xFFFF;

		int ret = 0;
		if (mAttributes.containsKey(attribute)) {
			ret = mAttributes.get(attribute);
		} else {
			return MA_PIM_ERR_ATTRIBUTE_COMBO_UNSUPPORTED;
		}

		setColumnValue(index, Im.TYPE, Integer.toString(ret));

		return MA_PIM_ERR_NONE;
	}

	void postProcessData() {
		for (int i = 0; i < mValues.size(); i++) {
			String[] val = mValues.get(i);
			val[2] = getProtocolId(val[2]);
			mValues.set(i, val);
		}
	}

	String getProtocolId(String value) {
		Integer id = ((Integer) PIMUtil.getKeyFromValue(mProtocols, value))
				.intValue();
		if (id == null)
			return Integer.toString(Im.PROTOCOL_CUSTOM);
		return id.toString();
	}

	/**
	 * Print field values.
	 */
	void print() {
		DebugPrint("*************IM************");
		DebugPrint("COUNT = " + mValues.size());
		for (int i = 0; i < mValues.size(); i++) {
			String[] val = mValues.get(i);
			DebugPrint("###Im " + i);
			for (int j = 0; j < val.length; j++) {
				DebugPrint(mNames[j] + ": " + val[j]);
			}
		}
		DebugPrint("***************************");
	}
}
