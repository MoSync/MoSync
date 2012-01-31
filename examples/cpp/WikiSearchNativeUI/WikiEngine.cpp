/*
Copyright (C) 2011 MoSync AB

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
 * @file WikiEngine.cpp
 * @author Emma Tresanszki
 *
 * This file contains the search engine for the application.
 * Check the http://en.wikipedia.org/w/api.php link for more request types.
 */

// The header file of the engine.
#include "WikiEngine.h"

namespace WikiNativeUI
{

/**
 * Constructor.
 * The constructor sets itself as the listener for the HttpConnection object,
 * so that the callbacks will be used.
 */
MediaWiki::MediaWiki(HomeScreen *parentScreen):
		mHomeScreen(parentScreen),
		mSearchTerm(""),
		mHttp(this),
		mIsConnected(false)
{
	// Call constructor to set default values.
    mWiki = new MediaWikiPrivate();
}

/**
 * Destructor.
 */
MediaWiki::~MediaWiki()
{
	if (mIsConnected){
		mHttp.close();
	}

	// Delete the engine.
	if (mWiki)
	{
		delete mWiki;
		mWiki = NULL;
	}
}

/**
 * Set the parent screen.
 * @param The screen.
 */
void MediaWiki::setScreen(HomeScreen *parentScreen)
{
	mHomeScreen = parentScreen;
}

/**
 * Parse the received XML, and search for title& snippet attributes.
 */
void MediaWiki::processSearchResults()
{
	/* for ex, try this request for "bear" in your browser:
	 *   http://en.wikipedia.org/w/api.php?action=query&format=xml&
	 *   list=search&srwhat=text&srsearch=bear
	 * the output looks like this:
	 * ...
	 * <search>
	 * <p ns="0" title="title1" snippet="text.." size="10283"
	 * wordcount="1324" timestamp="2011-04-12T14:25:24Z" />
	 * more paragraphs
	 * </search>
	 * ...
	 */

	// Search for each p paragraph, and get the title & snippet.
	MAUtil::String input = mBuffer;
	int openTag = input.find("<p ",0);

	while( openTag != -1 )
	{
		int closeTag = input.find("/>", openTag+1);
		if (closeTag != -1 && closeTag > openTag)
		{
			MAUtil::String  record = input.substr(
				openTag+2, closeTag - openTag +1);
			// A record, ex: <p ns="0" title="title1" snippet="text.."
			// size="10283" wordcount="1324" timestamp="2011-04-12T14:25:24Z" />
			// Add the title if it doesn't exist yet.
			MAUtil::String newRecord = getTitle(record);
			bool canAdd(true);
			for (int i=0; i < mWiki->titleResults.size(); i++)
			{
				if ( mWiki->titleResults[i] == newRecord )
				{
					canAdd = false;
				}
			}
			if (canAdd)
			{
				mWiki->titleResults.add(newRecord);
				mWiki->snippetResults.add(getSnippet(record));
			}
		}
		input.remove(0,closeTag);

		// Get the next tag.
		openTag = input.find("<p ",0);
	}
}

/**
 * Parse paragraph, and get the title attribute.
 * @param input The input paragraph.
 * @return The title.
 */
MAUtil::String MediaWiki::getTitle(MAUtil::String inputStr)
{
	MAUtil::String title = "";
	int start = inputStr.find("title=",0);
	if ( start != -1 ){
		int end = inputStr.find(" snippet=", start+1);
		if ( end != -1)
		{
			// Get rid of the quotes also
			// ( from beginning, and end): title="text.." snippet=etc
			title = inputStr.substr(start+7, end - start - 8);
		}
	}
	// Format the title to unicode.
	formatToUnicode(title);

	// Get rid of the span tags, and keep only the content: the searchTerm.
	trimSpanTags(title);

	return title;
}

/**
 * Parse paragraph, and get the snippet attribute.
 * @param input The input paragraph.
 * @return The snippet.
 */
MAUtil::String MediaWiki::getSnippet(MAUtil::String inputStr)
{
	MAUtil::String snippet = "";
	int start = inputStr.find("snippet=",0);
	if ( start != -1 ){
		int end = inputStr.find(" size=", start+1);
		if ( end != -1){
			// Get rid of the quotes also( from beginning, and end)
			snippet = inputStr.substr(start+9, end - start - 10 );
		}
	}
	// Format the snippet to unicode.
	formatToUnicode(snippet);

	// Get rid of the span tags, and keep only the content: the searchTerm.
	trimSpanTags(snippet);

	// Get rid of the bold tags from end, that enclose dots.
	trimBoldTags(snippet);

	return snippet;
}

/**
 * Set the request url using the search term and checked categories.
 */
void MediaWiki::constructApiUrl()
{
	// Search inside the article text. The search inside the titles
	// ("srwhat=title") is disabled by the server.
	mWiki->apiUrl =
		"http://en.wikipedia.org/w/api.php?action=query&format=xml&list=search&srlimit=";

	// Append the search limit and the search term.
	// Note: even if the search limit is 0, there will be one result returned.
	mWiki->apiUrl +=
		MAUtil::integerToString( mWiki->maxItems )+
		"&srwhat=text&srsearch=" + mSearchTerm ;

	// Add the categories as tags after the search word, separated by comma.
	mWiki->apiUrl += mHomeScreen->getCheckedCategories();

	// Replace spaces.
	replaceString(mWiki->apiUrl," ","%20");
}

/**
 * Perform a search based on user input.
 * Send a HTTP_GET request.
 * @param searchTerm The search term.
 * @param resultsLimit The number of maximum results expected.
 */
void MediaWiki::search(MAUtil::String searchTerm, int resultsLimit)
{
	// Set a maximum number of results.
	mWiki->maxItems = resultsLimit;

	setSearchTerm(searchTerm);

	// Clear old results.
	mWiki->titleResults.clear();
	mWiki->snippetResults.clear();

	// Construct the request url.
	constructApiUrl();

	int res = mHttp.create(mWiki->apiUrl.c_str(), HTTP_GET);
	if(res < 0) {
		// It have failed for some reason.
		mHomeScreen->engineError(ERROR_NO_CONNECTION);
	 }
	 else {
		mHttp.setRequestHeader("User-Agent","WikipediaSearchNativeUI");
		mHttp.finish();
		mIsConnected = true;
	 }
}

/**
 * Inherited from HttpConnectionListener.
 * This callback function is called when a connection with the server is
 * established.
 * @param http The HttpConnection that ran the operation.
 * @param result The HTTP response code (eg 200 or 404) on success,
 * or a \link #CONNERR_GENERIC CONNERR \endlink code \< 0 on failure.
 */
void MediaWiki::httpFinished(MAUtil::HttpConnection *conn, int result)
 {
	if ( result >= 0)
	{
		MAUtil::String contentLengthStr;
		// todo: check return value.
		mHttp.getResponseHeader("Content-Length",
			&contentLengthStr);

		int contentLength = 0;
		contentLength = atoi(contentLengthStr.c_str());

		if (contentLength >= CONNECTION_BUFFER_SIZE || contentLength == 0) {
			// Receive in chunks.
			// Calculate how many chunks we receive, so that we can increment
			// a progress bar for this action.
			div_t divide = div(contentLength, CONNECTION_BUFFER_SIZE);
			int steps = divide.quot;
			if (divide.rem != 0) {
				steps++;
				// Notify the UI of the number of increments the progress bar
				// will need for this action.
				mHomeScreen->engineNrSteps(steps);
			}

			mHttp.recv(mBuffer, CONNECTION_BUFFER_SIZE);
		}
		else
		{
			// In this case the progress bar for this action will jump
			// immediately to the max value.
			mHomeScreen->engineNrSteps(1);
			mBuffer[contentLength] = 0;
			mHttp.read(mBuffer, contentLength);
		}
	}
	else
	{
		// Notify UI on the error.
		mHomeScreen->engineError( ERROR_NO_CONNECTION );

		mHttp.close();
		mIsConnected = false;
	}
}

/**
 * It is called when the read is done.
 * @param conn The Connection that ran the operation.
 * @param result \> 0 on success,
 * or a \link #CONNERR_GENERIC CONNERR \endlink code \< 0 on failure.
 */
void MediaWiki::connReadFinished(MAUtil::Connection* conn, int result)
{
    if(result >= 0){
        // mBuffer now contains the result,we can now parse it.
		processSearchResults();
		if ( mWiki->titleResults.size() > 0 )
		{
			// The result is parsed, now display it.
			mHomeScreen->engineFinished();
		}
		else{
			// The results vector is empty.
			mHomeScreen->engineError(ERROR_NO_RESULTS);
		}
	}
	else{
		// Notify UI on the error.
		mHomeScreen->engineError(ERROR_INVALID_DATA+integerToString(result,10));
	}

	mHttp.close();

	mIsConnected = false;
}

/**
 * It is called each time a new chunk of data is received.
 * @param conn The Connection that ran the operation.
 * @param result The number of bytes read on success,
 * or a \link #CONNERR_GENERIC CONNERR \endlink code \< 0 on failure.
 */
void MediaWiki::connRecvFinished(MAUtil::Connection * conn, int result)
{
    if(result >= 0) {
		// Notify the UI that a new chunk was received
		mHomeScreen->engineChunkReceived();

		// Parse each chunk.
		// mBuffer now contains the result,we can now parse it.
		processSearchResults();
		// Read next chunk of data.
        mHttp.recv(mBuffer, CONNECTION_BUFFER_SIZE);

        return;
    }
    else if(result == CONNERR_CLOSED) {
        // The result is parsed, now display it( if there is any).
		if ( mWiki->titleResults.size() > 0 ){
			mHomeScreen->engineFinished();
		}
		else{
			// No results available,display some message.
			mHomeScreen->engineError(ERROR_NO_RESULTS);
		}
    } else {
		// Notify the UI of the error.
        mHomeScreen->engineError(ERROR_INVALID_DATA+integerToString(result, 10));
    }
    mHttp.close();
    mIsConnected = false;

}

/**
 * Provides all the available titles.
 * @return All the titles.
 */
MAUtil::Vector<MAUtil::String> MediaWiki::getAllTitles()
{
	return mWiki->titleResults;
}

/**
 * Provides the snippets.
 * @return All the snippets.
 */
MAUtil::Vector<MAUtil::String> MediaWiki::getAllSnippets()
{
	return mWiki->snippetResults;
}

/**
 * Provides the snippets along with the title.
 * Only for the checked ones.
 */
MAUtil::Vector<MAUtil::String> MediaWiki::getAllSnippetsWithTitle()
{
	MAUtil::Vector<MAUtil::String> list;
	for (int i=0; i < mWiki->titleResults.size(); i++)
	{
		if ( !isItemHidden(i) )
		{
			// Each entry has title and the corresponding snippet.
			MAUtil::String line =
				mWiki->titleResults[i] +
				"\n" +
				mWiki->snippetResults[i] ;
			list.add(line);
		}
	}
	return list;
}

/**
 * Provides the titles.
 * Only for the checked ones.
 */
MAUtil::Vector<MAUtil::String> MediaWiki::getAllCheckedTitles()
{
	MAUtil::Vector<MAUtil::String> list;
	for (int i=0; i < mWiki->titleResults.size(); i++)
	{
		if ( !isItemHidden(i) )
		{
			list.add(mWiki->titleResults[i]);
		}
	}
	return list;
}

/**
 * Get the title on a given position.
 * @param index The position for which to search the array of titles.
 */
MAUtil::String MediaWiki::getTitleForIndex(int index)
{
	return mWiki->titleResults[index];
}

/**
 * setter
 * Sets the current search term.
 * @param term The search term.
 */
void MediaWiki::setSearchTerm(MAUtil::String term)
{
	mSearchTerm = term;
}

/**
 * Delete the entries on selected indexes.
 * @param indexList The list with the indexes of the records that
 * need to be deleted.
 */
void MediaWiki::filterData(MAUtil::Vector<int> indexList)
{

	int shift = 0;
	// The vector is shifted to the left.
	for (int i=0; i < indexList.size(); i++)
	{
		if ( indexList[i]-shift >= 0)
		{
			if ( indexList[i]-shift < mWiki->titleResults.size() ){
				mWiki->titleResults.remove(indexList[i]-shift);
			}
			if ( indexList[i]-shift < mWiki->snippetResults.size() ){
				mWiki->snippetResults.remove(indexList[i]-shift);
			}
		}
		shift++;
	}
}

/**
 * Mark as hidden the entries that are not checked in TitlesScreen.
 * @param indexList The index of the item that needs to be hidden.
 */
void MediaWiki::markAsHidden(int indexList)
{
	mUncheckedItems.add(indexList);
}

/**
 * Clear all the indexes that are marked as hidden.
 */
void MediaWiki::clearHiddenIndexList()
{
	mUncheckedItems.clear();
}

/**
 * Checks if the wiki item on the indexArray is marked as hidden.
 * Item is marked as hidden when unchecked from TitlesScreen.
 * @param indexArray The item index from wiki array of results.
 * @return true if the item should be hidden on SummaryScreen, false otherwise.
 */
bool MediaWiki::isItemHidden(int indexArray)
{
	for (int i=0; i < mUncheckedItems.size(); i++)
	{
		if ( mUncheckedItems[i] == indexArray )
		{
			return true;
		}
	}
	return false;
}

}  // namespace WikiNativeUI
