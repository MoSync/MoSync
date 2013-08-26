/* Copyright 2013 David Axmark

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

	http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

/*
 * nfcutil.h
 *
 * \author Mattias Bybro
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
	int getSize();
};

class MimeMediaNdefRecord : NdefRecord {
public:
	MimeMediaNdefRecord(MAHandle handle) : NdefRecord(handle) {};
	virtual ~MimeMediaNdefRecord();
	static bool isMimeType(MAHandle handle, String mimeType);
	void setMimeType(String mimeType);
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
