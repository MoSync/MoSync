package com.mosync.internal.android.extensions;

public interface Marshaller {
	public Object unmarshal(byte[] data, int offset);
}
