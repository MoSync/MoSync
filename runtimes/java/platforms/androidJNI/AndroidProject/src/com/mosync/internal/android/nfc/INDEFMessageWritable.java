package com.mosync.internal.android.nfc;

import java.io.IOException;

public interface INDEFMessageWritable extends IResource {

	/**
	 * Writes an NDE message
	 * @param message
	 * @throws IOException
	 */
	public void writeNDEFMessage(NDEFMessage message) throws IOException;
}
