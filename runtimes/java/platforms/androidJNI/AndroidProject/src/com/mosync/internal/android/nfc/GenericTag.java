package com.mosync.internal.android.nfc;

import static com.mosync.internal.generated.MAAPI_consts.MA_NFC_TAG_TYPE_ISO_DEP;
import static com.mosync.internal.generated.MAAPI_consts.MA_NFC_TAG_TYPE_MIFARE_CL;
import static com.mosync.internal.generated.MAAPI_consts.MA_NFC_TAG_TYPE_MIFARE_UL;
import static com.mosync.internal.generated.MAAPI_consts.MA_NFC_TAG_TYPE_NDEF;
import static com.mosync.internal.generated.MAAPI_consts.MA_NFC_TAG_TYPE_NDEF_FORMATTABLE;
import static com.mosync.internal.generated.MAAPI_consts.MA_NFC_TAG_TYPE_NFC_A;
import static com.mosync.internal.generated.MAAPI_consts.MA_NFC_TAG_TYPE_NFC_B;

import java.io.IOException;

import android.nfc.Tag;
import android.nfc.tech.TagTechnology;

public class GenericTag extends ResourceBase implements INFCTag {

	private final Tag tag;

	public GenericTag(ResourcePool pool, Tag tag) {
		super(pool);
		this.tag = tag;
	}

	public Tag getTag() {
		return tag;
	}

	public boolean isType(int type) {
		return toTypedTag(ResourcePool.NULL, type) != null;
	}

	@Override
	public INFCTag toTypedTag(ResourcePool pool, int type) {
		switch (type) {
		case MA_NFC_TAG_TYPE_NDEF:
			return NdefTag.get(pool, this);
		case MA_NFC_TAG_TYPE_MIFARE_CL:
			return MifareClassicTag.get(pool, this);
		case MA_NFC_TAG_TYPE_MIFARE_UL:
			return MifareUltralightTag.get(pool, this);
		case MA_NFC_TAG_TYPE_NFC_A:
			return NfcATag.get(pool, this);
		case MA_NFC_TAG_TYPE_NFC_B:
			return NfcBTag.get(pool, this);
		case MA_NFC_TAG_TYPE_ISO_DEP:
			return IsoDepTag.get(pool, this);
		case MA_NFC_TAG_TYPE_NDEF_FORMATTABLE:
			return NdefFormatableTag.get(pool, this);
		default:
			return null;
		}
	}

	@Override
	public void close() throws IOException {
		// Does nothing
	}

	@Override
	public void connect() throws IOException {
		// Does nothing
	}

	@Override
	public TagTechnology nativeTag() {
		return null;
	}

}
