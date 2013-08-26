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

import static com.mosync.internal.generated.MAAPI_consts.EVENT_TYPE_NFC_TAG_DATA_WRITTEN;

import com.mosync.internal.android.nfc.INDEFMessageHolder;
import com.mosync.internal.android.nfc.INFCTag;
import com.mosync.internal.android.nfc.NDEFMessage;
import com.mosync.internal.android.nfc.NFCEvent;

public class WriteNDEF extends TagRunnable<INFCTag> {

	private NDEFMessage message;

	public WriteNDEF(INFCTag tag, NDEFMessage message) {
		super(tag, EVENT_TYPE_NFC_TAG_DATA_WRITTEN);
		this.message = message;
	}

	@Override
	protected NFCEvent doRun() throws Exception {
		((INDEFMessageHolder) tag).writeNDEFMessage(message);
		return new NFCEvent(EVENT_TYPE_NFC_TAG_DATA_WRITTEN, tag.getHandle(), 0, message.getHandle());
	}

	@Override
	protected NFCEvent createDefaultEvent(int result) {
		return new NFCEvent(eventType, tag.getHandle(), result, message.getHandle());
	}
}
