package com.mosync.pim;

import static com.mosync.internal.android.MoSyncHelpers.DebugPrint;

import com.mosync.api.Pointer;

import android.content.ContentResolver;

import static com.mosync.internal.generated.IX_PIM.MA_PIM_ERR_NONE;

import static com.mosync.internal.generated.IX_PIM.MA_PIM_FIELD_CONTACT_FORMATTED_NAME;
import static com.mosync.internal.generated.IX_PIM.MA_PIM_TYPE_STRING;

public class PIMFieldFormattedName extends UnsupportedPIMField {

	/**
	 * Constructor
	 */
	public PIMFieldFormattedName() {
		mType = MA_PIM_FIELD_CONTACT_FORMATTED_NAME;
		mDataType = MA_PIM_TYPE_STRING;
	}

	/**
	 * Print field values.
	 */
	void print() {
		DebugPrint("******FORMATTED NAMES******");
		DebugPrint("Not supported");
		DebugPrint("***************************");
	}
}
