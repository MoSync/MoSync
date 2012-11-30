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

/**
 * @file main.cpp
 * @author Mattias Bybro
 *
 * This is a sample NFC app.
 *
 * To enable NFC for an app, NFC permissions need to be set and
 *  an nfc.xml file defining the type of tags to trigger upon.
 *
 * This example makes use of MoSync HTML5/JavaScript functionality
 * for the user interface.
 */

// Include Moblet and message class for web applications.
#include <Wormhole/HybridMoblet.h>
#include <Wormhole/MessageStreamJSON.h>
// Utility functions for working with NDEF tags
#include "nfcutil.h"
// Other includes
#include <maapi.h>
#include <MAUtil/String.h>

// Namespaces we want to access.
using namespace MAUtil; // Class Moblet
using namespace NativeUI; // Class WebView
using namespace Wormhole; // Class HybridMoblet

// A buffer size used as a basis for text buffers and data buffers.
#define BUF_SIZE 4096
// The buffer size used for ids.
#define ID_SIZE 64

// Icon constants -- these correspond to the png images found in
// the LocalFiles/images directory.
#define INFO_ICON "info"
#define ERROR_ICON "error"
#define READ_TAG_ICON "read_tag"
#define WRITE_TAG_ICON "write_tag"
#define CONTACT_ICON "contact"
#define URL_ICON "at"
#define UNKNOWN_ICON "unknown"

// The vCard MIME type
#define VCARD_MIME_TYPE "text/x-vcard"

// The amount of time to show temporary messages
#define TOAST_PERIOD 3000

// The mode the app is in; either read or write
enum Mode { _read, _write };

/**
 * The application class.
 */
class NFCMoblet : public HybridMoblet, TimerListener
{
private:
	// This is the buffer used for writing messages in the UI.
	char fInfoBuffer[BUF_SIZE];
	// This buffer holds scripts/calls to the JavaScript engine
	char fJsFnCall[BUF_SIZE];
	// The last tag read
	MAHandle fCurrentTag;
	// A (sample) NDEF tag to write
	MAHandle fSampleTag;
	// The current state of the app; either 'read' or 'write'
	Mode fMode;
	// UI state
	bool fExitOnBack;
	bool fDisallowStatusMessages;

public:
	NFCMoblet() {
		initWebView();
		fDisallowStatusMessages = false;
	}

	void initWebView() {
		// Initialize the web view and show the main page.
		showPage("index.html");
		// Disable zooming of the page.
		getWebView()->disableZoom();
	}

	/**
	 * Here we handle messages sent from JavaScript. Our example
	 * uses three messages:
	 * - StartNFC, at startup
	 * - WriteSampleURL, for writing an example NDEF URI
	 * - WriteSampleVCard, for writing an example NDEF vCard
	 */
	void handleWebViewMessage(WebView* webView, MAHandle data) {
		MessageStreamJSON message(webView, data);
		while (message.next())
		{
			handleMessage(message);
		}
	}

	void handleMessage(MessageStreamJSON& message) {
		if (message.is("StartNFC")) {
			// First, let's start listening to NFC events...
			if (maNFCStart()) {
				// For the off-chance of a device not having NFC,
				// show an error message.
				showStatus(
					ERROR_ICON,
					"This device does not have NFC, or NFC is disabled.<br/>"
					"<b>Please quit.</b>",
					0);
				// And this flag is just a hack to never clear
				// the above message.
				fDisallowStatusMessages = true;
			}
			// ...and we're in read mode at this point.
			setMode(_read);
		} else if (message.is("WriteSampleURL")) {
			// If the user pressed the 'Write Web Address' button,
			// we enter write mode...
			setMode(_write);
			// ...and create a tag to write next time a tag is
			// held close to the device.
			fSampleTag = createSampleURLTag();
		} else if (message.is("WriteSampleVCard")) {
			// Same as above, but a different type of data.
			setMode(_write);
			fSampleTag = createSampleVCardTag();
		}
	}

	/**
	 * Handle key presses.
	 */
	void keyPressEvent(int keyCode, int nativeCode) {
		if (MAK_BACK == keyCode || MAK_0 == keyCode) {
			// If this flag is set, then quit.
			if (fExitOnBack) {
				// Call close to exit the application.
				destroyCurrentTag();
				destroySampleTag();
				maNFCStop();
				close();
			} else { // Otherwise, we just puts the app in some initial state.
				setMode(_read);
			}
		}
	}

	/**
	 * The NFC events come through this function.
	 */
	void customEvent(const MAEvent& event) {
		// We always just keep track of one tag, and
		// this tag gets replaced by whatever new tag comes
		// close to the device:
		if (EVENT_TYPE_NFC_TAG_RECEIVED == event.type) {
			MANFCEventData nfcEventData = event.nfc;
			// We got an event, now we need to get the actual tag.
			MAHandle tag = maNFCReadTag(nfcEventData.handle);
			setCurrentTag(tag);
			// And buzz a bit.
			maVibrate(100);
		}
		// Then, we just check whether we're in read or write
		// mode and the delegate to the proper
		// event handling code.
		switch (fMode) {
			case _write:
				customEventWrite(event);
				break;
			default:
				customEventRead(event);
		}
	}

	/**
	 * Here is where we take care of NFC events when we are in READ mode.
	 */
	void customEventRead(const MAEvent& event) {
		if (EVENT_TYPE_NFC_TAG_RECEIVED == event.type) {
			if (fCurrentTag) {
				if (maNFCIsType(fCurrentTag, MA_NFC_TAG_TYPE_NDEF)) {
					// Please note; we always need to convert to
					// a tag of the proper type. (Destruction of these
					// tags is automatic when the original tag is destroyed.)
					MAHandle ndef = maNFCGetTypedTag(
						fCurrentTag,
						MA_NFC_TAG_TYPE_NDEF);
					handleNDEF(ndef);
				} else if (maNFCIsType(fCurrentTag, MA_NFC_TAG_TYPE_MIFARE_CL)) {
					MAHandle mfc = maNFCGetTypedTag(
						fCurrentTag,
						MA_NFC_TAG_TYPE_MIFARE_CL);
					handleMifare("Mifare Classic", mfc);
				} else if (maNFCIsType(fCurrentTag, MA_NFC_TAG_TYPE_MIFARE_UL)) {
					MAHandle mfu = maNFCGetTypedTag(
						fCurrentTag,
						MA_NFC_TAG_TYPE_MIFARE_UL);
					int size = maNFCGetSize(mfu);
					handleMifare(
						size > 64 ?
							"Mifare Ultralight C" :
							"Mifare Ultralight",
						mfu);
				} else {
					handleUnknownTag();
				}
			}
		} else if (EVENT_TYPE_NFC_TAG_DATA_READ == event.type) {
			// This event type tells us we've just read some kind of tag data.
			MANFCEventData& data = (MANFCEventData&) event.nfc;
			int tag = data.handle;
			if (data.result < 0) {
				sprintf(
					fInfoBuffer,
					"Failed to read tag (try again.) Error code: %d",
					data.result);
				showStatus(ERROR_ICON, fInfoBuffer, TOAST_PERIOD);
			} else if (maNFCIsType(tag, MA_NFC_TAG_TYPE_NDEF)) {
				handleNDEF(tag);
			}
		}
	}

	/**
	 * Here is where we take care of NFC events when we are in WRITE mode.
	 */
	void customEventWrite(const MAEvent& event) {
		// We received a tag, and are prepared to write to it.
		if (EVENT_TYPE_NFC_TAG_RECEIVED == event.type) {
			MAHandle tagToWrite = 0;
			// Ok, we only write NDEF tags, and that means that
			// the tag is either a) An NDEF tag, or b) a tag
			// that can be formatted into an NDEF tag.
			if (maNFCIsType(fCurrentTag, MA_NFC_TAG_TYPE_NDEF)) {
				tagToWrite = maNFCGetTypedTag(
					fCurrentTag,
					MA_NFC_TAG_TYPE_NDEF);
			} else if (maNFCIsType(fCurrentTag, MA_NFC_TAG_TYPE_NDEF_FORMATTABLE)) {
				tagToWrite = maNFCGetTypedTag(
					fCurrentTag,
					MA_NFC_TAG_TYPE_NDEF_FORMATTABLE);
			}

			if (tagToWrite) {
				// Almost there now...
				if (maNFCIsReadOnly(tagToWrite) > 0) {
					setMode(_read);
					showStatus(
						ERROR_ICON,
						"Tag is read-only, cannot write.",
						TOAST_PERIOD);
				} else {
					// ...and since the tag is writeable,
					// let's write a sample tag to it!
					writeSampleTag(tagToWrite);
				}
			} else {
				setMode(_read);
				showStatus(
					ERROR_ICON,
					"Cannot write to this tag type.",
					TOAST_PERIOD);
			}
		} else if (EVENT_TYPE_NFC_TAG_DATA_WRITTEN == event.type) {
			// We get this event whenever we wrote something
			MANFCEventData nfcEventData = event.nfc;
			setMode(_read);
			if (nfcEventData.result >= 0) {
				// The dstId of NDEF write operations is the NDEF message handle
				int size = maNFCGetSize(nfcEventData.dstId);
				sprintf(fInfoBuffer, "Wrote tag. (%d bytes)", size);
				showStatus(INFO_ICON, fInfoBuffer, TOAST_PERIOD);
			} else {
				showStatus(
					ERROR_ICON,
					"<b>Unable to write tag</b><br/>"
					"<i>Note: The sample vCard is too large "
					"for a Mifare Ultralight tag.</i>",
					TOAST_PERIOD);
			}
		}
	}

	void setCurrentTag(MAHandle currentTag) {
		// Make sure to clean up first; otherwise we'd get a leak.
		destroyCurrentTag();
		fCurrentTag = currentTag;
	}

	void destroyCurrentTag() {
		if (fCurrentTag) {
			maNFCDestroyTag(fCurrentTag);
		}
	}

	/**
	 * The actual tag writing takes place here.
	 */
	void writeSampleTag(MAHandle ndef) {
		// All these calls are ASYNCHRONOUS and will trigger an
		// event that we'll receive in the customEvent method.
		if (fSampleTag) {
			// First we need to connect to the tag...
			maNFCConnectTag(ndef);
			// ...then we may write to it...
			maNFCWriteNDEFMessage(ndef, fSampleTag);
			// ...and then of course we need to close the connection.
			maNFCCloseTag(ndef);
		}
	}

	/**
	 * Set read/write mode and issue an appropriate message to the user.
	 */
	void setMode(Mode mode) {
		fMode = mode;
		switch (mode) {
			case _write:
				showStatus(
					WRITE_TAG_ICON,
					"<b>Ready to write</b><br/>Please hold a tag close to "
					"the device to write to it.<br/>Press BACK to cancel.",
					0);
				maVibrate(100);
				break;
			default:
				showStatus(
					READ_TAG_ICON,
					"<b>Waiting for a tag</b><br/>Hold a tag close to "
					"the device to show its contents.",
					0);
				fExitOnBack = true;
		}
	}

	/**
	 * Show a status message in the UI for a specified amount of time
	 * \param icon The icon to use (see constants above).
	 * \param msg The message to display
	 * \param period The amount of time (in ms) to wait before removing
	 * the message
	 */
	void showStatus(const char* icon, const char* msg, int period) {
		// Small hack: we always clear the tag id.
		showTagId("");
		if (fDisallowStatusMessages) {
			return;
		}
		fExitOnBack = false;
		sprintf(fJsFnCall, "updateInfo(\"%s\",\"%s\")", icon, msg);
		callJS(fJsFnCall);
		if (period > 0) {
			addTimer(this, period, 1);
		} else {
			removeTimer(this);
		}
	}

	void showTagId(const char* tagId) {
		sprintf(fJsFnCall, "updateTagId(\"%s\")", tagId);
		callJS(fJsFnCall);
	}

	void destroySampleTag() {
		// Note that NDEF messages created
		// by the maNFCCreateNDEFMessage need
		// to be explicitly destroyed.
		if (fSampleTag) {
			maNFCDestroyTag(fSampleTag);
		}
	}

	/**
	 * Creates a sample URL NDEF message
	 */
	MAHandle createSampleURLTag() {
		destroySampleTag();
		MAHandle msg = maNFCCreateNDEFMessage(1);
		MAHandle rec = maNFCGetNDEFRecord(msg, 0);
		UriNdefRecord uriNdef(rec);
		MAUtil::String uri = MAUtil::String("mosync.com");
		uriNdef.setUri(0x1, uri);
		return msg;
	}

	/**
	 * Creates a sample URL vCard message
	 */
	MAHandle createSampleVCardTag() {
		destroySampleTag();
		MAHandle msg = maNFCCreateNDEFMessage(1);
		MAHandle rec = maNFCGetNDEFRecord(msg, 0);
		MimeMediaNdefRecord vCardRec = MimeMediaNdefRecord(rec);
		vCardRec.setMimeType(String(VCARD_MIME_TYPE));
		const char* vCard =
			"BEGIN:VCARD\n"
			"VERSION:3.0"
			"N:X;Mr\n"
			"FN: Mr X\n"
			"END:VCARD";
		maNFCSetNDEFPayload(rec, vCard, strlen(vCard));
		return msg;
	}

	void handleNDEF(MAHandle tag) {
		MAHandle ndef = maNFCGetNDEFMessage(tag);
		// Ok, if the zero handle was returned, then we
		// do not already have the NDEF data (sometimes we do!),
		// and we now need to asynchronously read that data.
		// The next time we get here will be after receiving a
		// read event, and we are now ready to use the NDEF tag.
		if (ndef == 0) {
			maNFCConnectTag(tag);
			maNFCReadNDEFMessage(tag);
			maNFCCloseTag(tag);
		} else {
			int records = maNFCGetNDEFRecordCount(ndef);
			String id = getId(tag);
			if (records > 0) {
				MAHandle record = maNFCGetNDEFRecord(ndef, 0);
				// Try to idenfify what kind of NDEF it is...
				const char* icon = getIcon(record);
				String text = getText(record);
				// ...and output it to the UI with the serial id. Done!
				String displayThis = text + "<br/><i>" + id + "</i>";
				showStatus(icon, text.c_str(), 0);
				showTagId(id.c_str());
			}
		}
	}

	String toBase16(const char* data, int len) {
		char HEX_CHARS[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f' };
		char* result = (char*) malloc(2 * len + 1);
		for (int i = 0; i < len; i++) {
			result[2 * i] = HEX_CHARS[(data[i] >> 4) & 0xf];
			result[2 * i + 1] = HEX_CHARS[data[i] & 0xf];
		}
		result[2 * len] = '\0';
		String resultStr = String(result);
		free(result);
		return resultStr;
	}

	void handleNDEFFormattable(MAHandle msg) {
		showStatus(INFO_ICON, "Empty tag", TOAST_PERIOD);
	}

	void handleUnknownTag() {
		showStatus(UNKNOWN_ICON, "Unknown tag format", TOAST_PERIOD);
	}

	/**
	 * Returns an appropriate icon depending on the type
	 * of NDEF content.
	 */
	const char* getIcon(MAHandle record) {
		const char* icon = INFO_ICON;
		if (UriNdefRecord::isValid(record)) {
			icon = URL_ICON;
		} else if (MimeMediaNdefRecord::isMimeType(record, VCARD_MIME_TYPE)) {
			icon = CONTACT_ICON;
		}
		return icon;
	}

	/**
	 * Returns an appropriate text depending on the type
	 * of NDEF content.
	 */
	String getText(MAHandle record) {
		if (UriNdefRecord::isValid(record)) {
			UriNdefRecord uriNdef(record);
			return uriNdef.getUri();
		} else if (MimeMediaNdefRecord::isMimeType(record, VCARD_MIME_TYPE)) {
			return String("This tag contains a contact (vCard)");
		}
		return String("?");
	}


	/**
	 * Returns an appropriate text representing the tag's serial id.
	 */
	String getId(MAHandle tag) {
		char id[ID_SIZE];
		int idSize = maNFCGetId(tag, &id, ID_SIZE);
		if (idSize > 0) {
			return toBase16(id, idSize);
		} else {
			return "No or unknown id";
		}
	}

	void handleMifare(const char* type, MAHandle mfu) {
		int size = maNFCGetSize(mfu);
		sprintf(fInfoBuffer, "<b>%s</b><br/>Size: %d bytes", type, size);
		showStatus(UNKNOWN_ICON, fInfoBuffer, 0);
	}

	void runTimerEvent() {
		// Ok, timed out the current message -
		// now just show the default message.
		setMode(fMode);
	}
};

/**
 * Main function that is called when the program starts.
 * Here an instance of the NFCMoblet class is created and
 * the program enters the main event loop.
 */
extern "C" int MAMain()
{
	(new NFCMoblet())->enterEventLoop();
	return 0;
}
