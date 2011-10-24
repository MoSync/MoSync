package com.mosync.internal.android.nfc.ops;

import java.io.IOException;
import java.nio.ByteBuffer;

import android.nfc.tech.MifareUltralight;

import com.mosync.internal.android.nfc.MifareUltralightTag;

public class MFUReadPages extends MFRead<MifareUltralightTag> {

	private final MifareUltralight mifareTag;

	public MFUReadPages(MifareUltralightTag tag, int firstPage,
			ByteBuffer dstBuffer) {
		super(tag, firstPage, dstBuffer);
		this.mifareTag = tag.nativeTag();
	}

	@Override
	protected byte[] readChunk(int pageIndex) throws IOException {
		return mifareTag.readPages(pageIndex);
	}
}
