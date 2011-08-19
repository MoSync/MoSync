package com.mosync.internal.android.nfc;

import java.util.HashMap;


class ResourcePool {

	private final HashMap<Integer, IResource> handleToResource = new HashMap<Integer, IResource>();

	private int currentHandle = 1;

	public int register(IResource resource) {
		int handle = nextHandle();
		handleToResource.put(nextHandle(), resource);
		return handle;
	}

	public IResource getResource(int handle) {
		return handleToResource.get(handle);
	}

	private int nextHandle() {
		return currentHandle++;
	}

	public void destroy(int handle) {
		IResource resource = handleToResource.remove(handle);
		resource.destroy(this);
	}

	public void destroyAll() {
		for (Integer handle : handleToResource.keySet()) {
			destroy(handle);
		}
	}

}