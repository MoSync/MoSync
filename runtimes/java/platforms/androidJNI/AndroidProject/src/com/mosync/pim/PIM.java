package com.mosync.pim;

import static com.mosync.internal.android.MoSyncHelpers.DebugPrint;
import static com.mosync.internal.generated.IX_PIM.MA_PIM_CONTACTS;
import static com.mosync.internal.generated.IX_PIM.MA_PIM_ERR_HANDLE_INVALID;
import static com.mosync.internal.generated.IX_PIM.MA_PIM_ERR_LIST_ALREADY_OPENED;
import static com.mosync.internal.generated.IX_PIM.MA_PIM_ERR_LIST_TYPE_INVALID;
import static com.mosync.internal.generated.IX_PIM.MA_PIM_ERR_NONE;

import java.util.Hashtable;

import android.app.Activity;
import android.content.ContentResolver;

import com.mosync.internal.android.MoSyncThread;
import com.mosync.internal.android.SingletonObject;

public class PIM {

	/**
	 * The MoSync thread object.
	 */
	private MoSyncThread mMoSyncThread;

	/**
	 * PIM lists
	 */
	private Hashtable<Integer, PIMList> mPIMLists;
	private Hashtable<Integer, PIMItem> mPIMItems;
	private PIMList mPIMContactsList;

	/**
	 * Handle for PIM
	 */
	private int mResourceIndex = 0;

	/**
	 * @return The Activity object.
	 */
	public Activity getActivity() {
		return mMoSyncThread.getActivity();
	}

	/**
	 * @return The Content Resolver.
	 */
	public ContentResolver getContentResolver() {
		return getActivity().getContentResolver();
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
	 * Constructor.
	 * @param thread
	 *            The MoSync thread.
	 */
	public PIM(MoSyncThread thread) {
		mMoSyncThread = thread;
		mPIMLists = new Hashtable<Integer, PIMList>();
		mPIMItems = new Hashtable<Integer, PIMItem>();
	}

	/**
	 * Opens the PIM list depending on the listType
	 */
	public int maPimListOpen(int listType) {
		DebugPrint("maPimListOpen()");
		switch (listType) {
		case MA_PIM_CONTACTS:
			return openContactsList();
		}

		return throwError(MA_PIM_ERR_LIST_TYPE_INVALID,
				PIMError.PANIC_LIST_TYPE_INVALID, PIMError.sStrListTypeInvalid);
	}

	/**
	 * @return the contact list
	 */
	PIMList getContactList() {
		if (mPIMContactsList == null) {
			mPIMContactsList = new PIMList();
		}
		return mPIMContactsList;
	}

	/**
	 * @return false if the contact list is null
	 */
	boolean isContactListOpened() {
		return ((mPIMContactsList != null) ? true : false);
	}

	/**
	 * Opens the contacts list.
	 */
	int openContactsList() {
		DebugPrint("openContactsList()");
		// if opened, return error code
		if (isContactListOpened()) {
			return throwError(MA_PIM_ERR_LIST_ALREADY_OPENED,
					PIMError.PANIC_LIST_ALREADY_OPENED,
					PIMError.sStrListAlreadyOpened);
		}

		// try to read the items in the list
		// if failed, return error code
		int error = 0;
		if ((error = getContactList().read(getContentResolver())) < 0) {
			return error;
		}

		// associate a handle to the list
		mPIMLists.put(mResourceIndex, mPIMContactsList);

		return mResourceIndex++;
	}

	public int maPimListNext(int list) {
		DebugPrint("maPimListNext(" + list + ")");
		PIMList pimList = null;
		if ((list < 0) || ((pimList = mPIMLists.get(list)) == null)) {
			return throwError(MA_PIM_ERR_HANDLE_INVALID,
					PIMError.PANIC_HANDLE_INVALID, PIMError.sStrHandleInvalid);
		}

		if (pimList.hasNext()) {
			mPIMItems.put(mResourceIndex, pimList.next());
		} else {
			return MA_PIM_ERR_NONE;
		}

		return mResourceIndex++;
	}

	public int maPimListClose(int list) {
		DebugPrint("maPimListClose(" + list + ")");
		PIMList pimList = null;
		if ((list < 0) || ((pimList = mPIMLists.get(list)) == null)) {
			return throwError(MA_PIM_ERR_HANDLE_INVALID,
					PIMError.PANIC_HANDLE_INVALID, PIMError.sStrHandleInvalid);
		}

		pimList.close(getContentResolver());

		mPIMContactsList = mPIMLists.remove(list);
		mPIMContactsList = null;

		return MA_PIM_ERR_NONE;
	}

	public int maPimItemCount(int item) {
		DebugPrint("maPimItemCount(" + item + ")");
		PIMItem pimItem = null;
		if ((item < 0) || ((pimItem = mPIMItems.get(item)) == null)) {
			return throwError(MA_PIM_ERR_HANDLE_INVALID,
					PIMError.PANIC_HANDLE_INVALID, PIMError.sStrHandleInvalid);
		}

		return pimItem.length();
	}

	public int maPimItemGetField(int item, int n) {
		DebugPrint("maPimItemGetField(" + item + ", " + n + ")");
		PIMItem pimItem = null;
		if ((item < 0) || ((pimItem = mPIMItems.get(item)) == null)) {
			return throwError(MA_PIM_ERR_HANDLE_INVALID,
					PIMError.PANIC_HANDLE_INVALID, PIMError.sStrHandleInvalid);
		}

		return pimItem.getFieldType(n);
	}

	public int maPimItemFieldCount(int item, int field) {
		DebugPrint("maPimItemFieldCount(" + item + ", " + field + ")");
		PIMItem pimItem = null;
		if ((item < 0) || ((pimItem = mPIMItems.get(item)) == null)) {
			return throwError(MA_PIM_ERR_HANDLE_INVALID,
					PIMError.PANIC_HANDLE_INVALID, PIMError.sStrHandleInvalid);
		}

		return pimItem.getFieldLength(field);
	}

	public int maPimItemGetAttributes(int item, int field, int index) {
		DebugPrint("maPimItemGetAttributes(" + item + ", " + field + ", "
				+ index + ")");
		PIMItem pimItem = null;
		if ((item < 0) || ((pimItem = mPIMItems.get(item)) == null)) {
			return throwError(MA_PIM_ERR_HANDLE_INVALID,
					PIMError.PANIC_HANDLE_INVALID, PIMError.sStrHandleInvalid);
		}

		return pimItem.getFieldAttributes(field, index);
	}

	public int maPimItemSetLabel(int item, int field, int buffPointer,
			int buffSize, int index) {
		DebugPrint("maPimItemSetLabel(" + item + ", " + field + ", "
				+ buffPointer + ", " + buffSize + ", " + index + ")");
		PIMItem pimItem = null;
		if ((item < 0) || ((pimItem = mPIMItems.get(item)) == null)) {
			return throwError(MA_PIM_ERR_HANDLE_INVALID,
					PIMError.PANIC_HANDLE_INVALID, PIMError.sStrHandleInvalid);
		}

		return pimItem.setFieldLabel(field, index, buffPointer, buffSize);
	}

	public int maPimItemGetLabel(int item, int field, int buffPointer,
			int buffSize, int index) {
		DebugPrint("maPimItemGetLabel(" + item + ", " + field + ", "
				+ buffPointer + ", " + buffSize + ", " + index + ")");
		PIMItem pimItem = null;
		if ((item < 0) || ((pimItem = mPIMItems.get(item)) == null)) {
			return throwError(MA_PIM_ERR_HANDLE_INVALID,
					PIMError.PANIC_HANDLE_INVALID, PIMError.sStrHandleInvalid);
		}

		return pimItem.getFieldLabel(field, index, buffPointer, buffSize);
	}

	public int maPimFieldType(int list, int field) {
		DebugPrint("maPimFieldType(" + list + ", " + field + ")");
		PIMList pimList = null;
		if ((list < 0) || ((pimList = mPIMLists.get(list)) == null)) {
			return throwError(MA_PIM_ERR_HANDLE_INVALID,
					PIMError.PANIC_HANDLE_INVALID, PIMError.sStrHandleInvalid);
		}

		return pimList.getFieldDataType(field);
	}

	public int maPimItemGetValue(int item, int field, int buffPointer,
			int buffSize, int index) {
		DebugPrint("maPimItemGetValue(" + item + ", " + field + ", "
				+ buffPointer + ", " + buffSize + ", " + index + ")");
		PIMItem pimItem = null;
		if ((item < 0) || ((pimItem = mPIMItems.get(item)) == null)) {
			return throwError(MA_PIM_ERR_HANDLE_INVALID,
					PIMError.PANIC_HANDLE_INVALID, PIMError.sStrHandleInvalid);
		}

		return pimItem.getFieldValue(field, index, buffPointer, buffSize);
	}

	public int maPimItemSetValue(int item, int field, int buffPointer,
			int buffSize, int index, int attributes) {
		DebugPrint("maPimItemSetValue(" + item + ", " + field + ", "
				+ buffPointer + ", " + buffSize + ", " + index + ")");
		PIMItem pimItem = null;
		if ((item < 0) || ((pimItem = mPIMItems.get(item)) == null)) {
			return throwError(MA_PIM_ERR_HANDLE_INVALID,
					PIMError.PANIC_HANDLE_INVALID, PIMError.sStrHandleInvalid);
		}

		return pimItem.setFieldValue(field, index, buffPointer, buffSize,
				attributes);
	}

	public int maPimItemAddValue(int item, int field, int buffPointer,
			int buffSize, int attributes) {
		DebugPrint("maPimItemAddValue(" + item + ", " + field + ", "
				+ buffPointer + ", " + buffSize + ")");
		PIMItem pimItem = null;
		if ((item < 0) || ((pimItem = mPIMItems.get(item)) == null)) {
			return throwError(MA_PIM_ERR_HANDLE_INVALID,
					PIMError.PANIC_HANDLE_INVALID, PIMError.sStrHandleInvalid);
		}

		return pimItem.addFieldValue(field, buffPointer, buffSize, attributes);
	}

	public int maPimItemRemoveValue(int item, int field, int index) {
		DebugPrint("maPimItemRemoveValue(" + item + ", " + field + ", " + index
				+ ")");
		PIMItem pimItem = null;
		if ((item < 0) || ((pimItem = mPIMItems.get(item)) == null)) {
			return throwError(MA_PIM_ERR_HANDLE_INVALID,
					PIMError.PANIC_HANDLE_INVALID, PIMError.sStrHandleInvalid);
		}

		return pimItem.removeFieldValue(field, index);
	}

	public int maPimItemClose(int item) {
		DebugPrint("maPimItemClose(" + item + ")");
		PIMItem pimItem = null;
		if ((item < 0) || ((pimItem = mPIMItems.get(item)) == null)) {
			return throwError(MA_PIM_ERR_HANDLE_INVALID,
					PIMError.PANIC_HANDLE_INVALID, PIMError.sStrHandleInvalid);
		}
		pimItem.close(getContentResolver());

		return MA_PIM_ERR_NONE;
	}

	public int maPimItemCreate(int list) {
		DebugPrint("maPimListNext(" + list + ")");
		PIMList pimList = null;
		if ((list < 0) || ((pimList = mPIMLists.get(list)) == null)) {
			return throwError(MA_PIM_ERR_HANDLE_INVALID,
					PIMError.PANIC_HANDLE_INVALID, PIMError.sStrHandleInvalid);
		}

		mPIMItems.put(mResourceIndex, pimList.createItem());

		return mResourceIndex++;
	}

	public int maPimItemRemove(int list, int item) {
		DebugPrint("maPimItemRemove(" + list + ", " + item + ")");
		PIMList pimList = null;
		if ((list < 0) || ((pimList = mPIMLists.get(list)) == null)) {
			return throwError(MA_PIM_ERR_HANDLE_INVALID,
					PIMError.PANIC_HANDLE_INVALID, PIMError.sStrHandleInvalid);
		}
		PIMItem pimItem = null;
		if ((item < 0) || ((pimItem = mPIMItems.get(item)) == null)) {
			return throwError(MA_PIM_ERR_HANDLE_INVALID,
					PIMError.PANIC_HANDLE_INVALID, PIMError.sStrHandleInvalid);
		}
		pimItem.delete(getContentResolver());
		return pimList.removeItem(pimItem);
	}
}
