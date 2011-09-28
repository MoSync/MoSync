package com.mosync.pim;

import static com.mosync.internal.android.MoSyncHelpers.DebugPrint;

import java.nio.Buffer;
import java.nio.ByteBuffer;
import java.util.ArrayList;

import static com.mosync.internal.generated.IX_PIM.MA_PIM_ATTRPREFERRED;
import static com.mosync.internal.generated.IX_PIM.MA_PIM_ERR_ATTRIBUTE_COMBO_UNSUPPORTED;
import static com.mosync.internal.generated.IX_PIM.MA_PIM_ERR_FIELD_EMPTY;
import static com.mosync.internal.generated.IX_PIM.MA_PIM_ERR_INDEX_INVALID;
import static com.mosync.internal.generated.IX_PIM.MA_PIM_ERR_NONE;
import static com.mosync.internal.generated.IX_PIM.MA_PIM_FIELD_CONTACT_PHOTO;
import static com.mosync.internal.generated.IX_PIM.MA_PIM_TYPE_INT;

import android.content.ContentProviderOperation;
import android.content.ContentResolver;
import android.database.Cursor;
import android.provider.BaseColumns;
import android.provider.ContactsContract.CommonDataKinds.Photo;
import android.provider.ContactsContract.Contacts;
import android.provider.ContactsContract.Data;

public class PIMFieldPhoto extends PIMField {

	/**
	 * Constructor
	 */
	public PIMFieldPhoto() {
		MAX_SIZE = 1;

		mType = MA_PIM_FIELD_CONTACT_PHOTO;
		mStrType = Photo.CONTENT_ITEM_TYPE;
		mDataType = MA_PIM_TYPE_INT;

		mNames = new String[] { Photo._ID, Photo.PHOTO, Photo.IS_PRIMARY };
	}

	void createMaps() {
	}

	/**
	 * Read field
	 * @param cr
	 * @param contactId
	 */
	void read(ContentResolver cr, String contactId) {
		DebugPrint("PIMFieldPhoto.read(" + cr + ", " + contactId + ")");
		Cursor cursor = cr.query(Data.CONTENT_URI, mNames, Data.CONTACT_ID
				+ "=?" + " AND " + Data.MIMETYPE + "=?",
				new String[] { String.valueOf(contactId), mStrType }, null);

		while (cursor.moveToNext()) {
			String[] val = new String[mNames.length];
			for (int i = 0; i < mNames.length; i++) {
				val[i] = new String("");
				int index = cursor.getColumnIndex(mNames[i]);
				if (index >= 0) {
					if (mNames[i].equals(Photo.PHOTO)) {
						byte[] b = cursor.getBlob(index);
						if (b != null) {
							val[i] = Integer.toString(loadPhoto(b));
						}
					} else {
						val[i] = cursor.getString(index);
					}
				}
			}
			mValues.add(val);
			mStates.add(State.NONE);
		}

		print();
	}

	public int loadPhoto(byte[] buffer) {
		ByteBuffer bBuffer = ByteBuffer.wrap(buffer);

		int photoHandle = PIMUtil.getThread().nativeCreatePlaceholder();
		PIMUtil.getThread().loadBinary(photoHandle, bBuffer);
		return photoHandle;
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
		if (Integer.parseInt(getColumnValue(index, Photo.IS_PRIMARY)) != 0)
			return MA_PIM_ATTRPREFERRED;
		return 0;
	}

	int getAndroidAttribute(int index) {
		return 0;
	}

	int setAttribute(int index, int attribute) {
		if ((attribute | MA_PIM_ATTRPREFERRED) != 0) {
			setColumnValue(index, Photo.IS_PRIMARY, Integer.toString(1));
		}
		attribute &= 0xFFFF;

		if (attribute != 0) {
			return MA_PIM_ERR_ATTRIBUTE_COMBO_UNSUPPORTED;
		}

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

	void addToDisk(ArrayList<ContentProviderOperation> ops,
			int rawContactInsertIndex, String[] names, String[] values) {
		ContentProviderOperation.Builder builder = ContentProviderOperation
				.newInsert(Data.CONTENT_URI)
				.withValueBackReference(Data.RAW_CONTACT_ID,
						rawContactInsertIndex)
				.withValue(Data.MIMETYPE, mStrType);

		for (int i = 0; i < names.length; i++) {
			DebugPrint(names[i] + ": " + values[i]);
			if (values[i] != null) {
				if (names[i].equals(Photo.PHOTO)) {
					byte[] buffer = PIMUtil.getThread()
							.getUnloadedBinaryResourceAsByteArray(
									Integer.parseInt(values[i]));
					DebugPrint("Buffer: " + buffer);
					if (buffer != null) {
						builder = builder.withValue(names[i], buffer);
					}
				} else {
					builder = builder.withValue(names[i], values[i]);
				}
			}
		}

		ops.add(builder.build());
	}

	void updateToDisk(ArrayList<ContentProviderOperation> ops,
			int rawContactId, String[] names, String[] values) {
		DebugPrint("UPDATE");
		DebugPrint("Data: " + rawContactId + "   " + mStrType + "   "
				+ values[0]);
		ContentProviderOperation.Builder builder = ContentProviderOperation
				.newUpdate(Data.CONTENT_URI).withSelection(
						Data.CONTACT_ID + "=?" + " AND " + Data.MIMETYPE + "=?"
								+ " AND " + Data._ID + "=?",
						new String[] { Integer.toString(rawContactId),
								mStrType, values[0] });

		for (int i = 1; i < names.length; i++) {
			DebugPrint(names[i] + ": " + values[i]);
			if (values[i] != null) {
				if (names[i].equals(Photo.PHOTO)) {
					byte[] buffer = PIMUtil.getThread()
							.getUnloadedBinaryResourceAsByteArray(
									Integer.parseInt(values[i]));
					DebugPrint("Buffer: " + buffer);
					if (buffer != null) {
						builder = builder.withValue(names[i], buffer);
					}
				} else {
					// builder = builder.withValue(names[i], values[i]);
				}
			}
		}

		ops.add(builder.build());
	}

	void close() {
		for (int i = 0; i < mValues.size(); i++) {
			PIMUtil.getThread().destroyBinary(
					Integer.parseInt(mValues.get(i)[1]));
		}
	}

	/**
	 * Print field values.
	 */
	void print() {
		String[] val = null;
		DebugPrint("***********PHOTO***********");
		if ((mValues == null) || (mValues.size() == 0)
				|| ((val = mValues.get(0)) == null)) {
			DebugPrint("Not available");
		} else {
			DebugPrint("Photo: " + val[1]);
		}
		DebugPrint("***************************");
	}
}
