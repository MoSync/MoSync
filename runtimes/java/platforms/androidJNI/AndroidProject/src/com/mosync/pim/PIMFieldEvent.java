package com.mosync.pim;

import static com.mosync.internal.android.MoSyncHelpers.DebugPrint;
import static com.mosync.internal.generated.IX_PIM.MA_PIM_ATTRPREFERRED;
import static com.mosync.internal.generated.IX_PIM.MA_PIM_ERR_ATTRIBUTE_COMBO_UNSUPPORTED;
import static com.mosync.internal.generated.IX_PIM.MA_PIM_ERR_NONE;
import static com.mosync.internal.generated.IX_PIM.MA_PIM_TYPE_DATE;

import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.Calendar;
import java.util.Date;

import android.provider.ContactsContract.CommonDataKinds.Event;

import static com.mosync.internal.generated.IX_PIM.MA_PIM_ATTRPREFERRED;

import static com.mosync.internal.generated.IX_PIM.MA_PIM_ERR_NONE;
import static com.mosync.internal.generated.IX_PIM.MA_PIM_ERR_ATTRIBUTE_COMBO_UNSUPPORTED;
//import static com.mosync.internal.generated.IX_PIM.MA_PIM_FIELD_CONTACT_EVENT;

public class PIMFieldEvent extends PIMIntegerField {

	final static int MA_PIM_FIELD_CONTACT_EVENT = 122;

	final static int MA_PIM_ATTR_EVENT_BIRTHDAY = 1101;
	final static int MA_PIM_ATTR_EVENT_ANNIVERSARY = 1102;
	final static int MA_PIM_ATTR_EVENT_OTHER = 1103;
	final static int MA_PIM_ATTR_EVENT_CUSTOM = 1104;

	/**
	 * Constructor
	 */
	public PIMFieldEvent() {
		mType = MA_PIM_FIELD_CONTACT_EVENT;
		mStrType = Event.CONTENT_ITEM_TYPE;
		mDataType = MA_PIM_TYPE_DATE;

		mNames = new String[] { Event._ID, Event.START_DATE, Event.TYPE,
				Event.LABEL, Event.IS_PRIMARY };
	}

	void createMaps() {
		// attributes
		mAttributes.put(MA_PIM_ATTR_EVENT_BIRTHDAY, Event.TYPE_BIRTHDAY);
		mAttributes.put(MA_PIM_ATTR_EVENT_ANNIVERSARY, Event.TYPE_ANNIVERSARY);
		mAttributes.put(MA_PIM_ATTR_EVENT_OTHER, Event.TYPE_OTHER);
		mAttributes.put(MA_PIM_ATTR_EVENT_CUSTOM, Event.TYPE_CUSTOM);
	}

	void preProcessData() {
		String[] val = null;
		if ((mValues == null) || (mValues.size() == 0)) {
			return;
		} else {
			for (int i = 0; i < mValues.size(); i++) {
				val = mValues.get(i);
				if (val[1] != null) {
					val[1] = Integer
							.toString((int) (getUtcTimeInMillis(val[1]) / 1000));
					mValues.set(i, val);
				}
			}
		}
	}

	public long getUtcTimeInMillis(String datetime) {
		SimpleDateFormat sdf = new SimpleDateFormat("yyyy-MM-dd");
		Date date = new Date();
		try {
			date = sdf.parse(datetime);
		} catch (ParseException e) {
			e.printStackTrace();
		}

		// getInstance() provides TZ info which can be used to adjust to UTC
		Calendar cal = Calendar.getInstance();
		cal.setTime(date);

		// Get timezone offset then use it to adjust the return value
		int offset = cal.getTimeZone().getOffset(cal.getTimeInMillis());
		return cal.getTimeInMillis() + offset;
	}

	int checkForPreferredAttribute(int index) {
		if (Integer.parseInt(getColumnValue(index, Event.IS_PRIMARY)) != 0)
			return MA_PIM_ATTRPREFERRED;
		return 0;
	}

	/**
	 * Gets the field attribute.
	 */
	int getAndroidAttribute(int index) {
		String attribute = null;
		if ((attribute = getColumnValue(index, Event.TYPE)) == null) {
			return -1;
		}
		try {
			return Integer.parseInt(attribute);
		} catch (Exception e) {
			return -1;
		}
	}

	int setAttribute(int index, int attribute) {
		if ((attribute | MA_PIM_ATTRPREFERRED) != 0) {
			setColumnValue(index, Event.IS_PRIMARY, Integer.toString(1));
		}
		attribute &= 0xFFFF;

		int ret = 0;
		if (mAttributes.containsKey(attribute)) {
			ret = mAttributes.get(attribute);
		} else {
			return MA_PIM_ERR_ATTRIBUTE_COMBO_UNSUPPORTED;
		}

		setColumnValue(index, Event.TYPE, Integer.toString(ret));

		return MA_PIM_ERR_NONE;
	}

	/**
	 * Gets the field's custom label.
	 * @param index
	 * @return
	 */
	char[] getLabel(int index) {
		return getColumnValue(index, Event.LABEL).toCharArray();
	}

	/**
	 * Gets the field's custom label.
	 * @param index
	 * @return
	 */
	void setLabel(int index, String label) {
		setColumnValue(index, Event.LABEL, label);
	}

	/**
	 * Checks to see if the given field has a custom label.
	 * @param index
	 */
	boolean hasCustomLabel(int index) {
		return ((Integer.parseInt(getColumnValue(index, Event.TYPE)) == Event.TYPE_CUSTOM) ? true
				: false);
	}

	void postProcessData() {
		String[] val = null;
		if ((mValues == null) || (mValues.size() == 0)) {
			return;
		} else {
			for (int i = 0; i < mValues.size(); i++) {
				val = mValues.get(i);
				if (val[1] != null) {
					val[1] = getUtcTime(Long.parseLong(val[1]) * 1000);
					mValues.set(i, val);
				}
			}
		}
	}

	public String getUtcTime(long dateTime) {
		Calendar cal = Calendar.getInstance();
		int offset = cal.getTimeZone().getOffset(cal.getTimeInMillis());
		dateTime -= offset;

		SimpleDateFormat sdf = new SimpleDateFormat("yyyy-MM-dd");
		Date date = new Date();
		date.setTime(dateTime);
		return sdf.format(date);
	}

	/**
	 * Print field values.
	 */
	void print() {
		DebugPrint("***********EVENT***********");
		DebugPrint("COUNT = " + mValues.size());
		for (int i = 0; i < mValues.size(); i++) {
			String[] val = mValues.get(i);
			DebugPrint("###Event " + i);
			DebugPrint(mNames[1] + ": " + val[1]);
		}
		DebugPrint("***************************");
	}
}
