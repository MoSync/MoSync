package com.mosync.internal.android.nfc;

import java.io.IOException;

import android.nfc.tech.TagTechnology;

public interface ITransceivable<NativeTagType extends TagTechnology> extends INFCTag<NativeTagType> {
	byte[] transceive(byte[] buffer) throws IOException;
}
