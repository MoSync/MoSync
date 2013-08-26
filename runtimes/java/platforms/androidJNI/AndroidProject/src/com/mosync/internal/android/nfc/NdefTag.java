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

import static com.mosync.internal.generated.MAAPI_consts.MA_NFC_TAG_TYPE_NDEF;

import java.io.IOException;

import android.nfc.FormatException;
import android.nfc.NdefMessage;
import android.nfc.tech.Ndef;

public class NdefTag extends NFCTagBase<Ndef> implements INDEFMessageHolder {

	private NDEFMessage cachedNDEFMessage;

	static NdefTag get(ResourcePool pool, GenericTag tag) {
		Ndef ndef = Ndef.get(tag.getTag());
		if (ndef != null) {
			NdefTag result = new NdefTag(pool, ndef);
			result.cacheNDEFMessage(pool, ndef.getCachedNdefMessage());
			return result;
		}

		return null;
	}

	private void cacheNDEFMessage(ResourcePool pool, NdefMessage newMessage) {
		if (cachedNDEFMessage != null) {
			cachedNDEFMessage.destroy(pool);
		}
		cachedNDEFMessage = newMessage == null ? null : new NDEFMessage(pool, newMessage);
	}

	private NdefTag(ResourcePool pool, Ndef ndef) {
		super(pool, ndef, MA_NFC_TAG_TYPE_NDEF);
	}

	@Override
	public void requestNDEFMessage(ResourcePool pool) throws IOException {
		try {
			cacheNDEFMessage(pool, nativeTag.getNdefMessage());
		} catch (FormatException e) {
			throw new IOException("Malformed NDEF");
		}
	}

	@Override
	public NDEFMessage getNDEFMessage() {
		return cachedNDEFMessage;
	}

	@Override
	public void writeNDEFMessage(NDEFMessage message) throws IOException {
		try {
			nativeTag.writeNdefMessage(message.toNativeNDEFMessage());
		} catch (FormatException e) {
			throw new IOException(e);
		}
	}

	@Override
	public void destroy(ResourcePool pool) {
		super.destroy(pool);
		if (cachedNDEFMessage != null) {
			cachedNDEFMessage.destroy(pool);
		}
	}

}
