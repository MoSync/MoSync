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
 * @file HighLevelImageDownloader.cpp
 * @author Mikael Kindborg
 *
 * @brief A high-level object for downloading image files.
 */

#include "HighLevelImageDownloader.h"

using namespace MAUtil;

namespace Wormhole
{

/**
 * Constructor.
 */
HighLevelImageDownloader::HighLevelImageDownloader()
{
}

/**
 * Destructor.
 */
HighLevelImageDownloader::~HighLevelImageDownloader()
{
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
void HighLevelImageDownloader::dataDownloaded(MAHandle data, int result)
{
	// The resulting image.
	MAHandle image = 0;

	// Do we have any data?
	if (data)
	{
		// Convert data to image.
		image = maCreatePlaceholder();
		int res = maCreateImageFromData(
			image,
			data,
			0,
			maGetDataSize(data));

		// Do we have an error?
		if (RES_OUT_OF_MEMORY == res)
		{
			// The image could not be created, set data handle to zero.
			image = 0;
		}

		// Deallocate the data object, we are done with it.
		maDestroyPlaceholder(data);
	}

	// Notify download complete.
	onDownloadComplete(image);
}

} // namespace
