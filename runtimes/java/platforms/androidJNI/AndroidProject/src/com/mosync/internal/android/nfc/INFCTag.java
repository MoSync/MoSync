package com.mosync.internal.android.nfc;

public interface INFCTag extends IResource {

	void close();

	void connect();

	/**
	 * Non-blocking method to get the NDEF
	 * message of this {@link INFCTag}.
	 * @return
	 */
	NDEFMessage getNDEFMessage(ResourcePool pool);
}
