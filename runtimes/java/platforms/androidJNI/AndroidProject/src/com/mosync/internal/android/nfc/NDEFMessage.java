package com.mosync.internal.android.nfc;

import static com.mosync.internal.generated.MAAPI_consts.MA_NFC_TAG_TYPE_NDEF;
import android.nfc.NdefMessage;
import android.nfc.NdefRecord;
import android.nfc.tech.Ndef;


public class NDEFMessage extends ResourceBase implements INFCTag {

	private final NdefMessage msg;
	private final NDEFRecord[] records;

	static NDEFMessage get(ResourcePool pool, GenericTag tag) {
		Ndef ndef = Ndef.get(tag.getTag());
		if (ndef != null) {
			NdefMessage ndefMessage = ndef.getCachedNdefMessage();
			NDEFMessage result = new NDEFMessage(pool, ndefMessage);
			return result;
		}

		return null;
	}

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

	}

	@Override
	public void connect() {

	}

	@Override
	public INFCTag toTypedTag(ResourcePool pool, int type) {
		if (type == MA_NFC_TAG_TYPE_NDEF) {
			return this;
		}
		return null;
	}

}
