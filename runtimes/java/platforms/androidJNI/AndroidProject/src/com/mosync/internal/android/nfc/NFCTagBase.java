package com.mosync.internal.android.nfc;

import java.io.IOException;

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
	public INFCTag toTypedTag(ResourcePool pool, int type) {
		if (type == typeId) {
			return this;
		}
		return null;
	}

	@Override
	public NativeTagType nativeTag() {
		return nativeTag;
	}

}
