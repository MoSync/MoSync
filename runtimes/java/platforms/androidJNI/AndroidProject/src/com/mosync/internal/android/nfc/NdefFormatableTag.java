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

import static com.mosync.internal.generated.MAAPI_consts.MA_NFC_TAG_TYPE_NDEF_FORMATTABLE;

import java.io.IOException;

import android.nfc.FormatException;
import android.nfc.tech.NdefFormatable;

public class NdefFormatableTag extends NFCTagBase<NdefFormatable> implements INDEFMessageWritable {

	public static INFCTag get(ResourcePool pool, GenericTag tag) {
		NdefFormatable ndef = NdefFormatable.get(tag.getTag());
		if (ndef != null) {
			NdefFormatableTag result = new NdefFormatableTag(pool, ndef);
			return result;
		}

		return null;
	}

	public NdefFormatableTag(ResourcePool pool, NdefFormatable nativeTag) {
		super(pool, nativeTag, MA_NFC_TAG_TYPE_NDEF_FORMATTABLE);
	}

	@Override
	public void writeNDEFMessage(NDEFMessage message) throws IOException {
		try {
			nativeTag.format(message.toNativeNDEFMessage());
		} catch (FormatException e) {
			throw new IOException(e);
		}
	}

}
