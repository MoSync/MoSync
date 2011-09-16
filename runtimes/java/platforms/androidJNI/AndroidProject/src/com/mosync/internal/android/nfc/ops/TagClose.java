package com.mosync.internal.android.nfc.ops;

import java.io.IOException;

import com.mosync.internal.android.nfc.INFCTag;
import com.mosync.internal.android.nfc.NFCEvent;
import com.mosync.internal.android.nfc.RunnableWithResult;

public class TagClose implements RunnableWithResult<NFCEvent> {

	private final INFCTag tag;

	public TagClose(INFCTag tag) {
		this.tag = tag;
	}

	@Override
	public NFCEvent run() {
		try {
			tag.close();
		} catch (IOException e) {
			// TODO: EVENTS!
		}
		return null;
	}

	@Override
	public String toString() {
		return String.format("Closing tag %s, with handle %d", tag.toString(), tag.getHandle());
	}
}
