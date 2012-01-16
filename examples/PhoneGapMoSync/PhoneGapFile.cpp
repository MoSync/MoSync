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
#include "PhoneGapFile.h"
#include "PhoneGapMessageHandler.h"

using namespace MAUtil;

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


// For unknown type it is recommended to use 'application/octet-stream'.
// http://stackoverflow.com/questions/1176022/unknown-file-type-mime
static String MimeTypeDefault = "application/octet-stream";

/// Stores mime type for all necessary extension
static char* MimeTypeDictionary[][2] =
{
	{"avi", "video/x-msvideo"},
	{"bmp", "image/bmp"},
	{"gif", "image/gif"},
	{"jpe", "image/jpeg"},
	{"jpeg", "image/jpeg"},
	{"jpg", "image/jpeg"},
	{"mov", "video/quicktime"},
	{"mp2", "audio/mpeg"},
	{"mp3", "audio/mpeg"},
	{"mp4", "video/mp4"},
	{"mpe", "video/mpeg"},
	{"mpeg", "video/mpeg"},
	{"mpg", "video/mpeg"},
	{"mpga", "audio/mpeg"},
	{"pbm", "image/x-portable-bitmap"},
	{"pcm", "audio/x-pcm"},
	{"pct", "image/pict"},
	{"pgm", "image/x-portable-graymap"},
	{"pic", "image/pict"},
	{"pict", "image/pict"},
	{"png", "image/png"},
	{"pnm", "image/x-portable-anymap"},
	{"pnt", "image/x-macpaint"},
	{"pntg", "image/x-macpaint"},
	{"ppm", "image/x-portable-pixmap"},
	{"qt", "video/quicktime"},
	{"ra", "audio/x-pn-realaudio"},
	{"ram", "audio/x-pn-realaudio"},
	{"ras", "image/x-cmu-raster"},
	{"rgb", "image/x-rgb"},
	{"snd", "audio/basic"},
	{"txt", "text/plain"},
	{"tif", "image/tiff"},
	{"tiff", "image/tiff"},
	{"wav", "audio/x-wav"},
	{"wbmp", "image/vnd.wap.wbmp"}
};

static String FileGetMimeType(const String& filePath)
{
	// Get the file name extension.
	int pos = filePath.findLastOf('.');
	if (pos == String::npos)
	{
		// Extension not found.
		return MimeTypeDefault;
	}

	// Move to position after the '.'
	pos = pos + 1;

	int extensionLength = filePath.size() - pos;
	if (extensionLength > 7)
	{
		// Something is wrong, we do not support that long extensions.
		return MimeTypeDefault;
	}

	// Extract extension.
	String extension = filePath.substr(pos, extensionLength);
	char ext[8];
	strcpy(ext, extension.c_str());

	// Do linear search in MIME type table.
	// TODO: Optimize, use hash table.
	int size = sizeof(MimeTypeDictionary) / (sizeof(char*) * 2);
	for (int i = 0; i < size; ++i)
	{
		char* ext2 = MimeTypeDictionary[i][0];
		//lprintfln("@@@@@ FileGetMimeType size: %i ext: %s ext2: %s", size, ext, ext2);
		if (0 == stricmp(ext, ext2))
		{
			return MimeTypeDictionary[i][1];
		}
	}

	return MimeTypeDefault;
}

/**
 * TODO: This must work to get a directory name if
 * the path ends with a slash, e.g.: /sdcard/MyMusic/
 */
static String FileGetName(const String& filePath)
{
	// TODO: Remove last slash if path ends with a slash.

	// Find last slash.
	int pos = filePath.findLastOf('/');
	if (String::npos == pos)
	{
		// No slash found, just return the file path.
		return filePath;
	}

	// Move to position after the '/'
	pos = pos + 1;

	// Return file name.
	return filePath.substr(pos, filePath.size() - pos);
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

/**
 * TODO: Handle the case of a directory that does not end with /.
 */
static bool FileExists(const String& path)
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

	lprintfln("@@@@@@@ FileGetParentDirectory: %s", (path.substr(0, pos + 1)).c_str());

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

	// TODO: Now we assume file must exist. Is that ok?
	//int exists = maFileExists(file);

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
static int FileRead(const String& path, String& data)
{
	MAHandle file = maFileOpen(path.c_str(), MA_ACCESS_READ_WRITE);
	if (file < 0)
	{
		return -1;
	}

	int size = maFileSize(file);
	if (size < 0)
	{
		return -1;
	}

	char* buf = (char*) malloc(size + 1);
	if (NULL == buf)
	{
		return -1;
	}

	int result = maFileRead(file, buf, size);

	maFileClose(file);

	if (result < 0)
	{
		free(buf);
		return -1;
	}

	buf[size] = 0;

	data = buf;

	free(buf);

	return 1;
}

/**
 * Delete a file we know exists.
 */
static int FileDeleteFile(const String& path)
{
	MAHandle file = maFileOpen(path.c_str(), MA_ACCESS_READ_WRITE);
	if (file < 0)
	{
		return -1;
	}

	int result = maFileDelete(file);
	maFileClose(file);
	return 0 == result;
}

/**
 * Delete a directory recursively.
 * Path MUST end with a slash.
 */
static int FileDeleteDirectory(const String& path)
{
	char nameBuf[2048];

	// Open directory listing.
	MAHandle list = maFileListStart(path.c_str(), "", MA_FL_SORT_NONE);
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

		lprintfln("===== FileDeleteDirectory fullPath: %s", fullPath.c_str());

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
	FileDeleteFile(path);
}

/**
 * Delete file or directory. This is the top-level function.
 * Directories are deleted recursively.
 * TODO: In PhoneGap a directory MUST NOT be recursively deleted.
 * on remove. The directory must be empty. Delete recursively on
 * removeRecursively. Implement this.
 */
static int FileDelete(const String& path)
{
	// If path ends with a slash we know it should be
	// a directory.
	if (path[path.size() - 1] == '/')
	{
		if (FileExists(path))
		{
			return FileDeleteDirectory(path);
		}
		else
		{
			// Directory does not exist.
			return -1;
		}
	}
	else
	{
		// The file can still be a directory, but if the path
		// exists, it is just a file. If the path with a trailing
		// slash exists, it is a directory.
		if (FileExists(path))
		{
			return FileDeleteFile(path);
		}
		else if (FileExists(path + "/"))
		{
			return FileDeleteDirectory(path + "/");
		}
	}
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
	const String& sourcePath,
	const String& destinationPath)
{
	char nameBuf[2048];

	// Open directory listing of source dir.
	MAHandle list = maFileListStart(sourcePath.c_str(), "", MA_FL_SORT_NONE);
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

		lprintfln("===== FileCopyDirectory fullSourcePath: %s", fullSourcePath.c_str());
		lprintfln("===== FileCopyDirectory fullDestinationPath: %s", fullDestinationPath.c_str());

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
}

/**
 * Copy file or directory structure.
 */
static int FileCopy(
	const String& sourcePath,
	const String& destinationPath)
{

	// If path ends with a slash we know it should be
	// a directory.
	if (sourcePath[sourcePath.size() - 1] == '/')
	{
		if (FileExists(sourcePath))
		{
			return FileCopyDirectory(sourcePath, destinationPath);
		}
		else
		{
			// Directory does not exist.
			return -1;
		}
	}
	else
	{
		// The file can still be a directory, but if the path
		// exists, it is just a file. If the path with a trailing
		// slash exists, it is a directory.
		if (FileExists(sourcePath))
		{
			return FileCopyDirectory(sourcePath, destinationPath);
		}
		else if (FileExists(sourcePath + "/"))
		{
			return FileCopyDirectory(sourcePath + "/", destinationPath);
		}
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
	String args = "{\"code\":" + errorCode + "}";

	mMessageHandler->callError(
		callbackID,
		args);
}

/**
 * Implementation of File API exposed to JavaScript.
 * @return true if message was handled, false if not.
 */
void PhoneGapFile::handleMessage(PhoneGapMessage& message)
{
	if (message.getParam("action") == "requestFileSystem")
	{
		actionRequestFileSystem(message);
	}
	else if (message.getParam("action") == "getFile")
	{
		actionGetFile(message);
	}
	else if (message.getParam("action") == "getFileMetadata")
	{
		actionGetFileMetadata(message);
	}
	else if (message.getParam("action") == "getMetadata")
	{
		actionGetMetadata(message);
	}
	else if (message.getParam("action") == "write")
	{
		actionWrite(message);
	}
	else if (message.getParam("action") == "readAsText")
	{
		actionReadAsText(message);
	}
	else if (message.getParam("action") == "readAsDataURL")
	{
		actionReadAsDataURL(message);
	}
	else if (message.getParam("action") == "truncate")
	{
		actionTruncate(message);
	}
	else if (message.getParam("action") == "copyTo")
	{
		actionCopyToOrMoveTo(message, false);
	}
	else if (message.getParam("action") == "moveTo")
	{
		actionCopyToOrMoveTo(message, true);
	}
	else if (message.getParam("action") == "remove")
	{
		actionRemove(message);
	}

	// TODO: Implement getDirectory to create a directory.
	// TODO: Make getFile to create a file.
	// TODO: Implement DirectoryReader.readEntries.
}

/**
 * Return a FileSystem object.
 */
void PhoneGapFile::actionRequestFileSystem(PhoneGapMessage& message)
{
	lprintfln("@@@ actionRequestFileSystem\n");

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

	// TODO: Replace hard-coded path with platform aware path handling.
	String rootEntry = emitDirectoryEntry("sdcard", "/mnt/sdcard");
	String fileSystemInfo = emitFileSystemInfo("persistent", rootEntry);
	callSuccess(
		callbackID,
		fileSystemInfo,
		"window.localFileSystem._castFS");
}

/**
 * Return a FileEntry object.
 */
void PhoneGapFile::actionGetFile(PhoneGapMessage& message)
{
	lprintfln("@@@ actionGetFile\n");

	String callbackID = message.getParam("PhoneGapCallBackId");

	String fullPath = message.getArgsField("fullPath");
	String path = message.getArgsField("path");
	String fullFilePath = fullPath + "/" + path;

	// Get flags "create" and "exclusive".
	bool create = false;
	bool exclusive = false;

	// There must exist a root node for the arguments.
	YAJLDom::Value* argsNode = message.getJSONRoot();
	if (NULL == argsNode || YAJLDom::Value::NUL == argsNode->getType())
	{
		callFileError(callbackID, FILEERROR_NO_MODIFICATION_ALLOWED_ERR);
		return;
	}

	YAJLDom::Value* optionsNode = argsNode->getValueForKey("options");
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
			bool success = FileCreatePath(fullFilePath);
			if (!success)
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

/**
 * Return a File object.
 */
void PhoneGapFile::actionGetFileMetadata(PhoneGapMessage& message)
{
	lprintfln("@@@ actionGetFileMetadata\n");

	String callbackID = message.getParam("PhoneGapCallBackId");

	String fullPath = message.getArgsField("fullPath");

	char sizeBuf[64];
	sprintf(sizeBuf, "%i", FileGetSize(fullPath));

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
void PhoneGapFile::actionGetMetadata(PhoneGapMessage& message)
{
	lprintfln("@@@ actionGetMetadata\n");

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

void PhoneGapFile::actionWrite(PhoneGapMessage& message)
{
	lprintfln("@@@ actionWrite\n");

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

void PhoneGapFile::actionReadAsText(PhoneGapMessage& message)
{
	lprintfln("@@@ actionReadAsText\n");

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
		PhoneGapMessage::JSONStringify(content.c_str()));
}

void PhoneGapFile::actionReadAsDataURL(PhoneGapMessage& message)
{
	lprintfln("@@@ actionReadAsDataURL\n");

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
	base64URL += PhoneGapMessage::base64Encode(content.c_str());
	base64URL += "\"";

	// Send back the file content.
	callSuccess(callbackID, base64URL);
}

void PhoneGapFile::actionTruncate(PhoneGapMessage& message)
{
	lprintfln("@@@ actionTruncate\n");

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

//I/maWriteLog(20616): @@@ URL: mosync://PhoneGap?service=File&action=moveTo&args={"fullPath":"/mnt/sdcard/hello2.txt","pa
//rent":{"isFile":false,"isDirectory":true,"name":"sdcard","fullPath":"/mnt/sdcard","filesystem":null},"newName":"hello3.t
//xt"}&PhoneGapCallBackId=File19
void PhoneGapFile::actionCopyToOrMoveTo(PhoneGapMessage& message, bool move)
{
	lprintfln("@@@ actionMoveTo\n");

	String callbackID = message.getParam("PhoneGapCallBackId");

	String sourcePath = message.getArgsField("fullPath");

	String destinationName = message.getArgsField("newName");

	String destinationPath;

	// Get the destination path from the JSON tree.

	// Get the root node for the message parameters.
	YAJLDom::Value* argsNode = message.getJSONRoot();
	if (NULL == argsNode || YAJLDom::Value::NUL == argsNode->getType())
	{
		callFileError(callbackID, FILEERROR_NO_MODIFICATION_ALLOWED_ERR);
		return;
	}

	// Get the node for the parent directory.
	YAJLDom::Value* parentNode = argsNode->getValueForKey("parent");
	if (NULL != parentNode && YAJLDom::Value::MAP == parentNode->getType())
	{
		YAJLDom::Value* pathNode = parentNode->getValueForKey("fullPath");
		if (NULL != pathNode && YAJLDom::Value::NUL != pathNode->getType())
		{
			destinationPath = pathNode->toString();
		}
	}

	// Check that we have the required path names.
	if ((sourcePath.size() == 0) ||
		(destinationName.size() == 0) ||
		(destinationPath.size() == 0))
	{
		callFileError(callbackID, FILEERROR_NO_MODIFICATION_ALLOWED_ERR);
		return;
	}

	// Move the file.

	String fullDestinationPath = destinationPath + "/";
	fullDestinationPath += destinationName;

	lprintfln(">>>>> movefile sourcePath: %s", sourcePath.c_str());
	lprintfln(">>>>> movefile destinationPath: %s", destinationPath.c_str());
	lprintfln(">>>>> movefile destinationName: %s", destinationName.c_str());
	lprintfln(">>>>> movefile fullDestinationPath: %s", fullDestinationPath.c_str());

	// TODO: Implement this.
	maPanic(0, "File Copy/Move not implemented");

//	int result = FileCopyOrMove(sourcePath, fullDestinationPath, move);
//	if (result < 0)
//	{
//		lprintfln(">>>>> movefile callFileError(callbackID, FILEERROR_NOT_FOUND_ERR);");
//		callFileError(callbackID, FILEERROR_NOT_FOUND_ERR);
//		return;
//	}

	// Send back FileEntry data.
	String fileEntry = emitFileEntry(
		destinationName,
		fullDestinationPath);
	callSuccess(
		callbackID,
		fileEntry,
		"window.localFileSystem._castEntry");
}

void PhoneGapFile::actionRemove(PhoneGapMessage& message)
{
	lprintfln("@@@ actionRemove\n");

	String callbackID = message.getParam("PhoneGapCallBackId");

	String path = message.getArgsField("fullPath");

	FileDeleteDirectory(path);

//	bool success = FileDelete(path);
//	if (!success)
//	{
//		callFileError(callbackID, FILEERROR_NOT_FOUND_ERR);
//		return;
//	}

	// Send back FileEntry data.
	String fileEntry = emitFileEntry(
		FileGetName(path),
		path);
	callSuccess(
		callbackID,
		fileEntry,
		"window.localFileSystem._castEntry");
}
