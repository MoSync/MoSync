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
