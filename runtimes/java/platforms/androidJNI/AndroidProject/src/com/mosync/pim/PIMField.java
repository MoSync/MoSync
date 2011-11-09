package com.mosync.pim;

import static com.mosync.internal.android.MoSyncHelpers.DebugPrint;
import static com.mosync.internal.generated.IX_PIM.MA_PIM_ERR_NONE;
import static com.mosync.internal.generated.IX_PIM.MA_PIM_ERR_FIELD_EMPTY;
import static com.mosync.internal.generated.IX_PIM.MA_PIM_ERR_FIELD_READ_ONLY;
import static com.mosync.internal.generated.IX_PIM.MA_PIM_ERR_FIELD_WRITE_ONLY;
import static com.mosync.internal.generated.IX_PIM.MA_PIM_ERR_INDEX_INVALID;
import static com.mosync.internal.generated.IX_PIM.MA_PIM_ERR_NO_LABEL;
import static com.mosync.internal.generated.IX_PIM.MA_PIM_ERR_BUFFER_INVALID;
import static com.mosync.internal.generated.IX_PIM.MA_PIM_ERR_FIELD_COUNT_MAX;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.Map;

import com.mosync.internal.android.MoSyncError;

import android.content.ContentProviderOperation;
import android.content.ContentResolver;
import android.database.Cursor;
import android.provider.ContactsContract.Data;

abstract class PIMField {

	protected static final String DUMMY = Data.DATA15;
	protected int MAX_SIZE;

	enum State {
		NONE, ADDED, UPDATED
	}

	enum Permission {
		FULL, READ_ONLY, WRITE_ONLY
	}

	int mType;
	String mStrType;
	int mDataType;
	Permission mPermission;
	String[] mNames;
	ArrayList<String[]> mValues;
	ArrayList<State> mStates;
	ArrayList<Long> mDeletedValues;
	Map<Integer, Integer> mAttributes;

	PIMField() {
		MAX_SIZE = Integer.MAX_VALUE;

		mValues = new ArrayList<String[]>();
		mStates = new ArrayList<State>();
		mDeletedValues = new ArrayList<Long>();
		mAttributes = new HashMap<Integer, Integer>();
		mPermission = Permission.FULL;

		createMaps();
	}

	abstract void createMaps();

	/**
	 * @param errorCode
	 *            The error code returned by the syscall.
	 * @param panicCode
	 *            The panic code for this error.
	 * @param panicText
	 *            The panic text for this error.
	 * @return
	 */
	public int throwError(int errorCode, int panicCode, String panicText) {
		return MoSyncError.getSingletonObject().error(errorCode, panicCode,
				panicText);
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
				if (!mNames[i].equals(DUMMY)) {
					int index = cursor.getColumnIndex(mNames[i]);
					if (index >= 0) {
						val[i] = cursor.getString(index);
					}
				}
				if (val[i] == null) {
					val[i] = new String("");
				}
			}
			mValues.add(val);
			mStates.add(State.NONE);
		}

		cursor.close();
		cursor = null;

		preProcessData();

		print();
		DebugPrint("MAX SIZE = " + MAX_SIZE);
	}

	void preProcessData() {

	}

	abstract void print();

	boolean isEmpty() {
		return ((length() == 0) ? true : false);
	}

	int getType() {
		return mType;
	}

	int length() {
		return mValues.size();
	}

	/**
	 * Get field's attributes.
	 */
	int getAttributes(int index) {
		if (isEmpty()) {
			return throwError(MA_PIM_ERR_FIELD_EMPTY,
					PIMError.PANIC_FIELD_EMPTY, PIMError.sStrFieldEmpty);
		}
		if ((index < 0) || (index >= length())) {
			return throwError(MA_PIM_ERR_INDEX_INVALID,
					PIMError.PANIC_INDEX_INVALID, PIMError.sStrIndexInvalid);
		}

		int attr = getAndroidAttribute(index);

		if (attr < 0) {
			return 0;
		}

		int ret = ((Integer) PIMUtil.getKeyFromValue(mAttributes, attr))
				.intValue();

		ret |= checkForPreferredAttribute(index);

		return ret;
	}

	abstract int checkForPreferredAttribute(int index);

	/**
	 * Gets the field attribute.
	 */
	abstract int getAndroidAttribute(int index);

	/**
	 * Gets the custom label of the specified field.
	 */
	int getLabel(int index, int buffPointer, int buffSize) {
		if (isEmpty()) {
			return throwError(MA_PIM_ERR_FIELD_EMPTY,
					PIMError.PANIC_FIELD_EMPTY, PIMError.sStrFieldEmpty);
		}

		if ((index < 0) || (index >= length())) {
			return throwError(MA_PIM_ERR_INDEX_INVALID,
					PIMError.PANIC_INDEX_INVALID, PIMError.sStrIndexInvalid);
		}

		if (!hasCustomLabel(index)) {
			return MA_PIM_ERR_NO_LABEL;
		}

		char[] buffer = getLabel(index);

		if (buffer.length > (buffSize >> 1))
			return (buffer.length << 1);

		PIMUtil.copyBufferToMemory(buffPointer, buffer);

		return buffer.length;
	}

	/**
	 * Gets the field's custom label.
	 * @param index
	 * @return
	 */
	abstract char[] getLabel(int index);

	/**
	 * Sets the custom label of the specified field.
	 */
	int setLabel(int index, int buffPointer, int buffSize) {
		DebugPrint("PIMField.setLabel(" + index + ", " + buffPointer + ", "
				+ buffSize + ")");
		if (isEmpty()) {
			return throwError(MA_PIM_ERR_FIELD_EMPTY,
					PIMError.PANIC_FIELD_EMPTY, PIMError.sStrFieldEmpty);
		}

		if ((index < 0) || (index >= length())) {
			return throwError(MA_PIM_ERR_INDEX_INVALID,
					PIMError.PANIC_INDEX_INVALID, PIMError.sStrIndexInvalid);
		}

		if (isReadOnly()) {
			return MA_PIM_ERR_FIELD_READ_ONLY;
		}

		if (!hasCustomLabel(index)) {
			return MA_PIM_ERR_NO_LABEL;
		}

		char[] buffer = PIMUtil
				.readBufferFromMemory(buffPointer, buffSize >> 1);

		setLabel(index, new String(buffer));

		return MA_PIM_ERR_NONE;
	}

	/**
	 * Gets the field's custom label.
	 * @param index
	 * @return
	 */
	abstract void setLabel(int index, String label);

	/**
	 * Checks to see if the given field has a custom label.
	 * @param index
	 */
	abstract boolean hasCustomLabel(int index);

	/**
	 * Gets the value of the specified field.
	 */
	int getValue(int index, int buffPointer, int buffSize) {
		if (isEmpty()) {
			return throwError(MA_PIM_ERR_FIELD_EMPTY,
					PIMError.PANIC_FIELD_EMPTY, PIMError.sStrFieldEmpty);
		}

		if ((index < 0) || (index >= length())) {
			return throwError(MA_PIM_ERR_INDEX_INVALID,
					PIMError.PANIC_INDEX_INVALID, PIMError.sStrIndexInvalid);
		}

		if (isWriteOnly()) {
			return MA_PIM_ERR_FIELD_WRITE_ONLY;
		}

		char[] buffer = getData(index);

		if (buffer.length > (buffSize >> 1))
			return (buffer.length << 1);

		PIMUtil.copyBufferToMemory(buffPointer, buffer);

		return buffer.length;
	}

	abstract char[] getData(int index);

	/**
	 * Sets the value of the specified field.
	 */
	int setValue(int index, int buffPointer, int buffSize, int attributes) {
		if (isReadOnly()) {
			return MA_PIM_ERR_FIELD_READ_ONLY;
		}

		if (isEmpty()) {
			return throwError(MA_PIM_ERR_FIELD_EMPTY,
					PIMError.PANIC_FIELD_EMPTY, PIMError.sStrFieldEmpty);
		}

		if ((index < 0) || (index >= length())) {
			return throwError(MA_PIM_ERR_INDEX_INVALID,
					PIMError.PANIC_INDEX_INVALID, PIMError.sStrIndexInvalid);
		}

		char[] buffer = PIMUtil
				.readBufferFromMemory(buffPointer, buffSize >> 1);
		if (buffer.length == 0) {
			return MA_PIM_ERR_BUFFER_INVALID;
		}

		setData(index, buffer);

		if (mStates.get(index) != State.ADDED) {
			DebugPrint("set state to UPDATED");
			mStates.set(index, State.UPDATED);
		}

		return setAttribute(index, attributes);
	}

	abstract void setData(int index, char[] buffer);

	/**
	 * Sets the value of the specified field.
	 */
	int addValue(int buffPointer, int buffSize, int attributes) {
		if (isReadOnly()) {
			return MA_PIM_ERR_FIELD_READ_ONLY;
		}

		if (length() >= MAX_SIZE) {
			return MA_PIM_ERR_FIELD_COUNT_MAX;
		}

		char[] buffer = PIMUtil
				.readBufferFromMemory(buffPointer, buffSize >> 1);
		if (buffer.length == 0) {
			return MA_PIM_ERR_BUFFER_INVALID;
		}

		String[] val = new String[mNames.length];
		int index = mValues.size();
		mValues.add(val);
		mStates.add(State.ADDED);

		setData(index, buffer);

		int err = MA_PIM_ERR_NONE;
		if ((err = setAttribute(index, attributes)) != MA_PIM_ERR_NONE) {
			return err;
		}

		print();

		return index;
	}

	abstract int setAttribute(int index, int attribute);

	/**
	 * Removes the value of the specified field.
	 */
	int removeValue(int index) {
		if (isReadOnly()) {
			return MA_PIM_ERR_FIELD_READ_ONLY;
		}

		if (isEmpty()) {
			return throwError(MA_PIM_ERR_FIELD_EMPTY,
					PIMError.PANIC_FIELD_EMPTY, PIMError.sStrFieldEmpty);
		}

		if ((index < 0) || (index >= length())) {
			return throwError(MA_PIM_ERR_INDEX_INVALID,
					PIMError.PANIC_INDEX_INVALID, PIMError.sStrIndexInvalid);
		}

		if (mStates.get(index) != State.ADDED) {
			mDeletedValues.add(Long.parseLong(mValues.get(index)[0]));
		}

		mValues.remove(index);
		mStates.remove(index);

		return MA_PIM_ERR_NONE;
	}

	int getDataType() {
		return mDataType;
	}

	boolean isWriteOnly() {
		return (mPermission == Permission.WRITE_ONLY);
	}

	boolean isReadOnly() {
		return (mPermission == Permission.READ_ONLY);
	}

	boolean isSupported() {
		return true;
	}

	/**
	 * Gets the field's value corresponding to the given column name.
	 * @param index
	 * @param name
	 * @return
	 */
	String getColumnValue(int index, String name) {
		String[] val = mValues.get(index);
		for (int i = 0; i < mNames.length; i++) {
			if (mNames[i].equals(name)) {
				return val[i];
			}
		}
		return null;
	}

	/**
	 * Sets the field's value corresponding to the given column name.
	 * @param index
	 * @param name
	 * @return
	 */
	void setColumnValue(int index, String name, String value) {
		String[] val = mValues.get(index);
		for (int i = 0; i < mNames.length; i++) {
			if (mNames[i].equals(name)) {
				val[i] = value;
				mValues.set(index, val);
				break;
			}
		}
	}

	void postProcessData() {

	}

	void add(ArrayList<ContentProviderOperation> ops, int contactId) {
		if (isReadOnly()) {
			return;
		}
		DebugPrint("ADD " + mStrType);
		print();
		postProcessData();
		for (int i = 0; i < mValues.size(); i++) {
			addToDisk(ops, contactId, mNames, mValues.get(i));
		}
	}

	void update(ContentResolver cr, ArrayList<ContentProviderOperation> ops,
			int contactId) {
		if (isReadOnly()) {
			return;
		}
		postProcessData();

		for (int i = 0; i < mValues.size(); i++) {
			if (mStates.get(i) == State.ADDED) {
				addToDisk(ops, contactId, mNames, mValues.get(i));
			} else if (mStates.get(i) == State.UPDATED) {
				updateToDisk(ops, contactId, mNames, mValues.get(i));
			}
		}

		for (int i = 0; i < mDeletedValues.size(); i++) {
			deleteFromDisk(ops, contactId, mDeletedValues.get(i));
		}
	}

	void addToDisk(ArrayList<ContentProviderOperation> ops,
			int rawContactInsertIndex, String[] names, String[] values) {
		ContentProviderOperation.Builder builder = ContentProviderOperation
				.newInsert(Data.CONTENT_URI)
				.withValueBackReference(Data.RAW_CONTACT_ID,
						rawContactInsertIndex)
				.withValue(Data.MIMETYPE, mStrType);

		for (int i = 0; i < names.length; i++) {
			if ((values[i] != null) && (!names[i].equals(DUMMY))) {
				builder = builder.withValue(names[i], values[i]);
			}
		}

		ops.add(builder.build());
	}

	void updateToDisk(ArrayList<ContentProviderOperation> ops,
			int rawContactId, String[] names, String[] values) {
		DebugPrint("UPDATE");
		ContentProviderOperation.Builder builder = ContentProviderOperation
				.newUpdate(Data.CONTENT_URI).withSelection(
						Data.CONTACT_ID + "=?" + " AND " + Data.MIMETYPE + "=?"
								+ " AND " + Data._ID + "=?",
						new String[] { Integer.toString(rawContactId),
								mStrType, values[0] });

		for (int i = 1; i < names.length; i++) {
			if ((values[i] != null) && (!names[i].equals(DUMMY))) {
				builder = builder.withValue(names[i], values[i]);
			}
		}

		ops.add(builder.build());
	}

	void deleteFromDisk(ArrayList<ContentProviderOperation> ops,
			int rawContactId, long id) {
		DebugPrint("DELETE");
		ops.add(ContentProviderOperation
				.newDelete(Data.CONTENT_URI)
				.withSelection(
						Data.CONTACT_ID + "=?" + " AND " + Data.MIMETYPE + "=?"
								+ " AND " + Data._ID + "=?",
						new String[] { Integer.toString(rawContactId),
								mStrType, Long.toString(id) }).build());
	}

	void close() {
	}
}
