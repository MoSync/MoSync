package com.mosync.internal.android.nfc;

import java.io.IOException;

import android.nfc.tech.TagTechnology;

public interface INFCTag<NativeTagType extends TagTechnology> extends IResource {

	void close() throws IOException;

	void connect() throws IOException;

	INFCTag toTypedTag(ResourcePool pool, int type);

	NativeTagType nativeTag();

}
