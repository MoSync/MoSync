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
 * @file HighLevelTextDownloader.h
 * @author Mikael Kindborg
 *
 * @brief A high-level object for downloading text files.
 */

#ifndef WORMHOLE_HIGH_LEVEL_TEXT_DOWNLOADER_H
#define WORMHOLE_HIGH_LEVEL_TEXT_DOWNLOADER_H

#include "HighLevelHttpConnection.h"

namespace Wormhole
{

/**
 * I am an easy-to-use downloader for text data. Inherit me and
 * override onDownloadComplete.
 *
 * Call HighLevelTextDownloader::freeData() to free the downloaded data.
 *
 * Example of use:
 *
 * class MyTextDownloader : public HighLevelTextDownloader
 * {
 *   public:
 *     void onDownloadComplete(char* text)
 *     {
 *       // Do something with the text.
 *     }
 * };
 *
 * // Start download.
 * (new MyTextDownloader())->get("http://...");
 */
class HighLevelTextDownloader : public HighLevelHttpConnection
{
public:
	/**
	 * Constructor.
	 */
	HighLevelTextDownloader();

protected:
	/**
	 * Destructor.
	 */
	virtual ~HighLevelTextDownloader();

public:
	/**
	 * Override to do something with the text that is downloaded.
	 * If download fails, text is NULL.
	 * Note that text must be deallocated using HighLevelTextDownloader::freeData().
	 */
	virtual void onDownloadComplete(char* text) = 0;

	/**
	 * Use to free data passed by onDownloadComplete.
	 */
	static void freeData(char* text);

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
