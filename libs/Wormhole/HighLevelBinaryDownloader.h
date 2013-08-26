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

/*! \addtogroup WormHoleGroup
 *  @{
 */

/** @defgroup WormHoleGroup Wormhole Library
 *  @{
 */

/**
 * @file HighLevelBinaryDownloader.h
 * @author Mikael Kindborg
 *
 * @brief A high-level object for downloading binary files.
 */

#ifndef WORMHOLE_HIGH_LEVEL_BINARY_DOWNLOADER_H
#define WORMHOLE_HIGH_LEVEL_BINARY_DOWNLOADER_H

#include "HighLevelHttpConnection.h"

namespace Wormhole
{

/**
 * I am an easy-to-use downloader for binary data. Inherit me and
 * override onBinaryDownloaded.
 *
 * Call maDestroyPlaceholder to deallocate the downloaded data.
 *
 * Example of use:
 *
 * class MyBinaryDownloader : public HighLevelBinaryDownloader
 * {
 *   public:
 *     void onDownloadComplete(MAHandle data)
 *     {
 *       // Do something with the data.
 *     }
 * };
 *
 * // Start download.
 * (new MyBinaryDownloader())->get("http://...");
 */
class HighLevelBinaryDownloader : public HighLevelHttpConnection
{
public:
	// Public protocol.

	/**
	 * Constructor.
	 */
	HighLevelBinaryDownloader();

	/**
	 * Destructor.
	 */
	virtual ~HighLevelBinaryDownloader();

	/**
	 * Override to do something with the data that is downloaded.
	 * If download fails, data is NULL.
	 * Note that data must be deallocated using maDestroyPlaceholder().
	 */
	virtual void onDownloadComplete(MAHandle data) = 0;

protected:
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
	virtual void dataDownloaded(MAHandle data, int result);
};

}

#endif

 /*! @} */
