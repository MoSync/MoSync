package com.mosync.internal.android.nfc;

import java.nio.ByteBuffer;

import android.nfc.NdefRecord;


public class NDEFRecord extends ResourceBase {

	private final NdefRecord delegate;

	public NDEFRecord(ResourcePool pool, NdefRecord delegate) {
		super(pool);
		this.delegate = delegate;
	}

	int maNFCGetId(ByteBuffer result) {
		byte[] id = delegate.getId();
		int size = Math.min(result.remaining(), id.length);
		result.put(id, 0, size);
		return size;
	}

	int maNFCGetPayload(ByteBuffer result) {
		byte[] payload = delegate.getId();
		int size = Math.min(result.remaining(), payload.length);
		result.put(payload, 0, size);
		return size;
	}

	int maNFCGetTnf() {
		return delegate.getTnf();
	}

	int maNFCGetType(ByteBuffer result) {
		byte[] type = delegate.getType();
		int size = Math.min(result.remaining(), type.length);
		result.put(type, 0, size);
		return size;
	}

}
