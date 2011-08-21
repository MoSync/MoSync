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
 * @file TitlesScreen.h
 *
 * This file contains the second screen of the application.
 * This screen displays all the available article titles.
 *
 * @author Emma Tresanszki
 */

#ifndef TITLESSCREENNUI_H_
#define TITLESSCREENNUI_H_

// Include the library for event handling listener.
#include "MAUtil/Environment.h"

// The engine class.
#include "WikiEngine.h"

// The next screen.
#include "SummaryScreen.h"

// Include the base screen class.
#include "BasicScreen.h"

namespace WikiNativeUI
{

// Forward declaration.
class HomeScreen;

/*
 * TitlesScreen: native UI screen
 * It contains the available article titles.
 * Results are shown in a list view, and the user can select only the desired articles.
 * For each selected title, the corresponding article will be displayed.
 */
class TitleScreen : public BasicScreen,
			        public MAUtil::CustomEventListener
{
public:
	/*
	 * constructor
	 * Pass the previous screen and the engine.
	 */
	TitleScreen(HomeScreen *parentScreen, MediaWiki* engine);

	/*
	 * destructor
	 */
	~TitleScreen();

	/*
	 *  Show the screen.
	 *  If it does not need refresh, it is called from the SummaryScreen.
	 *  The list box need refresh when this is called from the home screen.
	 */
	void showScreen(bool needsRefresh);

	/*
	 * from CustomEventListener
	 * The custom event listener interface.
	 */
	void customEvent(const MAEvent& event);

	/*
	 * Handle events on screen's widgets.
	 */
	void widgetClicked(MAHandle widgetHandle);

	/*
	 * Fill the list box with data provided by the engine.
	 */
	void fillListBox();

	/*
	 * Update wiki list of titles, with the ones selected by the user.
	 */
	void updateWikiTitles();

	/*
	 * Get all the snippets.
	 * @return all snippets with user checked title.
	 */
	MAUtil::Vector<MAUtil::String> getAllSnippets();

	/*
	 * Called when user wants to start over, but he is in another screen.
	 */
	void showHomeScreen();

private: // methods

	/*
	 * Lay out the widgets (portrait mode).
	 */
	void setupUI();

private: // members

	/** NativeUI widgets handles. **/
	/** Handle to the screen. **/
	MAWidgetHandle 			mScreen;

	/** Handle to the previous screen. **/
	HomeScreen* 			mPrevScreen;

	/** This screen is shown when the user clicks Next. **/
	SummaryScreen*   	mSummaryScreen;

	/** The main vertical layout. **/
	MAWidgetHandle 			mMainLayout;

	/** label in the top layout. **/
	MAWidgetHandle 			mLabel;

	/** Next button. **/
	MAWidgetHandle 			mNextButton;

	/** Previous button. **/
	MAWidgetHandle          mBackButton;

	/** Check box for select/deselect All. **/
	MAWidgetHandle          mSelectAll;

	/** These vectors contain only the checked articles. **/
	MAUtil::Vector<MAUtil::String> mTitles;
	MAUtil::Vector<MAUtil::String> mSnippets;

	/** The handles for the check boxes. */
	MAUtil::Vector<MAWidgetHandle> mCheckBoxes;

	/** The handles for all the labels that contain the titles. */
	MAUtil::Vector<MAWidgetHandle> mTitleLabels;

	/** List box with all titles ( check box + label ). **/
	MAWidgetHandle 			mListView;

	/** The wikipedia engine. **/
	MediaWiki*              mWiki;
};

} // namespace WikiNativeUI

#endif /* TITLESSCREENNUI_H_ */
