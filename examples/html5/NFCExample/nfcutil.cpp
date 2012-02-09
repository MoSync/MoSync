/*
Copyright (C) 2012 MoSync AB

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License,
version 2, as published by the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
MA 02110-1301, USA.
*/

/*
 * nfcutil.cpp
 *
 * \author Mattias Bybro
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
	return maNFCGetNDEFTnf(fHandle);
}

Vector<byte> NdefRecord::getId() {
	Vector<byte> result(INITIAL_SIZE);
	byte dst[NFC_MAX_PAYLOAD_LENGTH];
	int length = maNFCGetNDEFId(fHandle, dst, sizeof(dst));
	result.add(dst, length);
	return result;
}

Vector<byte> NdefRecord::getType() {
	Vector<byte> result(INITIAL_SIZE);
	byte dst[NFC_MAX_PAYLOAD_LENGTH];
	int length = maNFCGetNDEFType(fHandle, dst, sizeof(dst));
	result.add(dst, length);
	return result;
}

Vector<byte> NdefRecord::getPayload() {
	Vector<byte> result(INITIAL_SIZE);
	byte dst[NFC_MAX_PAYLOAD_LENGTH];
	int length = maNFCGetNDEFPayload(fHandle, dst, sizeof(dst));
	result.add(dst, length);
	return result;
}

void NdefRecord::setTnf(int tnf) {
	maNFCSetNDEFTnf(fHandle, tnf);
}

void NdefRecord::setId(Vector<byte>& id) {
	maNFCSetNDEFId(fHandle, id.pointer(), id.size());
}

void NdefRecord::setType(Vector<byte>& type) {
	maNFCSetNDEFType(fHandle, type.pointer(), type.size());
}

void NdefRecord::setPayload(Vector<byte>& payload) {
	maNFCSetNDEFPayload(fHandle, payload.pointer(), payload.size());
}

int NdefRecord::getSize() {
	return maNFCGetSize(fHandle);
}

MimeMediaNdefRecord::~MimeMediaNdefRecord() {
	// Do nothing
}

void MimeMediaNdefRecord::setMimeType(String mimeType) {
	setTnf(MA_NFC_NDEF_TNF_MIME_MEDIA);
	maNFCSetNDEFType(fHandle, mimeType.c_str(), mimeType.size());
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
	int tnf = getTnf();
	switch (tnf) {
	case MA_NFC_NDEF_TNF_ABSOLUTE_URI:
		maNFCSetNDEFType(fHandle, NULL, 0);
		maNFCSetNDEFPayload(fHandle, uri.c_str(), uri.length());
		break;
	default:
		setUri(0x00, uri);
	}
}

void UriNdefRecord::setUri(byte prefixCode, MAUtil::String& remainingUri) {
	int length = remainingUri.length();
	setTnf(MA_NFC_NDEF_TNF_WELL_KNOWN);
	byte type[] = { (byte) 0x55 }; // URI
	maNFCSetNDEFType(fHandle, type, 1);
	Vector<byte> payload(length * sizeof(char) + 1);
	payload.add(prefixCode);
	const char* szRemainingUri = remainingUri.c_str();
	for (int i = 0; i < length; i++) {
		payload.insert(1 + sizeof(char) * i, szRemainingUri[i]);
	}
	setPayload(payload);
}

String UriNdefRecord::getUri() {
	valid = false;
	int tnf = getTnf();
	byte type[1];
	switch (tnf) {
	case MA_NFC_NDEF_TNF_ABSOLUTE_URI:
		valid = true;
		return String((char*)getPayload().pointer());
	case MA_NFC_NDEF_TNF_WELL_KNOWN:
		int typeLen = maNFCGetNDEFType(fHandle, type, 1);
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
