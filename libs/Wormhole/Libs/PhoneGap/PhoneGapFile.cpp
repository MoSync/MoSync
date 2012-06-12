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
#include <MAUtil/String.h>
#include <MAUtil/HashDict.h>
#include "../../Encoder.h"
#include "../../HighLevelHttpConnection.h"
#include "../../FileUtil.h"
#include "PhoneGapFile.h"
#include "PhoneGapMessageHandler.h"
#include "MimeTypes.h"

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
	 * List of mime-types is in MimeTypes.h.
	 */
	static String FileGetMimeType(const String& filePath)
	{
		// Get the file name extension.
		int pos = filePath.findLastOf('.');
		if (pos == String::npos)
		{
			// Extension not found.
			return sMimeTypeDefault;
		}

		// Move to position after the '.'
		pos = pos + 1;

		int extensionLength = filePath.size() - pos;
		if (extensionLength > 7)
		{
			// Something is wrong, we do not support that long extensions.
			return sMimeTypeDefault;
		}

		// Extract extension.
		String extension = filePath.substr(pos, extensionLength);
		char ext[8];
		strcpy(ext, extension.c_str());

		// Do linear search in MIME type table.
		// TODO: Optimize, use hash table.
		int size = sizeof(sMimeTypeDictionary) / (sizeof(char*) * 2);
		for (int i = 0; i < size; ++i)
		{
			const char* ext2 = sMimeTypeDictionary[i][0];
			if (0 == stricmp(ext, ext2))
			{
				return sMimeTypeDictionary[i][1];
			}
		}

		return sMimeTypeDefault;
	}

	static String FileGetLocalPath()
	{
		int bufferSize = 2048;
		char buffer[bufferSize];

		int size = maGetSystemProperty(
			"mosync.path.local",
			buffer,
			bufferSize);

		// If there was an error, return default root path.
		if (size < 0 || size > bufferSize)
		{
			return "/";
		}

		return buffer;
	}

	/**
	 * It should work also to get a directory name if the
	 * path ends with a slash, e.g.: "/sdcard/MyMusic/"
	 * or even for "/" in which case an empty string will
	 * be returned.
	 */
	static String FileGetName(const String& p)
	{
		String path = p;

		// Remove last slash if path ends with a slash.
		if ('/' == path[path.size() - 1])
		{
			path = path.substr(0, path.size() - 1);
		}

		// Find last slash.
		int pos = path.findLastOf('/');
		if (String::npos == pos)
		{
			// No slash found, just return the file path.
			return path;
		}

		// Move to position after the '/'
		pos = pos + 1;

		// Return file name.
		return path.substr(pos, path.size() - pos);
	}

	/**
	 * Get size of a file.
	 * @return File size on success, <0 on error.
	 */
	static int FileGetSize(const String& path)
	{
		MAHandle file = maFileOpen(path.c_str(), MA_ACCESS_READ_WRITE);
		if (file < 0)
		{
			return -1;
		}

		int size = maFileSize(file);
		maFileClose(file);
		return size;
	}

	/**
	 * Get date of a file.
	 * @return File date on the form "Mon Dec 19 2011 12:46:43 GMT+0100 (CET)".
	 * Returns empty string on error.
	 */
	static String FileGetDate(const String& path)
	{
		MAHandle file = maFileOpen(path.c_str(), MA_ACCESS_READ_WRITE);
		if (file < 0)
		{
			return "";
		}

		int date = maFileDate(file);
		if (date < 0)
		{
			// Error.
			return "";
		}
		maFileClose(file);

		// Return time in format "Mon Dec 19 2011 12:46:43 GMT+0100 (CET)".
		return sprint_time(date);
	}

	static bool FileExistsHelper(const String& path)
	{
		MAHandle file = maFileOpen(path.c_str(), MA_ACCESS_READ_WRITE);
		if (file < 0)
		{
			return false;
		}
		int exists = maFileExists(file);
		maFileClose(file);

		return 1 == exists;
	}

	/**
	 * Check that a file exists.
	 * Handle the case of a directory name that does not end with /.
	 * MoSync File API requires directory paths to end with a slash.
	 */
	static bool FileExists(const String& path)
	{
		if (FileExistsHelper(path))
		{
			return true;
		}
		else if (FileExistsHelper(path + '/'))
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	static bool FileIsDirectory(const String& path)
	{
		if (path[path.size() - 1] == '/')
		{
			return FileExistsHelper(path);
		}
		else if (FileExistsHelper(path + '/'))
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	/**
	 * Add a trailing slash to the path name if not present.
	 * The MoSync File API requires directory paths end with a slash.
	 * @param path Path modified if trailing slash is missing.
	 */
	static void FileMakeDirectoryPath(String& path)
	{
		if (path[path.size() - 1] != '/')
		{
			path += '/';
		}
	}

	/**
	 * @return path of parent directory, ending with a slash.
	 * Returns empty string on error.
	 */
	static String FileGetParentDirectory(const String& fullPath)
	{
		String path = fullPath;

		// Get path to parent directory.

		// The path name may end with a slash.
		char lastChar = path[path.size() - 1];
		if (lastChar == '/')
		{
			// In this case, strip off the ending slash.
			path = path.substr(0, path.size() - 1);
		}

		// Get last slash.
		int pos = path.findLastOf('/');
		if (String::npos == pos)
		{
			return "";
		}

		// Return parent path, including the slash.
		return path.substr(0, pos + 1);
	}

	/**
	 * Create file/directory that does not exist.
	 */
	static bool FileCreate(const String& path)
	{
		MAHandle file = maFileOpen(path.c_str(), MA_ACCESS_READ_WRITE);
		if (file < 0)
		{
			return false;
		}

		int result = maFileCreate(file);
		maFileClose(file);
		return 0 == result;
	}

	/**
	 * Truncate a file.
	 * @return New file length on success, <0 on error.
	 */
	static int FileTruncate(const String& path, int size)
	{
		MAHandle file = maFileOpen(path.c_str(), MA_ACCESS_READ_WRITE);
		if (file < 0)
		{
			return -1;
		}

		int exists = maFileExists(file);
		if (1 != exists)
		{
			// Error.
			maFileClose(file);
			return -1;
		}

		int fileSize = maFileSize(file);
		if (fileSize < 0)
		{
			// Error.
			maFileClose(file);
			return -1;
		}

		if (fileSize < size)
		{
			// No need to truncate, return current file size.
			maFileClose(file);
			return fileSize;
		}

		int result = maFileTruncate(file, size);
		maFileClose(file);
		if (0 == result)
		{
			// Success, return truncated size.
			return size;
		}

		// Error.
		return -1;
	}

	/**
	 * Create path recursively if parts of the path
	 * do not exist.
	 */
	static bool FileCreatePath(const String& path)
	{
		// Get parent directory and check if it exists.
		String parentPath = FileGetParentDirectory(path);
		if (0 == parentPath.size())
		{
			return false;
		}

		if (!FileExists(parentPath))
		{
			// It does not exist, create it recursively.
			bool success = FileCreatePath(parentPath);
			if (!success)
			{
				return false;
			}
		}

		// Create the path.
		return FileCreate(path);
	}

	/**
	 * @return >0 on success, <0 on error.
	 */
	static int FileWrite(const String& path, const String& data, int position)
	{
		MAHandle file = maFileOpen(path.c_str(), MA_ACCESS_READ_WRITE);	if (file < 0)
		{
			return -1;
		}

		// TODO: Should we check that position is within file size?
		// int size = maFileSize(file);

		// If we start writing from the beginning of the file, we truncate
		// the file. Not clear what the specification says about this.
		// TODO: Check how this should work! It makes no sense to truncate here.
	//	if (position == 0)
	//	{
	//		maFileTruncate(file, 0);
	//	}

		int result = maFileSeek(file, position, MA_SEEK_SET);

		// New position must equal requested position.
		if (result != position)
		{
			return -1;
		}

		result = maFileWrite(file, data.c_str(), data.size());
		if (result < 0)
		{
			return -1;
		}

		maFileClose(file);

		return 1;
	}

	/**
	 * @return >0 on success, <0 on error.
	 */
	static int FileReadBinary(
		const String& path,
		void** outParamFileData,
		int* outParamFileSize)
	{
		MAHandle file = maFileOpen(path.c_str(), MA_ACCESS_READ_WRITE);
		if (file < 0)
		{
			return -1;
		}

		int exists = maFileExists(file);
		if (1 != exists)
		{
			maFileClose(file);
			return -1;
		}

		int size = maFileSize(file);
		if (size < 0)
		{
			maFileClose(file);
			return -1;
		}

		// Alloc one extra byte in case the caller
		// wish to zero-terminate the buffer.
		void* buf = (void*) malloc(size + 1);
		if (NULL == buf)
		{
			maFileClose(file);
			return -1;
		}

		int result = maFileRead(file, buf, size);

		maFileClose(file);

		if (result < 0)
		{
			free(buf);
			return -1;
		}

		*outParamFileData = buf;
		*outParamFileSize = size;

		return 1;
	}

	/**
	 * @return >0 on success, <0 on error.
	 */
	static int FileRead(const String& path, String& outParamFileDataString)
	{
		char* fileData;
		int fileSize;

		int result = FileReadBinary(path, (void**)&fileData, &fileSize);
		if (result < 0)
		{
			return -1;
		}

		// Zero-terminate buffer.
		fileData[fileSize] = 0;

		outParamFileDataString = fileData;

		free(fileData);

		return 1;
	}

	/**
	 * Delete a file or directory. If the file is a
	 * directory it must me empty.
	 */
	static int FileDeleteFile(const String& pathParam)
	{
		String path = pathParam;

		if (FileIsDirectory(path))
		{
			FileMakeDirectoryPath(path);
		}

		MAHandle file = maFileOpen(path.c_str(), MA_ACCESS_READ_WRITE);
		if (file < 0)
		{
			return -1;
		}

		int result = maFileDelete(file);
		maFileClose(file);
		return result;
	}

	/**
	 * Delete a directory recursively.
	 */
	static int FileDeleteDirectory(const String& pathParam)
	{
		char nameBuf[2048];

		String path = pathParam;

		FileMakeDirectoryPath(path);

		// Open directory listing.
		MAHandle list = maFileListStart(path.c_str(), "*", MA_FL_SORT_NONE);
		if (list < 0)
		{
			return -1;
		}

		// Delete all files in this directory and in subdirectories.
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
				// Error.
				return -1;
			}

			String fullPath = path + nameBuf;

			// Is this a directory?
			if ('/' == nameBuf[result - 1])
			{

				// Delete recursively.
				FileDeleteDirectory(fullPath);
			}
			else
			{
				// Delete file.
				FileDeleteFile(fullPath);
			}
		}

		// Close the directory listing.
		maFileListClose(list);

		// Delete the directory.
		return FileDeleteFile(path);
	}

	/**
	 * Copy a file. Overwrites the destination file.
	 * @return 0 on success <0 on error.
	 */
	static int FileCopyFile(
		const String& sourcePath,
		const String& destinationPath)
	{
		// Open source file.
		MAHandle sourceFile = maFileOpen(sourcePath.c_str(), MA_ACCESS_READ_WRITE);
		if (sourceFile < 0)
		{
			return -1;
		}

		// Check that source file exists.
		int exists = maFileExists(sourceFile);
		if (1 != exists)
		{
			maFileClose(sourceFile);
			return -1;
		}

		// Get and check source size.
		int fileSize = maFileSize(sourceFile);
		if (fileSize < 0)
		{
			maFileClose(sourceFile);
			return -1;
		}

		// Create data object for source data to copy.
		MAHandle data = maCreatePlaceholder();
		int createDataResult = maCreateData(data, fileSize);
		if (RES_OK != createDataResult)
		{
			maFileClose(sourceFile);
			maDestroyPlaceholder(data);
			return -1;
		}

		int readResult = maFileReadToData(sourceFile, data, 0, fileSize);
		if (readResult < 0)
		{
			maFileClose(sourceFile);
			maDestroyPlaceholder(data);
			return -1;
		}

		// This deletes the destination file if it already exists.
		FileDeleteFile(destinationPath);

		// Create destination file.
		bool createSuccess = FileCreatePath(destinationPath);
		if (!createSuccess)
		{
			maFileClose(sourceFile);
			maDestroyPlaceholder(data);
			return -1;
		}

		// Open destination file.
		MAHandle destinationFile = maFileOpen(destinationPath.c_str(), MA_ACCESS_READ_WRITE);
		if (destinationFile < 0)
		{
			maFileClose(sourceFile);
			maDestroyPlaceholder(data);
			return -1;
		}

		// Write data to destination file.
		int writeResult = maFileWriteFromData(destinationFile, data, 0, fileSize);
		if (writeResult < 0)
		{
			maFileClose(sourceFile);
			maFileClose(destinationFile);
			maDestroyPlaceholder(data);
			return -1;
		}

		// Close files and free data object.
		maFileClose(sourceFile);
		maFileClose(destinationFile);
		maDestroyPlaceholder(data);

		// Success.
		return 0;
	}

	/**
	 * Copy the files in a directory recursively.
	 */
	static int FileCopyDirectory(
		const String& sourcePathParam,
		const String& destinationPathParam)
	{
		char nameBuf[2048];

		String sourcePath = sourcePathParam;
		String destinationPath = destinationPathParam;

		// Make sure both source and destination paths end with a slash.
		FileMakeDirectoryPath(sourcePath);
		FileMakeDirectoryPath(destinationPath);

		// Open directory listing of source dir.
		MAHandle list = maFileListStart(sourcePath.c_str(), "*", MA_FL_SORT_NONE);
		if (list < 0)
		{
			return -1;
		}

		// Copy all files in this directory and in subdirectories.
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
				// Error.
				return -1;
			}

			String fullSourcePath = sourcePath + nameBuf;
			String fullDestinationPath = destinationPath + nameBuf;

			// Is this a directory?
			if ('/' == nameBuf[result - 1])
			{
				// Copy recursively.
				FileCopyDirectory(fullSourcePath, fullDestinationPath);
			}
			else
			{
				// Copy file.
				FileCopyFile(fullSourcePath, fullDestinationPath);
			}
		}

		// Close the directory listing.
		maFileListClose(list);

		return 0;
	}

	/**
	 * Copy file or directory structure.
	 */
	static int FileCopy(
		const String& sourcePath,
		const String& destinationPath)
	{
		if (FileIsDirectory(sourcePath))
		{
			return FileCopyDirectory(sourcePath, destinationPath);
		}
		else if (FileExistsHelper(sourcePath))
		{
			return FileCopyFile(sourcePath, destinationPath);
		}
		else
		{
			return -1;
		}
	}

	/**
	 * Move file.
	 */
	static int FileMoveFile(
		const String& sourcePath,
		const String& destinationPath)
	{
		// Copy source to target.
		int result = FileCopyFile(sourcePath, destinationPath);
		if (result > -1)
		{
			// Delete source.
			return FileDeleteFile(sourcePath);
		}
		return -1;
	}

	/**
	 * Move directory structure.
	 */
	static int FileMoveDirectory(
		const String& sourcePath,
		const String& destinationPath)
	{
		// Copy source to target.
		int result = FileCopyDirectory(sourcePath, destinationPath);
		if (result > -1)
		{
			// Delete source.
			return FileDeleteDirectory(sourcePath);
		}
		return -1;
	}

	/**
	 * Move file or directory structure.
	 */
	static int FileMove(
		const String& sourcePath,
		const String& destinationPath)
	{
		if (FileIsDirectory(sourcePath))
		{
			return FileMoveDirectory(sourcePath, destinationPath);
		}
		else if (FileExistsHelper(sourcePath))
		{
			return FileMoveFile(sourcePath, destinationPath);
		}
		else
		{
			return -1;
		}
	}

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
			"PhoneGap.CallbackError",
			callbackID,
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
			actionTestFileExists(message);
		}
		else if (action == "testDirectoryExists")
		{
			actionTestDirectoryExists(message);
		}
		else if (action == "getFreeDiskSpace")
		{
			actionGetFreeDiskSpace(message);
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
		int type = message.getArgsFieldInt("type");
		if (LOCALFILESYSTEM_PERSISTENT != type)
		{
			callFileError(callbackID, FILEERROR_NO_MODIFICATION_ALLOWED_ERR);
			return;
		}

		// Size parameter must be zero.
		int size = message.getArgsFieldInt("size");
		if (0 != size)
		{
			callFileError(callbackID, FILEERROR_NO_MODIFICATION_ALLOWED_ERR);
			return;
		}

		// Get local root path and remove trailing slash, if any.
		String path = FileGetLocalPath();

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

		String fullPath = message.getArgsField("fullPath");
		String path = message.getArgsField("path");
		String fullFilePath = fullPath + "/" + path;

		// Get flags "create" and "exclusive".
		bool create = false;
		bool exclusive = false;
		bool success = message.getJSONParamsOptionsCreateExclusive(create, exclusive);
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

		String fullPath = message.getArgsField("fullPath");
		String path = message.getArgsField("path");
		String fullFilePath = fullPath + "/" + path;

		// Add a trailing slash if not present. MoSync API requires this.
		if (fullFilePath[fullFilePath.size() - 1] != '/')
		{
			fullFilePath += "/";
		}

		// Get flags "create" and "exclusive".
		bool create = false;
		bool exclusive = false;
		bool success = message.getJSONParamsOptionsCreateExclusive(create, exclusive);
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

		String fullPath = message.getArgsField("fullPath");

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

		String fullPath = message.getArgsField("fileName");
		String data = message.getArgsField("data");
		int position = message.getArgsFieldInt("position");

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

		String fullPath = message.getArgsField("fileName");

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

		String fullPath = message.getArgsField("fileName");

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

		String fullPath = message.getArgsField("fileName");

		int size = message.getArgsFieldInt("size");

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

		String sourcePath = message.getArgsField("fullPath");

		String destinationName = message.getArgsField("newName");

		// Get destination path from JSON data (from DirectoryEntry object).
		String destinationPath;
		bool success = message.getJSONParamParentFullPath(destinationPath);
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

		String path = message.getArgsField("fullPath");

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

		String path = message.getArgsField("fullPath");

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

		String path = message.getArgsField("fullPath");

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
	 * TODO: How should we implement this? Which file system are
	 * we talking about? Unsupported for now.
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
			YAJLDom::Value* params = args->getValueForKey("params");
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

// http://www.w3.org/Protocols/rfc1341/7_2_Multipart.html
// http://en.wikipedia.org/wiki/Multipart/form-data#Form_Data
// http://en.wikipedia.org/wiki/File_select

/*
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
		String fileKey = message.getArgsField("fileKey");
		if (0 == fileKey.size())
		{
			fileKey = "file";
		}

		// Name of uploaded file on the server.
		String fileName = message.getArgsField("fileName");
		if (0 == fileName.size())
		{
			fileName = "image.jpg";
		}

		// Mime type.
		String mimeType = message.getArgsField("mimeType");
		if (0 == mimeType.size())
		{
			mimeType = "image/jpeg";
		}

		// Server URL.
		String serverURL = message.getArgsField("server");

		// Source path of file to upload.
		String sourceFilePath = message.getArgsField("filePath");

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

		// Set HTTP params.
		String contentType = "multipart/form-data;boundary=";
		contentType += boundary;
		httpParams.add("Content-Type");
		httpParams.add(contentType);

		httpParams.add("Connection");
		httpParams.add("Keep-Alive");

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

} // namespace
