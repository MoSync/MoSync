package com.mosync.internal.android.nfc;

import android.nfc.NdefMessage;
import android.nfc.NdefRecord;


public class NDEFMessage extends ResourceBase {

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


}
