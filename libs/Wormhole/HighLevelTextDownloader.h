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
