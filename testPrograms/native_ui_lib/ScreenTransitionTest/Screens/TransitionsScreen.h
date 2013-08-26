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
 * @file FirstScreen.h
 * @author Bogdan Iusco & Mircea Vasiliniuc
 * @date 20 Nov 2012
 *
 * @brief Screen transitions screen.
 */

#ifndef SCREEN_TRANSITIONS_SCREEN_H_
#define SCREEN_TRANSITIONS_SCREEN_H_

#include <NativeUI/ButtonListener.h>
#include <NativeUI/Screen.h>
#include <NativeUI/ListViewListener.h>

#include "../Observers/TransitionsScreenObserver.h"

class MainStackScreen;

namespace NativeUI
{
	class Button;
	class Label;
	class VerticalLayout;
	class HorizontalLayout;
	class ListView;
	class ListViewItem;
}

namespace ScreenTransitionTest
{
	class TransitionsScreen:
		public NativeUI::Screen,
		public NativeUI::ButtonListener,
		public NativeUI::ListViewListener
	{
	public:
		/**
		 * Constructor.
		 * @param observer Observer for this screen.
		 */
		TransitionsScreen(TransitionsScreenObserver& observer);

		/**
		 * Destructor.
		 */
		virtual ~TransitionsScreen();

	private:
		/**
		 * This method is called if the touch-up event was inside the
		 * bounds of the button.
		 * Platform: iOS, Android, Windows Phone.
		 * @param button The button object that generated the event.
		 */
		virtual void buttonClicked(NativeUI::Widget* button);

	    /**
	     * This method is called when a list view item is clicked.
	     * @param listView The list view object that generated the event.
	     * @param listViewItem The ListViewItem object that was clicked.
	     */
	    virtual void listViewItemClicked(
	        NativeUI::ListView* listView,
	        NativeUI::ListViewItem* listViewItem);

	    /**
		 * This method is called when a list view item is clicked.
		 * @param listView The list view object that generated the event.
		 * @param index The index on which the list view item is positioned.
		 */
		virtual void listViewItemClicked(
			NativeUI::ListView* listView,
			int index);

		/**
		 * Create screen's UI.
		 */
		virtual void createUI();

		virtual void addTitleLable();

		virtual void addTransitionsList();

		virtual void addFooter();

		/**
		 * Fills the screen transition container.
		 */
		virtual void fillScreenTransitionContainer();

		/**
		 * Populates the screen transition list.
		 */
		virtual void populateTransitionList();

		//Workaround for Android 4.0 bug related to list view focus.
		void giveFocusToList();

		friend class MainStackScreen;

	private:
		/**
		 * Observer for this screen.
		 */
		TransitionsScreenObserver& mObserver;

		/**
		 * Screen's layout.
		 */
		NativeUI::VerticalLayout* mMainLayout;

		/**
		 * List's layout.
		 */
		NativeUI::HorizontalLayout* mListLayout;

		/**
		 * Footer layout.
		 */
		NativeUI::HorizontalLayout* mFooterLayout;

		/**
		 * Start screen transition button
		 */
		NativeUI::Button* mShowSecondScreenButton;

		/**
		 * Screen Title
		 */
		NativeUI::Label* mTitleLabel;

		/**
		 * Screen transitions list
		 */
		NativeUI::ListView* mScreenTransitionListView;

		/**
		 * Previous item selected. Used in list management.
		 */
		NativeUI::ListViewItem* mPreviousItem;

		/**
		 * Previous list item
		 */
		int mSelectedListItem;

		/**
		 * Vector of screen transitions available on the current platform.
		 */
		MAUtil::Vector<int> mPlatformTransitions;
	};
}


#endif /* SCREEN_TRANSITIONS_SCREEN_H_ */
