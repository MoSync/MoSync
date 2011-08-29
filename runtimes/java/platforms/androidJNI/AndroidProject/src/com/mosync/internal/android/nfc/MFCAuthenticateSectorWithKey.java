package com.mosync.internal.android.nfc;

import static com.mosync.internal.generated.MAAPI_consts.EVENT_TYPE_NFC_TAG_AUTH_COMPLETE;
import static com.mosync.internal.generated.MAAPI_consts.MA_NFC_TAG_CONNECTION_LOST;
import static com.mosync.internal.generated.MAAPI_consts.MA_NFC_TAG_IO_ERROR;
import android.nfc.TagLostException;
import android.nfc.tech.MifareClassic;

class MFCAuthenticateSectorWithKey implements
		RunnableWithResult<NFCEvent> {

	public enum KeyType {
		A, B
	}

	private final byte[] keyBuffer;
	private final MifareClassic mifareTag;
	private final int sectorIndex;
	private final KeyType keyType;
	private final int handle;

	MFCAuthenticateSectorWithKey(int handle, KeyType keyType, byte[] keyBuffer,
			MifareClassic mifareTag, int sectorIndex) {
		this.handle = handle;
		this.keyType = keyType;
		this.keyBuffer = keyBuffer;
		this.mifareTag = mifareTag;
		this.sectorIndex = sectorIndex;
	}

	@Override
	public NFCEvent run() {
		try {
			boolean auth = keyType == KeyType.A ?
				mifareTag.authenticateSectorWithKeyA(sectorIndex, keyBuffer) :
				mifareTag.authenticateSectorWithKeyB(sectorIndex, keyBuffer);
			return new NFCEvent(EVENT_TYPE_NFC_TAG_AUTH_COMPLETE, handle, auth ? 0 : -1, -1);
		} catch (TagLostException e) {
			return new NFCEvent(EVENT_TYPE_NFC_TAG_AUTH_COMPLETE, handle, MA_NFC_TAG_CONNECTION_LOST, -1);
		} catch (Exception e) {
			return new NFCEvent(EVENT_TYPE_NFC_TAG_AUTH_COMPLETE, handle, MA_NFC_TAG_IO_ERROR, -1);
		}
	}

	@Override
	public String toString() {
		return String.format("Authenticated sector %d of handle %d", sectorIndex, handle);
	}
}