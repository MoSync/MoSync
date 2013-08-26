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

import static com.mosync.internal.generated.MAAPI_consts.*;

import com.mosync.internal.android.nfc.INFCTag;
import com.mosync.internal.android.nfc.NFCEvent;
import com.mosync.internal.android.nfc.RunnableWithResult;

import android.nfc.FormatException;
import android.nfc.TagLostException;

/**
 * Utility class for handling the usual event handling for
 * asynchronous NFC operations. There is no other dependencies
 * for this class other than it centralizes some common behaviour.
 * @author Mattias Bybro
 *
 */
public abstract class TagRunnable<TagType extends INFCTag> implements RunnableWithResult<NFCEvent> {

	protected final int eventType;
	protected final TagType tag;

	public TagRunnable(TagType tag, int eventType) {
		this.tag = tag;
		this.eventType = eventType;
	}

	@Override
	public NFCEvent run() {
		if (!tag.nativeTag().isConnected()) {
			return createDefaultEvent(MA_NFC_TAG_NOT_CONNECTED);
		}
		try {
			return doRun();
		} catch (FormatException e) {
			return createDefaultEvent(MA_NFC_FORMAT_FAILED);
		} catch (TagLostException e) {
			return createDefaultEvent(MA_NFC_TAG_CONNECTION_LOST);
		} catch (Exception e) {
			e.printStackTrace();
			return createDefaultEvent(MA_NFC_TAG_IO_ERROR);
		}
	}

	protected abstract NFCEvent doRun() throws Exception;

	protected NFCEvent createDefaultEvent(int result) {
		return new NFCEvent(eventType, tag.getHandle(), MA_NFC_TAG_CONNECTION_LOST, -1);
	}
}
