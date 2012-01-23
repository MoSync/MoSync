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

/**
 * @file PhoneGapFile.h
 * @author Mikael Kindborg
 *
 * Implementation of PhoneGap calls made from JavaScript.
 */

#ifndef PHONEGAP_FILE_H_
#define PHONEGAP_FILE_H_

#include <Wormhole/WebViewMessage.h>
#include <NativeUI/WebView.h>
#include <MAUtil/String.h>
#include "PhoneGapMessage.h"

class PhoneGapMessageHandler;

/**
 * Class that implements PhoneGap File API.
 */
class PhoneGapFile
{
public:
	/**
	 * Constructor.
	 */
	PhoneGapFile(PhoneGapMessageHandler* messageHandler);

	/**
	 * Destructor.
	 */
	virtual ~PhoneGapFile();

	MAUtil::String emitFileSystemInfo(
		const MAUtil::String& name,
		const MAUtil::String& rootEntry);

	MAUtil::String emitDirectoryEntry(
		const MAUtil::String& name,
		const MAUtil::String& fullPath);

	MAUtil::String emitFileEntry(
		const MAUtil::String& name,
		const MAUtil::String& fullPath);

	MAUtil::String emitFile(
		const MAUtil::String& name,
		const MAUtil::String& fullPath,
		const MAUtil::String& type,
		const MAUtil::String& lastModifiedDate,
		const MAUtil::String& size);

	MAUtil::String emitMetadata(
		const MAUtil::String& modificationTime);

	void callSuccess(
		const MAUtil::String& callbackID,
		const MAUtil::String& args,
		const MAUtil::String& castFunction = ""
		);

	void callFileError(
		const MAUtil::String& callbackID,
		const MAUtil::String& errorCode
		);

	/**
	 * Implementation of File API exposed to JavaScript.
	 */
	void handleMessage(PhoneGapMessage& message);

	/**
	 * Return a FileSystem object.
	 */
	void actionRequestFileSystem(PhoneGapMessage& message);

	/**
	 * Return a FileEntry object.
	 */
	void actionGetFile(PhoneGapMessage& message);

	/**
	 * Return a File object.
	 */
	void actionGetFileMetadata(PhoneGapMessage& message);

	void actionGetMetadata(PhoneGapMessage& message);

	void actionWrite(PhoneGapMessage& message);

	void actionReadAsText(PhoneGapMessage& message);

	void actionReadAsDataURL(PhoneGapMessage& message);

	void actionTruncate(PhoneGapMessage& message);

	void actionCopyToOrMoveTo(PhoneGapMessage& message, bool move);

	void actionRemove(PhoneGapMessage& message);

private:
	PhoneGapMessageHandler* mMessageHandler;
};

#endif
