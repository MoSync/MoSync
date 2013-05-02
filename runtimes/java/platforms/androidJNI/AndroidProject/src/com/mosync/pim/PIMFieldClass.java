package com.mosync.pim;

import static com.mosync.internal.android.MoSyncHelpers.DebugPrint;
import static com.mosync.internal.generated.IX_PIM.MA_PIM_ERR_NONE;
import static com.mosync.internal.generated.IX_PIM.MA_PIM_FIELD_CONTACT_CLASS;
import static com.mosync.internal.generated.IX_PIM.MA_PIM_TYPE_INT;
import android.content.ContentResolver;

import com.mosync.api.Pointer;

public class PIMFieldClass extends UnsupportedPIMField {

	/**
	 * Constructor
	 */
	public PIMFieldClass() {
		mType = MA_PIM_FIELD_CONTACT_CLASS;
		mDataType = MA_PIM_TYPE_INT;
	}

	/**
	 * Print field values.
	 */
	void print() {
		DebugPrint("***********CLASS***********");
		DebugPrint("Not supported");
		DebugPrint("***************************");
	}
}
