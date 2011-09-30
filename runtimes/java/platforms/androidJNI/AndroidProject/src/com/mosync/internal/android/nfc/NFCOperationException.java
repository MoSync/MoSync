package com.mosync.internal.android.nfc;

public class NFCOperationException extends Exception {

	private final int errorCode;

	public NFCOperationException(String msg, int errorCode) {
		super(msg);
		this.errorCode = errorCode;
	}

	public int getErrorCode() {
		return errorCode;
	}
}
