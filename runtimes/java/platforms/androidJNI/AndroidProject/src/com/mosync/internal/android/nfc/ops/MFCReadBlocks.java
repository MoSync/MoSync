package com.mosync.internal.android.nfc.ops;

import static com.mosync.internal.generated.MAAPI_consts.EVENT_TYPE_NFC_TAG_DATA_READ;
import static com.mosync.internal.generated.MAAPI_consts.MA_NFC_TAG_CONNECTION_LOST;
import static com.mosync.internal.generated.MAAPI_consts.MA_NFC_TAG_IO_ERROR;

import java.io.IOException;
import java.nio.ByteBuffer;

import com.mosync.internal.android.nfc.MifareClassicTag;
import com.mosync.internal.android.nfc.NFCEvent;

import android.nfc.TagLostException;
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
