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
		 * This method is called when an alphabetical list view item is clicked.
		 * From ListViewListener.
		 * @param listView The list view object that generated the event.
		 * @param listViewSection The ListViewSection object that contains the selected item.
		 * @param listViewItem The ListViewItem objet clicked.
		 */
		virtual void segmentedListViewItemClicked(
			NativeUI::ListView* listView,
			NativeUI::ListViewSection* listViewSection,
			NativeUI::ListViewItem* listViewItem);

		/**
		 * This method is called when a list view item is clicked.
		 * From ListViewListener.
		 * @param listView The list view object that generated the event.
		 * @param listViewItem The ListViewItem object that was clicked.
		 */
		virtual void listViewItemClicked(
			NativeUI::ListView* listView,
			NativeUI::ListViewItem* listViewItem) {};

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
