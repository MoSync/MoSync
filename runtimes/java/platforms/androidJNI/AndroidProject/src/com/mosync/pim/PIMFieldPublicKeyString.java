package com.mosync.pim;

import static com.mosync.internal.android.MoSyncHelpers.DebugPrint;

import com.mosync.api.Pointer;

import android.content.ContentResolver;

import static com.mosync.internal.generated.IX_PIM.MA_PIM_ERR_NONE;

import static com.mosync.internal.generated.IX_PIM.MA_PIM_FIELD_CONTACT_PUBLIC_KEY_STRING;
import static com.mosync.internal.generated.IX_PIM.MA_PIM_TYPE_STRING;

public class PIMFieldPublicKeyString extends UnsupportedPIMField {

	/**
	 * Constructor
	 */
	public PIMFieldPublicKeyString() {
		mType = MA_PIM_FIELD_CONTACT_PUBLIC_KEY_STRING;
		mDataType = MA_PIM_TYPE_STRING;
	}

	/**
	 * Print field values.
	 */
	void print() {
		DebugPrint("*****PUBLIC KEY STRING*****");
		DebugPrint("Not supported");
		DebugPrint("***************************");
	}
}
