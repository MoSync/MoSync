package com.mosync.internal.android.nfc;

public class NFCEvent {

	int eventType;
	int handle;
	int errorCodeOrLength;
	int dst;

	public NFCEvent(int eventType, int handle, int errorCodeOrLength, int dst) {
		this.eventType = eventType;
		this.handle = handle;
		this.errorCodeOrLength = errorCodeOrLength;
		this.dst = dst;
	}

	@Override
	public String toString() {
		return eventType + ";" + handle + ";" + errorCodeOrLength + ";" + dst;
	}

}
