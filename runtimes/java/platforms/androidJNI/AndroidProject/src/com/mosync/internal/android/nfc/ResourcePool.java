/* Copyright 2013 David Axmark

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

	http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

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