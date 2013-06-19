package com.mosync.api;

import com.mosync.internal.android.MoSyncThread;
import com.mosync.internal.android.extensions.ExtensionModule;
import com.mosync.internal.android.extensions.MoSyncExtensionLoader;

public abstract class MoSyncExtension {

	public void onLoad(MoSyncContext context) { }

	public void onUnload(MoSyncContext context) { }

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
