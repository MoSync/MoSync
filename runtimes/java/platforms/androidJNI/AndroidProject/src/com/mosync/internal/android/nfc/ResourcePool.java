package com.mosync.internal.android.nfc;

import java.util.HashMap;
import java.util.HashSet;

import android.util.Log;


public class ResourcePool {

	public final static ResourcePool NULL = new ResourcePool() {
		@Override
		public int register(IResource resource) {
			// Do NOTHING :)
			return 0;
		}
	};

	private final HashMap<Integer, IResource> handleToResource = new HashMap<Integer, IResource>();

	private int currentHandle = 1;

	public int register(IResource resource) {
		int handle = nextHandle();
		handleToResource.put(handle, resource);
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
		if (resource != null) {
			resource.destroy(this);
		}
	}

	public void destroyAll() {
		if (!handleToResource.isEmpty()) {
			Log.w("@@@ MoSync", "Not all resource handles have been destroyed, implicitly destroying them now.");
		}
		HashSet<Integer> handles = new HashSet<Integer>(handleToResource.keySet());
		for (Integer handle : handles) {
			destroy(handle);
		}
	}

	public String toString() {
		return handleToResource.size() + " handles: " + handleToResource;
	}

}