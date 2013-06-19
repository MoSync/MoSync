package com.mosync.api;

import com.mosync.internal.android.extensions.MoSyncExtensionLoader;
import com.mosync.internal.android.extensions.StringType;
import com.mosync.internal.android.extensions.TypeDescriptor;



public abstract class Struct implements Marshaller {

	protected static final Marshaller _m(String extension, String name, int ptrDepth) {
		// Special case: String vs CString
		boolean out = StringType.isStringType(name);
		TypeDescriptor desc = MoSyncExtensionLoader.getDefault().getModule(extension).getTypeDescriptor(name, ptrDepth, out);
		return desc;
	}

	public abstract int size();

}
