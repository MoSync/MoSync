package com.mosync.pim;

import static com.mosync.internal.android.MoSyncHelpers.DebugPrint;

import java.util.ArrayList;
import java.util.Iterator;

import com.mosync.internal.android.SingletonObject;

import android.content.ContentResolver;
import android.database.Cursor;
import android.provider.ContactsContract.Contacts;

import static com.mosync.internal.generated.IX_PIM.MA_PIM_ERR_LIST_UNAVAILABLE;
import static com.mosync.internal.generated.IX_PIM.MA_PIM_ERR_NONE;

public class PIMList {

	ArrayList<PIMItem> mList;
	int mListIterator;

	PIMList() {
		mList = new ArrayList<PIMItem>();
		mListIterator = -1;
	}

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
		return SingletonObject.getSingletonObject().error(errorCode, panicCode,
				panicText);
	}

	/**
	 * Read the list
	 */
	int read(ContentResolver cr) {
		DebugPrint("PIMList.read(" + cr + ")");
		// try to query for contacts
		Cursor cur;
		try {
			cur = cr.query(Contacts.CONTENT_URI, new String[] { Contacts._ID },
					null, null, null);
		} catch (Exception e) {
			return throwError(MA_PIM_ERR_LIST_UNAVAILABLE,
					PIMError.PANIC_LIST_UNAVAILABLE,
					PIMError.sStrListUnavailable);
		}

		if (cur == null) {
			return throwError(MA_PIM_ERR_LIST_UNAVAILABLE,
					PIMError.PANIC_LIST_UNAVAILABLE,
					PIMError.sStrListUnavailable);
		}

		// read each item
		while (cur.moveToNext()) {
			String contactId = cur.getString(cur.getColumnIndex(Contacts._ID));

			PIMItem pimItem = new PIMItem(false);
			pimItem.read(cr, contactId);

			mList.add(pimItem);
		}

		mListIterator = 0;

		return MA_PIM_ERR_NONE;
	}

	/**
	 * Checks if we reached the end of the list.
	 */
	boolean hasNext() {
		return mListIterator < mList.size();
	}

	/**
	 * @return The next element in the list.
	 */
	PIMItem next() {
		return mList.get(mListIterator++);
	}

	/**
	 * @param field
	 * @return The data type of the field.
	 */
	int getFieldDataType(int field) {
		DebugPrint("PIMList.getFieldDataType(" + field + ")");
		return mList.get(0).getFieldDataType(field);
	}

	PIMItem createItem() {
		PIMItem p = new PIMItem(true);
		mList.add(p);
		return p;
	}

	int removeItem(PIMItem item) {
		int index = -1;
		for (int i = 0; i < mList.size(); i++) {
			if (mList.get(i) == item) {
				index = i;
				break;
			}
		}
		if (index == mListIterator) {
			mListIterator--;
		}
		if (index != -1) {
			mList.remove(index);
		}
		return MA_PIM_ERR_NONE;
	}

	/**
	 * Closes the list.
	 */
	void close(ContentResolver cr) {
		DebugPrint("PIMList.close(" + cr + ")");
		mListIterator = 0;
		while (hasNext()) {
			next().close(cr);
		}
	}
}
