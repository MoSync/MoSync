/*
Copyright (C) 2011 MoSync AB

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
