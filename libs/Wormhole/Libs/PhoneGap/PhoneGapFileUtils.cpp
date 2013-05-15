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
 * @file PhoneGapFileUtils.cpp
 * @author Mikael Kindborg
 *
 * Tools for implementation of PhoneGap calls made from JavaScript.
 */

#include <mastring.h>
#include <matime.h>
#include <conprint.h>
#include "PhoneGapFileUtils.h"
#include "PhoneGapMessageHandler.h"
#include "MimeTypes.h"

using namespace MAUtil;

namespace Wormhole
{

	/**
	 * List of mime-types is in MimeTypes.h.
	 */
	String FileGetMimeType(const String& filePath)
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

	/**
	 * It should work also to get a directory name if the
	 * path ends with a slash, e.g.: "/sdcard/MyMusic/"
	 * or even for "/" in which case an empty string will
	 * be returned.
	 */
	String FileGetName(const String& p)
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
	int FileGetSize(const String& path)
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
	String FileGetDate(const String& path)
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

	bool FileExistsHelper(const String& path)
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
	bool FileExists(const String& path)
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

	bool FileIsDirectory(const String& path)
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
	void FileMakeDirectoryPath(String& path)
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
	String FileGetParentDirectory(const String& fullPath)
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
	bool FileCreate(const String& path)
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
	int FileTruncate(const String& path, int size)
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
	bool FileCreatePath(const String& path)
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
	int FileWrite(const String& path, const String& data, int position)
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
	int FileReadBinary(
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
	int FileRead(const String& path, String& outParamFileDataString)
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
	int FileDeleteFile(const String& pathParam)
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
	int FileDeleteDirectory(const String& pathParam)
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
	int FileCopyFile(
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
	int FileCopyDirectory(
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
	int FileCopy(
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
	int FileMoveFile(
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
	int FileMoveDirectory(
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
	int FileMove(
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

} // namespace
