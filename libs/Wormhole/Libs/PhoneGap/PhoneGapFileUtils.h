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
