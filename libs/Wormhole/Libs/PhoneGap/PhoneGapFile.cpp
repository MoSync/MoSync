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
 * @file PhoneGapFile.cpp
 * @author Mikael Kindborg
 *
 * Implementation of PhoneGap calls made from JavaScript.
 */

#include <mastring.h>
#include <matime.h>
#include <conprint.h>
#include "../../Encoder.h"
#include "../../HighLevelHttpConnection.h"
#include "PhoneGapFile.h"
#include "PhoneGapFileUtils.h"
#include "PhoneGapMessageHandler.h"


// File error codes.
#define FILEERROR_NOT_FOUND_ERR "1"
#define FILEERROR_SECURITY_ERR "2"
#define FILEERROR_ABORT_ERR "3"
#define FILEERROR_NOT_READABLE_ERR "4"
#define FILEERROR_ENCODING_ERR "5"
#define FILEERROR_NO_MODIFICATION_ALLOWED_ERR "6"
#define FILEERROR_INVALID_STATE_ERR "7"
#define FILEERROR_SYNTAX_ERR "8"
#define FILEERROR_INVALID_MODIFICATION_ERR "9"
#define FILEERROR_QUOTA_EXCEEDED_ERR "10"
#define FILEERROR_TYPE_MISMATCH_ERR "11"
#define FILEERROR_PATH_EXISTS_ERR "12"

// FileTransfer error codes.
#define FILETRANSERERROR_FILE_NOT_FOUND_ERR "1"
#define FILETRANSERERROR_INVALID_URL_ERR "2"
#define FILETRANSERERROR_CONNECTION_ERR "3"

// File system options.
#define LOCALFILESYSTEM_TEMPORARY 0
#define LOCALFILESYSTEM_PERSISTENT 1
#define LOCALFILESYSTEM_RESOURCE 2
#define LOCALFILESYSTEM_APPLICATION 3

using namespace MAUtil;

namespace Wormhole
{
	/**
	 * Constructor.
	 */
	PhoneGapFile::PhoneGapFile(PhoneGapMessageHandler* messageHandler)
		: mMessageHandler(messageHandler)
	{
	}

	/**
	 * Destructor.
	 */
	PhoneGapFile::~PhoneGapFile()
	{
	}

	String PhoneGapFile::emitFileSystemInfo(
		const String& name,
		const String& rootEntry)
	{
		return "{\"name\":\"" + name + "\", \"root\":" + rootEntry + "}";
	}

	String PhoneGapFile::emitDirectoryEntry(
		const String& name,
		const String& fullPath)
	{
		return
			"{\"isFile\":false,\"isDirectory\":true,\"name\":\"" + name + "\"," +
			"\"fullPath\":\"" + fullPath + "\",\"filesystem\": null}";
	}

	String PhoneGapFile::emitFileEntry(
		const String& name,
		const String& fullPath)
	{
		return
			"{\"isFile\":true,\"isDirectory\":false,\"name\":\"" + name + "\"," +
			"\"fullPath\":\"" + fullPath + "\",\"filesystem\": null}";
	}

	String PhoneGapFile::emitFile(
		const String& name,
		const String& fullPath,
		const String& type,
		const String& lastModifiedDate,
		const String& size)
	{
		return
			"{\"name\":\"" + name + "\"," +
			"\"fullPath\":\"" + fullPath + "\"," +
			"\"type\":\"" + type + "\"," +
			"\"lastModifiedDate\":\"" + lastModifiedDate + "\"," +
			"\"size\":" + size + "}";
	}

	String PhoneGapFile::emitMetadata(
		const String& modificationTime)
	{
		return
			"{\"modificationTime\":\"" + modificationTime + "\"}";
	}

	void PhoneGapFile::callSuccess(
		const String& callbackID,
		const String& args,
		const String& castFunction
		)
	{
		mMessageHandler->callSuccess(
			callbackID,
			PHONEGAP_CALLBACK_STATUS_OK,
			args,
			false,
			castFunction);
	}

	void PhoneGapFile::callFileError(
		const String& callbackID,
		const String& errorCode
		)
	{
		mMessageHandler->callError(
			callbackID,
			errorCode);
	}

	void PhoneGapFile::callFileTransferError(
		const String& callbackID,
		const String& errorCode,
		const String& sourceURI,
		const String& targetURI
		)
	{
		// Construct FileTransferError object.
		String args =
			"{\"code\":" + errorCode +
			",\"source\":\"" + sourceURI + "\"" +
			",\"target\":\"" + targetURI + "\"}";

		mMessageHandler->callCallback(
			"cordova.callbackFromNative",
			callbackID,
			false,
			PHONEGAP_CALLBACK_STATUS_ERROR,
			args,
			false); // Don't keep callback.
	}

	void PhoneGapFile::callFileUploadResult(
		const String& callbackID,
		const String& responseCode, // long
		const String& bytesSent, // long
		const String& response // DOMString
		)
	{
		// Construct FileUploadResult object.
		String args =
			"{\"bytesSent\":" + bytesSent + "," +
			 "\"responseCode\":" + responseCode + "," +
			 "\"response\":" + Encoder::JSONStringify(response.c_str()) + "}";

		callSuccess(callbackID, args);
	}

	/**
	 * Implementation of the File API.
	 * @return true if message was handled, false if not.
	 */
	void PhoneGapFile::handleFileMessage(JSONMessage& message)
	{
		String action = message.getParam("action");

		if (action == "requestFileSystem")
		{
			actionRequestFileSystem(message);
		}
		else if (action == "resolveLocalFileSystemURI")
		{
			actionResolveLocalFileSystemURI(message);
		}
		else if (action == "getFile")
		{
			actionGetFile(message);
		}
		else if (action == "getDirectory")
		{
			actionGetDirectory(message);
		}
		else if (action == "getFileMetadata")
		{
			actionGetFileMetadata(message);
		}
		else if (action == "getMetadata")
		{
			actionGetMetadata(message);
		}
		else if (action == "write")
		{
			actionWrite(message);
		}
		else if (action == "readAsText")
		{
			actionReadAsText(message);
		}
		else if (action == "readAsDataURL")
		{
			actionReadAsDataURL(message);
		}
		else if (action == "truncate")
		{
			actionTruncate(message);
		}
		else if (action == "copyTo")
		{
			actionCopyTo(message);
		}
		else if (action == "moveTo")
		{
			actionMoveTo(message);
		}
		else if (action == "remove")
		{
			actionRemove(message);
		}
		else if (action == "removeRecursively")
		{
			actionRemoveRecursively(message);
		}
		else if (action == "readEntries")
		{
			actionReadEntries(message);
		}
		else if (action == "testFileExists")
		{
			actionTestFileExists(message); // Deprecated since MoSync 3.3
		}
		else if (action == "testDirectoryExists")
		{
			actionTestDirectoryExists(message); // Deprecated since MoSync 3.3
		}
		else if (action == "getFreeDiskSpace")
		{
			actionGetFreeDiskSpace(message); // Deprecated since MoSync 3.3
		}
	}

	/**
	 * Implementation of the FileTransfer API.
	 * @return true if message was handled, false if not.
	 */
	void PhoneGapFile::handleFileTransferMessage(JSONMessage& message)
	{
		String action = message.getParam("action");

		if (action == "upload")
		{
			actionUploadFile(message);
		}
	}

	/**
	 * Return a FileSystem object for the application's
	 * local file system.
	 */
	void PhoneGapFile::actionRequestFileSystem(JSONMessage& message)
	{
		String callbackID = message.getParam("PhoneGapCallBackId");

		// We support only persistent storage.
		int type = message.getArgsFieldInt(0);
		if (LOCALFILESYSTEM_PERSISTENT != type)
		{
			callFileError(callbackID, FILEERROR_NO_MODIFICATION_ALLOWED_ERR);
			return;
		}

		// Size parameter must be zero.
		int size = message.getArgsFieldInt(1);
		if (0 != size)
		{
			callFileError(callbackID, FILEERROR_NO_MODIFICATION_ALLOWED_ERR);
			return;
		}

		// Get local root path and remove trailing slash, if any.
		FileUtil *fu = mMessageHandler->getFileUtil();
		// This is a fix for a bug occurring when using the (deprecated)
		// WebAppMoblet class.
		if (fu == NULL) {
			fu = new FileUtil();
			mMessageHandler->setFileUtil(fu);
		}
		String path = fu->getAppPath();

		// If we get just a slash, we won't remove the last slash.
		// TODO: It is unclear if PhoneGap requires all path names
		// to not end with a slash, or if it does not matter. Typical
		// directory entry has name "sdcard" and full path "/sdcard".
		if (path.size() == 1 && '/' == path[0])
		{
			// Just a slash, do nothing.
		}
		else if (path.size() > 1 && '/' == path[path.size() - 1])
		{
			// Remove last slash.
			path = path.substr(0, path.size() - 1);
		}
		String rootEntry = emitDirectoryEntry(
			FileGetName(path),
			path);

		String fileSystemInfo = emitFileSystemInfo("persistent", rootEntry);
		callSuccess(
			callbackID,
			fileSystemInfo,
			"window.localFileSystem._castFS");
	}

	/**
	 * Return a FileSystem object for the given file URL.
	 */
	void PhoneGapFile::actionResolveLocalFileSystemURI(JSONMessage& message)
	{
		String callbackID = message.getParam("PhoneGapCallBackId");

		String uri = message.getArgsField("uri");

		// Get path.
		const char* pURL = uri.c_str();
		const char* pPath = strstr(pURL, "file://");

		// URL must start with "file://"
		if ((NULL == pPath) || (pURL != pPath))
		{
			callFileError(callbackID, FILEERROR_ENCODING_ERR);
			return;
		}

		// Move to after "file://"
		pPath += 7;

		// Check that this is an existing file.
		if (!FileExists(pPath))
		{
			callFileError(callbackID, FILEERROR_NOT_FOUND_ERR);
			return;
		}

		String entry = emitDirectoryEntry(
			FileGetName(pPath),
			pPath);
		callSuccess(
			callbackID,
			entry,
			"window.localFileSystem._castEntry");
	}

	/**
	 * Return a FileEntry object.
	 */
	void PhoneGapFile::actionGetFile(JSONMessage& message)
	{
		String callbackID = message.getParam("PhoneGapCallBackId");

		String fullPath = message.getArgsField(0);
		String path = message.getArgsField(1);
		String fullFilePath = fullPath + "/" + path;

		// Get flags "create" and "exclusive".
		bool create = false;
		bool exclusive = false;
		bool success = getJSONParamsOptionsCreateExclusive(message, create, exclusive);
		if (!success)
		{
			callFileError(callbackID, FILEERROR_NO_MODIFICATION_ALLOWED_ERR);
			return;
		}

		// Create file if requested.
		if (create)
		{
			if (exclusive)
			{
				// The file must not exist if "exclusive" is true.
				if (FileExists(fullFilePath))
				{
					callFileError(callbackID, FILEERROR_PATH_EXISTS_ERR);
					return;
				}
			}

			if (!FileExists(fullFilePath))
			{
				// Create the file.
				bool created = FileCreatePath(fullFilePath);
				if (!created)
				{
					callFileError(callbackID, FILEERROR_NO_MODIFICATION_ALLOWED_ERR);
					return;
				}
			}
		}

		// Send back FileEntry data.
		String fileEntry = emitFileEntry(
			path,
			fullFilePath);
		callSuccess(
			callbackID,
			fileEntry,
			"window.localFileSystem._castEntry");
	}


	//mosync://PhoneGap?service=File&action=getDirectory&args={"fullPath":"/mnt/sdcard","path":"
	//fob1"}&PhoneGapCallBackId=File21"

	/**
	 * Return a DirectoryEntry object.
	 */
	void PhoneGapFile::actionGetDirectory(JSONMessage& message)
	{
		String callbackID = message.getParam("PhoneGapCallBackId");

		String fullPath = message.getArgsField(0);
		String path = message.getArgsField(1);
		String fullFilePath = fullPath + "/" + path;

		// Add a trailing slash if not present. MoSync API requires this.
		if (fullFilePath[fullFilePath.size() - 1] != '/')
		{
			fullFilePath += "/";
		}

		// Get flags "create" and "exclusive".
		bool create = false;
		bool exclusive = false;
		bool success = getJSONParamsOptionsCreateExclusive(message, create, exclusive);
		if (!success)
		{
			callFileError(callbackID, FILEERROR_NO_MODIFICATION_ALLOWED_ERR);
			return;
		}

		// Create directory if requested.
		if (create)
		{
			if (exclusive)
			{
				// The file must not exist if "exclusive" is true.
				if (FileExists(fullFilePath))
				{
					callFileError(callbackID, FILEERROR_PATH_EXISTS_ERR);
					return;
				}
			}

			if (!FileExists(fullFilePath))
			{
				// Create the directory.
				// TODO: Invoke error if parent directory does not
				// exist to be compatible with the PhoneGap spec.
				bool created = FileCreatePath(fullFilePath);
				if (!created)
				{
					callFileError(callbackID, FILEERROR_NO_MODIFICATION_ALLOWED_ERR);
					return;
				}
			}
		}

		// Send back DirectoryEntry data.
		String directoryEntry = emitDirectoryEntry(
			path,
			// Remove the trailing slash from the full path.
			fullFilePath.substr(0, fullFilePath.size() - 1));
		callSuccess(
			callbackID,
			directoryEntry,
			"window.localFileSystem._castEntry");
	}

	/**
	 * Return a File object.
	 */
	void PhoneGapFile::actionGetFileMetadata(JSONMessage& message)
	{
		String callbackID = message.getParam("PhoneGapCallBackId");

		String fullPath = message.getArgsField(0);

		// TODO: What should we do if FileGetSize fails?
		// Return an error?
		// Set size to zero for now.
		int size = FileGetSize(fullPath);
		if (size < 0)
		{
			size = 0;
		}
		char sizeBuf[64];
		sprintf(sizeBuf, "%i", size);

		String file = emitFile(
			FileGetName(fullPath),
			fullPath,
			FileGetMimeType(fullPath),
			FileGetDate(fullPath),
			sizeBuf);

		// Note that _castDate also casts a File object.
		callSuccess(
			callbackID,
			file,
			"window.localFileSystem._castDate");
	}

	/**
	 * Return a Metadata object.
	 */
	void PhoneGapFile::actionGetMetadata(JSONMessage& message)
	{
		String callbackID = message.getParam("PhoneGapCallBackId");

		String fullPath = message.getArgsField("fullPath");

		String metadata = emitMetadata(
			FileGetDate(fullPath));

		// Note that _castDate is used here.
		callSuccess(
			callbackID,
			metadata,
			"window.localFileSystem._castDate");
	}

	void PhoneGapFile::actionWrite(JSONMessage& message)
	{
		String callbackID = message.getParam("PhoneGapCallBackId");

		String fullPath = message.getArgsField(0);
		String data = message.getArgsField(1);
		int position = message.getArgsFieldInt(2);

		int result = FileWrite(fullPath, data, position);
		if (result < 0)
		{
			callFileError(callbackID, FILEERROR_NO_MODIFICATION_ALLOWED_ERR);
			return;
		}

		// Send back the new file size.
		char sizeBuf[32];
		sprintf(sizeBuf, "%i", FileGetSize(fullPath));
		callSuccess(
			callbackID,
			sizeBuf);
	}

	//mosync://PhoneGap?service=File&action=readAsText&args=
	//{"fileName":"/mnt/sdcard/helloworld.txt","encoding":"UTF-8"}&PhoneGapCallBackId=File8
	void PhoneGapFile::actionReadAsText(JSONMessage& message)
	{
		String callbackID = message.getParam("PhoneGapCallBackId");

		String fullPath = message.getArgsField(0);

		// TODO: Encoding param is not used. This is the requested
		// encoding of the data send back to PhoneGap.
		//String encoding = message.getArgsField("encoding");

		String content;
		int result = FileRead(fullPath, content);
		if (result < 0)
		{
			callFileError(callbackID, FILEERROR_NO_MODIFICATION_ALLOWED_ERR);
			return;
		}

		// Send back the file content.
		callSuccess(
			callbackID,
			Encoder::JSONStringify(content.c_str()));
	}

	void PhoneGapFile::actionReadAsDataURL(JSONMessage& message)
	{
		String callbackID = message.getParam("PhoneGapCallBackId");

		String fullPath = message.getArgsField(0);

		String content;
		int result = FileRead(fullPath, content);
		if (result < 0)
		{
			callFileError(callbackID, FILEERROR_NO_MODIFICATION_ALLOWED_ERR);
			return;
		}

		String base64URL = "\"data:";
		base64URL += FileGetMimeType(fullPath);
		base64URL += ";base64,";
		base64URL += Encoder::base64Encode(content.c_str(), content.size());
		base64URL += "\"";

		// Send back the file content.
		callSuccess(callbackID, base64URL);
	}

	void PhoneGapFile::actionTruncate(JSONMessage& message)
	{
		String callbackID = message.getParam("PhoneGapCallBackId");

		String fullPath = message.getArgsField(0);

		int size = message.getArgsFieldInt(1);

		int result = FileTruncate(fullPath, size);
		if (result < 0)
		{
			callFileError(callbackID, FILEERROR_NOT_FOUND_ERR);
			return;
		}

		// Send back the result, the new length of the file.
		char lengthBuf[32];
		sprintf(lengthBuf, "%i", result);
		callSuccess(callbackID, lengthBuf);
	}

	void PhoneGapFile::actionCopyTo(JSONMessage& message)
	{
		actionCopyMoveHelper(message, false);
	}

	void PhoneGapFile::actionMoveTo(JSONMessage& message)
	{
		actionCopyMoveHelper(message, true);
	}

	void PhoneGapFile::actionCopyMoveHelper(JSONMessage& message, bool move)
	{
		String callbackID = message.getParam("PhoneGapCallBackId");

		String sourcePath = message.getArgsField(0);

		String destinationName = message.getArgsField(2);

		// Get destination path from JSON data (from DirectoryEntry object).
		String destinationPath;
		bool success = getJSONParamParentFullPath(message, destinationPath);
		if (!success)
		{
			callFileError(callbackID, FILEERROR_NO_MODIFICATION_ALLOWED_ERR);
			return;
		}

		// Check that we have the required path names.
		if ((sourcePath.size() == 0) ||
			(destinationName.size() == 0) ||
			(destinationPath.size() == 0))
		{
			callFileError(callbackID, FILEERROR_NO_MODIFICATION_ALLOWED_ERR);
			return;
		}

		bool isDirectory;

		// Check that sourcePath exists.
		if (FileIsDirectory(sourcePath))
		{
			isDirectory = true;
		}
		else if (FileExistsHelper(sourcePath))
		{
			isDirectory = false;
		}
		else
		{
			callFileError(callbackID, FILEERROR_NO_MODIFICATION_ALLOWED_ERR);
			return;
		}

		// Compose the full destination path.
		String fullDestinationPath = destinationPath + "/" + destinationName;

		int result;

		if (move)
		{
			result = FileMove(sourcePath, fullDestinationPath);
		}
		else
		{
			result = FileCopy(sourcePath, fullDestinationPath);
		}

		if (result < 0)
		{
			callFileError(callbackID, FILEERROR_NOT_FOUND_ERR);
			return;
		}

		// Send back entry data.
		String entry;
		if (isDirectory)
		{
			entry = emitDirectoryEntry(
				destinationName,
				fullDestinationPath);
		}
		else
		{
			entry = emitFileEntry(
				destinationName,
				fullDestinationPath);
		}
		callSuccess(
			callbackID,
			entry,
			"window.localFileSystem._castEntry");
	}

	void PhoneGapFile::actionRemove(JSONMessage& message)
	{
		String callbackID = message.getParam("PhoneGapCallBackId");

		String path = message.getArgsField(0);

		int result = FileDeleteFile(path);
		if (result < 0)
		{
			callFileError(callbackID, FILEERROR_NOT_FOUND_ERR);
			return;
		}

		callSuccess(callbackID, "\"ok\"");
	}

	/**
	 * This is only valid for directories.
	 */
	void PhoneGapFile::actionRemoveRecursively(JSONMessage& message)
	{
		String callbackID = message.getParam("PhoneGapCallBackId");

		String path = message.getArgsField(0);

		int result = FileDeleteDirectory(path);
		if (result < 0)
		{
			callFileError(callbackID, FILEERROR_NOT_FOUND_ERR);
			return;
		}

		callSuccess(callbackID, "\"ok\"");
	}

	// mosync://PhoneGap?service=File&action=readEntries&args={"fullPath":"/mnt/sdcard/fob1"}&
	// PhoneGapCallBackId=File21
	void PhoneGapFile::actionReadEntries(JSONMessage& message)
	{
		String callbackID = message.getParam("PhoneGapCallBackId");

		String path = message.getArgsField(0);

		// Open entry array.
		String entries = "[";

		char nameBuf[2048];

		// Make sure path end with a slash.
		FileMakeDirectoryPath(path);

		// Open directory listing.
		MAHandle list = maFileListStart(
			path.c_str(),
			"*",
			MA_FL_SORT_NAME | MA_FL_ORDER_ASCENDING);
		if (list < 0)
		{
			callFileError(callbackID, FILEERROR_NOT_FOUND_ERR);
			return;
		}

		// List all files in this directory.
		while (true)
		{
			// Move to next file.
			int result = maFileListNext(list, nameBuf, 2048);
			if (0 == result)
			{
				// No more files.
				break;
			}
			if (0 > result)
			{
				maFileListClose(list);
				callFileError(callbackID, FILEERROR_NOT_FOUND_ERR);
				return;
			}

			// Add separating comma if needed.
			if (entries.size() > 1)
			{
				entries += ",";
			}

			// Full path to entry.
			String fullPath = path + nameBuf;

			// Is this a directory?
			if ('/' == nameBuf[result - 1])
			{
				// We remove the trailing slash of the directory.
				String pathWithNoSlash = fullPath.substr(0, fullPath.size() - 1);
				String entry = emitDirectoryEntry(
					FileGetName(pathWithNoSlash),
					pathWithNoSlash);
				entries += entry;
			}
			else
			{
				String entry = emitFileEntry(
					FileGetName(fullPath),
					fullPath);
				entries += entry;
			}
		}

		// Close the directory listing.
		maFileListClose(list);

		// Close entry array.
		entries += "]";

		// Return result to PhoneGap.
		callSuccess(
			callbackID,
			entries,
			"window.localFileSystem._castEntries");
	}

	/**
	 * Handler of PhoneGap FileMgr.testFileExists
	 * @deprecated Use method actionGetFile.
	 * @param message JSON data with name of file to test for existence.
	*/
	void PhoneGapFile::actionTestFileExists(JSONMessage& message)
	{
		String callbackID = message.getParam("PhoneGapCallBackId");

		String path = message.getArgsField("fileName");

		String exists = "false";
		if (FileExists(path))
		{
			exists = "true";
		}

		// Return result to PhoneGap.
		callSuccess(
			callbackID,
			exists);
	}

	/**
	 * Handler of PhoneGap FileMgr.testDirectoryExists
	 * @deprecated Use method actionGetDirectory.
	 * @param message JSON data with path of directory to test for existence.
	*/
	void PhoneGapFile::actionTestDirectoryExists(JSONMessage& message)
	{
		String callbackID = message.getParam("PhoneGapCallBackId");

		String path = message.getArgsField("dirName");

		String exists = "false";
		if (FileExists(path))
		{
			exists = "true";
		}

		// Return result to PhoneGap.
		callSuccess(
			callbackID,
			exists);
	}

	/**
	 * Handler of PhoneGap FileMgr.getFreeDiskSpace
	 * @deprecated Cordova is missing this method.
	*/
	void PhoneGapFile::actionGetFreeDiskSpace(JSONMessage& message)
	{
		String callbackID = message.getParam("PhoneGapCallBackId");

		// Return -1 as the result to PhoneGap.
		callSuccess(
			callbackID,
			"-1");
	}

	/**
	 * HTTP connection class used for file uploading.
	 */
	class PhoneGapFileUploadConnection : public HighLevelHttpConnection
	{
	public:
		PhoneGapFileUploadConnection(
			PhoneGapFile* fileHandler,
			String callbackID,
			String sourceURI,
			String targetURI,
			String* requestBody,
			int bytesSent
			)
			: HighLevelHttpConnection()
		{
			mPhoneGapFile = fileHandler;
			mCallbackID = callbackID,
			mSourceURI = sourceURI;
			mTargetURI = targetURI;
			mRequestBody = requestBody;
			mBytesSent = bytesSent;
		}

		/**
		 * Called when the HTTP connection has finished downloading data.
		 * @param data Handle to the data, will be 0 on error, > 0 on success.
		 * @param result Result code, RES_OK on success, otherwise an HTTP error code.
		 */
		void dataDownloaded(MAHandle data, int result)
		{
			// Inform the PhoneGap file object that the upload
			// is complete and pass the result/error code.
			// We ignore the data sent back from the server.

			if (RES_OK != result || NULL == data)
			{
				mPhoneGapFile->callFileTransferError(
					mCallbackID,
					FILETRANSERERROR_CONNECTION_ERR,
					mSourceURI,
					mTargetURI);
			}
			else
			{
				char bytesSent[64];
				sprintf(bytesSent, "%u", mBytesSent);

				FileUtil fileUtil;
				String response = fileUtil.createTextFromHandle(data);

				mPhoneGapFile->callFileUploadResult(
					mCallbackID,
					"200", // We always send response code 200.
					bytesSent,
					response
					);
			}

			// Deallocate the data object.
			if (NULL != data)
			{
				maDestroyPlaceholder(data);
			}

			// Delete the body of the request.
			delete mRequestBody;

			// Delete upload connection.
			delete this;
		}

	private:
		PhoneGapFile* mPhoneGapFile;
		String mCallbackID;
		String mSourceURI;
		String mTargetURI;
		String* mRequestBody;
		int mBytesSent;
	};

	/**
	 * Helper function for actionUploadFile.
	 */
	static void actionUploadFileGetAdditionalParams(
		JSONMessage& message,
		Vector<String>& additionalParams)
	{
		YAJLDom::Value* args = message.getParamNode("args");
		if (NULL != args && YAJLDom::Value::MAP == args->getType())
		{
			YAJLDom::Value* params = args->getValueByIndex(5);
			if (NULL != params && YAJLDom::Value::ARRAY == params->getType())
			{
				for (int i = 0; i < params->getNumChildValues(); ++i)
				{
					YAJLDom::Value* param = params->getValueByIndex(i);
					if (NULL != param && YAJLDom::Value::MAP == param->getType())
					{
						String key = "";
						String value = "";

						YAJLDom::Value* keyNode = param->getValueForKey("Key");
						if (NULL != keyNode && YAJLDom::Value::STRING == keyNode->getType())
						{
							key = keyNode->toString();
						}

						YAJLDom::Value* valueNode = param->getValueForKey("Value");
						if (NULL != valueNode && YAJLDom::Value::STRING == valueNode->getType())
						{
							value = valueNode->toString();
						}

						if (key.size() > 0 && value.size() > 0)
						{
							additionalParams.add(key);
							additionalParams.add(value);
						}
					}
				}
			}
		}
	}

/*
Android PhoneGap 1.2 sample file upload:

C:\tmp>node upload.js
Upload server started
Server address: 192.168.0.145:4042
Socket connected
server data:
POST / HTTP/1.1
Connection: Keep-Alive
Content-Type: multipart/form-data;boundary=*****
User-Agent: Dalvik/1.4.0 (Linux; U; Android 2.3.6; Nexus One Build/GRK39F)
Host: 192.168.0.145:4042
Transfer-Encoding: chunked
Accept-Encoding: gzip


server data:
fb
--*****
Content-Disposition: form-data; name="value1";

v1
--*****
Content-Disposition: form-data; name="value2";

v2
--*****
Content-Disposition: form-data; name="file"; filename="testfile.txt"
Content-Type: text/plain

HELLO
--*****--

0
*/

/*
Sample MoSync upload:
Socket connected
server data:
POST / HTTP/1.1
Content-Type: multipart/form-data;boundary===========================2231600360
Connection: Keep-Alive
User-Agent: Dalvik/1.4.0 (Linux; U; Android 2.3.6; Nexus One Build/GRK39F)
Host: 192.168.0.145:4042
Content-Length: 381
Accept-Encoding: gzip


server data:
--==========================2231600360
Content-Disposition: form-data; name="param1";

value1
--==========================2231600360
Content-Disposition: form-data; name="param2";

value2
--==========================2231600360
Content-Disposition: form-data; name="file"; filename="testfile.txt"
Content-Type: text/plain

HELLO
--==========================2231600360--
*/

/*
Examples of request headers:

Android:

POST / HTTP/1.1
Content-Type: multipart/form-data;boundary===========================250350775
connection: close
content-length: 46306
User-Agent: Dalvik/1.4.0 (Linux; U; Android 2.3.6; Nexus One Build/GRK39F)
Host: 192.168.0.104:4044
Accept-Encoding: gzip

--==========================250350775
Content-Disposition: form-data; name="file"; filename="img1351268646.jpg"
Content-Type: image/jpeg


And on iOS:

POST / HTTP/1.0
host: 192.168.0.104
content-type: multipart/form-data;boundary===========================29854
content-length: 631267
connection: close

--==========================29854
Content-Disposition: form-data; name="file"; filename="img1351263665.png"
Content-Type: image/png
*/

/*
Documentation links:
http://www.w3.org/Protocols/rfc1341/7_2_Multipart.html
http://en.wikipedia.org/wiki/Multipart/form-data#Form_Data
http://en.wikipedia.org/wiki/File_select
*/

/*
Format of PhoneGap message:

I/maWriteLog(12821): ma:[{
"messageName":"PhoneGap",
"service":"FileTransfer",
"action":"upload",
"args":{"fileKey":"file",
"fileName":"testfile.txt",
"mimeType":"text/plain","params":[
{"Key":"param1","Value":"value1"},
{"Key":"param2","Value":"value2"}],
"filePath":"file:///data/data/com.mosync.app_WormholeFileTransfer/files/testfile.txt",
"server":"http://192.168.0.145:4042/"},
"PhoneGapCallBackId":"FileTransfer7"}]
*/

	/**
	 * Upload a file to a server.
	 *
	 * FileUploadOptions:
	 *   Options to customize the HTTP request used to upload files.
	 *   @constructor
	 *   @param fileKey {String}   Name of file request parameter. Defaults to "file".
	 *   @param fileName {String}  Filename to be used by the server. Defaults to "image.jpg".
	 *   @param mimeType {String}  Mimetype of the uploaded file. Defaults to "image/jpeg".
	 *   @param params {Object}    Object with key: value params to send to the server.
	 */
	void PhoneGapFile::actionUploadFile(JSONMessage& message)
	{
		String callbackID = message.getParam("PhoneGapCallBackId");

		// This is the form name of the post request.
		String fileKey = message.getArgsField(2);
		if (0 == fileKey.size())
		{
			fileKey = "file";
		}

		// Name of uploaded file on the server.
		String fileName = message.getArgsField(3);
		if (0 == fileName.size())
		{
			fileName = "image.jpg";
		}

		// Mime type.
		String mimeType = message.getArgsField(4);
		if (0 == mimeType.size())
		{
			mimeType = "image/jpeg";
		}

		// Server URL.
		String serverURL = message.getArgsField(1);

		// Source path of file to upload.
		String sourceFilePath = message.getArgsField(0);

		// Check that source file URI is valid.
		if (0 != sourceFilePath.find("file://"))
		{
			callFileTransferError(
				callbackID,
				FILETRANSERERROR_FILE_NOT_FOUND_ERR,
				sourceFilePath,
				serverURL);
			return;
		}

		// Read source file.
		void* fileData;
		int fileSize;
		int result = FileReadBinary(
			sourceFilePath.substr(7), // Strip off "file://"
			&fileData,
			&fileSize);
		if (result < 0)
		{
			callFileTransferError(
				callbackID,
				FILETRANSERERROR_FILE_NOT_FOUND_ERR,
				sourceFilePath,
				serverURL);
			return;
		}

		// HTTP params.
		Vector<String> httpParams;

		// Create boundary string.
		int ms = maGetMilliSecondCount();
		char boundary[1024];
		sprintf(boundary, "==========================%u", ms);

		// Set HTTP content type.
		String contentType = "multipart/form-data;boundary=";
		contentType += boundary;
		httpParams.add("Content-Type");
		httpParams.add(contentType);

		// No need to keep connection open.
		httpParams.add("connection");
		httpParams.add("close");

		// Build request body. The reason we dynamically
		// allocate the request data is that this buffer
		// needs to live during the processing of the
		// connection, and cannot be deallocated until
		// the connection is completed.
		String* requestBody = new String();
		*requestBody = "";

		// Common strings.
		String doubleDash = "--";
		String crnl = "\r\n";

		// Get additional request params.
		Vector<String> requestParams;
		actionUploadFileGetAdditionalParams(message, requestParams);

		// Write additional request params.
		for (int i = 0; i < requestParams.size(); i = i + 2)
		{
			*requestBody += doubleDash + boundary + crnl;
			*requestBody += "Content-Disposition: form-data; name=\"";
			*requestBody += requestParams[i] + "\";" + crnl + crnl;
			*requestBody += requestParams[i + 1] + crnl;
		}

		// Get file mime type.
		String fileMimeType = FileGetMimeType(sourceFilePath);

		// Write file data part.
		*requestBody += doubleDash + boundary + crnl;
		*requestBody += "Content-Disposition: form-data; name=\"";
		*requestBody += fileKey + "\"; filename=\"";
		*requestBody += fileName + "\"" + crnl;
		*requestBody += "Content-Type: " + fileMimeType + crnl + crnl;
		// File data may contain null bytes, use append.
		requestBody->append((const char*) fileData, fileSize);
		*requestBody += crnl + doubleDash + boundary + doubleDash;

		// Now we can free the fileData buffer.
		free(fileData);

		// Set content-length HTTP param, now that we know
		// the size of the data.
		char sizeBuffer[64];
		sprintf(sizeBuffer, "%d", requestBody->size());
		httpParams.add("content-length");
		httpParams.add(sizeBuffer);

		// Create the connection.
		PhoneGapFileUploadConnection* connection =
			new PhoneGapFileUploadConnection(
				this,
				callbackID,  // PhoneGap callback id.
				sourceFilePath, // For error message.
				serverURL,   // For error message.
				requestBody, // Deallocated when connection is completed.
				fileSize     // We assume all bytes are sent upon success.
				);

		// Initiate the request. The callback method of the
		// connection object will be called upon completion.
		result = connection->postRequest(
			serverURL.c_str(),
			httpParams,
			(const void*) requestBody->pointer(),
			requestBody->size()
			);

		if (WORMHOLE_HTTP_ERROR == result)
		{
			// Must be deallocated.
			delete requestBody;

			callFileTransferError(
				callbackID,
				FILETRANSERERROR_CONNECTION_ERR,
				sourceFilePath,
				serverURL);
		}

		// The result of the request is returned to PhoneGap
		// in the callback of the connection object.
	}

	/**
	 * Get the options parameters "create" and "exclusive"
	 * from the JSON tree.
	 * @return true on success, false on error.
	 */
	bool PhoneGapFile::getJSONParamsOptionsCreateExclusive(
		JSONMessage& message,
		bool& create,
		bool& exclusive)
	{
		// Default values.
		create = false;
		exclusive = false;

		// Get the root node for the message parameters.
		YAJLDom::Value* argsNode = message.getParamNode("args");
		if (NULL == argsNode || YAJLDom::Value::ARRAY != argsNode->getType())
		{
			return false;
		}

		YAJLDom::Value* optionsNode = argsNode->getValueByIndex(2);
		if (NULL != optionsNode && YAJLDom::Value::NUL != optionsNode->getType())
		{
			YAJLDom::Value* createNode = optionsNode->getValueForKey("create");
			if (NULL != optionsNode && YAJLDom::Value::NUL != optionsNode->getType())
			{
				create = createNode->toBoolean();
			}
			YAJLDom::Value* exclusiveNode = optionsNode->getValueForKey("exclusive");
			if (NULL != exclusiveNode && YAJLDom::Value::NUL != exclusiveNode->getType())
			{
				exclusive = exclusiveNode->toBoolean();
			}

			return true;
		}

		return false;
	}

	/**
	 * Get the parent fullPath of a directory entry from the JSON tree.
	 * @return true on success, false on error.
	 */
	bool PhoneGapFile::getJSONParamParentFullPath(
		JSONMessage& message,
		String& destinationPath)
	{
		// Get the root node for the message parameters.
		YAJLDom::Value* argsNode = message.getParamNode("args");
		if (NULL == argsNode || YAJLDom::Value::ARRAY != argsNode->getType())
		{
			return false;
		}

		// Get the node for the parent directory.
		YAJLDom::Value* pathNode = argsNode->getValueByIndex(1);
		if (NULL != pathNode && YAJLDom::Value::NUL != pathNode->getType())
		{
			destinationPath = pathNode->toString();
			return true;
		}
		return false;
	}

} // namespace
