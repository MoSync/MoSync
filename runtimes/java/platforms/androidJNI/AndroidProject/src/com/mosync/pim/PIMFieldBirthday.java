package com.mosync.pim;

import static com.mosync.internal.android.MoSyncHelpers.DebugPrint;

import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.Calendar;
import java.util.Date;

import static com.mosync.internal.generated.IX_PIM.MA_PIM_ATTRPREFERRED;
import static com.mosync.internal.generated.IX_PIM.MA_PIM_ERR_ATTRIBUTE_COMBO_UNSUPPORTED;
import static com.mosync.internal.generated.IX_PIM.MA_PIM_ERR_FIELD_EMPTY;
import static com.mosync.internal.generated.IX_PIM.MA_PIM_ERR_INDEX_INVALID;
import static com.mosync.internal.generated.IX_PIM.MA_PIM_ERR_NONE;
import static com.mosync.internal.generated.IX_PIM.MA_PIM_FIELD_CONTACT_BIRTHDAY;
import static com.mosync.internal.generated.IX_PIM.MA_PIM_TYPE_DATE;

import android.provider.ContactsContract.CommonDataKinds.Email;
import android.provider.ContactsContract.CommonDataKinds.Event;
import android.provider.ContactsContract.CommonDataKinds.StructuredName;

public class PIMFieldBirthday extends PIMField {

	/**
	 * Constructor
	 */
	public PIMFieldBirthday() {
		MAX_SIZE = 1;
		mType = MA_PIM_FIELD_CONTACT_BIRTHDAY;
		mStrType = Event.CONTENT_ITEM_TYPE;
		mDataType = MA_PIM_TYPE_DATE;

		mNames = new String[] { Event._ID, Event.START_DATE, Event.TYPE,
				Event.LABEL, Event.IS_PRIMARY };
	}

	void createMaps() {
	}

	void preProcessData() {
		String[] val = null;
		if ((mValues == null) || (mValues.size() == 0)
				|| ((val = mValues.get(0)) == null)) {
			return;
		} else {
			val[1] = Integer
					.toString((int) (getUtcTimeInMillis(val[1]) / 1000));
			mValues.set(0, val);
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
		if (Integer.parseInt(getColumnValue(index, StructuredName.IS_PRIMARY)) != 0)
			return MA_PIM_ATTRPREFERRED;
		return 0;
	}

	int getAndroidAttribute(int index) {
		return 0;
	}

	int setAttribute(int index, int attribute) {
		if ((attribute | MA_PIM_ATTRPREFERRED) != 0) {
			setColumnValue(index, Email.IS_PRIMARY, Integer.toString(1));
		}
		attribute &= 0xFFFF;

		if (attribute != 0) {
			return MA_PIM_ERR_ATTRIBUTE_COMBO_UNSUPPORTED;
		}

		setColumnValue(index, Event.TYPE, Integer.toString(Event.TYPE_BIRTHDAY));

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

	char[] getData(int index) {
		String val = getSpecificData(index);
		char[] buffer = new char[getDataSize(val)];
		PIMUtil.writeInt(Integer.parseInt(val), buffer, 0);
		return buffer;
	}

	String getSpecificData(int index) {
		String[] val = mValues.get(index);
		return val[1];
	}

	int getDataSize(String val) {
		return (Integer.SIZE / 8);
	}

	void setData(int index, char[] buffer) {
		String val = Integer.toString(PIMUtil.readInt(buffer, 0));
		setSpecificData(val, index);
	}

	void setSpecificData(String data, int index) {
		String[] val = mValues.get(index);
		val[1] = data;
		mValues.set(index, val);
	}

	void postProcessData() {
		String[] val = null;
		if ((mValues == null) || (mValues.size() == 0)
				|| ((val = mValues.get(0)) == null)) {
			return;
		} else {
			val[1] = getUtcTime(Long.parseLong(val[1]) * 1000);
			mValues.set(0, val);
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
		String[] val = null;
		DebugPrint("**********BIRTHDAY*********");
		if ((mValues == null) || (mValues.size() == 0)
				|| ((val = mValues.get(0)) == null)) {
			DebugPrint("Not available");
		} else {
			DebugPrint("Birthday: " + val[1]);
		}
		DebugPrint("***************************");
	}
}
