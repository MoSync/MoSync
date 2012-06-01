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
