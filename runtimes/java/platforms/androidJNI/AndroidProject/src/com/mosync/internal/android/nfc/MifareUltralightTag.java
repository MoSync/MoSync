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

import static com.mosync.internal.generated.MAAPI_consts.MA_NFC_TAG_TYPE_MIFARE_CL;

import java.io.IOException;

import android.nfc.tech.MifareUltralight;

public class MifareUltralightTag extends NFCTagBase<MifareUltralight> implements ITransceivable<MifareUltralight>, ISizeHolder {

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

	@Override
	public int getSize() {
		int type = nativeTag.getType();
		if (type == MifareUltralight.TYPE_ULTRALIGHT) {
			return 64;
		} else if (type == MifareUltralight.TYPE_ULTRALIGHT_C) {
			return 192;
		}
		return -1;
	}
}
