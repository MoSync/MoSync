package com.mosync.internal.android.nfc.ops;

import java.io.IOException;
import java.nio.ByteBuffer;

import com.mosync.internal.android.nfc.MifareClassicTag;

import android.nfc.tech.MifareClassic;

public class MFCReadBlocks extends MFRead<MifareClassicTag> {

	private final MifareClassic mifareTag;

	public MFCReadBlocks(MifareClassicTag mifareTag, int firstBlock,
			ByteBuffer dstBuffer) {
		super(mifareTag, firstBlock, dstBuffer);
		this.mifareTag = mifareTag.nativeTag();
	}

	@Override
	protected byte[] readChunk(int blockIndex) throws IOException {
		return mifareTag.readBlock(blockIndex);
	}

}
