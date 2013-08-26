/* Copyright 2013 David Axmark

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

	http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

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
