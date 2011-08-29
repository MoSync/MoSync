package com.mosync.internal.android.nfc;

import static com.mosync.internal.generated.MAAPI_consts.MA_NFC_TAG_TYPE_MIFARE_UL;

import java.io.IOException;

import android.nfc.tech.MifareClassic;

public class MifareClassicTag extends ResourceBase implements INFCTag {


	public static INFCTag get(ResourcePool pool, GenericTag tag) {
		MifareClassic mfc = MifareClassic.get(tag.getTag());
		if (mfc != null) {
			return new MifareClassicTag(pool, mfc);
		}
		return null;
	}

	private final MifareClassic mfc;

	private MifareClassicTag(ResourcePool pool, MifareClassic mfc) {
		super(pool);
		this.mfc = mfc;
	}

	@Override
	public void close() throws IOException {
		mfc.close();
	}

	@Override
	public void connect() throws IOException {
		mfc.connect();
	}

	public MifareClassic getTag() {
		return mfc;
	}

	@Override
	public INFCTag toTypedTag(ResourcePool pool, int type) {
		if (type == MA_NFC_TAG_TYPE_MIFARE_UL) {
			return this;
		}
		return null;
	}
}
