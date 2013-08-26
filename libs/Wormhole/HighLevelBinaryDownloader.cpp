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
 * @file HighLevelBinaryDownloader.cpp
 * @author Mikael Kindborg
 *
 * @brief A high-level object for downloading binary files.
 */

#include "HighLevelBinaryDownloader.h"

using namespace MAUtil;

namespace Wormhole
{

/**
 * Constructor.
 */
HighLevelBinaryDownloader::HighLevelBinaryDownloader()
{
}

/**
 * Destructor.
 */
HighLevelBinaryDownloader::~HighLevelBinaryDownloader()
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
void HighLevelBinaryDownloader::dataDownloaded(MAHandle data, int result)
{
	// We just pass on the data handle, which will
	// be zero (NULL) on error.
	onDownloadComplete(data);
}

}
