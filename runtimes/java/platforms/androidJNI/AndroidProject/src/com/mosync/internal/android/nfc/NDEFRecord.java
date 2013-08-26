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

import java.nio.ByteBuffer;

import android.nfc.FormatException;
import android.nfc.NdefRecord;


public class NDEFRecord extends ResourceBase implements ISizeHolder {

	private NdefRecord delegate;
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

	int getId(ByteBuffer result) {
		byte[] id = internalGetId();
		if (result == null) {
			return id.length;
		}
		int size = Math.min(result.remaining(), id.length);
		result.put(id, 0, size);
		return size;
	}

	private byte[] internalGetId() {
		return this.id == null ? delegate.getId() : this.id;
	}

	void setId(ByteBuffer id) {
		this.id = new byte[id.limit()];
		id.get(this.id);
	}

	int getPayload(ByteBuffer result) {
		byte[] payload = internalGetPayload();
		if (result == null) {
			return payload.length;
		}
		int size = Math.min(result.remaining(), payload.length);
		result.put(payload, 0, size);
		return size;
	}

	private byte[] internalGetPayload() {
		return this.payload == null ? delegate.getPayload() : this.payload;
	}

	void setPayload(ByteBuffer payload) {
		this.payload = new byte[payload.limit()];
		payload.get(this.payload);
	}

	int getTnf() {
		return tnf == null ? delegate.getTnf() : tnf;
	}

	void setTnf(short tnf) {
		this.tnf = tnf;
	}

	int getType(ByteBuffer result) {
		byte[] type = internalGetType();
		if (result == null) {
			return type.length;
		}
		int size = Math.min(result.remaining(), type.length);
		result.put(type, 0, size);
		return size;
	}

	private byte[] internalGetType() {
		return this.type == null ? delegate.getType() : this.type;
	}

	void setType(ByteBuffer type) {
		this.type = new byte[type.limit()];
		type.get(this.type);
	}

	int setRaw(ByteBuffer raw) {
		byte[] rawData = new byte[raw.limit()];
		try {
			NdefRecord rawRecord = new NdefRecord(rawData);
			initFromNativeNDEFRecord(rawRecord);
			return rawData.length;
		} catch (FormatException e) {
			return 0;
		}
	}

	void getRaw(ByteBuffer raw) {

	}

	void initFromNativeNDEFRecord(NdefRecord delegate) {
		this.tnf = null;
		this.id = null;
		this.payload = null;
		this.type = null;
		this.delegate = delegate;
	}

	public NdefRecord toNativeNDEFRecord() {
		short tnf = (short) getTnf();
		byte[] type = internalGetType();
		byte[] id = internalGetId();
		byte[] payload = internalGetPayload();
		return new NdefRecord(tnf, type, id, payload);
	}

	@Override
	public int getSize() {
		// TODO Not very efficient, right?
		return toNativeNDEFRecord().toByteArray().length;
	}

}
