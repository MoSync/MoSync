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
 * @file HighLevelTextDownloader.cpp
 * @author Mikael Kindborg
 *
 * @brief A high-level object for downloading text files.
 */

#include "HighLevelTextDownloader.h"

using namespace MAUtil;

namespace Wormhole
{

/**
 * Constructor.
 */
HighLevelTextDownloader::HighLevelTextDownloader()
{
}

/**
 * Destructor.
 */
HighLevelTextDownloader::~HighLevelTextDownloader()
{
}

/**
 * Use to free text string passed by onDownloadComplete.
 */
void HighLevelTextDownloader::freeData(char* text)
{
	if (NULL != text)
	{
		delete[] text;
	}
}

/**
 * Inherited from HighLevelHttpConnection.
 * Called when the HTTP connection has finished
 * downloading data.
 * Calls onDownloadComplete.
 * @param data Handle to the data, will be
 * 0 on error, > 0 on success.
 * @param result Result code, RES_OK on success,
 * otherwise an HTTP error code.
 */
void HighLevelTextDownloader::dataDownloaded(MAHandle data, int result)
{
	// The resulting text.
	char* text = NULL;

	// Do we have any data?
	if (data)
	{
		// Copy data to string.
		int size = maGetDataSize(data);
		text = new char[size + 1];
		if (text)
		{
			maReadData(data, text, 0, size);
			text[size] = '\0'; // Zero terminate string
		}

		// Deallocate the data object, we are done with it.
		maDestroyPlaceholder(data);
	}

	// Notify download complete.
	onDownloadComplete(text);
}

} // namespace
