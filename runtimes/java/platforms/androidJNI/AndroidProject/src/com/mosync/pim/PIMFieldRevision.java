package com.mosync.pim;

import static com.mosync.internal.android.MoSyncHelpers.DebugPrint;
import android.content.ContentResolver;

import static com.mosync.internal.generated.IX_PIM.MA_PIM_ERR_NONE;

import static com.mosync.internal.generated.IX_PIM.MA_PIM_FIELD_CONTACT_REVISION;
import static com.mosync.internal.generated.IX_PIM.MA_PIM_TYPE_DATE;

public class PIMFieldRevision extends UnsupportedPIMField {

	/**
	 * Constructor
	 */
	public PIMFieldRevision() {
		mType = MA_PIM_FIELD_CONTACT_REVISION;
		mDataType = MA_PIM_TYPE_DATE;
	}

	/**
	 * Print field values.
	 */
	void print() {
		DebugPrint("**********REVISION*********");
		DebugPrint("Not supported");
		DebugPrint("***************************");
	}
}
