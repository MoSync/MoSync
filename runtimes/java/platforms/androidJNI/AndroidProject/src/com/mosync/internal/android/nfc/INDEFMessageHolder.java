package com.mosync.internal.android.nfc;

import java.io.IOException;

public interface INDEFMessageHolder extends INDEFMessageWritable {

	/**
	 * Sends a request to the tag to get the current
	 * NDEF message on it.
	 * @throws IOException
	 */
	public void requestNDEFMessage(ResourcePool pool) throws IOException;

	/**
	 * Returns a cached ndef message of this holder.
	 * The holder is responsible for destroying it.
	 * @return
	 */
	public NDEFMessage getNDEFMessage();

}
