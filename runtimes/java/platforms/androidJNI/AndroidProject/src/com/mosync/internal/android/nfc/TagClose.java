package com.mosync.internal.android.nfc;

import java.io.IOException;

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
