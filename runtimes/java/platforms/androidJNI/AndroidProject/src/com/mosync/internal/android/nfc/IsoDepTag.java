package com.mosync.internal.android.nfc;

import static com.mosync.internal.generated.MAAPI_consts.MA_NFC_TAG_TYPE_ISO_DEP;

import java.io.IOException;

import android.nfc.tech.IsoDep;

public class IsoDepTag extends NFCTagBase<IsoDep> {

	public static IsoDepTag get(ResourcePool pool, GenericTag tag) {
		IsoDep isoDep = IsoDep.get(tag.getTag());
		return isoDep == null ? null : new IsoDepTag(pool, isoDep);
	}

	private IsoDepTag(ResourcePool pool, IsoDep isoDep) {
		super(pool, isoDep, MA_NFC_TAG_TYPE_ISO_DEP);
	}

	@Override
	public byte[] transceive(byte[] buffer) throws IOException {
		return nativeTag.transceive(buffer);
	}

}
