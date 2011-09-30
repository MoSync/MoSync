package com.mosync.pim;

import java.nio.ByteBuffer;
import java.util.Map;

import com.mosync.internal.android.MoSyncThread;

public class PIMUtil {

	public static MoSyncThread sMoSyncThread;

	/**
	 * Gets the key from a map by it's value.
	 * @param hm
	 * @param value
	 * @return
	 */
	static Object getKeyFromValue(Map<?, ?> hm, Object value) {
		for (Object o : hm.keySet()) {
			if (hm.get(o).equals(value)) {
				return o;
			}
		}
		return null;
	}

	/**
	 * Copy a String value to the given address in the system memory.
	 * @param address
	 * @param str
	 */
	static void copyBufferToMemory(int address, char[] str) {
		copyBufferToMemory(address, str, str.length);
	}

	/**
	 * Copy a String value to the given address in the system memory.
	 * @param address
	 * @param str
	 * @param length
	 */
	static void copyBufferToMemory(int address, char[] str, int length) {
		getMemDataSection().position(address);
		for (int i = 0; i < length; i++) {
			getMemDataSection().putChar(str[i]);
		}
		getMemDataSection().put((byte) 0); // Terminating null char.
	}

	/**
	 * Read string from system memory.
	 * @param address
	 * @param length
	 * @return
	 */
	static char[] readBufferFromMemory(int address, int length) {
		char[] buffer = new char[length];
		getMemDataSection().position(address);
		for (int i = 0; i < length; i++) {
			buffer[i] = getMemDataSection().getChar();
		}
		return buffer;
	}

	/**
	 * @return The MoSync data section memory buffer.
	 */
	static public ByteBuffer getMemDataSection() {
		return sMoSyncThread.mMemDataSection;
	}

	/**
	 * @return The MoSync data section memory buffer.
	 */
	static public MoSyncThread getThread() {
		return sMoSyncThread;
	}

	static void writeInt(int val, char[] buffer, int index) {
		buffer[index++] = (char) (val & 0xFFFF);
		buffer[index++] = (char) ((val >> 16) & 0xFFFF);
	}

	static int readInt(char[] buffer, int index) {
		if (buffer.length < 2)
			return buffer[index];
		return (buffer[index] | (buffer[index + 1] << 16));
	}

	static void writeStringArray(String[] value, char[] buffer) {
		int index = 0;
		writeInt(value.length, buffer, index);
		index += 2;
		for (int i = 0; i < value.length; i++) {
			if (value[i] != null) {
				System.arraycopy(value[i].toCharArray(), 0, buffer, index,
						value[i].length());
				index += value[i].length();
			}
			buffer[index++] = 0;
		}
	}

	static void writeString(String value, char[] buffer) {
		int index = 0;
		if (value != null) {
			System.arraycopy(value.toCharArray(), 0, buffer, index,
					value.length());
			index += value.length();
		}
		buffer[index++] = 0;
	}

	static String[] readStringArray(char[] buffer) {
		int buffIndex = 0;
		int length = readInt(buffer, buffIndex);
		buffIndex += 2;
		String[] val = new String[length];
		for (int i = 0; i < length; i++) {
			int crtIndex = buffIndex;
			while ((crtIndex < buffer.length) && (buffer[crtIndex++] != 0))
				;
			crtIndex--;
			int len = crtIndex - buffIndex;
			char[] tmp = new char[len];
			System.arraycopy(buffer, buffIndex, tmp, 0, len);
			val[i] = new String(tmp);
			tmp = null;
			buffIndex += val[i].length();
			buffIndex++;
		}

		return val;
	}

	static String readString(char[] buffer) {
		int buffIndex = 0;
		while ((buffIndex < buffer.length) && (buffer[buffIndex++] != 0)) {
		}
		;
		buffIndex--;
		char[] tmp = new char[buffIndex];
		System.arraycopy(buffer, 0, tmp, 0, buffIndex);

		return new String(tmp);
	}
}
