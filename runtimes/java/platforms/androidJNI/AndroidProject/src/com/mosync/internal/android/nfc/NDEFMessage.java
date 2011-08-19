package com.mosync.internal.android.nfc;

import android.nfc.NdefMessage;
import android.nfc.NdefRecord;


public class NDEFMessage extends ResourceBase implements INFCTag {

	private final NdefMessage msg;
	private final NDEFRecord[] records;

	public NDEFMessage(ResourcePool pool, NdefMessage msg) {
		super(pool);
		this.msg = msg;
		NdefRecord[] records = msg.getRecords();
		this.records = new NDEFRecord[records.length];
		for (int i = 0; i < records.length; i++) {
			this.records[i] = new NDEFRecord(pool, records[i]);
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

	@Override
	public void close() {
		// TODO Auto-generated method stub

	}

	@Override
	public void connect() {
		// TODO Auto-generated method stub

	}

	@Override
	public NDEFMessage getNDEFMessage(ResourcePool pool) {
		return this;
	}

}
