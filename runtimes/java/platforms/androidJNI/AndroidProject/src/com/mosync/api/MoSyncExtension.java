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

package com.mosync.api;

import com.mosync.internal.android.MoSyncThread;
import com.mosync.internal.android.extensions.ExtensionModule;
import com.mosync.internal.android.extensions.MoSyncExtensionLoader;

public abstract class MoSyncExtension {

	private MoSyncContext context;

	public void onLoad(MoSyncContext context) {
		this.context = context;
	}

	public void onUnload(MoSyncContext context) {
		this.context = null;
	}

	protected static int notImplemented() {
		return -1;
	}

	protected MoSyncContext getContext() {
		return context;
	}

	protected void postEvent(int type, int... data) {
		int extId = MoSyncExtensionLoader.getDefault().getModuleId(this);
		int extType = extId << 0x3fffff | (type & 0x3fffff);
		int[] event = new int[data.length + 1];
		System.arraycopy(data, 0, event, 1, data.length);
		event[0] = extType;
		MoSyncThread.getInstance().postEvent(event);
	}

	protected void maPanic(int panicId, String msg) {
		int extId = MoSyncExtensionLoader.getDefault().getModuleId(this);
		ExtensionModule module = MoSyncExtensionLoader.getDefault().getModule(extId);
		String extMsg = module.getName() + ": " + msg;
		MoSyncThread.getInstance().maPanic(panicId, extMsg);
	}
}
