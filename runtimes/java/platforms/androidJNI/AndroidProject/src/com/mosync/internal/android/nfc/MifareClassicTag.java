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
