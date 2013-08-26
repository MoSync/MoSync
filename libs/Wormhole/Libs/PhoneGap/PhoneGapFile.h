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

		virtual MAUtil::String emitFileSystemInfo(
			const MAUtil::String& name,
			const MAUtil::String& rootEntry);

		virtual MAUtil::String emitDirectoryEntry(
			const MAUtil::String& name,
			const MAUtil::String& fullPath);

		virtual MAUtil::String emitFileEntry(
			const MAUtil::String& name,
			const MAUtil::String& fullPath);

		virtual MAUtil::String emitFile(
			const MAUtil::String& name,
			const MAUtil::String& fullPath,
			const MAUtil::String& type,
			const MAUtil::String& lastModifiedDate,
			const MAUtil::String& size);

		virtual MAUtil::String emitMetadata(
			const MAUtil::String& modificationTime);

		virtual void callSuccess(
			const MAUtil::String& callbackID,
			const MAUtil::String& args,
			const MAUtil::String& castFunction = ""
			);

		virtual void callFileError(
			const MAUtil::String& callbackID,
			const MAUtil::String& errorCode
			);

		virtual void callFileTransferError(
			const MAUtil::String& callbackID,
			const MAUtil::String& errorCode,
			const MAUtil::String& sourceURI,
			const MAUtil::String& targetURI
			);

		virtual void callFileUploadResult(
			const MAUtil::String& callbackID,
			const MAUtil::String& responseCode, // long
			const MAUtil::String& bytesSent, // long (unused)
			const MAUtil::String& response // DOMString (unused)
			);

		/**
		 * Implementation of the File API.
		 * @return true if message was handled, false if not.
		 */
		virtual void handleFileMessage(JSONMessage& message);

		/**
		 * Implementation of the FileTransfer API.
		 * @return true if message was handled, false if not.
		 */
		virtual void handleFileTransferMessage(JSONMessage& message);

		/**
		 * Return a FileSystem object for the application's
		 * local file system.
		 */
		virtual void actionRequestFileSystem(JSONMessage& message);

		/**
		 * Return a FileSystem object for the given file URL.
		 */
		virtual void actionResolveLocalFileSystemURI(JSONMessage& message);

		/**
		 * Return a FileEntry object.
		 */
		virtual void actionGetFile(JSONMessage& message);

		/**
		 * Return a DirectoryEntry object.
		 */
		virtual void actionGetDirectory(JSONMessage& message);

		/**
		 * Return a File object.
		 */
		virtual void actionGetFileMetadata(JSONMessage& message);

		virtual void actionGetMetadata(JSONMessage& message);

		virtual void actionWrite(JSONMessage& message);

		virtual void actionReadAsText(JSONMessage& message);

		virtual void actionReadAsDataURL(JSONMessage& message);

		virtual void actionTruncate(JSONMessage& message);

		virtual void actionCopyTo(JSONMessage& message);

		virtual void actionMoveTo(JSONMessage& message);

		virtual void actionCopyMoveHelper(JSONMessage& message, bool move);

		virtual void actionRemove(JSONMessage& message);

		virtual void actionRemoveRecursively(JSONMessage& message);

		virtual void actionReadEntries(JSONMessage& message);

		virtual void actionTestFileExists(JSONMessage& message);

		virtual void actionTestDirectoryExists(JSONMessage& message);

		/**
		 * TODO: How should we implement this? Which file system are
		 * we talking about? Unsupported for now.
		 */
		virtual void actionGetFreeDiskSpace(JSONMessage& message);

		virtual void actionUploadFile(JSONMessage& message);

		/**
		 * Get the options parameters "create" and "exclusive"
		 * from the JSON tree.
		 * @return true on success, false on error.
		 */
		virtual bool getJSONParamsOptionsCreateExclusive(
			JSONMessage& message,
			bool& create,
			bool& exclusive);

		/**
		 * Get the parent fullPath of a directory entry from the JSON tree.
		 * @return true on success, false on error.
		 */
		virtual bool getJSONParamParentFullPath(
			JSONMessage& message,
			MAUtil::String& destinationPath);

	protected:
		/**
		 * Pointer to message handler.
		 */
		PhoneGapMessageHandler* mMessageHandler;
	};

} // namespace

#endif
