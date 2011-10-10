package com.mosync.internal.android.nfc;

import static com.mosync.internal.generated.MAAPI_consts.MA_NFC_TAG_TYPE_NDEF_FORMATTABLE;

import java.io.IOException;

import android.nfc.FormatException;
import android.nfc.tech.NdefFormatable;

public class NdefFormatableTag extends NFCTagBase<NdefFormatable> implements INDEFMessageHolder {

	public static INFCTag get(ResourcePool pool, GenericTag tag) {
		NdefFormatable ndef = NdefFormatable.get(tag.getTag());
		if (ndef != null) {
			NdefFormatableTag result = new NdefFormatableTag(pool, ndef);
			return result;
		}

		return null;
	}

	public NdefFormatableTag(ResourcePool pool, NdefFormatable nativeTag) {
		super(pool, nativeTag, MA_NFC_TAG_TYPE_NDEF_FORMATTABLE);
	}

	@Override
	public void requestNDEFMessage(ResourcePool pool) throws IOException {
		throw new IOException("Not supported.");
	}

	@Override
	public NDEFMessage getNDEFMessage() {
		throw new UnsupportedOperationException("Not supported.");
	}

	@Override
	public void writeNDEFMessage(NDEFMessage message) throws IOException {
		try {
			nativeTag.format(message.toNativeNDEFMessage());
		} catch (FormatException e) {
			throw new IOException(e);
		}
	}

}
