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
 * @file WikiEngine.h
 * author Emma Tresanszki
 *
 * This file contains the search engine for the application.
 * Check the http://en.wikipedia.org/w/api.php link for more request types.
 */

#ifndef WIKIENGINE_H_
#define WIKIENGINE_H_

// The library for strings.
#include <MAUtil/String.h>

// The vector container.
#include <MAUtil/Vector.h>

// Included to provide the basic connection functionality.
#include <MAUtil/Connection.h>

// Include the library that provides atoi function, which converts the contents of an array to an int.
#include <mastdlib.h>

// Include the class with utility functions.
#include "Util.h"

// The home screen.
#include "HomeScreen.h"

namespace WikiNativeUI
{

// The response data size.
#define CONNECTION_BUFFER_SIZE 1024

/** For each request, the response consists of an array of titles and snippets. */
struct MediaWikiPrivate {
    MAUtil::Vector<MAUtil::String> titleResults;
    MAUtil::Vector<MAUtil::String> snippetResults;
    MAUtil::String apiUrl;
    int maxItems;
    // constructor
    MediaWikiPrivate():
		apiUrl(""),
        // By default, set max 20 results.
		maxItems(20){
    }
};

/** Possible XML attributes found in the response. */
const MAUtil::String ATTR_TITLE = "title";
const MAUtil::String ATTR_SNIPPET = "snippet" ;

// Forward declaration.
class HomeScreen;

/**
 * Wiki Engine class
 * This class is responsible for sending requests to Wikipedia, and handle the
 * XML responses.
 *
 */
class MediaWiki : private MAUtil::HttpConnectionListener
{
public:

	/**
	 * Constructor.
	 * The constructor sets itself as the listener for the HttpConnection object,
	 * so that the callbacks will be used.
	 */
	MediaWiki(HomeScreen *parentScreen);

	/**
	 * Destructor.
	 */
	~MediaWiki();

	/**
	 * Set the parent screen.
	 * @param The screen.
	 */
	void setScreen(HomeScreen *parentScreen);

	/**
	 * Set the request url using the search term and checked categories.
	 */
	void constructApiUrl();

	/**
	 * Perform a search based on user input.
	 * Send a HTTP_GET request.
	 * @param searchTerm The search term.
	 * @param resultsLimit The number of maximum results expected.
	 */
	void search(MAUtil::String searchTerm, int resultsLimit=20);

	/**
	 * Parse the received XML, and search for title& snippet attributes.
	 */
	void processSearchResults();

	/**
	 * Parse paragraph, and get the title attribute.
	 * @param input The input paragraph.
	 * @return The title.
	 */
	MAUtil::String getTitle(MAUtil::String input);

	/**
	 * Parse paragraph, and get the snippet attribute.
	 * @param  input The input paragraph.
	 * @return The snippet.
	 */
	MAUtil::String getSnippet(MAUtil::String input);

	/**
	 * Inherited from HttpConnectionListener.
	 * This callback function is called when a connection with the server is
	 * established.
	 * @param http The HttpConnection that ran the operation.
	 * @param result The HTTP response code (eg 200 or 404) on success,
	 * or a \link #CONNERR_GENERIC CONNERR \endlink code \< 0 on failure.
	 */
	void httpFinished(MAUtil::HttpConnection *conn, int result);

	/**
	 * It is called each time a new chunk of data is received.
	 * @param conn The Connection that ran the operation.
	 * @param result The number of bytes read on success,
	 * or a \link #CONNERR_GENERIC CONNERR \endlink code \< 0 on failure.
	 */
	void connRecvFinished(MAUtil::Connection *conn, int result);

	/**
	 * It is called when the read is done.
	 * @param conn The Connection that ran the operation.
	 * @param result \> 0 on success,
	 * or a \link #CONNERR_GENERIC CONNERR \endlink code \< 0 on failure.
	 */
	void connReadFinished(MAUtil::Connection *conn, int result);

	/**
	 * Provides all the available titles.
	 * @return All the titles.
	 */
	MAUtil::Vector<MAUtil::String> getAllTitles();

	/**
	 * Provides the snippets.
	 * @return All the snippets.
	 */
	MAUtil::Vector<MAUtil::String> getAllSnippets();

	/**
	 * Provides the snippets along with the title.
	 * Only for the checked ones.
	 */
	MAUtil::Vector<MAUtil::String> getAllSnippetsWithTitle();

	/**
	 * Get the title on a given position.
	 * @param index The position for which to search the array of titles.
	 */
	MAUtil::String getTitleForIndex(int index);

	/**
	 * setter
	 * Sets the current search term.
	 * @param term The search term.
	 */
	void setSearchTerm(MAUtil::String term);

	/**
	 * Delete the entries on selected indexes.
	 * @param indexList The list with the indexes of the records that
	 * need to be deleted.
	 */
	void filterData(MAUtil::Vector<int> indexList);

	/**
	 * Mark as hidden the entries that are not checked in TitlesScreen.
	 * @param indexList The index of the item that needs to be hidden.
	 */
	void markAsHidden(int indexList);

	/**
	 * Clear all the indexes that are marked as hidden.
	 */
	void clearHiddenIndexList();

	/**
	 * Checks if the wiki item on the indexArray is marked as hidden.
	 * Item is marked as hidden when unchecked from TitlesScreen.
	 * @param indexArray The item index from wiki array of results.
	 * @return true if the item should be shown on SummaryScreen, false otherwise.
	 */
	bool isItemHidden(int indexArray);

private:
    // members

    /** Parent screen. **/
    HomeScreen*           mHomeScreen;

	/** The structure that holds the response. **/
	struct MediaWikiPrivate* mWiki;

	/**
	 * The index array of unchecked items from TitleScreen.
	 * These items need not to be displayed in SummaryScreen.
	 * Used so that titleResults and snippetResults are consistent between
	 * screen switches.
	 */
	MAUtil::Vector<int>     mUncheckedItems;

	/** The search Term, obtained from the edit box. **/
	MAUtil::String           mSearchTerm;

	/* The response buffer. **/
	char 					 mBuffer[CONNECTION_BUFFER_SIZE];

	/** The connection. **/
	MAUtil::HttpConnection   mHttp;

	/** Connection state. **/
	bool 					 mIsConnected;
};

} // namespace WikiNativeUI

#endif /* WIKIENGINE_H_ */
