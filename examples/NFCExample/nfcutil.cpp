/*
 * nfcutil.cpp
 *
 *  Created on: Oct 11, 2011
 *      Author: mattias
 */

#include <maapi.h>
#include <maheap.h>
#include <MAUtil/String.h>
#include <MAUtil/Vector.h>

using namespace MAUtil;

class UriNdefRecord {
private:
	MAHandle fHandle;
public:
	UriNdefRecord(MAHandle handle) {
		fHandle = handle;
	}

	virtual ~UriNdefRecord() {
		// Do nothing
	}

	static bool isValid(MAHandle rec) {
		return UriNdefRecord(rec).getUri() != NULL;
	}

	void setUri(String& uri) {
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

	void setUri(byte prefixCode, MAUtil::String& remainingUri) {
		int length = remainingUri.length();
		maNFCSetTnf(fHandle, MA_NFC_NDEF_TNF_WELL_KNOWN);
		byte type[] = { (byte) 0x55 }; // URI
		maNFCSetType(fHandle, type, 1);
		Vector<byte> payload(length * sizeof(char) + 1);
		payload[0] = prefixCode;
		const char* szRemainingUri = remainingUri.c_str();
		for (int i = 0; i < length; i++) {
			payload[1 + sizeof(char) * i] = szRemainingUri[i];
		}
		maNFCSetPayload(fHandle, payload.pointer(), payload.size());
	}

	String getUri() {
		int tnf = maNFCGetTnf(fHandle);
		byte type[1];
		byte payload[256];
		String result = String();
		switch (tnf) {
		case MA_NFC_NDEF_TNF_ABSOLUTE_URI:
			break;
		case MA_NFC_NDEF_TNF_WELL_KNOWN:
			int typeLen = maNFCGetType(fHandle, type, 1);
			if (typeLen == 1 && type[0] == 0x55) {
				int length = maNFCGetPayload(fHandle, payload, sizeof(payload));
				if (length > 1) {
					result += getPrefix(payload[0]);
					for (int i = 0; i < length; i++) {
						result[i] = payload[i];
					}
					return result;
				}
			}
			break;
		}

		// No match.
		return NULL;
	}

	String getPrefix(int prefixCode) {
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
};
