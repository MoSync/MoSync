package com.mosync.internal.android.nfc.ops;

import static com.mosync.internal.generated.MAAPI_consts.EVENT_TYPE_NFC_TAG_AUTH_COMPLETE;
import static com.mosync.internal.generated.MAAPI_consts.MA_NFC_MIFARE_KEY_A;

import java.io.IOException;

import com.mosync.internal.android.nfc.MifareClassicTag;
import com.mosync.internal.android.nfc.NFCEvent;

import android.nfc.tech.MifareClassic;

public class MFCAuthenticateSectorWithKey extends TagRunnable<MifareClassicTag> {

	private final byte[] keyBuffer;
	private final MifareClassic mifareTag;
	private final int sectorIndex;
	private final int keyType;

	public MFCAuthenticateSectorWithKey(MifareClassicTag tag, int keyType, byte[] keyBuffer, int sectorIndex) {
		super(tag, EVENT_TYPE_NFC_TAG_AUTH_COMPLETE);
		this.mifareTag = tag.nativeTag();
		this.keyType = keyType;
		this.keyBuffer = keyBuffer;
		this.sectorIndex = sectorIndex;
	}

	@Override
	public NFCEvent doRun() throws IOException {
		boolean auth = keyType == MA_NFC_MIFARE_KEY_A ?
			mifareTag.authenticateSectorWithKeyA(sectorIndex, keyBuffer) :
			mifareTag.authenticateSectorWithKeyB(sectorIndex, keyBuffer);
		return new NFCEvent(EVENT_TYPE_NFC_TAG_AUTH_COMPLETE, tag.getHandle(), auth ? 0 : -1, -1);
	}

	@Override
	public String toString() {
		return String.format("Authenticated sector %d of handle %d", sectorIndex, tag.getHandle());
	}
}