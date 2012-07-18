/*
Copyright (C) 2012 MoSync AB

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
 * Call maDestroyObject to deallocate the downloaded image.
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
	 * Note that image must be deallocated using maDestroyObject.
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
