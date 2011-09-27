package com.mosync.internal.android.nfc;

import static com.mosync.internal.generated.MAAPI_consts.MA_NFC_TAG_TYPE_NFC_B;

import java.io.IOException;

import android.nfc.tech.NfcB;

public class NfcBTag extends NFCTagBase<NfcB> {

	public static NfcBTag get(ResourcePool pool, GenericTag tag) {
		NfcB nfcB = NfcB.get(tag.getTag());
		return nfcB == null ? null : new NfcBTag(pool, nfcB);
	}

	private NfcBTag(ResourcePool pool, NfcB nfcB) {
		super(pool, nfcB, MA_NFC_TAG_TYPE_NFC_B);
	}

	@Override
	public byte[] transceive(byte[] buffer) throws IOException {
		return nativeTag.transceive(buffer);
	}

}
