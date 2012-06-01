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
