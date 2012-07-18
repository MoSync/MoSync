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
 * Call maDestroyObject to deallocate the downloaded data.
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
	 * Note that data must be deallocated using maDestroyObject().
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
