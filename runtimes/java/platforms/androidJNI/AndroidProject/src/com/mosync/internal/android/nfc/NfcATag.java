package com.mosync.internal.android.nfc;

import static com.mosync.internal.generated.MAAPI_consts.MA_NFC_TAG_TYPE_NFC_A;

import java.io.IOException;

import android.nfc.tech.NfcA;

public class NfcATag extends NFCTagBase<NfcA> implements ITransceivable<NfcA> {

	public static NfcATag get(ResourcePool pool, GenericTag tag) {
		NfcA nfcA = NfcA.get(tag.getTag());
		return nfcA == null ? null : new NfcATag(pool, nfcA);
	}

	private NfcATag(ResourcePool pool, NfcA nfcA) {
		super(pool, nfcA, MA_NFC_TAG_TYPE_NFC_A);
	}

	@Override
	public byte[] transceive(byte[] buffer) throws IOException {
		return nativeTag.transceive(buffer);
	}

}
