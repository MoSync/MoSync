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
