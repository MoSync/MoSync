package com.mosync.api;

public interface Marshaller {
	public Object unmarshal(byte[] data, int offset);
}
