package com.mosync.internal.android.nfc.ops;

import java.io.IOException;
import java.nio.ByteBuffer;

import android.nfc.tech.MifareUltralight;

import com.mosync.internal.android.nfc.MifareUltralightTag;

public class MFUWritePages extends MFWrite<MifareUltralightTag> {

	private static final int PAGE_SIZE = 4;

	private final MifareUltralight mifareTag;

	public MFUWritePages(MifareUltralightTag tag, int firstPage,
			ByteBuffer srcBuffer) {
		super(tag, firstPage, srcBuffer);
		this.mifareTag = tag.nativeTag();
	}

	@Override
	protected void writeChunk(int pageIndex, byte[] page) throws IOException {
		mifareTag.writePage(pageIndex, page);
	}

	@Override
	protected int chunkSize() {
		return PAGE_SIZE;
	}
}
