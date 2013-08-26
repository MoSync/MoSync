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

/**
 * @file MainScreen.h.
 * @author Bogdan Iusco.
 */

#ifndef MAINSCREEN_H_
#define MAINSCREEN_H_

#include <maapi.h>
#include <MAUtil/util.h>

// Include all the wrappers.
#include <NativeUI/Widgets.h>

using namespace NativeUI;

class MainScreen:
	public Screen,
	public SearchBarListener
{

public:
	/**
	 * Constructor.
	 */
		MainScreen();

	/**
	 * Destructor.
	 */
	~MainScreen();

private:

	/**
	 * Creates and adds main layout to the screen.
	 */
	void createMainLayout();

    /**
     * This method is called when the search button is clicked.
     * @param searchBar The search bar object that generated the event.
     */
    virtual void searchBarSearchButtonClicked(SearchBar* searchBar);

    /**
     * This method is called when the cancel button is clicked.
     * @param searchBar The search bar object that generated the event.
     */
    virtual void searchBarCancelButtonClicked(SearchBar* searchBar);

private:
	/**
	 * Main layout.
	 */
	VerticalLayout* mMainLayout;

	SearchBar* mSearchBar;

	/*
	 * Label used to observe the events received by the search bar
	 */
	Label *mDebugLabel;
};

#endif /* MAINSCREEN_H_ */
