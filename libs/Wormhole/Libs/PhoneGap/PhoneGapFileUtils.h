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
 * @file PhoneGapFileUtils.h
 * @author Mikael Kindborg
 *
 * Tools for implementation of PhoneGap calls made from JavaScript.
 */

#ifndef WORMHOLE_LIBS_PHONEGAP_PHONEGAPFILEUTILS_H_
#define WORMHOLE_LIBS_PHONEGAP_PHONEGAPFILEUTILS_H_

#include <NativeUI/WebView.h>
#include <MAUtil/String.h>

namespace Wormhole
{

	MAUtil::String FileGetMimeType(const MAUtil::String& filePath);

	MAUtil::String FileGetName(const MAUtil::String& p);

	int FileGetSize(const MAUtil::String& path);

	MAUtil::String FileGetDate(const MAUtil::String& path);

	bool FileExistsHelper(const MAUtil::String& path);

	bool FileExists(const MAUtil::String& path);

	bool FileIsDirectory(const MAUtil::String& path);

	void FileMakeDirectoryPath(MAUtil::String& path);

	MAUtil::String FileGetParentDirectory(const MAUtil::String& fullPath);

	bool FileCreate(const MAUtil::String& path);

	int FileTruncate(const MAUtil::String& path, int size);

	bool FileCreatePath(const MAUtil::String& path);

	int FileWrite(const MAUtil::String& path, const MAUtil::String& data, int position);

	int FileReadBinary(
			const MAUtil::String& path,
			void** outParamFileData,
			int* outParamFileSize);

	int FileRead(const MAUtil::String& path, MAUtil::String& outParamFileDataString);

	int FileDeleteFile(const MAUtil::String& pathParam);

	int FileDeleteDirectory(const MAUtil::String& pathParam);

	int FileCopyFile(
			const MAUtil::String& sourcePath,
			const MAUtil::String& destinationPath);

	int FileCopyDirectory(
			const MAUtil::String& sourcePathParam,
			const MAUtil::String& destinationPathParam);

	int FileCopy(
			const MAUtil::String& sourcePath,
			const MAUtil::String& destinationPath);

	int FileMoveFile(
			const MAUtil::String& sourcePath,
			const MAUtil::String& destinationPath);

	int FileMoveDirectory(
			const MAUtil::String& sourcePath,
			const MAUtil::String& destinationPath);

	int FileMove(
			const MAUtil::String& sourcePath,
			const MAUtil::String& destinationPath);

} // namespace

#endif
