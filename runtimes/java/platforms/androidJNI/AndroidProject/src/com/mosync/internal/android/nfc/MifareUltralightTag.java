package com.mosync.internal.android.nfc;

import static com.mosync.internal.generated.MAAPI_consts.MA_NFC_TAG_TYPE_MIFARE_CL;

import java.io.IOException;

import android.nfc.tech.MifareUltralight;

public class MifareUltralightTag extends NFCTagBase<MifareUltralight> {

	public static INFCTag get(ResourcePool pool, GenericTag tag) {
		MifareUltralight mfu = MifareUltralight.get(tag.getTag());
		if (mfu != null) {
			return new MifareUltralightTag(pool, mfu);
		}
		return null;
	}

	private MifareUltralightTag(ResourcePool pool, MifareUltralight mfu) {
		super(pool, mfu, MA_NFC_TAG_TYPE_MIFARE_CL);
	}

	@Override
	public byte[] transceive(byte[] buffer) throws IOException {
		return nativeTag.transceive(buffer);
	}
}
