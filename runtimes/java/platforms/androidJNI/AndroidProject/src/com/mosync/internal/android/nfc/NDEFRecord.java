package com.mosync.internal.android.nfc;

import java.nio.ByteBuffer;

import android.nfc.NdefRecord;


public class NDEFRecord extends ResourceBase {

	private final NdefRecord delegate;
	private byte[] id;
	private byte[] payload;
	private byte[] type;
	private Short tnf;

	/**
	 * Creates an NDEF record, based on an Android NdefRecord delegate.
	 * @param pool
	 * @param delegate
	 */
	public NDEFRecord(ResourcePool pool, NdefRecord delegate) {
		super(pool);
		this.delegate = delegate;
	}

	/**
	 * Creates an empty record
	 * @param pool
	 */
	public NDEFRecord(ResourcePool pool) {
		super(pool);
		this.delegate = null;
		this.id = new byte[0];
		this.payload = new byte[0];
		this.type = new byte[0];
		this.tnf = new Short((short) 0);
	}

	int maNFCGetId(ByteBuffer result) {
		byte[] id = internalGetId();
		int size = Math.min(result.remaining(), id.length);
		result.put(id, 0, size);
		return size;
	}

	private byte[] internalGetId() {
		return this.id == null ? delegate.getId() : this.id;
	}

	void maNFCSetId(ByteBuffer id) {
		this.id = new byte[id.limit()];
		id.get(this.id);
	}

	int maNFCGetPayload(ByteBuffer result) {
		byte[] payload = internalGetPayload();
		int size = Math.min(result.remaining(), payload.length);
		result.put(payload, 0, size);
		return size;
	}

	private byte[] internalGetPayload() {
		return this.payload == null ? delegate.getPayload() : this.payload;
	}

	void maNFCSetPayload(ByteBuffer payload) {
		this.payload = new byte[payload.limit()];
		payload.get(this.payload);
	}

	int maNFCGetTnf() {
		return tnf == null ? delegate.getTnf() : tnf;
	}

	void maNFCSetTnf(short tnf) {
		this.tnf = tnf;
	}

	int maNFCGetType(ByteBuffer result) {
		byte[] type = internalGetType();
		int size = Math.min(result.remaining(), type.length);
		result.put(type, 0, size);
		return size;
	}

	private byte[] internalGetType() {
		return this.type == null ? delegate.getType() : this.type;
	}

	void maNFCSetType(ByteBuffer type) {
		this.type = new byte[type.limit()];
		type.get(this.type);
	}

	public NdefRecord toNativeNDEFRecord() {
		short tnf = (short) maNFCGetTnf();
		byte[] type = internalGetType();
		byte[] id = internalGetId();
		byte[] payload = internalGetPayload();
		return new NdefRecord(tnf, type, id, payload);
	}

}
