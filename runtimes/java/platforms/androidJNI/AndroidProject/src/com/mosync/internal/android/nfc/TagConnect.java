package com.mosync.internal.android.nfc;

import java.io.IOException;

public class TagConnect implements RunnableWithResult<NFCEvent> {

	private final INFCTag tag;

	public TagConnect(INFCTag tag) {
		this.tag = tag;
	}

	@Override
	public NFCEvent run() {
		try {
			tag.connect();
		} catch (IOException e) {
			// TODO: EVENTS!
		}
		return null;
	}

	@Override
	public String toString() {
		return String.format("Connecting to tag %s, with handle %d", tag.toString(), tag.getHandle());
	}
}
