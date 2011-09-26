package com.mosync.internal.android;

import com.mosync.pim.PIM;
import com.mosync.pim.PIMUtil;

import android.app.Activity;

/**
 * Wrapper for PIM Syscalls to avoid cluttering the MoSyncSyscalls file.
 */
class MoSyncPIM {
	/**
	 * The MoSync thread object.
	 */
	MoSyncThread mMoSyncThread;

	private PIM mPIM;

	/**
	 * Constructor.
	 * @param thread
	 *            The MoSync thread.
	 */
	public MoSyncPIM(MoSyncThread thread) {
		mMoSyncThread = thread;
		mPIM = new PIM(thread);
		PIMUtil.sMoSyncThread = thread;
	}

	/**
	 * @return The Activity object.
	 */
	private Activity getActivity() {
		return mMoSyncThread.getActivity();
	}

	/**
	 * PIM related methods
	 */
	int maPimListOpen(int listType) {
		return mPIM.maPimListOpen(listType);
	}

	int maPimListNext(int list) {
		return mPIM.maPimListNext(list);
	}

	int maPimListClose(int list) {
		return mPIM.maPimListClose(list);
	}

	int maPimItemCount(int item) {
		return mPIM.maPimItemCount(item);
	}

	int maPimItemGetField(int item, int n) {
		return mPIM.maPimItemGetField(item, n);
	}

	int maPimItemFieldCount(int item, int field) {
		return mPIM.maPimItemFieldCount(item, field);
	}

	int maPimItemGetAttributes(int item, int field, int index) {
		return mPIM.maPimItemGetAttributes(item, field, index);
	}

	int maPimItemSetLabel(int item, int field, int buffPointer, int buffSize,
			int index) {
		return mPIM
				.maPimItemSetLabel(item, field, buffPointer, buffSize, index);
	}

	int maPimItemGetLabel(int item, int field, int buffPointer, int buffSize,
			int index) {
		return mPIM
				.maPimItemGetLabel(item, field, buffPointer, buffSize, index);
	}

	int maPimFieldType(int list, int field) {
		return mPIM.maPimFieldType(list, field);
	}

	int maPimItemGetValue(int item, int field, int buffPointer, int buffSize,
			int index) {
		return mPIM
				.maPimItemGetValue(item, field, buffPointer, buffSize, index);
	}

	int maPimItemSetValue(int item, int field, int buffPointer, int buffSize,
			int index, int attributes) {
		return mPIM.maPimItemSetValue(item, field, buffPointer, buffSize,
				index, attributes);
	}

	int maPimItemAddValue(int item, int field, int buffPointer, int buffSize,
			int attributes) {
		return mPIM.maPimItemAddValue(item, field, buffPointer, buffSize,
				attributes);
	}

	int maPimItemRemoveValue(int item, int field, int index) {
		return mPIM.maPimItemRemoveValue(item, field, index);
	}

	int maPimItemClose(int item) {
		return mPIM.maPimItemClose(item);
	}

	int maPimItemCreate(int list) {
		return mPIM.maPimItemCreate(list);
	}

	int maPimItemRemove(int list, int item) {
		return mPIM.maPimItemRemove(list, item);
	}
}
