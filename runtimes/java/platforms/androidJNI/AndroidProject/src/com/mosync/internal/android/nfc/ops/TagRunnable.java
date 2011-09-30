package com.mosync.internal.android.nfc.ops;

import static com.mosync.internal.generated.MAAPI_consts.*;

import com.mosync.internal.android.nfc.INFCTag;
import com.mosync.internal.android.nfc.NFCEvent;
import com.mosync.internal.android.nfc.RunnableWithResult;

import android.nfc.TagLostException;

/**
 * Utility class for handling the usual event handling for
 * asynchronous NFC operations. There is no other dependencies
 * for this class other than it centralizes some common behaviour.
 * @author Mattias Bybro
 *
 */
public abstract class TagRunnable<TagType extends INFCTag> implements RunnableWithResult<NFCEvent> {

	private final int eventType;
	protected final TagType tag;

	public TagRunnable(TagType tag, int eventType) {
		this.tag = tag;
		this.eventType = eventType;
	}

	@Override
	public NFCEvent run() {
		if (!tag.nativeTag().isConnected()) {
			return new NFCEvent(eventType, tag.getHandle(), MA_NFC_TAG_NOT_CONNECTED, -1);
		}
		try {
			return doRun();
		} catch (TagLostException e) {
			return new NFCEvent(eventType, tag.getHandle(), MA_NFC_TAG_CONNECTION_LOST, -1);
		} catch (Exception e) {
			return new NFCEvent(eventType, tag.getHandle(), MA_NFC_TAG_IO_ERROR, -1);
		}
	}

	protected abstract NFCEvent doRun() throws Exception;

}
