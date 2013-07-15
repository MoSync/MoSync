package com.mosync.pim;

import static com.mosync.internal.android.MoSyncHelpers.DebugPrint;
import static com.mosync.internal.generated.IX_PIM.MA_PIM_ERR_NONE;
import static com.mosync.internal.generated.IX_PIM.MA_PIM_FIELD_CONTACT_PUBLIC_KEY;
import static com.mosync.internal.generated.IX_PIM.MA_PIM_TYPE_BINARY;
import android.content.ContentResolver;

import com.mosync.api.Pointer;

public class PIMFieldPublicKey extends UnsupportedPIMField {

	/**
	 * Constructor
	 */
	public PIMFieldPublicKey() {
		mType = MA_PIM_FIELD_CONTACT_PUBLIC_KEY;
		mDataType = MA_PIM_TYPE_BINARY;
	}

	/**
	 * Print field values.
	 */
	void print() {
		DebugPrint("*********PUBLIC KEY********");
		DebugPrint("Not supported");
		DebugPrint("***************************");
	}
}
