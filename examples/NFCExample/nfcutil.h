/*
 * nfcutil.h
 *
 *  Created on: Oct 11, 2011
 *      Author: mattias
 */

#ifndef NFCUTIL_H_
#define NFCUTIL_H_

#include <MAUtil/String.h>
#include <MAUtil/Vector.h>

#define NFC_MAX_PAYLOAD_LENGTH 4096

using namespace MAUtil;

class NdefRecord {
protected:
	MAHandle fHandle;
public:
	NdefRecord(MAHandle handle);
	MAHandle getHandle();
	int getTnf();
	Vector<byte> getId();
	Vector<byte> getType();
	Vector<byte> getPayload();
	void setTnf(int tnf);
	void setId(Vector<byte>& id);
	void setType(Vector<byte>& type);
	void setPayload(Vector<byte>& payload);
};

class MimeMediaNdefRecord : NdefRecord {
public:
	MimeMediaNdefRecord(MAHandle handle) : NdefRecord(handle) {};
	virtual ~MimeMediaNdefRecord();
	static bool isMimeType(MAHandle handle, String mimeType);
	String getMimeType();
};

/**
 * A utility class for handling NDEF URI records.
 * These have a format as per defined by the
 * "URI Record Type Definition" (NFC Forum).
 */
class UriNdefRecord : NdefRecord {
private:
	bool valid;
	char* getPrefix(int prefixCode);
public:
	UriNdefRecord(MAHandle handle) : NdefRecord(handle) {};
	virtual ~UriNdefRecord();
	static bool isValid(MAHandle handle);
	void setUri(String& uri);
	void setUri(byte prefixCode, String& remainingUri);
	String getUri();
	const char* getPrefix(byte prefixCode);
};

#endif /* NFCUTIL_H_ */
