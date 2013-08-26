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

package com.mosync.internal.android.nfc;

import static com.mosync.internal.generated.MAAPI_consts.MA_NFC_TAG_TYPE_NFC_B;

import java.io.IOException;

import android.nfc.tech.NfcB;

public class NfcBTag extends NFCTagBase<NfcB> implements ITransceivable<NfcB> {

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
