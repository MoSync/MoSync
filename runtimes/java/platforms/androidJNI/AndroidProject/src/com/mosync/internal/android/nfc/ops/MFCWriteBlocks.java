package com.mosync.internal.android.nfc.ops;

import java.io.IOException;
import java.nio.ByteBuffer;

import android.nfc.tech.MifareClassic;

import com.mosync.internal.android.nfc.MifareClassicTag;

public class MFCWriteBlocks extends MFWrite<MifareClassicTag> {

	private static final int BLOCK_SIZE = 16;
	private final MifareClassic mifareTag;

	public MFCWriteBlocks(MifareClassicTag mifareTag, int firstBlock, ByteBuffer srcBuffer) {
		super(mifareTag, firstBlock, srcBuffer);
		this.mifareTag = mifareTag.nativeTag();
	}

	@Override
	protected void writeChunk(int chunkIndex, byte[] chunk) throws IOException {
		mifareTag.writeBlock(chunkIndex, chunk);
	}

	@Override
	protected int chunkSize() {
		return BLOCK_SIZE;
	}

}
