package com.mosync.internal.android.nfc;

interface IResource {
	int getHandle();
	void destroy(ResourcePool pool);
	void register(ResourcePool pool);
	boolean isDestroyed();
}