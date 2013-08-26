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
 * @file HighLevelImageDownloader.h
 * @author Mikael Kindborg
 *
 * @brief A high-level object for downloading image files.
 */

#ifndef WORMHOLE_HIGH_LEVEL_IMAGE_DOWNLOADER_H
#define WORMHOLE_HIGH_LEVEL_IMAGE_DOWNLOADER_H

#include "HighLevelHttpConnection.h"

namespace Wormhole
{

/**
 * I am an easy-to-use downloader for images. Inherit me and
 * override onImageDownloaded.
 *
 * Call maDestroyPlaceholder to deallocate the downloaded image.
 *
 * Example of use:
 *
 * class MyImageDownloader : public HighLevelImageDownloader
 * {
 *   public:
 *     void onDownloadComplete(MAHandle image)
 *     {
 *       // Do something with the image.
 *     }
 * };
 *
 * // Start download.
 * (new MyDownloader())->get("http://...");
 */
class HighLevelImageDownloader : public HighLevelHttpConnection
{
public:
	/**
	 * Constructor.
	 */
	HighLevelImageDownloader();

protected:
	/**
	 * Destructor.
	 */
	virtual ~HighLevelImageDownloader();

public:
	/**
	 * Override to do something with the image that is downloaded.
	 * If download fails, image is 0.
	 * Note that image must be deallocated using maDestroyPlaceholder.
	 */
	virtual void onDownloadComplete(MAHandle image) = 0;

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
