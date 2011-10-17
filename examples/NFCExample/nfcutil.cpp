/*
 * nfcutil.cpp
 *
 *  Created on: Oct 11, 2011
 *      Author: mattias
 */

#include <maapi.h>
#include <maheap.h>
#include <mastring.h>
#include <MAUtil/String.h>
#include <MAUtil/Vector.h>
#include "nfcutil.h"

#define INITIAL_SIZE 8

using namespace MAUtil;

NdefRecord::NdefRecord(MAHandle handle) {
	fHandle = handle;
}

MAHandle NdefRecord::getHandle() {
	return fHandle;
}

int NdefRecord::getTnf() {
	return maNFCGetTnf(fHandle);
}

Vector<byte> NdefRecord::getId() {
	Vector<byte> result(INITIAL_SIZE);
	byte dst[NFC_MAX_PAYLOAD_LENGTH];
	int length = maNFCGetId(fHandle, dst, sizeof(dst));
	result.add(dst, length);
	return result;
}

Vector<byte> NdefRecord::getType() {
	Vector<byte> result(INITIAL_SIZE);
	byte dst[NFC_MAX_PAYLOAD_LENGTH];
	int length = maNFCGetType(fHandle, dst, sizeof(dst));
	result.add(dst, length);
	return result;
}

Vector<byte> NdefRecord::getPayload() {
	Vector<byte> result(INITIAL_SIZE);
	byte dst[NFC_MAX_PAYLOAD_LENGTH];
	int length = maNFCGetPayload(fHandle, dst, sizeof(dst));
	result.add(dst, length);
	return result;
}

void NdefRecord::setTnf(int tnf) {
	maNFCSetTnf(fHandle, tnf);
}

void NdefRecord::setId(Vector<byte>& id) {
	maNFCSetId(fHandle, id.pointer(), id.size());
}

void NdefRecord::setType(Vector<byte>& type) {
	maNFCSetType(fHandle, type.pointer(), type.size());
}

void NdefRecord::setPayload(Vector<byte>& payload) {
	maNFCSetPayload(fHandle, payload.pointer(), payload.size());
}

MimeMediaNdefRecord::~MimeMediaNdefRecord() {
	// Do nothing
}

String MimeMediaNdefRecord::getMimeType() {
	Vector<byte> type = getType();
	return String((char*) type.pointer(), type.size());
}

bool MimeMediaNdefRecord::isMimeType(MAHandle recHandle, String mimeType) {
	MimeMediaNdefRecord rec = MimeMediaNdefRecord(recHandle);
	return rec.getTnf() == MA_NFC_NDEF_TNF_MIME_MEDIA && mimeType == rec.getMimeType();
}

UriNdefRecord::~UriNdefRecord() {
	// Do nothing
}

bool UriNdefRecord::isValid(MAHandle recHandle) {
	UriNdefRecord rec = UriNdefRecord(recHandle);
	// Just to init that valid thing.
	rec.getUri();
	return rec.valid;
}

void UriNdefRecord::setUri(String& uri) {
	// There are two ways to encode uris
	// in NDEF records:
	// 1. TNF = absolute URI
	// 2. TNF = well-known, with the type set
	int tnf = maNFCGetTnf(fHandle);
	switch (tnf) {
	case MA_NFC_NDEF_TNF_ABSOLUTE_URI:
		maNFCSetType(fHandle, NULL, 0);
		maNFCSetPayload(fHandle, uri.c_str(), uri.length());
		break;
	default:
		setUri(0x00, uri);
	}
}

void UriNdefRecord::setUri(byte prefixCode, MAUtil::String& remainingUri) {
	int length = remainingUri.length();
	maNFCSetTnf(fHandle, MA_NFC_NDEF_TNF_WELL_KNOWN);
	byte type[] = { (byte) 0x55 }; // URI
	maNFCSetType(fHandle, type, 1);
	Vector<byte> payload(length * sizeof(char) + 1);
	payload.add(prefixCode);
	const char* szRemainingUri = remainingUri.c_str();
	for (int i = 0; i < length; i++) {
		payload.insert(1 + sizeof(char) * i, szRemainingUri[i]);
	}
	maNFCSetPayload(fHandle, payload.pointer(), payload.size());
}

String UriNdefRecord::getUri() {
	valid = false;
	int tnf = maNFCGetTnf(fHandle);
	byte type[1];
	byte payload[256];
	switch (tnf) {
	case MA_NFC_NDEF_TNF_ABSOLUTE_URI:
		valid = true;
		return String((char*)getPayload().pointer());
	case MA_NFC_NDEF_TNF_WELL_KNOWN:
		int typeLen = maNFCGetType(fHandle, type, 1);
		if (typeLen > 0 && type[0] == 0x55) {
			Vector<byte> payload = getPayload();
			int length = payload.size();
			if (length > 0) {
				String result = String();
				const char* prefix = getPrefix(payload[0]);
				result.append(prefix, strlen(prefix));
				for (int i = 1; i < length; i++) {
					result.insert(result.size(), (char)payload[i]);
				}
				valid = true;
				return result;
			}
		}
		break;
	}

	// No match.
	return String();
}

const char* UriNdefRecord::getPrefix(byte prefixCode) {
	switch (prefixCode) {
	case 0x00:
		return "";
	case 0x01:
		return "http://www.";
	case 0x02:
		return "https://www.";
	case 0x03:
		return "http://";
	case 0x04:
		return "https://";
	case 0x05:
		return "tel:";
	case 0x06:
		return "mailto:";
	case 0x07:
		return "ftp://anonymous:anonymous@";
	case 0x08:
		return "ftp://ftp.";
	case 0x09:
		return "ftps://";
	case 0x0A:
		return "sftp://";
	case 0x0B:
		return "smb://";
	case 0x0C:
		return "nfs://";
	case 0x0D:
		return "ftp://";
	case 0x0E:
		return "dav://";
	case 0x0F:
		return "news:";
	case 0x10:
		return "telnet://";
	case 0x11:
		return "imap:";
	case 0x12:
		return "rtsp://";
	case 0x13:
		return "urn:";
	case 0x14:
		return "pop:";
	case 0x15:
		return "sip:";
	case 0x16:
		return "sips:";
	case 0x17:
		return "tftp:";
	case 0x18:
		return "btspp://";
	case 0x19:
		return "btl2cap://";
	case 0x1A:
		return "btgoep://";
	case 0x1B:
		return "tcpobex://";
	case 0x1C:
		return "irdaobex://";
	case 0x1D:
		return "file://";
	case 0x1E:
		return "urn:epc:id:";
	case 0x1F:
		return "urn:epc:tag:";
	case 0x20:
		return "urn:epc:pat:";
	case 0x21:
		return "urn:epc:raw:";
	case 0x22:
		return "urn:epc:";
	case 0x23:
		return "urn:nfc:";
	default: // Same as 0x00
		return "";
	}
}
