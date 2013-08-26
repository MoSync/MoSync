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
 * @file CountriesListScreen.h
 * @author Bogdan Iusco
 *
 * @brief NativeUI Screen used to show a list of countries.
 */

#ifndef EC_COUNTRIES_LIST_SCREEN_H_
#define EC_COUNTRIES_LIST_SCREEN_H_

#include <NativeUI/Screen.h>
#include <NativeUI/ListViewListener.h>
#include <MAUtil/Map.h>

// Forward declaration for NativeUI namespace classes.
namespace NativeUI
{
	class ListView;
	class RelativeLayout;
}

namespace EuropeanCountries
{
	// Forward declaration for EuropeanCountries namespace classes.
	class ICountryDatabase;
	class CountriesListScreenObserver;

	/**
	 * @brief Screen used to show a list of countries.
	 */
	class CountriesListScreen:
		public NativeUI::Screen,
		public NativeUI::ListViewListener
	{
	public:
		/**
		 * Constructor.
		 * @param database Used to read data.
		 * @param observer Will be notified when user selects a country from list.
		 */
		CountriesListScreen(ICountryDatabase& database,
			CountriesListScreenObserver& observer);

		/**
		 * Destructor.
		 */
		virtual ~CountriesListScreen();

		/**
		 * This method is called when a list view item is clicked.
		 * @param listView The list view object that generated the event.
		 * @param listViewSection The section object that contains the selected item.
		 * Will be null for default type list views.
		 * @param listViewItem The item object that was clicked.
		 */
		virtual void listViewItemClicked(
			NativeUI::ListView *listView,
			NativeUI::ListViewSection *listViewSection,
			NativeUI::ListViewItem *listViewItem);

		/**
		 * Show a screen. Only one screen at a time is visible.
		 * The previous screen will be hidden when showing a screen.
		 */
		virtual void show();

	private:
		/**
		 * Create screen UI.
		 */
		void createUI();

		/**
		 * Create and add an Image widget to the screen's main layout.
		 * It will take the screen size.
		 */
		void createImageWidget();

		/**
		 * Create and add an ListView widget to the screen's main layout.
		 * It will take the screen size and will be placed above the image widget.
		 */
		void createListViewWidget();

		/**
		 * Add data from database into list view.
		 */
		void addDataToListView();

	private:
		/**
		 * Database used to read data displayed by list view.
		 */
		ICountryDatabase& mDatabase;

		/**
		 * Will be notified when user selects an country from list.
		 */
		CountriesListScreenObserver& mObserver;

		/**
		 * Screen's main layout.
		 */
		NativeUI::RelativeLayout* mMainLayout;

		/**
		 * ListView that shows the country names.
		 */
		NativeUI::ListView* mListView;

		/**
		 * Store pairs of:
		 * - key: ListViewItem handle.
		 * - value: Country's id used to populate the ListViewItem.
		 */
		MAUtil::Map<int, int> mCountryMap;

	}; // end of CountriesListScreen

} // end of EuropeanCountries

#endif /* EC_COUNTRIES_LIST_SCREEN_H_ */
