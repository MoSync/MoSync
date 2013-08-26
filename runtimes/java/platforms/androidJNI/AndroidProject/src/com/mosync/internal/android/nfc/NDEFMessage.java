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

package com.mosync.internal.android.nfc;

import android.nfc.NdefMessage;
import android.nfc.NdefRecord;


public class NDEFMessage extends ResourceBase implements ISizeHolder {

	private final NDEFRecord[] records;

	public NDEFMessage(ResourcePool pool, NdefMessage msg) {
		super(pool);
		NdefRecord[] records = msg.getRecords();
		this.records = new NDEFRecord[records.length];
		for (int i = 0; i < records.length; i++) {
			this.records[i] = new NDEFRecord(pool, records[i]);
		}
	}

	/**
	 * Creates an empty NDEFMessage
	 */
	public NDEFMessage(ResourcePool pool, int recordCount) {
		super(pool);
		this.records = new NDEFRecord[recordCount];
		for (int i = 0; i < recordCount; i++) {
			this.records[i] = new NDEFRecord(pool);
		}
	}

	@Override
	public void destroy(ResourcePool pool) {
		for (NDEFRecord record : records) {
			record.destroy(pool);
		}
		super.destroy(pool);
	}

	public int getRecordCount() {
		return records.length;
	}

	public NDEFRecord getRecord(int ix) {
		return records[ix];
	}

	public NdefMessage toNativeNDEFMessage() {
		NdefRecord[] nativeRecords = new NdefRecord[records.length];
		int ix = 0;
		for (NDEFRecord record : records) {
			nativeRecords[ix] = record.toNativeNDEFRecord();
			ix++;
		}
		NdefMessage nativeMessage = new NdefMessage(nativeRecords);
		return nativeMessage;
	}

	@Override
	public int getSize() {
		int size = 0;
		for (NDEFRecord record : records) {
			size += record.getSize();
		}
		return size;
	}


}
