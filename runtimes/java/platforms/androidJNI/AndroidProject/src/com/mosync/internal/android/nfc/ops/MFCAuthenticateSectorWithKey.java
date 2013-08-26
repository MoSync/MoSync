/* Copyright 2013 David Axmark

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

	http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

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