package com.mosync.internal.android.nfc;

import static com.mosync.internal.generated.MAAPI_consts.EVENT_TYPE_NFC_TAG_AUTH_COMPLETE;
import static com.mosync.internal.generated.MAAPI_consts.MA_NFC_TAG_CONNECTION_LOST;
import static com.mosync.internal.generated.MAAPI_consts.MA_NFC_TAG_IO_ERROR;

import java.io.IOException;

import android.nfc.TagLostException;
import android.nfc.tech.MifareClassic;

class MFCAuthenticateSectorWithKey extends TagRunnable<MifareClassicTag> {

	public enum KeyType {
		A, B
	}

	private final byte[] keyBuffer;
	private final MifareClassic mifareTag;
	private final int sectorIndex;
	private final KeyType keyType;

	MFCAuthenticateSectorWithKey(MifareClassicTag tag, KeyType keyType, byte[] keyBuffer, int sectorIndex) {
		super(tag, EVENT_TYPE_NFC_TAG_AUTH_COMPLETE);
		this.mifareTag = tag.nativeTag();
		this.keyType = keyType;
		this.keyBuffer = keyBuffer;
		this.sectorIndex = sectorIndex;
	}

	@Override
	public NFCEvent doRun() throws IOException {
		boolean auth = keyType == KeyType.A ?
			mifareTag.authenticateSectorWithKeyA(sectorIndex, keyBuffer) :
			mifareTag.authenticateSectorWithKeyB(sectorIndex, keyBuffer);
		return new NFCEvent(EVENT_TYPE_NFC_TAG_AUTH_COMPLETE, tag.getHandle(), auth ? 0 : -1, -1);
	}

	@Override
	public String toString() {
		return String.format("Authenticated sector %d of handle %d", sectorIndex, tag.getHandle());
	}
}