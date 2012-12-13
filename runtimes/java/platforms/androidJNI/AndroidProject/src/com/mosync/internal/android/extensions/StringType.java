package com.mosync.internal.android.extensions;

import java.nio.ByteBuffer;
import java.nio.charset.Charset;

import com.mosync.internal.android.MoSyncThread;

public class StringType extends TypeDescriptor {
	@Override
	public Class getNativeClass() {
		return String.class;
	}

	@Override
	public Object convert(int[] args, int offset) {
		int charPtr = args[offset];
		if (charPtr == 0) {
			return null;
		}
		ByteBuffer strBuf = MoSyncThread.getInstance().getMemorySlice(
				charPtr, -1);
		int size = 0;
		while (strBuf.get() != '\0' && size < strBuf.limit()) {
			size++;
		}
		strBuf.position(0);
		byte[] buf = new byte[size];
		strBuf.get(buf);

		return new String(buf, Charset.forName("UTF-8"));
	}

}