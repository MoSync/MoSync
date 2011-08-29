package com.mosync.internal.android.nfc;

import java.io.IOException;

public interface INFCTag extends IResource {

	void close() throws IOException;

	void connect() throws IOException;

	INFCTag toTypedTag(ResourcePool pool, int type);

}
