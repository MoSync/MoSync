package com.mosync.api;

import com.mosync.internal.android.extensions.Marshaller;
import com.mosync.internal.android.extensions.MoSyncExtensionLoader;
import com.mosync.internal.android.extensions.TypeDescriptor;



public abstract class Struct implements Marshaller {

	protected static final Marshaller _m(String extension, String name, int ptrDepth) {
		TypeDescriptor desc = MoSyncExtensionLoader.getDefault().getModule(extension).getTypeDescriptor(name, ptrDepth, false);
		return desc;
	}

	public abstract int size();

}
