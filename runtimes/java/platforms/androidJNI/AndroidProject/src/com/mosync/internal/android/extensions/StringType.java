package com.mosync.internal.android.extensions;

import java.nio.ByteBuffer;
import java.nio.charset.Charset;

import android.annotation.SuppressLint;

import com.mosync.api.CString;
import com.mosync.api.CString.Storage;
import com.mosync.internal.android.MoSyncThread;

public class StringType extends TypeDescriptor {

	private final static Charset UTF8 = Charset.forName("UTF-8");
	private static StringType constInstance = new StringType(false);
	private static StringType varInstance = new StringType(true);
	private final static int charSize = 1;
	private final static int wcharSize = MoSyncThread.getInstance().isNative() ? 4 : 2;
	private final static Charset wcharset = MoSyncThread.getInstance().isNative() ?
			Charset.forName("UTF-32LE") :
			Charset.forName("UTF-16LE");

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
		return isOut() ? new CStringImpl(charPtr, -1) : unmarshalCString(charPtr, -1);
	}

	public static String unmarshalCString(int charPtr, int maxLen) {
		return unmarshalString(charPtr, maxLen, charSize, UTF8);
	}

	public static String unmarshalWString(int charPtr, int maxLen) {
		return unmarshalString(charPtr, maxLen, wcharSize, wcharset);
	}

	@SuppressLint("NewApi")
	private static String unmarshalString(int charPtr, int maxLen, int sizeof, Charset cs) {
		if (charPtr == 0) {
			return null;
		}

		int cursor = charPtr;
		byte[] chunk = new byte[16 * sizeof];
		int terminatePos = -1;
		while (terminatePos == -1) {
			ByteBuffer strBuf = MoSyncThread.getInstance().getMemorySlice(cursor, chunk.length);
			strBuf.get(chunk);
			terminatePos = findTerminatePos(chunk, sizeof);
			int len = terminatePos == -1 ? chunk.length : terminatePos;
			cursor += len;
		}
		int size = cursor - charPtr;
		byte[] buf = new byte[size];
		// Kind of wasteful, we may consider a growing byte array instead.
		ByteBuffer fullBuf = MoSyncThread.getInstance().getMemorySlice(charPtr, buf.length);
		fullBuf.get(buf);
		return new String(buf, cs);
	}

	private static int findTerminatePos(byte[] chunk, int sizeof) {
		// TODO: Modified UTF?
		for (int i = 0; i < chunk.length; i += sizeof) {
			boolean found = true;
			for (int j = 0; j < sizeof; j++) {
				if (chunk[i + j] != 0) {
					found = false;
				}
			}
			if (found) {
				return i;
			}
		}
		return -1;
	}

	public int size() {
		return 4;
	}

	@Override
	public void marshal(Object o, byte[] data, int offset) {
		// Note: Just marshals the *pointer*, use CString.set.
		CStringImpl str = (CStringImpl) o;
		IntType.marshalInt(str.getPointer().getAddress(), data, offset);
	}

	public static int marshalCString(int charPtr, String str, int maxLen) {
		return marshalString(null, 0, charPtr, str, maxLen, charSize, UTF8);
	}

	public static int marshalWString(int charPtr, String str, int maxLen) {
		return marshalString(null, 0, charPtr, str, maxLen, wcharSize, wcharset);
	}

	public static int marshalCString(byte[] buffer, int offset, String str, int maxLen) {
		return marshalString(buffer, offset, 0, str, maxLen, charSize, UTF8);
	}

	public static int marshalWString(byte[] buffer, int offset, String str, int maxLen) {
		return marshalString(buffer, offset, 0, str, maxLen, wcharSize, wcharset);
	}

	private static int marshalString(byte[] array, int offset, int charPtr, String str, int maxLen, int sizeof, Charset cs) {
		if (maxLen < 1 || (array != null && array.length == 0)) {
			return 0;
		}
		byte[] bytes = str.getBytes(cs);
		int len = Math.min(sizeof * (maxLen - 1), bytes.length);
		// Array or not array?
		if (array == null) {
			ByteBuffer strBuf = MoSyncThread.getInstance().getMemorySlice(
					charPtr, len + sizeof);
			strBuf.put(bytes, 0, len);
			// Null terminate
			for (int i = 0; i < sizeof; i++) {
				strBuf.put((byte) 0x00);
			}
		} else {
			len = Math.min(len, array.length - offset - sizeof);
			System.arraycopy(bytes, 0, array, offset, len);
			// Null terminate
			for (int i = 0; i < sizeof; i++) {
				array[offset + len + i] = (byte)0x00;
			}
		}
		// Return the number of bytes, not number of chars
		return len + sizeof;
	}

	public static boolean isStringType(String type) {
		return "NCString".equals(type) || "MAString".equals(type);
	}

	public static int getCharSize(Storage storage) {
		return storage == Storage.CHAR ? charSize : wcharSize;
	}

}