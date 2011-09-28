/* Copyright (C) 2011 MoSync AB

 This program is free software; you can redistribute it and/or modify it under
 the terms of the GNU General Public License, version 2, as published by
 the Free Software Foundation.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 for more details.

 You should have received a copy of the GNU General Public License
 along with this program.  If not, write to the Free
 Software Foundation, 59 Temple Place - Suite 330, Boston, MA
 02111-1307, USA.
 */

/** @file main.cpp
 *
 *
 *
 * @author The MoSync Team
 */

#include <MAUtil/Moblet.h>
#include <ma.h>
#include <conprint.h>
#ifdef USE_NEWLIB
#include <math.h>
#include <malloc.h>
#include <float.h>
#else
#include <madmath.h>
#include <maheap.h>
#endif	//USE_NEWLIB
using namespace MAUtil;

#define BUF_SIZE 128

/**
 * NFC DEMO: ADD INFO HERE!
 */
class NFCDemo: public Moblet {
private:
	byte payload[BUF_SIZE];
	char hexEncoded[2 * BUF_SIZE];
	const char* HEX_CHARS;

public:

	/**
	 * Initialize the application in the constructor.
	 */
	NFCDemo() {
		int result = maNFCStart();
		if (result == MA_NFC_NOT_AVAILABLE) {
			printf("NFC not available on this device.");
		} else if (result == MA_NFC_NOT_ENABLED) {
			printf(
					"NFC not enabled on this device.\nPlease enable NFC and restart.");
		} else {
			printf("Hold a tag close to your handset to dump its contents.");
		}
		printf("\n---Press zero or back to exit.");
		HEX_CHARS = "0123456789ABCDEF";
	}

	/**
	 * Quit the program if back key or 0 is pressed.
	 */
	void keyPressEvent(int keyCode, int nativeCode) {
		if (MAK_BACK == keyCode || MAK_0 == keyCode) {
			// Call close to exit the application.
			close();
		}
	}

	void pointerPressEvent(MAPoint2d p) {
		printf("X=%d,Y=%d", p.x, p.y);
	}

	void customEvent(const MAEvent& event) {
		if (EVENT_TYPE_NFC_TAG_RECEIVED == event.type) {
			MANFCEventData nfcEventData = event.nfc;
			printf("EVENT STATE: %d", event.state);
			printf("RECV : %X,%X,%X,%X", event.type, nfcEventData.handle,
					nfcEventData.result, nfcEventData.dstId);
			MAHandle tag = maNFCReadTag(nfcEventData.handle);
			if (tag) {
				if (maNFCIsType(tag, MA_NFC_TAG_TYPE_NDEF)) {
					MAHandle ndef = maNFCGetTypedTag(tag, MA_NFC_TAG_TYPE_NDEF);
					printf("Found RFID (NDEF) tag");
					handleNDEF(ndef);
				} else if (maNFCIsType(tag, MA_NFC_TAG_TYPE_MIFARE_CL)) {
					MAHandle mfc = maNFCGetTypedTag(tag,
							MA_NFC_TAG_TYPE_MIFARE_CL);
					printf("Found RFID (MiFare Classic) tag");
					maNFCConnectTag(mfc);
					maNFCReadBlocks(mfc, 0, payload, BUF_SIZE);
					maNFCCloseTag(mfc);
				} else if (maNFCIsType(tag, MA_NFC_TAG_TYPE_MIFARE_UL)) {
					MAHandle mfu = maNFCGetTypedTag(tag,
							MA_NFC_TAG_TYPE_MIFARE_UL);
					printf("Found RFID (MiFare Ultralight) tag");
					maNFCConnectTag(mfu);
					maNFCReadPages(mfu, 0, payload, BUF_SIZE);
					maNFCCloseTag(mfu);
				}
				maNFCDestroyTag(tag);
			}
		} else if (EVENT_TYPE_NFC_TAG_DATA_READ == event.type) {
			MANFCEventData& data = (MANFCEventData&) event.nfc;
			printf("%d,0x%X,0x%X,0x%X", event.type, data.handle, data.result,
					data.dstId);
			int tag = data.handle;
			if (data.result > 0) {
				dumpPayload(data.result);
			} else {
				printf("Failed to read tag (try again.)");
			}
			maNFCDestroyTag(tag);
		}
	}

	void dumpPayload(int len) {
		toHex(payload, len, hexEncoded);
		printf("Read tag, payload: %s", hexEncoded);
	}

	void dumpPayloadAsText(int len) {
		printf("Read tag, payload as text: %s", payload);
	}

	void handleNDEF(MAHandle msg) {
		MAHandle ndef = maNFCGetNDEFMessage(msg);
		printf("Tag handle %i, msg handle %i", msg, ndef);
		int records = maNFCGetNDEFRecordCount(ndef);
		printf("Record count: %i", records);
		for (int i = 0; i < records; i++) {
			MAHandle record = maNFCGetNDEFRecord(ndef, i);
			int len = maNFCGetPayload(record, payload, BUF_SIZE);
			if (len >= 0) {
				dumpPayloadAsText(len);
				dumpPayloadAsText(len);
			}
		}
	}

	void toHex(byte* data, int len, char* output) {
		for (int i = 0; i < len; i++) {
			output[2 * i] = HEX_CHARS[(data[i] >> 4) & 0xf];
			output[2 * i + 1] = HEX_CHARS[data[i] & 0xf];
		}
		output[2 * len] = '\0';
	}

	void closeEvent() {
	}
};

/**
 * Entry point of the program. The MAMain function
 * needs to be declared as extern "C".
 */
extern "C" int MAMain() {
	Moblet::run(new NFCDemo());
	return 0;
}
