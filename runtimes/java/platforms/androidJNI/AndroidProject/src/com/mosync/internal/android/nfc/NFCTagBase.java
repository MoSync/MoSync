package com.mosync.internal.android.nfc;

import java.io.IOException;
import java.nio.ByteBuffer;

import android.nfc.tech.TagTechnology;

public abstract class NFCTagBase<NativeTagType extends TagTechnology> extends ResourceBase implements INFCTag<NativeTagType> {

	protected NativeTagType nativeTag;
	private final int typeId;

	public NFCTagBase(ResourcePool pool, NativeTagType nativeTag, int typeId) {
		super(pool);
		this.nativeTag = nativeTag;
		this.typeId = typeId;
	}

	@Override
	public void close() throws IOException {
		nativeTag.close();
	}

	@Override
	public void connect() throws IOException {
		nativeTag.connect();
	}

	@Override
	public INFCTag getTypedTag(ResourcePool pool, int type) {
		if (type == typeId) {
			return this;
		}
		return null;
	}

	@Override
	public int getId(ByteBuffer result) {
		byte[] id = nativeTag.getTag().getId();
		if (result == null) {
			return id.length;
		}
		int size = Math.min(result.remaining(), id.length);
		result.put(id, 0, size);
		return size;
	}

	@Override
	public NativeTagType nativeTag() {
		return nativeTag;
	}

}
