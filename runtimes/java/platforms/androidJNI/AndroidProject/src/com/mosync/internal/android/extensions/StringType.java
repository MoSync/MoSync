package com.mosync.internal.android.extensions;

import java.nio.ByteBuffer;
import java.nio.charset.Charset;

import android.annotation.SuppressLint;

import com.mosync.api.CString;
import com.mosync.internal.android.MoSyncThread;

public class StringType extends TypeDescriptor {

	private final static Charset UTF8 = Charset.forName("UTF-8");
	private static StringType constInstance = new StringType(false);
	private static StringType varInstance = new StringType(true);

	public static TypeDescriptor getInstance(boolean out) {
		return out ? varInstance : constInstance;
	}

	private StringType(boolean out) {
		setOut(out);
	}

	@Override
	public Class getNativeClass() {
		return isOut() ? CString.class : String.class;
	}

	@Override
	public Object unmarshal(byte[] data, int offset) {
		int charPtr = IntType.unmarshalInt(data, offset);
		return isOut() ? new CStringImpl(charPtr) : unmarshalString(charPtr, -1);
	}

	@SuppressLint("NewApi")
	public static String unmarshalString(int charPtr, int maxLen) {
		if (charPtr == 0) {
			return null;
		}
		ByteBuffer strBuf = MoSyncThread.getInstance().getMemorySlice(
				charPtr, maxLen);
		int size = 0;
		while (strBuf.get() != '\0' && size < strBuf.limit()) {
			size++;
		}
		strBuf.position(0);
		byte[] buf = new byte[size];
		strBuf.get(buf);

		return new String(buf, Charset.forName("UTF-8"));
	}

	public int size() {
		return 4;
	}

	@Override
	public void marshal(Object o, byte[] data, int offset) {
		CStringImpl str = (CStringImpl) o;
		IntType.marshalInt(str.getAddress(), data, offset);
	}

	public static int marshalString(int charPtr, String str, int maxLen) {
		if (maxLen < 1) {
			return 0;
		}
		byte[] bytes = str.getBytes();
		ByteBuffer strBuf = MoSyncThread.getInstance().getMemorySlice(
				charPtr, maxLen);
		strBuf.put(bytes, 0, Math.min(maxLen - 1, bytes.length));
		strBuf.put((byte) 0x00);
		return bytes.length;
	}

	public static boolean isStringType(String type) {
		return "NCString".equals(type) || "MAString".equals(type);
	}

}