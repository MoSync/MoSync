package com.mosync.internal.android.nfc;

import static com.mosync.internal.generated.MAAPI_consts.MA_NFC_TAG_TYPE_MIFARE_CL;

import java.io.IOException;

import android.nfc.tech.MifareUltralight;

public class MifareUltralightTag extends ResourceBase implements INFCTag {

	public static INFCTag get(ResourcePool pool, GenericTag tag) {
		MifareUltralight mfu = MifareUltralight.get(tag.getTag());
		if (mfu != null) {
			return new MifareUltralightTag(pool, mfu);
		}
		return null;
	}

	private final MifareUltralight mfu;

	private MifareUltralightTag(ResourcePool pool, MifareUltralight mfu) {
		super(pool);
		this.mfu = mfu;
	}

	@Override
	public void close() throws IOException {
		mfu.close();
	}

	@Override
	public void connect() throws IOException {
		mfu.connect();
	}

	public MifareUltralight getTag() {
		return mfu;
	}

	@Override
	public INFCTag toTypedTag(ResourcePool pool, int type) {
		if (type == MA_NFC_TAG_TYPE_MIFARE_CL) {
			return this;
		}
		return null;
	}
}
