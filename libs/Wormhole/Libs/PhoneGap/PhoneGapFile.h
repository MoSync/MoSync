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

#ifndef WORMHOLE_LIBS_PHONEGAP_PHONEGAPFILE_H_
#define WORMHOLE_LIBS_PHONEGAP_PHONEGAPFILE_H_

#include <NativeUI/WebView.h>
#include <MAUtil/String.h>

namespace Wormhole
{
	class JSONMessage;
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

		void callFileTransferError(
			const MAUtil::String& callbackID,
			const MAUtil::String& errorCode,
			const MAUtil::String& sourceURI,
			const MAUtil::String& targetURI
			);

		void callFileUploadResult(
			const MAUtil::String& callbackID,
			const MAUtil::String& responseCode, // long
			const MAUtil::String& bytesSent, // long (unused)
			const MAUtil::String& response // DOMString (unused)
			);

		/**
		 * Implementation of the File API.
		 * @return true if message was handled, false if not.
		 */
		void handleFileMessage(JSONMessage& message);

		/**
		 * Implementation of the FileTransfer API.
		 * @return true if message was handled, false if not.
		 */
		void handleFileTransferMessage(JSONMessage& message);

		/**
		 * Return a FileSystem object for the application's
		 * local file system.
		 */
		void actionRequestFileSystem(JSONMessage& message);

		/**
		 * Return a FileSystem object for the given file URL.
		 */
		void actionResolveLocalFileSystemURI(JSONMessage& message);

		/**
		 * Return a FileEntry object.
		 */
		void actionGetFile(JSONMessage& message);

		/**
		 * Return a DirectoryEntry object.
		 */
		void actionGetDirectory(JSONMessage& message);

		/**
		 * Return a File object.
		 */
		void actionGetFileMetadata(JSONMessage& message);

		void actionGetMetadata(JSONMessage& message);

		void actionWrite(JSONMessage& message);

		void actionReadAsText(JSONMessage& message);

		void actionReadAsDataURL(JSONMessage& message);

		void actionTruncate(JSONMessage& message);

		void actionCopyTo(JSONMessage& message);

		void actionMoveTo(JSONMessage& message);

		void actionCopyMoveHelper(JSONMessage& message, bool move);

		void actionRemove(JSONMessage& message);

		void actionRemoveRecursively(JSONMessage& message);

		void actionReadEntries(JSONMessage& message);

		void actionTestFileExists(JSONMessage& message);

		void actionTestDirectoryExists(JSONMessage& message);

		/**
		 * TODO: How should we implement this? Which file system are
		 * we talking about? Unsupported for now.
		 */
		void actionGetFreeDiskSpace(JSONMessage& message);

		void actionUploadFile(JSONMessage& message);

	private:
		PhoneGapMessageHandler* mMessageHandler;
	};

} // namespace

#endif
