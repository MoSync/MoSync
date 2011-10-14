package com.mosync.internal.android.nfc;

import static com.mosync.internal.generated.MAAPI_consts.MA_NFC_TAG_TYPE_MIFARE_UL;

import java.io.IOException;

import android.nfc.tech.MifareClassic;

public class MifareClassicTag extends NFCTagBase<MifareClassic> implements ITransceivable<MifareClassic>, ISizeHolder {


	public static INFCTag get(ResourcePool pool, GenericTag tag) {
		MifareClassic mfc = MifareClassic.get(tag.getTag());
		if (mfc != null) {
			return new MifareClassicTag(pool, mfc);
		}
		return null;
	}

	private MifareClassicTag(ResourcePool pool, MifareClassic mfc) {
		super(pool, mfc, MA_NFC_TAG_TYPE_MIFARE_UL);
	}

	@Override
	public byte[] transceive(byte[] buffer) throws IOException {
		return nativeTag.transceive(buffer);
	}

	@Override
	public int getSize() {
		return nativeTag.getSize();
	}


}
