package com.mosync.internal.android.nfc;

public class ResourceBase implements IResource {

	private int handle;

	public ResourceBase(ResourcePool pool) {
		handle = pool.register(this);
	}

	@Override
	public int getHandle() {
		return handle;
	}

	@Override
	public void register(ResourcePool pool) {
		pool.register(this);
	}

	@Override
	public void destroy(ResourcePool pool) {
		pool.destroy(handle);
		handle = 0;
	}

	public boolean isDestroyed() {
		return handle == 0;
	}

}
